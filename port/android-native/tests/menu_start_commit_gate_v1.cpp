#include "../app/src/main/cpp/menu_start_commit_gate_v1.hpp"

int main() {
    using dh2::android_ui::menu_start_commit_gate_v1;
    if (!menu_start_commit_gate_v1(true, true)) return 1;
    if (menu_start_commit_gate_v1(true, false)) return 2;
    if (menu_start_commit_gate_v1(false, true)) return 3;
    if (menu_start_commit_gate_v1(false, false)) return 4;
    return 0;
}
