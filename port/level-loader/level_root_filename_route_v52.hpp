#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>
namespace dh2::loader {
struct LevelRootFilenameServicesV52 {
 std::shared_ptr<void> actual_filesystem_owner;
 // Actual Application receiver query, once for each fresh prefixed name.
 std::function<bool(const std::string&,bool& uncompiled,std::string&)> is_using_uncompiled_data;
 // Exact reached FileSystemWin32.openResource boundary: missing is successful
 // found=false, errors are false. Actual mounted namespace returns canonical URI.
 std::function<bool(const std::string&,bool& found,std::string& canonical,std::string&)> open_resource;
};
struct LevelRootFilenameTraceV52 {std::vector<std::string> mode_queries,open_resource_queries;};
class LevelRootFilenameResolverV52 {
 LevelRootFilenameServicesV52 services_;bool busy_{};
public:
 explicit LevelRootFilenameResolverV52(LevelRootFilenameServicesV52 services):services_(std::move(services)){}
 // Only filename-open branch (Level field140==NULL). An assigned generated
 // stream bypasses this original branch; do not resolve its rule as Level XML.
 bool resolve(const std::string& original_C1_name,std::string& canonical,
              LevelRootFilenameTraceV52* trace,std::string& error);
};
// Original320678 scalar body: nonempty source cc/d0 range plus case-sensitive
// substring in [.mlx,.mgp,.mvp,.xml]. Range must be borrowed from SAME Application.
bool original_level_uses_uncompiled_v52(bool actual_range_nonempty,const std::string& name);
} // namespace dh2::loader
