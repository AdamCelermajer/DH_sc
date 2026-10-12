#include "net-session-online/glx_player_mp_base.hpp"

#include <cstring>

#include "net-session-online/deps/glx_stand_ins.hpp"

// 0x84ab14 _ZN15GLXPlayerMPBaseC1EP23GLXPlayerMPBaseObserver
// 0x84ab54 _ZN15GLXPlayerMPBaseC2EP23GLXPlayerMPBaseObserver
GLXPlayerMPBase::GLXPlayerMPBase(GLXPlayerMPBaseObserver* observer) : observer_(observer) {
    field71_ = 0;
    Init();
}

// 0x84aea8 _ZN15GLXPlayerMPBaseD1Ev, 0x84af30 _ZN15GLXPlayerMPBaseD2Ev
GLXPlayerMPBase::~GLXPlayerMPBase() = default;

// 0x84ab00 _ZN15GLXPlayerMPBase4InitEv
void GLXPlayerMPBase::Init() {
    field70_ = 0;
    aux68_.reset();
    field6c_ = 0;
    LoadConfig();
}

// 0x84aaf0 _ZN15GLXPlayerMPBase10LoadConfigEv
bool GLXPlayerMPBase::LoadConfig() {
    field58_ = 0;
    return true;
}

// 0x84ab94 _ZN15GLXPlayerMPBase9getErrMsgEv
const char* GLXPlayerMPBase::getErrMsg() const { return errMsg_.get(); }

// 0x84ab9c _ZN15GLXPlayerMPBase9setErrMsgEPc
// The original frees the old message, then copies the new one with XP_API_STRNEW
// when it is non-null. The copy is made with new[] here.
void GLXPlayerMPBase::setErrMsg(const char* message) {
    errMsg_.reset();
    if (message != nullptr) {
        const std::size_t length = std::strlen(message) + 1;
        errMsg_.reset(new char[length]);
        std::memcpy(errMsg_.get(), message, length);
    }
}
