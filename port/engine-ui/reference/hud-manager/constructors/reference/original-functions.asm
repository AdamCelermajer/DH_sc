
# _ZN20DebugCachedCharacterC1Ev
0041aeec: mov      r2, #0
0041aef0: add      r1, r0, #8
0041aef4: str      r2, [r0, #0x2c]
0041aef8: str      r1, [r0, #0x1c]
0041aefc: strb     r2, [r0]
0041af00: str      r2, [r0, #4]
0041af04: str      r1, [r0, #0x18]
0041af08: strb     r2, [r0, #8]
0041af0c: str      r2, [r0, #0x20]
0041af10: str      r2, [r0, #0x24]
0041af14: str      r2, [r0, #0x28]
0041af18: bx       lr

# _ZN14InfoHUDManagerC1Ev
0041ecb0: push     {r4, r5, r6, lr}
0041ecb4: mvn      r3, #0
0041ecb8: mov      r6, #0
0041ecbc: str      r3, [r0, #8]
0041ecc0: mov      r4, r0
0041ecc4: strb     r6, [r0, #4]
0041ecc8: add      r0, r0, #0xc
0041eccc: bl       #0x41aeec
0041ecd0: add      r0, r4, #0x3c
0041ecd4: bl       #0x41aeec
0041ecd8: add      r0, r4, #0x6c
0041ecdc: bl       #0x41aeec
0041ece0: add      r0, r4, #0x9c
0041ece4: bl       #0x41aeec
0041ece8: add      r0, r4, #0xcc
0041ecec: bl       #0x41aeec
0041ecf0: add      r0, r4, #0xfc
0041ecf4: bl       #0x41aeec
0041ecf8: add      r0, r4, #0x12c
0041ecfc: bl       #0x41aeec
0041ed00: add      r0, r4, #0x15c
0041ed04: bl       #0x41aeec
0041ed08: add      r0, r4, #0x18c
0041ed0c: bl       #0x41aeec
0041ed10: add      r0, r4, #0x1bc
0041ed14: bl       #0x41aeec
0041ed18: add      r0, r4, #0x1ec
0041ed1c: bl       #0x41aeec
0041ed20: add      r0, r4, #0x21c
0041ed24: bl       #0x41aeec
0041ed28: add      r0, r4, #0x24c
0041ed2c: bl       #0x41aeec
0041ed30: add      r0, r4, #0x27c
0041ed34: bl       #0x41aeec
0041ed38: add      r0, r4, #0x2ac
0041ed3c: bl       #0x41aeec
0041ed40: add      r0, r4, #0x2dc
0041ed44: bl       #0x41aeec
0041ed48: add      r0, r4, #0x30c
0041ed4c: bl       #0x41aeec
0041ed50: add      r0, r4, #0x33c
0041ed54: bl       #0x41aeec
0041ed58: add      r0, r4, #0x36c
0041ed5c: bl       #0x41aeec
0041ed60: add      r0, r4, #0x39c
0041ed64: bl       #0x41aeec
0041ed68: add      r0, r4, #0x3cc
0041ed6c: bl       #0x41aeec
0041ed70: add      r0, r4, #0x3fc
0041ed74: bl       #0x41aeec
0041ed78: add      r0, r4, #0x420
0041ed7c: add      r0, r0, #0xc
0041ed80: bl       #0x41aeec
0041ed84: add      r0, r4, #0x450
0041ed88: add      r0, r0, #0xc
0041ed8c: bl       #0x41aeec
0041ed90: add      r0, r4, #0x480
0041ed94: add      r0, r0, #0xc
0041ed98: bl       #0x41aeec
0041ed9c: add      r0, r4, #0x4b0
0041eda0: add      r5, r4, #0x4e0
0041eda4: add      r0, r0, #0xc
0041eda8: bl       #0x41aeec
0041edac: add      r0, r5, #0xc
0041edb0: bl       #0x41aeec
0041edb4: add      r0, r5, #0x3c
0041edb8: bl       #0x41aeec
0041edbc: add      r0, r5, #0x6c
0041edc0: bl       #0x41aeec
0041edc4: str      r6, [r4, #0x57c]
0041edc8: mov      r0, r4
0041edcc: pop      {r4, r5, r6, pc}
