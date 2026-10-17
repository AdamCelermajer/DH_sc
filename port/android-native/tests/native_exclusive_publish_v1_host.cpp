#include "../app/src/main/cpp/native_exclusive_publish_v1.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    if (argc != 2) return 2;
    const fs::path root(argv[1]);
    fs::create_directories(root);
    const auto temporary = root / "campaign.creating";
    const auto target = root / "campaign.dat";
    std::string error;

    { std::ofstream file(temporary, std::ios::binary); file << "fresh"; }
    if (!dh2::native::player_profile::publish_new_campaign_file(
            temporary, target, error)) {
        std::cerr << "fresh publish failed: " << error << '\n';
        return 1;
    }
    if (fs::exists(temporary) || !fs::exists(target)) return 1;
    { std::ifstream file(target); std::string value; file >> value;
      if (value != "fresh") return 1; }

    { std::ofstream file(temporary, std::ios::binary); file << "replacement"; }
    error.clear();
    if (dh2::native::player_profile::publish_new_campaign_file(
            temporary, target, error)) {
        std::cerr << "collision unexpectedly replaced destination\n";
        return 1;
    }
    if (error.empty() || !fs::exists(temporary)) return 1;
    { std::ifstream file(target); std::string value; file >> value;
      if (value != "fresh") return 1; }
    std::cout << "PASS: atomic publication and existing-target preservation\n";
    return 0;
}
