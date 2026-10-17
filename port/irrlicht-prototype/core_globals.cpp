// Core constant definitions copied in meaning from upstream Irrlicht.cpp.
// The Null-driver-only build omits Irrlicht.cpp because its desktop device
// factory selects X11 on Android and the upstream tag has no Android device.
#include "irrlicht.h"

namespace irr {
namespace core {
const matrix4 IdentityMatrix(matrix4::EM4CONST_IDENTITY);
stringc LOCALE_DECIMAL_POINTS(".");
}
namespace video {
SMaterial IdentityMaterial;
}
}
