
# _ZN10AISDefault8OnUpdateEv
003dc798: push     {r4, lr}
003dc79c: ldr      r3, [r0, #0xbc]
003dc7a0: mov      r4, r0
003dc7a4: cmp      r3, #0xc7
003dc7a8: bhi      #0x3dc7b0
003dc7ac: pop      {r4, pc}
003dc7b0: ldr      r0, [r0, #0x98]
003dc7b4: mov      r3, #0
003dc7b8: str      r3, [r4, #0xbc]
003dc7bc: add      r0, r0, #0x3c8
003dc7c0: mov      r1, #0x3e8
003dc7c4: bl       #0x3cb748
003dc7c8: ldr      r3, [r4, #0x98]
003dc7cc: ldr      r0, [r3, #0x378]
003dc7d0: pop      {r4, lr}
003dc7d4: b        #0x40559c
