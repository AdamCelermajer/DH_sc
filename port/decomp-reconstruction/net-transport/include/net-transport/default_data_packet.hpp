// DefaultDataPacket: the concrete DataPacket with a fixed 4096-byte message body, a read
// cursor and a write cursor, and big-endian integer and length-prefixed string helpers.
// Reconstructed from the pseudocode of libDungeonHunter2.so (vtable _ZTV17DefaultDataPacket).
// The original 32-bit member offsets (from `this`) are given as comments only.
#pragma once

#include <cstddef>
#include <cstdint>

#include "net-transport/data_packet.hpp"

class DefaultDataPacket : public DataPacket {
public:
    static constexpr int kBodySize = 0x1000;  // message body capacity in bytes

    // 0x84a988 / 0x84aa7c _ZN17DefaultDataPacketC1Ev, _ZN17DefaultDataPacketC2Ev
    DefaultDataPacket();
    // 0x84a740 / 0x84a7a8 _ZN17DefaultDataPacketD1Ev, _ZN17DefaultDataPacketD2Ev
    ~DefaultDataPacket() override;
    DefaultDataPacket(const DefaultDataPacket&) = delete;
    DefaultDataPacket& operator=(const DefaultDataPacket&) = delete;

    // Writers append at the write cursor and return 1, or 0 when the body has no room.
    // Readers consume at the read cursor and return 1, or 0 when the data is short.
    // The 2-byte, 4-byte and length-prefixed integers are big-endian; floats are little-endian,
    // as in the original.
    virtual int addQueryString(const char* value, std::int16_t length);             // 0x849fcc (stub)
    virtual int addShortLenString(const char* value, std::int16_t length);          // 0x849fd4 (stub)
    virtual int getShortLenString(char*& value, std::int16_t& length);              // 0x849fdc (stub)
    virtual int addShortLenWideString(const wchar_t* value, unsigned char length);  // 0x849fe4 (stub)
    virtual int getShortLenWideString(wchar_t*& value, unsigned char& length);      // 0x849fec (stub)
    virtual int addByte(unsigned char value);                                       // 0x849ff4
    virtual int getByte(unsigned char& value);                                      // 0x84a01c
    virtual int addShort(std::int16_t value);                                       // 0x84a060
    virtual int getShort(std::int16_t& value);                                      // 0x84a0a4
    virtual int addInt(int value);                                                  // 0x84a114
    virtual int getInt(int& value);                                                 // 0x84a17c
    virtual int addFloat(float value);                                              // 0x84a224
    virtual int getFloat(float& value);                                             // 0x84a2a4
    virtual int addByteLenString(const char* value, unsigned char length);          // 0x84a6c4
    virtual int getByteLenString(char*& value, unsigned char& length);              // 0x84a8e0
    virtual int addByteLenWideString(const wchar_t* value, unsigned char length);   // 0x84a640
    virtual int getByteLenWideString(wchar_t*& value, unsigned char& length);       // 0x84a830

    // 0x84a9fc _ZN17DefaultDataPacket9getPacketERP10DataPacket (returns 1; see the ledger)
    virtual int getPacket(DataPacket*& packet);
    // 0x84a344 packMessage: empty in the original.
    virtual void packMessage();
    virtual int getMessageLen();                                                    // 0x84a330
    virtual char* getMessageBody();                                                 // 0x84a33c
    // 0x84a4dc _ZN17DefaultDataPacket14setMessageBodyEPhi: copies `length` bytes into the body
    // and resets the cursors (read 0, write length). Not bounded by kBodySize, as in the original.
    virtual void setMessageBody(const unsigned char* data, int length);
    virtual DataPacket* getNextPacket();                                            // 0x84a348
    virtual void setNextPacket(DataPacket* next);                                   // 0x84a354
    virtual bool IsSendByTcp();                                                     // 0x84a360
    virtual bool IsSendByUdp();                                                     // 0x84a37c
    virtual int getDataType();                                                      // 0x84a394
    virtual void setDataType(int type);                                             // 0x84a3a0
    virtual int getPort();                                                          // 0x84a3ac
    virtual void setPort(int port);                                                 // 0x84a3b8
    virtual char* getIP();                                                          // 0x84a3c4
    virtual void setIP(char* ip);                                                   // 0x84a7f4
    virtual void Packet2String(char* out, int& out_size);                           // 0x84a410
    virtual int String2Packet(char* data, int size);                                // 0x84a3d0

private:
    char body_[kBodySize] = {};                 // +0x04..+0x1003 message body (zeroed)
    int read_pos_ = 0;                          // +0x1004 read cursor (byte index)
    int write_pos_ = 0;                         // +0x1008 write cursor = message length
    DataPacket* next_packet_ = nullptr;         // +0x100c next packet in a chain
    int data_type_ = 1;                         // +0x1010 1 and 3 send by TCP, 2 by UDP
    char* ip_ = nullptr;                        // +0x1014 owned copy of the address string
    int port_ = 0;                              // +0x1018
};
