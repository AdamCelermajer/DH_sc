#!/usr/bin/env bash
set -euo pipefail
ROOT=/mnt/c/Users/adamc/Desktop/workspace/DH_sc
OUT="$ROOT/.local-inputs/character-menu-native-v1-host"
g++ -std=c++17 -Wall -Wextra -Werror -c "$ROOT/port/engine-ui/character_menu_inventory_mutation_v1.cpp" -o "$OUT/inventory-mutation-gcc.o"
g++ -std=c++17 -Wall -Wextra -Werror -Wno-misleading-indentation -c "$ROOT/port/engine-ui/character_menu_item_actions_v1.cpp" -o "$OUT/item-actions-gcc.o"
g++ -std=c++17 -Wall -Wextra -Werror -Wno-misleading-indentation -c "$ROOT/port/engine-ui/character_menu_save_actions_v1.cpp" -o "$OUT/save-actions-gcc.o"
g++ -std=c++17 -Wall -Wextra -Werror -Wno-misleading-indentation -c "$ROOT/port/engine-ui/character_menu_faery_actions_v1.cpp" -o "$OUT/faery-actions-gcc.o"
