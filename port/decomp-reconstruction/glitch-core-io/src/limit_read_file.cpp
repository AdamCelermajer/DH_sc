#include "glitch-core-io/io/limit_read_file.hpp"

namespace glitch::io {

// 0x6b49c4 _ZN6glitch2io14CLimitReadFileC1EPNS0_9IReadFileElPKc
// 0x6b4aa8 _ZN6glitch2io14CLimitReadFileC2EPNS0_9IReadFileElPKc (same body)
// The wrapped file is replaced by its clone (the constructor calls the clone slot).
CLimitReadFile::CLimitReadFile(IReadFile* base, long limit, const char* fileName)
    : name(fileName ? fileName : ""), size(limit), file(base) {
    if (file) file = file->clone();
    init(nullptr);
}

// 0x6b4810 _ZN6glitch2io14CLimitReadFileC1EPNS0_9IReadFileElPKcS5_
// 0x6b4918 _ZN6glitch2io14CLimitReadFileC2EPNS0_9IReadFileElPKcS5_ (same body)
CLimitReadFile::CLimitReadFile(IReadFile* base, long limit, const char* fileName, const char* name2)
    : name(fileName ? fileName : ""), size(limit), file(base) {
    if (file) file = file->clone();
    init(name2);
}

// 0x6b4740 _ZN6glitch2io14CLimitReadFile4initEPKc
// PARTIAL: the pseudocode tail-calls strlen on the second name, and the use of that name
// is not recovered, so it is ignored here. The rest matches: the window starts at the
// wrapped file's position, the position is set to the start, and the full name is the
// wrapped file's full path, a slash and the name.
void CLimitReadFile::init(const char* name2) {
    (void)name2;
    if (!file) return;
    start = file->getPos();
    end = start + size;
    file->seek(start, false);
    position = start;
    fullName = file->getFullPath();
    fullName += "/";
    fullName += name;
}

// 0x6b48bc _ZNK6glitch2io14CLimitReadFile5cloneEv
// The copy is built over this reader's wrapped file (cloned again by its constructor),
// then takes the window and position of this reader. The second-name argument is passed
// as null (see init).
IReadFile* CLimitReadFile::clone() const {
    auto* copy = new CLimitReadFile(file, size, name.c_str(), nullptr);
    copy->end = end;
    copy->start = start;
    copy->position = position;
    return copy;
}

// 0x6b4bcc _ZN6glitch2io14CLimitReadFileD1Ev (D2 at 0x6b4b54 is the same body)
CLimitReadFile::~CLimitReadFile() {
    if (file) file->drop();
}

// 0x6b4a70 _ZN6glitch2io19createLimitReadFileEPKcPNS0_9IReadFileEl
IReadFile* createLimitReadFile(const char* name, IReadFile* file, long size) {
    return new CLimitReadFile(file, size, name);
}

}  // namespace glitch::io
