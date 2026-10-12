// Batch e1-glitch-core-io-glitch_io_CLimitReadFile-0: the IReadFile slots of CLimitReadFile.
// Offsets were checked against the ARM listing (decomp_asm.py). Members are the ones declared in
// glitch-core-io/io/limit_read_file.hpp: start is +0x40, end is +0x44, file is +0x48,
// position is +0x4c, size is +0x3c.
#include "glitch-core-io/io/limit_read_file.hpp"

namespace glitch::io {

// 0x6b44e8 _ZN6glitch2io14CLimitReadFile4readEPvj
// PARTIAL: the position update is missing. The original adds the byte count returned by the
// wrapped file's read slot (+0x0c) to position. IReadFile::read is void in the port, so that
// count is not available, and position is left unchanged after the read. The rest matches:
// the wrapped position is synced to position first (seek with relative = false), reads at or
// past end return, and the count is clamped to end.
void CLimitReadFile::read(void* buffer, unsigned int count) {
    long pos = file->getPos();
    if (pos != position) {
        file->seek(position, false);
        pos = position;
    }
    if (pos >= end) return;
    if (end <= pos + static_cast<long>(count)) {
        count = static_cast<unsigned int>(end - pos);
    }
    file->read(buffer, count);
    // MISSING: position += bytes read (see PARTIAL note above).
}

// 0x6b4580 _ZN6glitch2io14CLimitReadFile9readAsyncEPvjPFviiPNS0_9IReadFileES2_ES2_
// The original returns the result of the position overload. The port's slot +0x10 is void
// (same as CReadFile::readAsync), so the bool is dropped here.
void CLimitReadFile::readAsync(void* buffer, unsigned int count, ReadAsyncCallBack callBack, void* context) {
    readAsync(buffer, count, getPos(), callBack, context);
}

// 0x6b45d0 _ZN6glitch2io14CLimitReadFile9readAsyncEPvjlPFviiPNS0_9IReadFileES2_ES2_
// The argument is relative to the window start. The position is stored before the range
// check, so a read at or past end still leaves position at the window-relative target.
bool CLimitReadFile::readAsync(void* buffer, unsigned int count, long pos, ReadAsyncCallBack callBack,
                               void* context) {
    const long absolute = pos + start;
    position = absolute;
    if (absolute >= end) return false;
    unsigned int n = count;
    if (end <= absolute + static_cast<long>(count)) {
        n = static_cast<unsigned int>(end - absolute);
    }
    const bool result = file->readAsync(buffer, n, absolute, callBack, context);
    position += static_cast<long>(n);
    return result;
}

// 0x6b4644 _ZN6glitch2io14CLimitReadFile4seekElb
// Both branches end in file->seek(target, relative) on the wrapped file.
//   relative: target = offset - position + filePos; clamped to end - filePos when
//             offset + filePos > end; position = target + filePos.
//   absolute: target = offset - position + filePos + start; taken only when target <= end
//             (then position = target), otherwise returns false without seeking.
bool CLimitReadFile::seek(long offset, bool relative) {
    const long filePos = file->getPos();
    long target = offset - position + filePos;
    if (relative) {
        if (offset + filePos > end) {
            target = end - filePos;
        }
        position = target + filePos;
        return file->seek(target, relative);
    }
    target += start;
    if (target <= end) {
        position = target;
        return file->seek(target, relative);
    }
    return false;
}

// 0x6b46d0 _ZNK6glitch2io14CLimitReadFile7getSizeEv
long CLimitReadFile::getSize() const {
    return size;
}

// 0x6b46d8 _ZNK6glitch2io14CLimitReadFile6getPosEv
long CLimitReadFile::getPos() const {
    return position - start;
}

// 0x6b46e8 _ZNK6glitch2io14CLimitReadFile11getFileNameEv
// The original returns the character data of the name string (+0x20). The port returns the
// std::string itself, as CReadFile::getFileName does.
const std::string& CLimitReadFile::getFileName() const {
    return name;
}

// 0x6b46f0 _ZNK6glitch2io14CLimitReadFile11getFullPathEv
// The original returns the character data of the full name string (+0x38). See getFileName.
const std::string& CLimitReadFile::getFullPath() const {
    return fullName;
}

}  // namespace glitch::io
