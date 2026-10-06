#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace dh2::loader {
struct XmlElementV1 {
    std::string tag;
    std::vector<std::pair<std::string,std::string>> attributes;
    std::vector<std::uint32_t> children;
    std::string text; // Parser whitespace/entity semantics; raw bytes remain owned.
    // Needed by original callers which traverse unfiltered NextSibling nodes.
    // The element tree alone must not hide a comment/text node from such callers.
    bool next_sibling_is_non_element{};
    bool first_child_is_non_element{};
    std::int32_t parent{-1}, row{}, column{};
    const std::string* attribute(const std::string&) const noexcept;
};
struct XmlDiagnosticV1 {
    std::int32_t code{}, row{}, column{};
    std::string message;
    // Diagnostic codes belong to this candidate parser, not the original ELF ABI.
};
struct XmlTopLevelNodeV1 {
    // TinyXML NodeType: element=1, comment=2, unknown=3, text=4, declaration=5.
    // Level::LoadFile iterates these unfiltered before selecting child elements.
    std::uint32_t kind{},element{UINT32_MAX};
    std::string value;
};
class XmlDocumentV1 {
    struct Snapshot;
    std::shared_ptr<const Snapshot> snapshot_;
    bool capture_impl(std::string,std::vector<std::uint8_t>,bool level_buffer,std::string&);
public:
    class Borrow {
        friend class XmlDocumentV1;
        std::shared_ptr<const Snapshot> snapshot_;
        explicit Borrow(std::shared_ptr<const Snapshot> p):snapshot_(std::move(p)){}
    public:
        Borrow()=default;
        explicit operator bool()const noexcept{return bool(snapshot_);}
        const std::string& uri() const;
        const std::vector<std::uint8_t>& source() const;
        const std::vector<XmlElementV1>& elements() const;
        const std::vector<std::uint32_t>& roots() const;
        const std::vector<XmlTopLevelNodeV1>& top_level_nodes() const;
        bool used_level_buffer_route() const;
        const XmlDiagnosticV1& diagnostic() const;
        bool parsed() const;
    };
    // Captures parser error/partial tree as evidence, not permission to instantiate
    // a malformed level. Synchronous publication is all-or-nothing; the owning
    // facade must not be mutated concurrently with borrow(). A successful capture
    // may still have parsed()==false, requiring caller error-policy reconstruction.
    bool capture(std::string uri,std::vector<std::uint8_t> bytes,std::string& error);
    // Original TiXmlDocument::LoadFromBuffer converts CR and CRLF to LF before
    // parsing. Raw source bytes stay retained; direct capture stays unchanged.
    bool capture_level_buffer(std::string uri,std::vector<std::uint8_t> bytes,std::string& error);
    Borrow borrow()const{return Borrow(snapshot_);}
};
}
