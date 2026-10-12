// DataPacketLobby: lobby protocol message. Reconstructed from libDungeonHunter2.so
// pseudocode (Hex-Rays, ARM).
//
// Original layout (32-bit offsets, kept as comments):
//   +0x00 vptr
//   +0x04 capacity           body buffer size, starts at 4096, doubles when full
//   +0x08 buffer             body bytes
//   +0x0C position           read/write index, starts at 2 (room for the 2-byte header)
//   +0x10 length             valid bytes (set by packMessage, setMessageBody, Packet2String)
//   +0x14 next packet        DataPacket*
//   +0x18 data type          starts at 1
//   +0x1C IP string          char*, copied by setIP
//   +0x20 port
// Integers in the body are big-endian (addShort, addInt, getShort, getInt). Floats are
// four little-endian bytes (addFloat, getFloat).
//
// The lobby table is a different virtual layout from DataPacket's (addBlob is +0x68 here,
// +0x4c in DataPacket). This port derives from DataPacket for the shared addBlob/getBlob
// interface and makes the slot-dispatched calls virtual.
#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

#include "net-transport/data_packet.hpp"

class DataPacketLobby : public DataPacket {
public:
    DataPacketLobby();                              // 0x849e78 _ZN15DataPacketLobbyC1Ev (C2 0x849f5c)
    explicit DataPacketLobby(DataPacket* source);   // 0x849d20 _ZN15DataPacketLobbyC1EP10DataPacket (C2 0x849dcc)
    ~DataPacketLobby() override;                    // 0x849624 _ZN15DataPacketLobbyD1Ev (D2 0x8496b4, D0 0x849684)
    DataPacketLobby(const DataPacketLobby&) = delete;
    DataPacketLobby& operator=(const DataPacketLobby&) = delete;

    // Byte- and wide-string helpers. The original bodies return 0 and do nothing.
    int addByteLenString(const char* text, unsigned char length);             // 0x8491f8
    int getByteLenString(char*& text, unsigned char& length);                 // 0x849200
    int addByteLenWideString(const wchar_t* text, unsigned char length);      // 0x849208
    int getByteLenWideString(wchar_t*& text, unsigned char& length);          // 0x849210
    int addShortLenWideString(const wchar_t* text, unsigned char length);     // 0x849218
    int getShortLenWideString(wchar_t*& text, unsigned char& length);         // 0x849220

    int InitMessage(int type);                      // 0x849228 _ZN15DataPacketLobby11InitMessageEi
    int InitMMOMessage(std::int16_t type);          // 0x849270 _ZN15DataPacketLobby14InitMMOMessageEs
    int GetMessageType();                           // 0x849284 _ZN15DataPacketLobby14GetMessageTypeEv
    int GetMMOMessageType();                        // 0x8492b0 _ZN15DataPacketLobby17GetMMOMessageTypeEv
    bool WillBeFull(std::size_t bytes) const;       // 0x8492dc _ZN15DataPacketLobby10WillBeFullEi

    int getByte(std::uint8_t& value);               // 0x8492f8 _ZN15DataPacketLobby7getByteERh
    virtual int getShort(std::int16_t& value);      // 0x849334 _ZN15DataPacketLobby8getShortERs
    virtual int getInt(int& value);                 // 0x84939c _ZN15DataPacketLobby6getIntERi
    int getFloat(float& value);                     // 0x84943c _ZN15DataPacketLobby8getFloatERf
    std::size_t getMessageLen() const;              // 0x8494c0 _ZN15DataPacketLobby13getMessageLenEv
    const std::uint8_t* getMessageBody() const;     // 0x8494c8 _ZN15DataPacketLobby14getMessageBodyEv
    DataPacketLobby* packMessage();                 // 0x8494d0 _ZN15DataPacketLobby11packMessageEv
    DataPacket* getNextPacket() const;              // 0x8494fc _ZN15DataPacketLobby13getNextPacketEv
    void setNextPacket(DataPacket* packet);         // 0x849504 _ZN15DataPacketLobby13setNextPacketEP10DataPacket
    bool IsSendByTcp() const;                       // 0x84950c _ZN15DataPacketLobby11IsSendByTcpEv
    bool IsSendByUdp() const;                       // 0x849524 _ZN15DataPacketLobby11IsSendByUdpEv
    int getDataType() const;                        // 0x849538 _ZN15DataPacketLobby11getDataTypeEv
    void setDataType(int type);                     // 0x849540 _ZN15DataPacketLobby11setDataTypeEi
    int getPort() const;                            // 0x849548 _ZN15DataPacketLobby7getPortEv
    void setPort(int port);                         // 0x849550 _ZN15DataPacketLobby7setPortEi
    const char* getIP() const;                      // 0x849558 _ZN15DataPacketLobby5getIPEv
    int String2Packet(char* data, int size);        // 0x849560 _ZN15DataPacketLobby13String2PacketEPci
    void Packet2String(char* out, int& out_length); // 0x8495b8 _ZN15DataPacketLobby13Packet2StringEPcRi
    void setMessageBody(const std::uint8_t* data, std::size_t size);  // 0x849600 _ZN15DataPacketLobby14setMessageBodyEPhi
    const char* setIP(const char* ip);              // 0x849714 _ZN15DataPacketLobby5setIPEPc
    int getShortLenString(char*& text, std::uint16_t& length);        // 0x849748 _ZN15DataPacketLobby17getShortLenStringERPcRs
    int getBlob(char*& data, std::int16_t& length) override;          // 0x84980c _ZN15DataPacketLobby7getBlobERPcRs
    bool AllocateMoreMomery();                      // 0x8498bc _ZN15DataPacketLobby18AllocateMoreMomeryEv
    void addPacket(DataPacket* packet);             // 0x849924 _ZN15DataPacketLobby9addPacketEP10DataPacket
    int addShortLenString(const char* text, std::int16_t length);     // 0x8499d8 _ZN15DataPacketLobby17addShortLenStringEPKcs
    int addQueryString(const char* text, std::int16_t length);        // 0x849a68 _ZN15DataPacketLobby14addQueryStringEPKcs
    int addBlob(char* data, std::int16_t length) override;            // 0x849ad8 _ZN15DataPacketLobby7addBlobEPcs
    int addFloat(float value);                      // 0x849b6c _ZN15DataPacketLobby8addFloatEf
    virtual int addInt(int value);                  // 0x849bfc _ZN15DataPacketLobby6addIntEi
    virtual int addShort(std::int16_t value);       // 0x849c78 _ZN15DataPacketLobby8addShortEs
    virtual int addByte(std::uint8_t value);        // 0x849cd4 _ZN15DataPacketLobby7addByteEh
    int getPacket(DataPacket*& packet);             // 0x849ee8 _ZN15DataPacketLobby9getPacketERP10DataPacket

private:
    // Grows the buffer (doubling) until `bytes` more bytes fit.
    bool EnsureRoom(std::size_t bytes);

    std::size_t capacity_ = 4096;                   // +0x04
    std::unique_ptr<std::uint8_t[]> buffer_;        // +0x08
    std::size_t position_ = 2;                      // +0x0C
    std::size_t length_ = 0;                        // +0x10
    DataPacket* next_packet_ = nullptr;             // +0x14
    int data_type_ = 1;                             // +0x18
    std::unique_ptr<char[]> ip_;                    // +0x1C
    int port_ = 0;                                  // +0x20
};
