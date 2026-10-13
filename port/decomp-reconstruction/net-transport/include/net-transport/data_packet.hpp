// DataPacket: message payload base with blob and message-type accessors.
// Reconstructed from libDungeonHunter2.so pseudocode. The base bodies do nothing
// and return -1 for the message types, as in the original.
#pragma once

#include <cstddef>
#include <cstdint>

class DataPacket {
public:
    virtual ~DataPacket() = default;                    // 0x8491cc _ZN10DataPacketD1Ev
    virtual int addBlob(char* data, std::int16_t length);   // +0x4c, 0x8491d0
    virtual int getBlob(char*& data, std::int16_t& length); // +0x50, 0x8491d8

    // Message accessors. In the original base vtable these slots hold __cxa_pure_virtual
    // (+0x60, +0x64 and +0x74 from the object pointer), so the base declares them pure. Every
    // packet class defines them with these signatures. Read by the lobby and default packets.
    virtual std::size_t getMessageLen() const = 0;           // vptr +0x60 (96)
    virtual const std::uint8_t* getMessageBody() const = 0;  // vptr +0x64 (100)
    virtual int getDataType() const = 0;                     // vptr +0x74 (116)

    void InitMessage(int type);                         // 0x8491e0 _ZN10DataPacket11InitMessageEi
    int GetMessageType() const;                         // 0x8491e4 _ZN10DataPacket14GetMessageTypeEv
    void InitMMOMessage(std::int16_t type);             // 0x8491ec _ZN10DataPacket14InitMMOMessageEs
    int GetMMOMessageType() const;                      // 0x8491f0 _ZN10DataPacket17GetMMOMessageTypeEv
};
