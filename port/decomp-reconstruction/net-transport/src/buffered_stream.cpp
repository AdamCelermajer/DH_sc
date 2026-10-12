#include "net-transport/buffered_stream.hpp"

// 0x42b1e4 _ZN14BufferedStream10FillBufferEP11IStreamBase
// Reads the whole source into a new array. Deviation: a read that returns no bytes
// ends the loop (the original would spin); the buffer is then short. Partial.
void BufferedStream::FillBuffer(IStreamBase* source) {
    size_ = static_cast<int>(source->size());
    if (size_ > 0) {
        data_.reset(new char[size_]);
    }
    int filled = 0;
    while (size_ != 0 && filled != size_) {
        int got = static_cast<int>(source->read(data_.get() + filled, static_cast<std::uint64_t>(size_ - filled)));
        if (got <= 0) {
            break;
        }
        filled += got;
    }
}
