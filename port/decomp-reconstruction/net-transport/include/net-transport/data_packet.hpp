// DataPacket: message payload base with blob and message-type accessors.
// Reconstructed from libDungeonHunter2.so pseudocode. The base bodies do nothing
// and return -1 for the message types, as in the original.
#pragma once

#include <cstdint>

class DataPacket {
public:
    virtual ~DataPacket() = default;                    // 0x8491cc _ZN10DataPacketD1Ev
    virtual int addBlob(char* data, std::int16_t length);   // +0x4c, 0x8491d0
    virtual int getBlob(char*& data, std::int16_t& length); // +0x50, 0x8491d8

    void InitMessage(int type);                         // 0x8491e0 _ZN10DataPacket11InitMessageEi
    int GetMessageType() const;                         // 0x8491e4 _ZN10DataPacket14GetMessageTypeEv
    void InitMMOMessage(std::int16_t type);             // 0x8491ec _ZN10DataPacket14InitMMOMessageEs
    int GetMMOMessageType() const;                      // 0x8491f0 _ZN10DataPacket17GetMMOMessageTypeEv
};
