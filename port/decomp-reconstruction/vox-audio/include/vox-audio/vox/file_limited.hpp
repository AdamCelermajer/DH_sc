// vox::FileLimited: a window over a file. Only the accessors recovered so far
// are declared; Read, Write, Seek and the constructors are not reconstructed.
#pragma once

namespace vox {

class FileLimited {
public:
    ~FileLimited() = default;  // 0x8939f0 and 0x893e74: empty bodies

    int Tell() const { return position_; }  // 0x893e20: +0x14

private:
    int position_ = 0;  // +0x14
};

}  // namespace vox
