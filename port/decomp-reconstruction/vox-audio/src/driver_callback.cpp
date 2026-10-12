// vox::DriverCallbackSourceInterface: state, ring-buffer and byte-offset members.
// Constructors, Set3D*, UploadData, FillBuffer* and the GetWork/Distance helpers are not
// ported yet. Functions whose float helpers are not identified are marked partial.
#include "vox-audio/vox/driver_callback.hpp"

namespace vox {

std::int32_t DriverCallbackSourceInterface::s_driverSampleRate_ = 0;

// 0x88ed68 _ZN3vox29DriverCallbackSourceInterface20AllowBufferReferenceEv
int DriverCallbackSourceInterface::AllowBufferReference() {
    return 1;
}

// 0x88ed70 _ZN3vox29DriverCallbackSourceInterface14GetBufferCountEv
int DriverCallbackSourceInterface::GetBufferCount() const {
    return buffer_count_;
}

// 0x88fce4 _ZN3vox29DriverCallbackSourceInterface6UpdateEf (partial)
// The pseudocode is a tail call to __aeabi_fmul(dt, 16384.0f), so the body multiplies
// by 2^14 in the original. Whether that product is the function's result or is
// dropped is not confirmed, so the port only scales the argument and discards it.
void DriverCallbackSourceInterface::Update(float dt) {
    (void)(dt * 16384.0f);
}

// 0x890148 _ZN3vox29DriverCallbackSourceInterface13FreeAllBufferEv
void DriverCallbackSourceInterface::FreeAllBuffer() {
    for (std::int32_t i = 0; i < buffer_count_; ++i) {
        buffers_begin_[i].ready_ = 1;
        write_index_ = 0;
        read_index_ = 0;
    }
}

// 0x890288 _ZN3vox29DriverCallbackSourceInterface19SetDriverSampleRateEi
void DriverCallbackSourceInterface::SetDriverSampleRate(int rate) {
    s_driverSampleRate_ = rate;
}

// 0x890fd0 _ZN3vox29DriverCallbackSourceInterface4PlayEv
void DriverCallbackSourceInterface::Play() {
    mutex_.Lock();
    if (state_ != -1) {
        state_ = 1;
    }
    mutex_.Unlock();
}

// 0x890f48 _ZN3vox29DriverCallbackSourceInterface5PauseEv
void DriverCallbackSourceInterface::Pause() {
    mutex_.Lock();
    if (state_ == 1) {
        field_2c_ = 0;
        state_ = 2;
        field_30_ = 0;
    }
    mutex_.Unlock();
}

// 0x890f84 _ZN3vox29DriverCallbackSourceInterface4StopEv
void DriverCallbackSourceInterface::Stop() {
    mutex_.Lock();
    if (state_ != -1) {
        state_ = 3;
        FreeAllBuffer();
        field_2c_ = 0;
        byte_offset_ = 0;
        field_30_ = 0;
    }
    mutex_.Unlock();
}

// 0x890ee8 _ZN3vox29DriverCallbackSourceInterface5ResetEv
// The ARM divides (end - begin) by the 0x18 buffer stride with a multiply-by-inverse;
// the test is simply "there is at least one buffer".
void DriverCallbackSourceInterface::Reset() {
    mutex_.Lock();
    if (buffers_end_ != buffers_begin_) {
        state_ = 0;
        FreeAllBuffer();
        byte_offset_ = 0;
    }
    mutex_.Unlock();
}

// 0x890e8c _ZN3vox29DriverCallbackSourceInterface8GetStateEv
int DriverCallbackSourceInterface::GetState() {
    mutex_.Lock();
    int result = state_;
    // The original reads the write-index buffer without a count check; the port skips
    // the read when there is no buffer array at all, which avoids a null dereference.
    if (state_ == 1 && buffers_begin_ != nullptr && buffers_begin_[write_index_].ready_ != 0) {
        result = 3;
    }
    mutex_.Unlock();
    return result;
}

// 0x890e0c _ZN3vox29DriverCallbackSourceInterface8NeedDataEv
int DriverCallbackSourceInterface::NeedData() {
    mutex_.Lock();
    int result = 0;
    if (state_ != -1 && buffers_end_ != buffers_begin_) {
        result = buffers_begin_[read_index_].ready_;
    }
    mutex_.Unlock();
    return result;
}

// 0x890b28 _ZN3vox29DriverCallbackSourceInterface13SetByteOffsetEi
void DriverCallbackSourceInterface::SetByteOffset(int offset) {
    mutex_.Lock();
    byte_offset_ = offset;
    mutex_.Unlock();
}

// 0x890b50 _ZN3vox29DriverCallbackSourceInterface13GetByteOffsetEv
int DriverCallbackSourceInterface::GetByteOffset() {
    mutex_.Lock();
    int offset = byte_offset_;
    mutex_.Unlock();
    return offset;
}

// 0x890af8 _ZN3vox29DriverCallbackSourceInterface7CleanupEv
void DriverCallbackSourceInterface::Cleanup() {
    mutex_.Lock();
    if (buffers_begin_ != buffers_end_) {
        buffers_end_ = buffers_begin_;
    }
    mutex_.Unlock();
}

// 0x890b78 _ZN3vox29DriverCallbackSourceInterface8GetPitchEv (partial)
// ARM: int-to-float of the word at +0x34, then a multiply by 2^-15 (helpers 0x30e964
// and 0x30ed6c, not identified by name).
float DriverCallbackSourceInterface::GetPitch() {
    mutex_.Lock();
    float pitch = static_cast<float>(field_34_) * (1.0f / 32768.0f);
    mutex_.Unlock();
    return pitch;
}

// 0x890bb0 _ZN3vox29DriverCallbackSourceInterface7GetGainEv (partial)
// Same helper shape as GetPitch, on the word at +0x28.
float DriverCallbackSourceInterface::GetGain() {
    mutex_.Lock();
    float gain = static_cast<float>(field_28_) * (1.0f / 32768.0f);
    mutex_.Unlock();
    return gain;
}

}  // namespace vox
