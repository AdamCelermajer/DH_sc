#pragma once
#include <array>
#include <memory>
#include <string>
namespace dh2::loader {
// One shared source static global per engine/Application boot, not per actor/cache.
// Initial original ELF data at99b138 is 1024 zero bytes. Never reset on GL restore.
struct OriginalWorkingDirectoryV50 {
 std::array<char,1024> source_buffer{};
 std::string value()const{return source_buffer.data();}
 // Genuine host chdir has already run on the runtime thread. Its result0 copies
 // the source path; failure retains all bytes. This method never invokes chdir.
 bool observe_chdir_result(const std::string&,int actual_host_result,
                          bool& original_return,std::string& error);
};
// One reconstructed source static global for this engine/process. Main borrows
// this SAME owner across cache copies, World changes and GL recreation.
std::shared_ptr<OriginalWorkingDirectoryV50> source_filesystem_globals_v50();
struct OriginalFileSystemPathsV50 {
 std::array<char,260> root2c{},copy130{},resources234{},trace338{},save43c{};
};
// Whole original C1 buffer/init projection. RES_PATH is a source platform global,
// initially the original empty literal; subsequent Android delivery is host input.
// It is not current main WorldScriptContext::files_directory/private save root.
bool construct_original_filesystem_paths_v50(const std::string& actual_RES_PATH,
 OriginalFileSystemPathsV50&,std::string& error);
} // namespace dh2::loader
