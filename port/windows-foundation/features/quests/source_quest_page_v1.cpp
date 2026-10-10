#include "source_quest_page_v1.hpp"

namespace dh::foundation {

SourceQuestPageV1::SourceQuestPageV1(
    std::shared_ptr<SourceQuestServiceBinding> binding,
    QuestLogFunctorV108 category, QuestLogTextV108 text)
    : binding_(std::move(binding)), category_(std::move(category)),
      text_(std::move(text)) {}

bool SourceQuestPageV1::contains(
    const std::vector<SourceQuestPageItemV1>& rows,
    OriginalQuestId id) const noexcept {
    for (const auto& row : rows)
        if (row.id.collection == id.collection &&
            row.id.difficulty == id.difficulty && row.id.row == id.row)
            return true;
    return false;
}

bool SourceQuestPageV1::refresh(bool source_multiplayer,
                                SourceQuestPageSnapshotV1& out,
                                std::string& error) const {
    if (!binding_ || !category_ || !text_) {
        error = "Quest Page requires the existing same-owner Quest Log providers";
        return false;
    }
    std::vector<QuestLogEntryV108> assigned, completed;
    if (!binding_->quest_log(QuestLogCategoryV108::active, category_, text_,
                             assigned, error))
        return false;
    if (!binding_->quest_log(QuestLogCategoryV108::closed, category_, text_,
                             completed, error))
        return false;

    SourceQuestPageSnapshotV1 staged;
    staged.multiplayer = source_multiplayer;
    staged.assigned.reserve(assigned.size());
    staged.completed.reserve(completed.size());
    for (auto& row : assigned)
        staged.assigned.push_back({row.id, std::move(row.title), row.current});
    for (auto& row : completed)
        staged.completed.push_back({row.id, std::move(row.title), row.current});
    out = std::move(staged);
    error.clear();
    return true;
}

bool SourceQuestPageV1::select(const SourceQuestPageSnapshotV1& page,
                               SourceQuestLogListV1 list,
                               OriginalQuestId id,
                               SourceQuestPageSelectionV1& out,
                               std::string& error) const {
    if (!binding_ || !text_) {
        error = "Quest Page requires the existing same-owner details provider";
        return false;
    }
    const auto& rows = list == SourceQuestLogListV1::assigned
        ? page.assigned : page.completed;
    if (!contains(rows, id)) {
        error = "Selected Quest ID is not present in the authored page list";
        return false;
    }
    QuestLogDetailsV108 details;
    if (!binding_->quest_log_details(id, text_, details, error)) return false;
    SourceQuestPageSelectionV1 staged;
    staged.id = id;
    staged.details = std::move(details);
    staged.activate_visible = list == SourceQuestLogListV1::assigned;
    out = std::move(staged);
    error.clear();
    return true;
}

bool SourceQuestPageV1::activate(const SourceQuestPageSnapshotV1& page,
                                 OriginalQuestId id,
                                 std::string& error) const {
    if (!binding_ || !category_) {
        error = "Quest Page requires the existing same-owner activation provider";
        return false;
    }
    if (!contains(page.assigned, id)) {
        error = "Only a currently Assigned Quest row exposes the source Activate action";
        return false;
    }
    return binding_->quest_log_activate(id, category_, error);
}

} // namespace dh::foundation
