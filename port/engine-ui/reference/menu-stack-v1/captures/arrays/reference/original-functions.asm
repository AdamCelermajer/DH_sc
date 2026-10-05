
# _ZN7gameswf5arrayIP6MenuFXE6removeEi
00437ae8: push     {r4, lr}
00437aec: ldr      r2, [r0, #4]
00437af0: mov      r4, r0
00437af4: mov      r3, r1
00437af8: cmp      r2, #1
00437afc: beq      #0x437b30
00437b00: ldr      r0, [r0]
00437b04: sub      r2, r2, #1
00437b08: rsb      r2, r1, r2
00437b0c: add      r1, r1, #1
00437b10: add      r1, r0, r1, lsl #2
00437b14: lsl      r2, r2, #2
00437b18: add      r0, r0, r3, lsl #2
00437b1c: bl       #0x30df38
00437b20: ldr      r3, [r4, #4]
00437b24: sub      r3, r3, #1
00437b28: str      r3, [r4, #4]
00437b2c: pop      {r4, pc}
00437b30: mov      r3, #0
00437b34: str      r3, [r0, #4]
00437b38: pop      {r4, pc}

# _ZN7gameswf5arrayIPN6MenuFX5StateEE6removeEi
00437c98: push     {r4, lr}
00437c9c: ldr      r2, [r0, #4]
00437ca0: mov      r4, r0
00437ca4: mov      r3, r1
00437ca8: cmp      r2, #1
00437cac: beq      #0x437ce0
00437cb0: ldr      r0, [r0]
00437cb4: sub      r2, r2, #1
00437cb8: rsb      r2, r1, r2
00437cbc: add      r1, r1, #1
00437cc0: add      r1, r0, r1, lsl #2
00437cc4: lsl      r2, r2, #2
00437cc8: add      r0, r0, r3, lsl #2
00437ccc: bl       #0x30df38
00437cd0: ldr      r3, [r4, #4]
00437cd4: sub      r3, r3, #1
00437cd8: str      r3, [r4, #4]
00437cdc: pop      {r4, pc}
00437ce0: mov      r3, #0
00437ce4: str      r3, [r0, #4]
00437ce8: pop      {r4, pc}
