#pragma once
// B068: original dialog tables used by the cutscene caption box.
//  - DialogActors (dialogs_pyarray.bin, first table): rows of {FrameID int, FrameLabel CString, Name StringID}. The speaker
//    name plate shows StringManager::getString(row.Name) (IDA DialogMsg::SetActorName 0x433dd8: row stride 20, string id at +16).
//  - DialogStyles (dialogs_pycst.bin): style value -> DialogBox frame label (the Flash dialog box variant, sprite 727).
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace dh::foundation::cinematic_runner {

struct DialogActorRow {
    std::int32_t frame_id = 0;
    std::string frame_label;
    std::int32_t name_string_id = -1;
};

// Decodes the DialogActors table from dialogs_pyarray.bin (little-endian: u32 count, then per row i32, CString, i32).
bool decode_dialog_actors(const std::vector<std::uint8_t>& bytes, std::vector<DialogActorRow>& rows, std::string& error);

// Decodes the DialogStyles constant group from dialogs_pycst.bin into value -> style name.
bool decode_dialog_styles(const std::vector<std::uint8_t>& bytes, std::map<std::int32_t, std::string>& styles, std::string& error);

} // namespace dh::foundation::cinematic_runner
