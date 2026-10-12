#include "level_file_walk_v1.hpp"
namespace dh2::loader {
bool prepare_level_file_walk_v1(XmlDocumentV1::Borrow document,std::string requested,
    LevelFileWalkV1& out,std::string& error) {
    error.clear();
    if(!document||!document.used_level_buffer_route()||requested.find('\0')!=std::string::npos) {
        error="File walk requires retained level-buffer XML and a non-NUL root name";return false;
    }
    LevelFileWalkV1 candidate;candidate.document=std::move(document);candidate.requested_root=std::move(requested);
    out=std::move(candidate);return true;
}
LevelFileWalkStepV1 step_level_file_walk_v1(LevelFileWalkV1& state,LevelFileWalkServicesV1& services) {
    const auto fail=[&](const char* operation) {
        state.failed=true;
        state.error=std::string(operation)+(state.error.empty()?": failed":": "+state.error);
        return LevelFileWalkStepV1::failed;
    };
    if(state.failed)return LevelFileWalkStepV1::failed;
    if(state.completed)return LevelFileWalkStepV1::complete;
    if(!state.document)return fail("No retained file walk source");
    if(!state.started) {
        state.started=true;
        const bool success=state.document.diagnostic().code==0;
        if(!services.parse_result(success,state.error))return fail("parse result service");
        if(!success) {
            state.error=state.document.uri()+": "+state.document.diagnostic().message;
            return fail("XML parse");
        }
    }else if(state.advance_root) {
        ++state.root_cursor;state.child_cursor=0;state.advance_root=false;
    }else if(state.root_cursor<state.document.top_level_nodes().size()) {
        const auto& root=state.document.top_level_nodes().at(state.root_cursor);
        const auto& children=state.document.elements().at(root.element).children;
        if(state.child_cursor<children.size()) {
            if(!services.load_element(state.document,children[state.child_cursor],state.error))return fail("Level::_LoadFromXML service");
            ++state.child_cursor;
            if(state.child_cursor==children.size())state.advance_root=true;
            return LevelFileWalkStepV1::pending;
        }
    }
    if(state.root_cursor==state.document.top_level_nodes().size()) {
        // A scan reaching the end returns false; the next poll releases state.
        if(!state.advance_root) {state.advance_root=true;return LevelFileWalkStepV1::pending;}
    }
    if(state.root_cursor>=state.document.top_level_nodes().size()) {
        if(!services.release_load_state(state.error))return fail("release load state");
        state.released=true;state.document={};state.completed=true;
        return LevelFileWalkStepV1::complete;
    }
    const auto& root=state.document.top_level_nodes().at(state.root_cursor);
    if(root.value!=state.requested_root) {state.advance_root=true;return LevelFileWalkStepV1::pending;}
    if(root.kind!=1||root.element==UINT32_MAX)return fail("Matching top-level node is not an element");
    state.advance_root=state.document.elements().at(root.element).children.empty();
    return LevelFileWalkStepV1::pending;
}
bool discard_level_file_walk_v1(LevelFileWalkV1& state,LevelFileWalkServicesV1& services,std::string& error) {
    error.clear();
    if(state.released)return true;
    if(!services.release_load_state(error))return false;
    state.released=true;state.document={};return true;
}
}
