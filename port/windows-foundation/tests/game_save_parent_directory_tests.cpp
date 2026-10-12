// B153: ensure_parent_directory must accept a bare relative save name (empty parent = current directory), create missing
// parent directories, and report an explicit error when a parent path is blocked by a regular file.
#include "../game_save.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
} // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply a writable scratch directory");
        const fs::path root = fs::path(argv[1]) / "parent-directory-tests";
        std::error_code ignored;
        fs::remove_all(root, ignored);
        fs::create_directories(root);

        std::string error = "unset";
        // Direct-launch default: "character.save" in the working directory has no parent to create.
        check(dh::foundation::ensure_parent_directory("character.save", error), "bare relative name refused: " + error);
        check(error.empty(), "bare relative name left an error text");

        // A missing nested parent is created and the file itself is not created here.
        const fs::path nested = root / "a" / "b" / "character.save";
        check(dh::foundation::ensure_parent_directory(nested, error), "nested parent not created: " + error);
        check(fs::is_directory(root / "a" / "b"), "nested parent directory is missing on disk");
        check(!fs::exists(nested), "helper must not create the save file");

        // An already-existing parent is accepted again (the checkpoint retries every frame until it saves).
        check(dh::foundation::ensure_parent_directory(nested, error), "existing parent refused on retry: " + error);

        // A parent path blocked by a regular file fails with a message naming the directory.
        const fs::path blocker = root / "blocked";
        { std::ofstream file(blocker); file << "not a directory"; }
        check(!dh::foundation::ensure_parent_directory(blocker / "character.save", error), "blocked parent was accepted");
        check(error.find("Cannot create directory") != std::string::npos, "blocked parent error is not explicit: " + error);

        std::cout << "game_save parent directory tests passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "game_save parent directory test failed: " << e.what() << '\n';
        return 1;
    }
}
