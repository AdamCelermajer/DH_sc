// Stand-ins for the SSEncDec blob codec (SSEncDec_String2BlobLC and
// SSEncDec_Blob2StringLC). The codec belongs to another category. Both return null
// here, and callers treat null as an empty result.
#pragma once

inline char* SSEncDec_String2BlobLC(const char* /*text*/) { return nullptr; }
inline char* SSEncDec_Blob2StringLC(const char* /*blob*/) { return nullptr; }
