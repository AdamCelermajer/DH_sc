#pragma once
#include "xml_document_v1.hpp"
namespace dh2::loader {
enum class LevelFileWalkStepV1 { pending, complete, failed };
struct LevelFileWalkV1 {
    XmlDocumentV1::Borrow document;
    std::string requested_root,error;
    std::uint32_t root_cursor{},child_cursor{};
    bool started{},advance_root{},completed{},failed{},released{};
};
class LevelFileWalkServicesV1 {
public:
    virtual ~LevelFileWalkServicesV1()=default;
    // Bool means execution succeeded, never object activation/condition state.
    // This is an internal candidate, not an agreed main/menu ABI.
    virtual bool parse_result(bool success,std::string& error)=0;
    virtual bool load_element(const XmlDocumentV1::Borrow&,std::uint32_t element,std::string& error)=0;
    virtual bool release_load_state(std::string& error)=0;
};
// Source preparation has already captured an available raw buffer through
// capture_level_buffer. Initial resource open/async stream ownership is outside
// this kernel. Failed preparation leaves out unchanged.
bool prepare_level_file_walk_v1(XmlDocumentV1::Borrow,std::string requested_root,
    LevelFileWalkV1& out,std::string& error);
// Matches the source's ready-buffer traversal through first completion. A
// parser error or unsafe non-element root match is a distinct adapter failure,
// not a ready level. Failures latch without replay; caller explicitly discards
// retained diagnostic state. Completion is stable under repeated polls.
LevelFileWalkStepV1 step_level_file_walk_v1(LevelFileWalkV1&,LevelFileWalkServicesV1&);
bool discard_level_file_walk_v1(LevelFileWalkV1&,LevelFileWalkServicesV1&,std::string& error);
}
