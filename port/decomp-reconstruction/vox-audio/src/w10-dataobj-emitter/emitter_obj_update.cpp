// vox::EmitterObj::Update: one frame of an emitter. It loads a pending cursor (LoadAsync),
// runs the gain and pitch fades, pushes the changed gain and pitch to the source, reads the
// source state and moves the emitter between its play states. Every path of the listing ends in
// Mutex::Unlock, so the port returns that int too.
// Partial: the listing calls the cursor and the source with no null check. The port returns early
// when either is missing. Those calls fault in the original, and an emitter whose load failed
// (state -1, ready flag still set) reaches them. The mono 3D test reads the cursor's channel count
// (cursor +0x04). The 3D and DSP helpers it calls are ported in their own files.
#include "vox-audio/vox/data_obj.hpp"
#include "vox-audio/vox/decoder.hpp"
#include "vox-audio/vox/driver.hpp"
#include "vox-audio/vox/emitter_obj.hpp"
#include "vox-audio/vox/gain_fade.hpp"

namespace vox {

namespace {

// Advances a fade's elapsed time by dt while it runs; once it has run its course the flag is
// set (the original stores 1 in the flag byte). Returns the elapsed time after the step.
float StepFade(VoxGainFade& fade, float dt) {
    if (fade.elapsed < fade.duration) {
        fade.elapsed += dt;
    } else {
        fade.flag = true;
    }
    return fade.elapsed;
}

}  // namespace

// 0x86d5b8 _ZN3vox10EmitterObj6UpdateEf
int EmitterObj::Update(float dt) {
    mutex.Lock();
    const int previous_state = state_;  // +0x90
    if (ready_flag_ != 0 && previous_state != -1) {
        if (LoadAsync() == 0) {
            return mutex.Unlock();
        }
        ready_flag_ = 0;
    }

    bool run_frame = true;
    if (reset_flag_ != 0) {  // +0x99
        reset_flag_ = 0;
        if (decoder_cursor_ != nullptr) {
            decoder_cursor_->Reset();  // cursor +0x1c
        }
        state_ = dsp_source_ != nullptr ? dsp_source_->GetState() : 0;  // source +0x18
        run_frame = state_ == 0;
    }
    if (!run_frame) {
        if (dsp_source_ != nullptr) {
            dsp_source_->Reset();  // source +0x14
        }
        return mutex.Unlock();
    }
    if (decoder_cursor_ == nullptr || dsp_source_ == nullptr) {
        return mutex.Unlock();
    }

    // Loop flag: the setter wrote +0x8d, the getter reads +0x8c. The listing passes the +0x8d byte
    // to the cursor's SetLoop (cursor +0x2c) as its argument.
    if (loop_read_ != loop_) {
        decoder_cursor_->SetLoop(loop_ != 0);
        loop_read_ = loop_;
    }
    // A mono cursor (cursor +0x04 channels == 1) pushes the 3D parameters.
    if (decoder_cursor_->GetTrackParams().channels == 1) {
        Update3D();
    }
    UpdateDSP(dt);

    // Gain: the volume fade and the resume fade multiply the modifier.
    StepFade(gain_fade_, dt);
    StepFade(resume_fade_, dt);
    const float resume_value = CurrentOfFade(resume_fade_);
    const float gain_value = CurrentOfFade(gain_fade_);
    gain_ = gain_modifier_ * resume_value * gain_value;
    source_gain_ = dsp_source_->GetGain();  // source +0x34
    if (source_gain_ != gain_) {
        dsp_source_->SetGain(gain_);        // source +0x2c
        source_gain_ = gain_;
    }

    // Pitch fade.
    StepFade(pitch_fade_, dt);
    pitch_ = CurrentOfFade(pitch_fade_);
    source_pitch_ = dsp_source_->GetPitch();  // source +0x38
    if (source_pitch_ != pitch_) {
        dsp_source_->SetPitch(pitch_);        // source +0x30
        source_pitch_ = pitch_;
    }

    state_ = dsp_source_->GetState();
    if (state_ == 3 && state_set_ != 3 && !decoder_cursor_->HasData()) {
        // The source has finished and the cursor has nothing left: request a reset.
        state_set_ = 3;
        reset_flag_ = 1;
        flag_145_ = 1;
    }
    if (state_ == -1) {
        if (previous_state == -1) {
            return mutex.Unlock();
        }
        callback_pending_ = true;  // +0x98
        return mutex.Unlock();
    }

    if (decoder_type_ == 4) {  // +0x140
        ProcessNativeData(dt);
    } else {
        ProcessNonNativeData(dt);
    }

    int next_state = state_;  // the state the frame ends in
    if (state_set_ != state_) {
        switch (state_set_) {
            case 0:
                state_set_ = state_;
                break;
            case 1:
                flag_145_ = 0;
                dsp_source_->Play();  // source +0x08
                next_state = dsp_source_->GetState();
                state_ = next_state;
                break;
            case 2:
                if (resume_fade_.flag) {
                    dsp_source_->SetGain(0.0f);
                    dsp_source_->Pause();  // source +0x10
                    next_state = dsp_source_->GetState();
                    state_ = next_state;
                }
                break;
            case 3:
                if (state_ != 0) {
                    if (resume_fade_.flag) {
                        dsp_source_->Stop();  // source +0x0c
                        const int after = dsp_source_->GetState();
                        state_ = after != 0 ? after : 3;
                        flag_145_ = 1;
                        reset_flag_ = 1;
                        next_state = state_;
                    }
                } else {
                    state_set_ = state_;
                    next_state = state_;
                }
                break;
            default:
                next_state = -1;
                state_ = -1;
                state_set_ = -1;
                break;
        }
    }
    if (previous_state == next_state || next_state == 0) {
        return mutex.Unlock();
    }
    callback_pending_ = true;  // +0x98
    return mutex.Unlock();
}

}  // namespace vox
