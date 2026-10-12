// GLXPlayerMPBase and its observer. The observer's vtable (_ZTV23GLXPlayerMPBaseObserver)
// lists the callbacks below in this order: slot 0 OnMPFinishGameSuccess,
// slot 1 OnMPuPnPTestSuccess, ... slot 13 OnMPJoinPreDefinedRoomSessionSuccess.
// The callbacks have empty bodies in the original, so they are no-op virtuals here.
//
// GLXPlayerMPBase layout (original 32-bit offsets, kept as comments):
//   +0x00 vptr  +0x04 observer  +0x4c error message (char*, owned)
//   +0x58 set by LoadConfig     +0x68 aux object (virtual dtor)
//   +0x6c zeroed by Init        +0x70 zeroed by Init (byte)  +0x71 zeroed by ctor (byte)
#pragma once

#include <cstdint>
#include <memory>

class DataPacket;

class GLXPlayerMPBaseObserver {
public:
    virtual ~GLXPlayerMPBaseObserver() = default;
    virtual void OnMPFinishGameSuccess() {}                                  // slot 0
    virtual void OnMPuPnPTestSuccess() {}                                    // slot 1
    virtual void OnMPLeaveGameSuccess() {}                                   // slot 2
    virtual void OnMPSetParamSuccess(std::int32_t) {}                        // slot 3
    virtual void OnMPProcessPushMessages(DataPacket*, std::int32_t) {}       // slot 4
    virtual void OnMPRejoinLobbySuccess(DataPacket*, bool) {}                // slot 5
    virtual void OnMPGetLobbyInfo(DataPacket*) {}                            // slot 6
    virtual void OnMPStartGameFailed(std::int32_t) {}                        // slot 7
    virtual void OnMPGetLobbyForName(DataPacket*) {}                         // slot 8
    virtual void OnMPGetLobbyForFriend(DataPacket*) {}                       // slot 9
    virtual void OnMPAutoMatchRequeset(std::int32_t) {}                      // slot 10 (sic)
    virtual void OnMPAutoMatchRequesetCancel() {}                            // slot 11 (sic)
    virtual void OnMPGetPlayerCounterSuccess(DataPacket*) {}                 // slot 12
    virtual void OnMPJoinPreDefinedRoomSessionSuccess(DataPacket*) {}        // slot 13
};

class ThreadHandle;

class GLXPlayerMPBase {
public:
    explicit GLXPlayerMPBase(GLXPlayerMPBaseObserver* observer);  // 0x84ab14, 0x84ab54
    virtual ~GLXPlayerMPBase();                                    // 0x84aea8, 0x84af30

    // 0x84ab00 _ZN15GLXPlayerMPBase4InitEv
    void Init();
    // 0x84aaf0 _ZN15GLXPlayerMPBase10LoadConfigEv: sets +0x58 to 0 and returns true.
    bool LoadConfig();

    // 0x84ab94 _ZN15GLXPlayerMPBase9getErrMsgEv
    const char* getErrMsg() const;
    // 0x84ab9c _ZN15GLXPlayerMPBase9setErrMsgEPc
    void setErrMsg(const char* message);

    // Empty virtual defaults in the original (4-byte bodies).
    // 0x84ae28 _ZN15GLXPlayerMPBase16mpSendDisconnectEv, vtable +0x14. Partial, see the cpp.
    virtual void mpSendDisconnect();
    virtual void mpPrepareGameData() {}                 // 0x83760c
    virtual void mpSendKickOutPlayer(char*) {}          // 0x837614
    virtual void mpSendKickOutPlayer(unsigned short*) {}  // 0x837618
    virtual void mpSendLeaveGame() {}                   // 0x83761c
    virtual void mpSendStartGame() {}                   // 0x837620
    virtual void mpSendFinishGame() {}                  // 0x837624
    virtual void mpSendUPnPPortMapping() {}             // 0x837628
    virtual void mpProcessPushMessages() {}             // 0x83762c

protected:
    GLXPlayerMPBaseObserver* observer_ = nullptr;       // +0x04
    std::unique_ptr<char[]> errMsg_;                    // +0x4c
    std::int32_t field58_ = 0;                          // +0x58
    std::int32_t field50_ = 0;                          // +0x50 (set by mpSendDisconnect)
    ConnectionTransport* transport_ = nullptr;          // +0x54 (not owned)
    std::unique_ptr<ThreadHandle> aux68_;               // +0x68
    std::int32_t field6c_ = 0;                          // +0x6c
    std::uint8_t field70_ = 0;                          // +0x70
    std::uint8_t field71_ = 0;                          // +0x71
};
