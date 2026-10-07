from pathlib import Path
import struct
p=Path(__file__).resolve().parents[3]/'port/engine-ui/reference/character-menu-native-v1/inventory-order-gold-v1.bin'
raw=p.read_bytes()
for j in range(45):print(j,struct.unpack_from('<iiiiiiii',raw,4+j*32))
