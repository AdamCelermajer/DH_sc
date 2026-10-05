
# _ZNK13ItemInventory6HasBowEv
00400080: push     {r4, lr}
00400084: mov      r1, #1
00400088: mov      r4, r0
0040008c: bl       #0x3fc6a8
00400090: mov      r3, #0xc
00400094: mul      r3, r3, r0
00400098: ldr      r2, [r4, #0x14]
0040009c: ldr      r3, [r2, r3]
004000a0: ldr      r0, [r3, #4]
004000a4: cmp      r0, #0
004000a8: beq      #0x4000c4
004000ac: ldr      r0, [r0]
004000b0: bl       #0x3f9e08
004000b4: ldr      r0, [r0, #0x94]
004000b8: cmp      r0, #4
004000bc: movne    r0, #0
004000c0: moveq    r0, #1
004000c4: pop      {r4, pc}

# _ZN9Character15UnEquipItemAutoEj
003a9f6c: push     {r4, lr}
003a9f70: mov      r4, r0
003a9f74: add      r0, r0, #0x37c
003a9f78: bl       #0x400630
003a9f7c: add      r0, r4, #0x560
003a9f80: bl       #0x3e08a8
003a9f84: mov      r0, r4
003a9f88: bl       #0x3a9d10
003a9f8c: mov      r0, r4
003a9f90: bl       #0x3a999c
003a9f94: mov      r0, r4
003a9f98: pop      {r4, lr}
003a9f9c: b        #0x3bd140

# _ZN9Character19UnEquipItemFromSlotEj
003a9eb0: push     {r4, lr}
003a9eb4: mvn      r2, #0
003a9eb8: mov      r4, r0
003a9ebc: add      r0, r0, #0x37c
003a9ec0: bl       #0x40050c
003a9ec4: add      r0, r4, #0x560
003a9ec8: bl       #0x3e08a8
003a9ecc: mov      r0, r4
003a9ed0: bl       #0x3a9d10
003a9ed4: mov      r0, r4
003a9ed8: bl       #0x3a999c
003a9edc: mov      r0, r4
003a9ee0: pop      {r4, lr}
003a9ee4: b        #0x3bd140

# _ZNK12ItemInstance12IsEquippableEv
003f9e68: push     {r4, lr}
003f9e6c: bl       #0x3f9e08
003f9e70: ldr      r0, [r0, #0x68]
003f9e74: adds     r0, r0, #1
003f9e78: movne    r0, #1
003f9e7c: pop      {r4, pc}

# _ZNK13ItemInventory8HasStaffEv
004000c8: push     {r4, lr}
004000cc: mov      r1, #1
004000d0: mov      r4, r0
004000d4: bl       #0x3fc6a8
004000d8: mov      r3, #0xc
004000dc: mul      r3, r3, r0
004000e0: ldr      r2, [r4, #0x14]
004000e4: ldr      r3, [r2, r3]
004000e8: ldr      r0, [r3, #4]
004000ec: cmp      r0, #0
004000f0: beq      #0x40010c
004000f4: ldr      r0, [r0]
004000f8: bl       #0x3f9e08
004000fc: ldr      r0, [r0, #0x94]
00400100: cmp      r0, #5
00400104: movne    r0, #0
00400108: moveq    r0, #1
0040010c: pop      {r4, pc}

# _ZN9Character15EquipItemToSlotEjj
003a9f30: push     {r4, lr}
003a9f34: mov      r4, r0
003a9f38: add      r0, r0, #0x37c
003a9f3c: bl       #0x4009f0
003a9f40: add      r0, r4, #0x560
003a9f44: bl       #0x3e08a8
003a9f48: mov      r0, r4
003a9f4c: bl       #0x3a9d10
003a9f50: mov      r0, r4
003a9f54: bl       #0x3a999c
003a9f58: mov      r0, r4
003a9f5c: pop      {r4, lr}
003a9f60: b        #0x3bd140
