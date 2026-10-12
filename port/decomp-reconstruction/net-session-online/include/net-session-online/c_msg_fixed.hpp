// Fixed-size CMsg* message types. Each one is a CMessage whose wire payload is
// a fixed number of bytes at original offset +0x50. The payload field layout is
// not recovered, so it is kept as opaque bytes; see NOTES.md.
// SetProperties and the constructors that take a bool assign the class name to typeName_
// (the CMessage string at +0x14), as the original does.
#pragma once

#include <cstdint>

#include "net-session-online/c_message.hpp"

class CMsgIsHost : public CMessage {
public:
    void* GetDataPtr() override { return payload_; }
    std::uint32_t GetDataSize() const override { return 4; }
    // 0x3278d4 _ZN10CMsgIsHost13SetPropertiesEv
    void SetProperties() {
        typeName_ = "CMsgIsHost";
        flag32_ = true;
        sendMode_ = 0;
    }

private:
    std::uint8_t payload_[4] = {};  // +0x50
};

class CMsgMenuReady : public CMessage {
public:
    void* GetDataPtr() override { return payload_; }
    std::uint32_t GetDataSize() const override { return 8; }
    // 0x3279e0 _ZN13CMsgMenuReady13SetPropertiesEv
    void SetProperties() {
        typeName_ = "CMsgMenuReady";
        sendMode_ = 1;
    }

private:
    std::uint8_t payload_[8] = {};  // +0x50
};

class CMsgAskResendAttributes : public CMessage {
public:
    void* GetDataPtr() override { return payload_; }
    std::uint32_t GetDataSize() const override { return 1; }
    // 0x327ad8 _ZN23CMsgAskResendAttributes13SetPropertiesEv
    void SetProperties() {
        typeName_ = "CMsgAskResendAttributes";
        flag32_ = true;
        sendMode_ = 0;
    }

private:
    std::uint8_t payload_[1] = {};  // +0x50
};

class CMsgScriptCmd : public CMessage {
public:
    void* GetDataPtr() override { return payload_; }
    std::uint32_t GetDataSize() const override { return 12; }
    // 0x3279a8 _ZN13CMsgScriptCmd13SetPropertiesEv
    void SetProperties() {
        typeName_ = "CMsgScriptCmd";
        sendMode_ = 1;
        includeLocalMember_ = true;
        flag32_ = true;
        reliable_ = true;
    }

private:
    std::uint8_t payload_[12] = {};  // +0x50
};

class CMsgStartGame : public CMessage {
public:
    void* GetDataPtr() override { return payload_; }
    std::uint32_t GetDataSize() const override { return 28; }
    // 0x327aa0 _ZN13CMsgStartGame13SetPropertiesEv
    void SetProperties() {
        typeName_ = "CMsgStartGame";
        includeLocalMember_ = false;
        flag32_ = true;
        sendMode_ = 1;
    }

private:
    std::uint8_t payload_[28] = {};  // +0x50
};

class CMsgInitialSetup : public CMessage {
public:
    // 0x328be4 _ZN16CMsgInitialSetupC1Eb
    explicit CMsgInitialSetup(bool withSerial) : CMessage("CMsgInitialSetup", withSerial) {
        typeName_ = "CMsgInitialSetup";
        includeLocalMember_ = false;
        sendMode_ = 1;
    }
    void* GetDataPtr() override { return payload_; }
    std::uint32_t GetDataSize() const override { return 12; }
    // 0x327974 _ZN16CMsgInitialSetup13SetPropertiesEv
    void SetProperties() {
        typeName_ = "CMsgInitialSetup";
        includeLocalMember_ = false;
        sendMode_ = 1;
    }

private:
    std::uint8_t payload_[12] = {};  // +0x50
};

class CMsgSpawnObject : public CMessage {
public:
    void* GetDataPtr() override { return payload_; }
    std::uint32_t GetDataSize() const override { return 28; }
    // 0x32789c _ZN15CMsgSpawnObject13SetPropertiesEv
    // Partial: the original also assigns the type name string at +0x14 (not modelled).
    void SetProperties() {
        flag32_ = true;
        includeLocalMember_ = false;
        sendMode_ = 0;
    }

private:
    std::uint8_t payload_[28] = {};  // +0x50
};

class CMsgRaisedEvent : public CMessage {
public:
    void* GetDataPtr() override { return payload_; }
    std::uint32_t GetDataSize() const override { return 20; }
    // Original also assigns the type name string at +0x14 (not modelled).
    void SetProperties() {
        reliable_ = true;
        sendMode_ = 1;
        includeLocalMember_ = true;
    }

private:
    std::uint8_t payload_[20] = {};  // +0x50
};

class CMsgControllerAction : public CMessage {
public:
    // 0x328a64 _ZN20CMsgControllerActionC1Eb
    // Partial: calls CMessage(const char*, bool); the type-name string is not modelled.
    // The original does not touch the local-member flag here, so the base value stays.
    explicit CMsgControllerAction(bool withSerial) : CMessage("CMsgControllerAction", withSerial) {
        sendMode_ = 1;
    }
    void* GetDataPtr() override { return payload_; }
    std::uint32_t GetDataSize() const override { return 20; }
    // 0x327a74 _ZN20CMsgControllerAction13SetPropertiesEv
    // Partial: the original also assigns the type name string at +0x14 (not modelled).
    void SetProperties() {
        sendMode_ = 1;
    }

private:
    std::uint8_t payload_[20] = {};  // +0x50
};

class CMsgAttackResult : public CMessage {
public:
    void* GetDataPtr() override { return payload_; }
    std::uint32_t GetDataSize() const override { return 52; }
    // 0x327a40 _ZN16CMsgAttackResult13SetPropertiesEv
    // Partial: the original also assigns the type name string at +0x14 (not modelled).
    void SetProperties() {
        includeLocalMember_ = false;
        sendMode_ = 1;
    }

private:
    std::uint8_t payload_[52] = {};  // +0x50
};

// CMsgGlobalDeath has no payload: GetDataPtr is the base CMessage one.
class CMsgGlobalDeath : public CMessage {
public:
    std::uint32_t GetDataSize() const override { return 0; }
    // 0x327868 _ZN15CMsgGlobalDeath13SetPropertiesEv
    // Partial: the original also assigns the type name string at +0x14 (not modelled).
    void SetProperties() {
        reliable_ = true;
        sendMode_ = 1;
        flag32_ = true;
    }
};
