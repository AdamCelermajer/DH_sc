#include "glitch-core-io/io/xml_writer.hpp"

#include <cwchar>
#include <vector>

namespace glitch::io {

namespace {

// Writes each code unit as a 4-byte little-endian word (the original's wchar_t is 4 bytes).
void writeWords(IWriteFile* file, const wchar_t* text, std::size_t length) {
    std::vector<std::uint32_t> words(length);
    for (std::size_t i = 0; i < length; ++i) words[i] = static_cast<std::uint32_t>(text[i]);
    if (!words.empty()) file->write(words.data(), static_cast<unsigned int>(words.size() * 4));
}

void writeWords(IWriteFile* file, const wchar_t* text) {
    writeWords(file, text, std::wcslen(text));
}

}  // namespace

// 0x5763a4 _ZN6glitch2io10CXMLWriterC2EPNS0_10IWriteFileE (C1 at 0x5763f4 is the same body)
CXMLWriter::CXMLWriter(IWriteFile* writeFile) : file(writeFile) {
    if (file) ++file->referenceCounter;
}

// 0x576444 _ZN6glitch2io10CXMLWriterD2Ev (D1 at 0x576484 is the same body)
CXMLWriter::~CXMLWriter() {
    if (file) file->drop();
}

// 0x576b00 _ZN6glitch2io10CXMLWriter14writeXMLHeaderEv
void CXMLWriter::writeXMLHeader() {
    if (!file) return;
    std::uint32_t byteOrderMark = 0xfeff;
    file->write(&byteOrderMark, 4);
    writeWords(file, L"<?xml version=\"1.0\"?>");
    writeLineBreak();
    flag = false;
}

// 0x5764c4 _ZN6glitch2io10CXMLWriter12writeCommentEPKw
void CXMLWriter::writeComment(const wchar_t* text) {
    if (!file || !text) return;
    writeWords(file, L"<!--");
    writeText(text);
    writeWords(file, L"-->");
}

// 0x576590 _ZN6glitch2io10CXMLWriter15writeClosingTagEPKw
void CXMLWriter::writeClosingTag(const wchar_t* name) {
    if (!file || !name) return;
    --indent;
    if (indent > 0 && !flag) {
        for (int i = 0; i < indent; ++i) writeWords(file, L"\t");
    }
    writeWords(file, L"</");
    writeWords(file, name);
    writeWords(file, L">");
    file->flush();
    flag = false;
}

// 0x576bb8 _ZN6glitch2io10CXMLWriter9writeTextEPKw
// PARTIAL: the original replaces the characters listed in the XML escape table (and '&')
// before writing. That table is not read here, so the text is written as it is. The flag
// is set afterwards, as in the original.
void CXMLWriter::writeText(const wchar_t* text) {
    if (!file || !text) return;
    writeWords(file, text);
    flag = true;
}

// 0x57653c _ZN6glitch2io10CXMLWriter14writeLineBreakEv
void CXMLWriter::writeLineBreak() {
    if (file) writeWords(file, L"\n");
}

// 0x576694 _ZN6glitch2io10CXMLWriter14writeAttributeEPKwS3_
void CXMLWriter::writeAttribute(const wchar_t* name, const wchar_t* value) {
    if (!file || !name || !value) return;  // the original has no null check on the file
    writeWords(file, L" ");
    writeWords(file, name);
    writeWords(file, L"=\"");
    writeText(value);
    writeWords(file, L"\"");
}

}  // namespace glitch::io
