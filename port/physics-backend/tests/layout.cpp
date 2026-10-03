#include <memory>
#include <cstddef>
#define private public
#define protected public
#include <Box2D.h>
#undef private
#undef protected
extern "C" const unsigned dh2_backend_original_layout[]={sizeof(b2World),sizeof(b2Body),sizeof(b2CircleShape),sizeof(b2PolygonShape),offsetof(b2World,m_lock),offsetof(b2World,m_broadPhase),offsetof(b2World,m_bodyCount),offsetof(b2World,m_contactCount),offsetof(b2World,m_contactFilter),offsetof(b2World,m_contactListener),offsetof(b2World,m_inv_dt0),offsetof(b2Body,m_shapeList),offsetof(b2Body,m_mass),offsetof(b2Shape,m_sweepRadius),offsetof(b2Shape,m_proxyId)};
extern "C" const unsigned dh2_backend_pair_layout[]={sizeof(b2Pair),offsetof(b2PairManager,m_pairCount),offsetof(b2BroadPhase,m_pairManager),sizeof(b2PairManager)};
