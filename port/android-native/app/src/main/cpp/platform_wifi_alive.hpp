#pragma once

#include <string>

namespace dh2::platform {

// Source-equivalent nativeIsWifiAlive query. `alive` is true only when the
// current platform network is Wi-Fi; lack of Wi-Fi is a successful false
// result. Platform/API failures also return false and describe the failure.
bool source_wifi_alive(bool& alive, std::string& error);

} // namespace dh2::platform
