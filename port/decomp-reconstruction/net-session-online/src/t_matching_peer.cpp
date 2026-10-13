#include "net-session-online/t_matching_peer.hpp"

// 0x808208 _ZN13tMatchingPeerC1ERKS_
// Member-wise copy in the original's field order. The body moved into the class template
// (t_matching_peer_t.hpp) so that CMatchingGLLive can instantiate it with the real CRoomAttributes;
// this explicit instantiation keeps one definition of the stand-in instance in the library.
template class tMatchingPeerT<CRoomAttributes, 0>;
