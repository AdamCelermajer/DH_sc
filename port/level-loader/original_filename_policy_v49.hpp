#pragma once
#include <functional>
#include <memory>
#include <string>
#include <array>
namespace dh2::loader {
struct OriginalFilenameResultV49 {
 std::string input,working_directory,filename;
 bool working_directory_matched{},texture_basename_relocated{};
};
// Exact source projection of 34e96c; working-directory is an actual FS virtual
// producer, not q:/iphone or GPU policy. Embedded NUL/undefined end-match reject.
bool original_filename_policy_v49(const std::string& working_directory,
 const std::string& input,OriginalFilenameResultV49&,std::string& error);
struct OriginalFilesystemBorrowV49 {
 std::shared_ptr<void> actual_filesystem_owner;
 std::function<bool(std::string& working_directory,std::string& error)> borrow_working_directory;
};
// Call synchronously on the FS owner's runtime thread; working-directory must
// remain unchanged throughout the borrow. No detached current-directory cache.
bool borrowed_original_filename_policy_v49(const OriginalFilesystemBorrowV49&,
 const std::string& input,OriginalFilenameResultV49&,std::string& error);
// CResFactory6594a0 order. Preserve duplicates and exact authored path. Caller
// must try second only if actual first open fails; no format variant added here.
bool original_image_search_candidates_v49(const std::string& resource_directory,
 const std::string& full_authored_image_path,std::array<std::string,2>&,
 std::string& error);
} // namespace dh2::loader
