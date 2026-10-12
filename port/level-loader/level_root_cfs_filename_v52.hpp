#pragma once
#include "original_filesystem_fields_v50.hpp"
#include <map>
#include <string>
namespace dh2::loader {
// SAME original initialized CFS ObfuscationMap, not inferred ZIP-name aliases.
using OriginalCfsFilenameMapV52=std::map<std::string,std::string>;
// Original56dc40 filename projection only, ending before fopen/physical I/O.
bool original_cfs_open_filename_v52(const std::string& source_filename,
 const std::string& actual_working_directory,const OriginalCfsFilenameMapV52&,
 std::string& physical_filename,bool& map_applied,std::string& error);
// Exact FileHandle source buffer: root+name, .savegame selects RES_PATH+name,
// original Application.byteA8 controls ASCII lowercase. No ApplyFilenameHacks.
bool original_root_resource_join_v52(const std::string& root,const std::string& name,
 const std::string& actual_RES_PATH,bool application_lowercase_a8,
 std::string& out,std::string& error);
} // namespace dh2::loader
