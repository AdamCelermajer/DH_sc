
# _ZN6CharAI13OnScriptTimerEj
003d0ca0: push     {r4, lr}
003d0ca4: ldr      r3, [r0, #0x1c]
003d0ca8: cmp      r3, #0
003d0cac: beq      #0x3d0cc0
003d0cb0: mov      r0, r3
003d0cb4: ldr      r3, [r3]
003d0cb8: mov      lr, pc
003d0cbc: ldr      pc, [r3, #0x90]
003d0cc0: pop      {r4, pc}
