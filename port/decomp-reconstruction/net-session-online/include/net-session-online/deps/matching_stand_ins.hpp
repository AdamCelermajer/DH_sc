// Stand-ins for the types and calls that CMatchingLocal uses from other categories
// (CRoomAttributes and CRoomSearchFilter in the room layer; CConnectionManager, CTransportManager
// and the online clock moved to connection_manager_stand_in.hpp, which this file includes).
// Each one names the original symbol it replaces. The fakes record what they are
// asked to do so the tests can check the callers. Nothing here is the real behaviour.
// A translation unit that uses the real CRoomAttributes (c_room_attributes.hpp) must not include
// this file: it redefines CRoomAttributes and CRoomSearchFilter. Include
// connection_manager_stand_in.hpp instead.
#pragma once

#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "net-session-online/deps/net_bit_stream.hpp"
#include "net-session-online/deps/network_id.hpp"
#include "net-session-online/deps/platform_stand_ins.hpp"

// CRoomAttributes: the room settings that travel with a server record. Not modelled:
// Serialize and Unserialize do not touch the stream.
class CRoomAttributes {
public:
    // CRoomAttributes::Serialize (stand-in: writes nothing)
    void Serialize(NetBitStream& /*stream*/, unsigned /*bits*/) const {}
    // CRoomAttributes::Unserialize (stand-in: reads nothing)
    void Unserialize(NetBitStream& /*stream*/) {}
};

// CRoomSearchFilter: the search criteria. Not modelled: every room passes the test.
class CRoomSearchFilter {
public:
    // CRoomSearchFilter::TestRoomAttributes (stand-in: always true)
    bool TestRoomAttributes(const CRoomAttributes& /*attributes*/) const { return true; }
};

#include "net-session-online/deps/connection_manager_stand_in.hpp"
