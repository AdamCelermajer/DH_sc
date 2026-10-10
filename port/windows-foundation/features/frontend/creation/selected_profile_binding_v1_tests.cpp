#include "selected_profile_binding_v1.hpp"
#include "../../../../game-data/fresh_player_profile_v1.hpp"
#include "../../../../level-world/application_services_owner_v5.hpp"
#include "../../../../level-world/campaign_save_filename_v45.hpp"
#include "../../../../level-world/savegame_stream_v2.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace {
using namespace dh2;
using namespace dh::foundation::frontend::creation;
unsigned checks{};
void check(bool value, const std::string& what) {
    ++checks;
    if (!value) throw std::runtime_error(what);
}
std::vector<std::uint8_t> read(const std::filesystem::path& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("required test source cache " + path.string());
    return {std::istreambuf_iterator<char>(input), {}};
}
data::Bytes bytes(const std::vector<std::uint8_t>& value) {
    return {value.data(), value.size()};
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "expected actual game-data asset root");
        const std::filesystem::path assets = std::filesystem::path(argv[1]) / "data";
        std::string error;

        auto character_records = read(assets / "character_properties_pyarray.bin");
        auto character_names = read(assets / "character_properties_pyarraynames.bin");
        auto character_schema = read(assets / "character_properties_pystructnames.bin");
        data::CharacterTable characters;
        check(data::load_characters(bytes(character_records), bytes(character_names),
                                    bytes(character_schema), characters, error), error);
        data::FreshPlayerProfileV1 fresh;
        check(data::fresh_player_profile_v1(characters, "KnightPlayerBase", "Native Binding",
                                            123456, 654321, fresh, error), error);
        const auto directory = std::filesystem::temp_directory_path() /
            ("dh2-selected-profile-v1-" + std::to_string(
                std::chrono::steady_clock::now().time_since_epoch().count()));
        check(std::filesystem::create_directories(directory), "create isolated test private directory");
        auto application = std::make_shared<application::ApplicationServicesOwnerV5>();
        std::shared_ptr<application::ApplicationSaveFilesOwnerV61> files;
        check(application::ApplicationSaveFilesOwnerV61::acquire(
                  application, directory.string(), files, error), error);
        const auto filename = level::campaign_save_filename_v45(2, false, false);
        auto framed = std::make_unique<level::SavegameStreamV2>(bytes(fresh.bytes));
        check(files->jobs()->add_write(filename, std::move(framed), error), error);
        check(files->flush(filename.c_str(), error), error);

        auto save = std::make_shared<data::PlayerSavegameV1>();
        constexpr std::uintptr_t character = 0x512340;
        save->set_character(character);
        auto load = std::make_shared<data::PlayerSaveLoadOwnerV1>(save);
        // This test calls the existing same-Save source setter, then records
        // its receipt. Production callers must pass the receipt issued by the
        // actual Character SetSlot route; this test does not stand in for it.
        save->set_slot(2);
        const character::CharacterProfileSlotStoreReceiptV59 slot_receipt{save.get(), save->slot()};
        auto actual_cells = std::make_shared<int>(1);
        const auto save_identity = reinterpret_cast<std::uintptr_t>(save.get());
        const auto* direct_slot = save->source_slot_field_v122();
        std::vector<std::int32_t> level_defaults, map_defaults;
        character::CharacterProfileBootstrapInputsV59 source;
        source.save = save;
        source.load = load;
        source.character = character;
        source.actual_source_cells_lease = actual_cells;
        source.source_save14e8 = &save_identity;
        source.source_direct_save_slot_v122 = direct_slot;
        source.source_set_slot = [save](std::int32_t value, std::string&) {
            save->set_slot(value);
            return true;
        };
        source.reads.tables = actual_cells;
        source.reads.characters = &characters;
        source.reads.level_defaults28 = &level_defaults;
        source.reads.map_defaults8 = &map_defaults;

        SelectedProfileBindingV1 binding;
        check(binding.prepare(files, 2, slot_receipt, source, error), error);
        check(binding.ready() && binding.profile() && binding.profile()->ready() &&
              binding.profile()->cached(), "actual CampaignProfile C1 ready from same Application owner");
        const auto receipt = binding.file_receipt();
        check(receipt && receipt->files == files && receipt->filename == filename && receipt->slot == 2,
              "typed selected-file receipt pins the existing Application transport");
        check(receipt->bytes == fresh.bytes &&
              binding.profile()->cache().bytes() == receipt->bytes,
              "CampaignProfile cache exactly matches bytes returned by read_save");
        const auto& bootstrap_inputs = binding.bootstrap_inputs();
        check(bootstrap_inputs.save.get() == save.get() && bootstrap_inputs.load.get() == load.get() &&
              bootstrap_inputs.profile.get() == binding.profile().get() &&
              bootstrap_inputs.selected_file_lease.get() == receipt.get() &&
              bootstrap_inputs.selected_slot == 2 && bootstrap_inputs.prior_slot_store.save == save.get() &&
              bootstrap_inputs.prior_slot_store.value == 2 && !load->profile().identity,
              "same Character bootstrap input prepared without preview PlayerInfo Save or duplicate C1");
        check(binding.bootstrap_inputs().reads.characters == &characters &&
              binding.bootstrap_inputs().reads.tables == actual_cells &&
              binding.bootstrap_inputs().reads.level_defaults28 == &level_defaults &&
              binding.bootstrap_inputs().reads.map_defaults8 == &map_defaults,
              "caller-provided source read owner dependencies are preserved");
        check(!binding.prepare(files, 2, slot_receipt, source, error),
              "selected profile owner rejects replay after retained native prefix");

        // A receipt for another Save must fail before any profile or SetSlot
        // work can be attached to the Character graph.
        auto wrong_save = std::make_shared<data::PlayerSavegameV1>();
        wrong_save->set_character(character);
        wrong_save->set_slot(2);
        auto wrong_source = source;
        wrong_source.save = wrong_save;
        wrong_source.load = std::make_shared<data::PlayerSaveLoadOwnerV1>(wrong_save);
        const auto wrong_save_identity = reinterpret_cast<std::uintptr_t>(wrong_save.get());
        wrong_source.source_save14e8 = &wrong_save_identity;
        wrong_source.source_direct_save_slot_v122 = wrong_save->source_slot_field_v122();
        SelectedProfileBindingV1 rejected;
        check(!rejected.prepare(files, 2, slot_receipt, wrong_source, error) &&
              error.find("same-Character Save/SetSlot receipt") != std::string::npos,
              "cross-Save SetSlot receipt rejected before CampaignProfile construction");

        check(files->jobs()->release(error), error);
        std::filesystem::remove_all(directory);
        std::cout << "PASS selected profile V1: same Application FileManager/jobs, read receipt/cache equality, same Character SetSlot and Save, bootstrap inputs prepared; no Load1/2/4 or InitPost readiness claimed; checks="
                  << checks << '\n';
        return 0;
    } catch (const std::exception& failure) {
        std::cerr << failure.what() << '\n';
        return 1;
    }
}
