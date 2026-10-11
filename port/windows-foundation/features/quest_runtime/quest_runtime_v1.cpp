#include "quest_runtime_v1.hpp"

#include "../../character_quest_blob.hpp"

#include <algorithm>
#include <tuple>

namespace dh::foundation::quest_runtime {
namespace {

// Authored quest description StringID: Quest::GetPreDescription (definition +8 = text_fields[1]).
// IDA Quest::SetState case 6 (NEW QUEST) and case 12 (QUEST COMPLETED) use this string, and the Quest
// Log pane shows it as the description. The objective-list sentence (Quest::GetObjectiveDescription,
// text_fields[3] then ObjectiveList::GetDesc) is empty for every Act 1 row because the objective
// description ids are -1, so it is not used here. The none sentinel (1835016) means "no text".
constexpr std::int32_t kSourceNoneStringV1 = 1835016;
std::int32_t row_objective_text_v1(const dh2::data::QuestDefinitionV51& def) {
    const auto id = def.text_fields[1];
    return (id >= 0 && id != kSourceNoneStringV1) ? id : -1;
}

using Objective = dh2::data::QuestObjectiveDefinitionV51;
using ConditionDefinition = dh2::data::QuestConditionDefinitionV51;
using Definition = dh2::data::QuestDefinitionV51;

constexpr std::int32_t kStateCount = 14;
constexpr std::int32_t kDifficultyCount = 3;
// Slot kinds for the event windows of an authored quest.
constexpr std::int32_t kSlotAccept = 0;
constexpr std::int32_t kSlotObjectives = 1;
constexpr std::int32_t kSlotEnd = 2;

// Objective types whose progress is a counter (native has_quantity: not 4, 6, 12).
bool objective_has_quantity(std::int32_t type) noexcept {
    return type != static_cast<std::int32_t>(QuestObjectiveTypeV1::move_in_zone) &&
           type != static_cast<std::int32_t>(QuestObjectiveTypeV1::automatic) &&
           type != static_cast<std::int32_t>(QuestObjectiveTypeV1::gather_loot);
}

// An authored objective is supported when its matching rule is fully determined by
// the table. MoveInZone rows without a trigger name (str2) have no authored rule here.
// clear_* need the level's loaded-enemy query; gather_loot needs the inventory.
bool objective_supported(const Objective& def) noexcept {
    switch (static_cast<QuestObjectiveTypeV1>(def.type)) {
    case QuestObjectiveTypeV1::kill_x_enemies:
    case QuestObjectiveTypeV1::trigger_plate:
    case QuestObjectiveTypeV1::destroy_game_object:
    case QuestObjectiveTypeV1::talk_to_npc:
    case QuestObjectiveTypeV1::automatic:
    case QuestObjectiveTypeV1::open_game_object:
    case QuestObjectiveTypeV1::trigger_on:
    case QuestObjectiveTypeV1::picked_up_liftable:
    case QuestObjectiveTypeV1::kill_enemy_template:
        return true;
    case QuestObjectiveTypeV1::move_in_zone:
        return !def.str2.empty();
    default:
        return false;
    }
}

std::string objective_name(const char* prefix, std::int32_t row, std::int32_t objective, std::int32_t type) {
    return std::string(prefix) + " row " + std::to_string(row) + " objective " +
           std::to_string(objective) + " type " + std::to_string(type);
}

std::int32_t reward_total(const Definition& def, std::int32_t difficulty, QuestRewardTypeV1 kind) {
    std::int64_t total = 0;
    for (const auto& reward : def.rewards[std::size_t(difficulty)])
        if (reward.type == static_cast<std::int32_t>(kind) && reward.parameter1 > 0) total += reward.parameter1;
    return static_cast<std::int32_t>(std::min<std::int64_t>(total, 0x7fffffff));
}

} // namespace

QuestRuntimeV1::QuestRuntimeV1(CharacterState& character, std::shared_ptr<const QuestTableV1> table,
    QuestRuntimeServicesV1 services)
    : character_(character), table_(std::move(table)), services_(std::move(services)) {}

std::int32_t QuestRuntimeV1::difficulty() const {
    return character_current_difficulty(character_);
}

bool QuestRuntimeV1::row_valid(std::int32_t row) const noexcept {
    return table_ && row >= 0 && std::size_t(row) < table_->rows().size();
}

QuestRowProgressV1& QuestRuntimeV1::row_progress(std::int32_t difficulty, std::int32_t row) {
    auto& entry = rows_[{difficulty, row}];
    const auto& def = table_->rows()[std::size_t(row)];
    if (entry.objectives.size() != def.objectives.size()) entry.objectives.resize(def.objectives.size());
    return entry;
}

void QuestRuntimeV1::report_once(const std::string& text) {
    if (std::find(reported_.begin(), reported_.end(), text) != reported_.end()) return;
    reported_.push_back(text);
    diagnostics_.push_back(text);
}

void QuestRuntimeV1::report_unsupported(const Objective& def, std::int32_t row, std::int32_t objective) {
    report_once(objective_name("Quest objective is not supported by the thin runtime (type unhandled or no authored match rule):", row, objective, def.type));
}

bool QuestRuntimeV1::state_of(std::int32_t row, QuestStateV1& state) const {
    if (!row_valid(row)) return false;
    CharacterQuestStateV1 found;
    std::string error;
    if (!progress_.query(character_, CharacterQuestIdV1{0, difficulty(), row}, found, error)) return false;
    state = QuestStateV1(found.state);
    return true;
}

const QuestRowProgressV1* QuestRuntimeV1::progress_of(std::int32_t row) const {
    const auto it = rows_.find({difficulty(), row});
    return it == rows_.end() ? nullptr : &it->second;
}

std::int32_t QuestRuntimeV1::current_quest() const {
    CharacterQuestProgressV1::BucketView view;
    std::string error;
    if (!progress_.bucket(character_, 0, difficulty(), view, error)) return -1;
    return view.current_quest;
}

bool QuestRuntimeV1::load(std::string& error) {
    if (!table_ || !table_->ready()) { error = "Quest runtime requires the decoded original table"; return false; }
    rows_.clear();
    reported_.clear();
    diagnostics_.clear();
    banners_.clear();
    const auto& bytes = character_.source_quest_progress_cqpg;
    if (bytes.empty()) {
        // New character: every row takes its authored initial state and is written back.
        if (!progress_.initialize_fresh(character_, *table_, error)) return false;
        if (!save(error)) return false;
    } else {
        if (!progress_.decode(character_, *table_, bytes, error)) return false;
        std::vector<QuestObjectiveCounterV2> counters;
        if (!read_quest_counters(character_.id, bytes, counters, error)) return false;
        for (const auto& c : counters) {
            if (c.collection != 0 || c.difficulty >= kDifficultyCount || !row_valid(std::int32_t(c.row))) {
                error = "Quest counter names a row outside the original table";
                return false;
            }
            const auto& def = table_->rows()[c.row];
            auto& row = row_progress(std::int32_t(c.difficulty), std::int32_t(c.row));
            QuestObjectiveProgressV1* slot = nullptr;
            if (c.objective == kAcceptObjectiveV1) slot = &row.accept;
            else if (c.objective == kEndObjectiveV1) slot = &row.end;
            else if (c.objective < def.objectives.size()) slot = &row.objectives[c.objective];
            if (!slot) { error = "Quest counter names an objective outside its quest"; return false; }
            slot->quantity = c.quantity;
            slot->completed = c.completed;
        }
    }
    return update(error);
}

bool QuestRuntimeV1::write_counters(std::string& error) {
    std::vector<QuestObjectiveCounterV2> counters;
    for (const auto& entry : rows_) {
        const auto& key = entry.first;
        const auto& row = entry.second;
        auto add = [&](std::uint32_t objective, const QuestObjectiveProgressV1& slot) {
            if (slot.quantity == 0 && !slot.completed) return;
            QuestObjectiveCounterV2 c;
            c.collection = 0;
            c.difficulty = std::uint8_t(key.first);
            c.row = std::uint32_t(key.second);
            c.objective = objective;
            c.quantity = slot.quantity;
            c.completed = slot.completed;
            counters.push_back(c);
        };
        for (std::size_t i = 0; i < row.objectives.size(); ++i) add(std::uint32_t(i), row.objectives[i]);
        add(kAcceptObjectiveV1, row.accept);
        add(kEndObjectiveV1, row.end);
    }
    std::sort(counters.begin(), counters.end(), [](const auto& a, const auto& b) {
        return std::tie(a.collection, a.difficulty, a.row, a.objective) <
               std::tie(b.collection, b.difficulty, b.row, b.objective);
    });
    return write_quest_counters(character_.id, character_.source_quest_progress_cqpg, counters, error);
}

bool QuestRuntimeV1::save(std::string& error) {
    std::vector<std::uint8_t> bytes;
    if (!progress_.encode(*table_, bytes, error)) return false;
    character_.source_quest_progress_cqpg = std::move(bytes);
    return write_counters(error);
}

std::int32_t quest_wait_slot_v1(QuestStateV1 state) noexcept {
    // IDA Quest::UpdatePostLocked 9, UpdatePreAvailable 11, UpdateAvailable 1, UpdatePostAvailable 6,
    // UpdatePreActive 10, UpdateActive 0, UpdatePostActive 8 (IDA waits on 8, not on the PostActive slot 5),
    // UpdatePreCompleted 13, UpdateCompleted 3, UpdatePostCompleted 8, UpdatePreClosed 12, UpdateClosed 2.
    switch (state) {
    case QuestStateV1::post_locked: return 9;
    case QuestStateV1::pre_available: return 11;
    case QuestStateV1::available: return 1;
    case QuestStateV1::post_available: return 6;
    case QuestStateV1::pre_active: return 10;
    case QuestStateV1::active: return 0;
    case QuestStateV1::post_active: return 8;
    case QuestStateV1::pre_completed: return 13;
    case QuestStateV1::completed: return 3;
    case QuestStateV1::post_completed: return 8;
    case QuestStateV1::pre_closed: return 12;
    case QuestStateV1::closed: return 2;
    default: return -1;
    }
}

std::int32_t quest_script_slot_v1(QuestStateV1 next, QuestStateV1 previous) noexcept {
    switch (next) {
    case QuestStateV1::post_locked: return 9;
    case QuestStateV1::pre_available: return 11;
    case QuestStateV1::available: return previous == QuestStateV1::active ? 4 : 1;
    case QuestStateV1::post_available: return 6;
    case QuestStateV1::pre_active: return 10;
    case QuestStateV1::active: return 0;
    case QuestStateV1::post_active: return 5;
    case QuestStateV1::pre_completed: return 13;
    case QuestStateV1::completed: return 3;
    case QuestStateV1::post_completed: return 8;
    case QuestStateV1::pre_closed: return 12;
    case QuestStateV1::closed: return 2;
    case QuestStateV1::post_closed: return 7;
    default: return -1;
    }
}

bool QuestRuntimeV1::slot_script_running(std::int32_t row, std::int32_t slot) const {
    if (!services_.script_running || slot < 0 || row < 0 || std::size_t(row) >= table_->rows().size()) return false;
    const auto& scripts = table_->rows()[std::size_t(row)].scripts;
    if (std::size_t(slot) >= scripts.size() || scripts[std::size_t(slot)].empty()) return false;
    const std::string& full = scripts[std::size_t(slot)];
    const auto dot = full.rfind('.');
    return services_.script_running(dot == std::string::npos ? full : full.substr(dot + 1));
}

bool QuestRuntimeV1::set_state(std::int32_t row, QuestStateV1 next, std::string& error) {
    const auto d = difficulty();
    QuestStateV1 previous{};
    if (!state_of(row, previous)) previous = QuestStateV1::locked;
    if (!progress_.record_source_state(character_, CharacterQuestIdV1{0, d, row},
                                       std::int32_t(next), error))
        return false;
    const auto& def = table_->rows()[std::size_t(row)];
    // OPENING2: the state's authored script starts through the script owner (the Swamp opening starts this way).
    const auto slot = quest_script_slot_v1(next, previous);
    if (slot >= 0 && std::size_t(slot) < def.scripts.size() && !def.scripts[std::size_t(slot)].empty()) {
        const std::string full = def.scripts[std::size_t(slot)];
        const auto dot = full.rfind('.');
        const std::string script = dot == std::string::npos ? full : full.substr(dot + 1);
        std::string scriptError;
        if (!services_.start_script) report_once("Quest script " + full + " not started (no script owner bound)");
        else if (!services_.start_script(script, scriptError)) report_once("Quest script " + full + " not started: " + scriptError);
    }
    switch (next) {
    case QuestStateV1::active: {
        // SetState case 6: NEW QUEST banner and current quest.
        QuestBannerV1 banner;
        banner.kind = QuestBannerV1::Kind::new_quest;
        banner.row = row;
        banner.difficulty = d;
        banner.objective_text_id = row_objective_text_v1(def);
        if (banner.objective_text_id >= 0 && services_.text) {
            std::string text;
            if (services_.text(banner.objective_text_id, text)) banner.text = text;
        }
        banners_.push_back(std::move(banner));
        if (!progress_.set_current_quest(character_, 0, d, row, error)) return false;
        break;
    }
    case QuestStateV1::post_active:
        // SetState case 7: SG_SetCurrentQuest(-1).
        if (!progress_.set_current_quest(character_, 0, d, -1, error)) return false;
        break;
    case QuestStateV1::completed:
        // SetState case 9: the act of the completed quest becomes the current act.
        if (def.act > 0) character_.menu_metadata.current_act[std::size_t(d)] = def.act;
        break;
    case QuestStateV1::closed: {
        // SetState case 12: QUEST COMPLETED panel (authored objective and reward values).
        QuestBannerV1 banner;
        banner.kind = QuestBannerV1::Kind::completed;
        banner.row = row;
        banner.difficulty = d;
        banner.objective_text_id = row_objective_text_v1(def);
        if (banner.objective_text_id >= 0 && services_.text) {
            std::string text;
            if (services_.text(banner.objective_text_id, text)) banner.text = text;
        }
        banner.reward_xp = reward_total(def, d, QuestRewardTypeV1::xp);
        banner.reward_gold = reward_total(def, d, QuestRewardTypeV1::gold);
        banners_.push_back(std::move(banner));
        break;
    }
    default:
        break;
    }
    return true;
}

bool QuestRuntimeV1::condition_met(const ConditionDefinition& c, bool& met) {
    met = false;
    const auto op = static_cast<QuestConditionOperatorV1>(c.type);
    if (op == QuestConditionOperatorV1::is_player_in_level) {
        // Original Condition_IsPlayerInLevel::Eval: parameter1 == current level row.
        const auto level = services_.current_level_row ? services_.current_level_row() : -1;
        if (level < 0) {
            report_once("Quest prerequisite IsPlayerInLevel has no current level row (condition evaluates false)");
            return true;
        }
        met = level == c.parameter1;
        return true;
    }
    if (op != QuestConditionOperatorV1::is_quest_in_state &&
        op != QuestConditionOperatorV1::is_quest_state_lower &&
        op != QuestConditionOperatorV1::is_quest_state_higher) {
        report_once("Quest prerequisite condition type " + std::to_string(c.type) +
                    " is not supported by the thin runtime (condition evaluates false)");
        return true;
    }
    QuestStateV1 other;
    if (!state_of(c.parameter1, other)) {
        report_once("Quest prerequisite names an unknown quest row " + std::to_string(c.parameter1));
        return true;
    }
    const auto state = std::int32_t(other);
    switch (op) {
    case QuestConditionOperatorV1::is_quest_in_state: met = state == c.parameter2; break;
    case QuestConditionOperatorV1::is_quest_state_lower: met = state < c.parameter2; break;
    case QuestConditionOperatorV1::is_quest_state_higher: met = state > c.parameter2; break;
    case QuestConditionOperatorV1::is_player_in_level: break; // handled above
    }
    return true;
}

bool QuestRuntimeV1::prerequisites_met(std::int32_t row, bool& met) {
    met = true;
    for (const auto& c : table_->rows()[std::size_t(row)].prerequisites) {
        bool ok = false;
        if (!condition_met(c, ok)) return false;
        if (!ok) { met = false; return true; }
    }
    return true;
}

bool QuestRuntimeV1::objective_matches(const Objective& def, const QuestEvent& event) const {
    switch (static_cast<QuestObjectiveTypeV1>(def.type)) {
    case QuestObjectiveTypeV1::kill_x_enemies:
        return event.kind == QuestEvent::Kind::kill && event.property_id == def.oid1;
    case QuestObjectiveTypeV1::kill_enemy_template:
        return event.kind == QuestEvent::Kind::kill && event.template_id == def.oid1;
    case QuestObjectiveTypeV1::move_in_zone:
        // Accept zone (str2 = authored trigger prim, oid1 = level row). Unnamed rows are unsupported.
        return event.kind == QuestEvent::Kind::zone_enter && !def.str2.empty() && event.zone == def.str2 &&
               (event.level_row < 0 || event.level_row == def.oid1);
    case QuestObjectiveTypeV1::talk_to_npc:
        return event.kind == QuestEvent::Kind::talk_to_npc && event.object_id == def.oid1 &&
               (event.secondary_id < 0 || event.secondary_id == def.oid2);
    case QuestObjectiveTypeV1::trigger_plate:
    case QuestObjectiveTypeV1::destroy_game_object:
    case QuestObjectiveTypeV1::open_game_object:
    case QuestObjectiveTypeV1::trigger_on:
    case QuestObjectiveTypeV1::picked_up_liftable:
        return event.kind == QuestEvent::Kind::object_interact && event.object_id == def.oid1;
    default:
        return false;
    }
}

bool QuestRuntimeV1::apply_to(const Objective& def, QuestObjectiveProgressV1& slot, const QuestEvent& event) {
    if (slot.completed || !objective_matches(def, event)) return false;
    if (!objective_has_quantity(def.type)) {
        slot.completed = true;
        return true;
    }
    slot.quantity += 1;
    if (slot.quantity >= std::max(def.value, 1)) slot.completed = true;
    return true;
}

bool QuestRuntimeV1::complete_automatic(const Objective& def, QuestObjectiveProgressV1& slot) {
    if (slot.completed || def.type != static_cast<std::int32_t>(QuestObjectiveTypeV1::automatic)) return false;
    slot.completed = true;
    return true;
}

bool QuestRuntimeV1::grant_rewards(std::int32_t row, std::string& error) {
    const auto d = difficulty();
    const auto& def = table_->rows()[std::size_t(row)];
    // Source RewardList::Give stops at the first failure. XP is applied first so a
    // failed XP owner leaves gold untouched and the reward is retried whole.
    for (const auto& reward : def.rewards[std::size_t(d)]) {
        if (reward.type != static_cast<std::int32_t>(QuestRewardTypeV1::xp)) continue;
        if (!services_.give_experience) {
            report_once("Quest reward XP was not granted (no experience owner bound) row " + std::to_string(row));
            continue;
        }
        if (!services_.give_experience(reward.parameter1, error)) return false;
    }
    for (const auto& reward : def.rewards[std::size_t(d)]) {
        switch (static_cast<QuestRewardTypeV1>(reward.type)) {
        case QuestRewardTypeV1::xp:
            break;
        case QuestRewardTypeV1::gold:
            character_.gold += std::uint64_t(std::max(reward.parameter1, 0));
            break;
        default:
            report_once("Quest reward type " + std::to_string(reward.type) +
                        " is not granted by the thin runtime (row " + std::to_string(row) + ")");
            break;
        }
    }
    return true;
}

bool QuestRuntimeV1::update(std::string& error) {
    const auto d = difficulty();
    if (d < 0 || d >= kDifficultyCount) { error = "Quest runtime current difficulty is outside 0..2"; return false; }
    // Each pass applies at most one transition per row, as Quest::Update does per call.
    const std::size_t limit = table_->rows().size() * std::size_t(kStateCount) + 1;
    for (std::size_t pass = 0; pass < limit; ++pass) {
        bool changed = false;
        for (std::int32_t row = 0; row < std::int32_t(table_->rows().size()); ++row) {
            QuestStateV1 state{};
            if (!state_of(row, state)) { error = "Quest state is unavailable for a table row"; return false; }
            const auto& def = table_->rows()[std::size_t(row)];
            auto& progress = row_progress(d, row);
            auto advance = [&](QuestStateV1 next) {
                if (!set_state(row, next, error)) return false;
                changed = true;
                return true;
            };
            bool met = false;
            switch (state) {
            case QuestStateV1::locked:
                if (!prerequisites_met(row, met)) return false;
                if (met && !advance(QuestStateV1::post_locked)) return false;
                break;
            case QuestStateV1::post_locked:
                if (!slot_script_running(row, 9) && !advance(QuestStateV1::pre_available)) return false;
                break;
            case QuestStateV1::pre_available:
                if (!slot_script_running(row, 11) && !advance(QuestStateV1::available)) return false;
                break;
            case QuestStateV1::available:
                if (!prerequisites_met(row, met)) return false;
                if (!met) { if (!advance(QuestStateV1::locked)) return false; break; }
                complete_automatic(def.accept, progress.accept);
                if (!objective_supported(def.accept) && !progress.accept.completed)
                    report_unsupported(def.accept, row, int(kAcceptObjectiveV1));
                if (progress.accept.completed && !slot_script_running(row, 1) && !advance(QuestStateV1::post_available)) return false;
                break;
            case QuestStateV1::post_available:
                if (!slot_script_running(row, 6) && !advance(QuestStateV1::pre_active)) return false;
                break;
            case QuestStateV1::pre_active:
                if (!slot_script_running(row, 10) && !advance(QuestStateV1::active)) return false;
                break;
            case QuestStateV1::active: {
                bool all = true;
                for (std::size_t i = 0; i < def.objectives.size(); ++i) {
                    complete_automatic(def.objectives[i], progress.objectives[i]);
                    if (!objective_supported(def.objectives[i]) && !progress.objectives[i].completed)
                        report_unsupported(def.objectives[i], row, std::int32_t(i));
                    all = all && progress.objectives[i].completed;
                }
                if (all && !slot_script_running(row, 0) && !advance(QuestStateV1::post_active)) return false;
                break;
            }
            case QuestStateV1::post_active:
                if (!slot_script_running(row, 8) && !advance(QuestStateV1::pre_completed)) return false;
                break;
            case QuestStateV1::pre_completed:
                if (!slot_script_running(row, 13) && !advance(QuestStateV1::completed)) return false;
                break;
            case QuestStateV1::completed:
                complete_automatic(def.end, progress.end);
                if (progress.end.completed && !slot_script_running(row, 3)) {
                    // UpdateCompleted: rewards once, then PostCompleted. A failed owner
                    // keeps the state so the next update retries the whole reward list.
                    if (!grant_rewards(row, error)) return false;
                    if (!slot_script_running(row, 8) && !advance(QuestStateV1::post_completed)) return false;
                }
                break;
            case QuestStateV1::post_completed:
                if (!advance(QuestStateV1::pre_closed)) return false;
                break;
            case QuestStateV1::pre_closed:
                if (!advance(QuestStateV1::closed)) return false;
                break;
            case QuestStateV1::closed:
                if (!advance(QuestStateV1::post_closed)) return false;
                break;
            case QuestStateV1::post_closed:
                break;
            }
        }
        if (!changed) { error.clear(); return true; }
    }
    error = "Quest transitions did not settle";
    return false;
}

bool QuestRuntimeV1::accept_quest(std::int32_t row, std::string& error) {
    if (!row_valid(row)) { error = "accept_quest: row is outside the table"; return false; }
    QuestStateV1 state{};
    if (!state_of(row, state)) { error = "accept_quest: quest state is unavailable"; return false; }
    if (state != QuestStateV1::available) { error = "accept_quest: quest is not Available"; return false; }
    row_progress(difficulty(), row).accept.completed = true;
    return update(error) && write_counters(error);
}

bool QuestRuntimeV1::make_active(std::int32_t row, std::string& error) {
    if (!row_valid(row)) { error = "make_active: row is outside the table"; return false; }
    CharacterQuestLogPolicyV1 policy;
    policy.debug_priority = kQuestPriorityDebugV1;
    return progress_.activate(character_, *table_, CharacterQuestIdV1{0, difficulty(), row},
                              policy, error);
}

std::size_t QuestRuntimeV1::handle(const QuestEvent& event, std::string& error) {
    const auto d = difficulty();
    std::size_t applied = 0;
    for (std::int32_t row = 0; row < std::int32_t(table_->rows().size()); ++row) {
        QuestStateV1 state{};
        if (!state_of(row, state)) continue;
        const auto& def = table_->rows()[std::size_t(row)];
        auto& progress = row_progress(d, row);
        if (state == QuestStateV1::available && apply_to(def.accept, progress.accept, event)) ++applied;
        if (state == QuestStateV1::active) {
            for (std::size_t i = 0; i < def.objectives.size(); ++i) {
                const auto before = progress.objectives[i].quantity;
                if (!apply_to(def.objectives[i], progress.objectives[i], event)) continue;
                ++applied;
                if (!progress.objectives[i].completed && progress.objectives[i].quantity != before)
                    push_objective_update(row, d, i);
            }
        }
        if ((state == QuestStateV1::post_active || state == QuestStateV1::pre_completed ||
             state == QuestStateV1::completed) && apply_to(def.end, progress.end, event))
            ++applied;
    }
    if (!update(error)) return applied;
    return applied;
}

void QuestRuntimeV1::push_objective_update(std::int32_t row, std::int32_t difficulty, std::size_t objective) {
    const auto& def = table_->rows()[std::size_t(row)];
    const auto& authored = def.objectives[objective];
    const auto& slot = rows_.at(RowKey{difficulty, row}).objectives[objective];
    QuestBannerV1 banner;
    banner.kind = QuestBannerV1::Kind::updated;
    banner.row = row;
    banner.difficulty = difficulty;
    banner.objective = std::int32_t(objective);
    banner.quantity = slot.quantity;
    banner.required = std::max(authored.value, 1);
    banner.objective_text_id = row_objective_text_v1(def);
    if (banner.objective_text_id >= 0 && services_.text) {
        std::string text;
        if (services_.text(banner.objective_text_id, text)) banner.text = text;
    }
    banners_.push_back(std::move(banner));
}

std::vector<QuestBannerV1> QuestRuntimeV1::take_banners() {
    std::vector<QuestBannerV1> out;
    out.swap(banners_);
    return out;
}

} // namespace dh::foundation::quest_runtime
