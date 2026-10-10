#pragma once

#include "source_quest_service_binding.hpp"
#include "source_quest_log_services_v1.hpp"

namespace dh::foundation {

enum class SourceQuestLogListV1 { assigned, completed };

// Typed values passed to the retained authored menu_QuestLogSheetNEW movie.
// They mirror its duplicateMovieClip row fields ID, Title and Current; page
// art and HTML rendering remain owned by the actual SWF.
struct SourceQuestPageItemV1 {
    OriginalQuestId id;
    std::string title;
    bool current{};
};
struct SourceQuestPageSnapshotV1 {
    bool multiplayer{};
    std::vector<SourceQuestPageItemV1> assigned;
    std::vector<SourceQuestPageItemV1> completed;
};
struct SourceQuestPageSelectionV1 {
    OriginalQuestId id;
    QuestLogDetailsV108 details;
    bool activate_visible{};
};

class SourceQuestPageV1 {
public:
    static constexpr const char* source_movie = "dqcharmenu_droid.swf";
    static constexpr const char* source_page = "menu_QuestLogSheetNEW";
    static constexpr std::uint32_t source_sprite = 599;
    static constexpr const char* source_sha256 =
        "43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0";
    static constexpr std::array<float, 6> source_matrix{1.0f, 0.0f, 0.0f, 1.0f, 170.0f, 2126.0f};

private:
    std::shared_ptr<SourceQuestServiceBinding> binding_;
    QuestLogFunctorV108 category_;
    QuestLogTextV108 text_;

    bool contains(const std::vector<SourceQuestPageItemV1>&,
                  OriginalQuestId) const noexcept;
public:
    SourceQuestPageV1(std::shared_ptr<SourceQuestServiceBinding>,
                      QuestLogFunctorV108, QuestLogTextV108);

    // Refresh both authored AllQuests lists from one current Character/Save
    // binding. The service's source-title sort is preserved without a second
    // ordering or a private quest copy.
    bool refresh(bool source_multiplayer, SourceQuestPageSnapshotV1&,
                 std::string& error) const;

    // Corresponds to the authored row onRelease -> NativeGetQuestDetails.
    // Completed-row selection has no activate button in the source sheet.
    bool select(const SourceQuestPageSnapshotV1&, SourceQuestLogListV1,
                OriginalQuestId, SourceQuestPageSelectionV1&,
                std::string& error) const;

    // Corresponds only to the authored btn_Activate action and delegates to
    // the same Save-backed currentquest store and native Active predicate.
    bool activate(const SourceQuestPageSnapshotV1&, OriginalQuestId,
                  std::string& error) const;
};

} // namespace dh::foundation
