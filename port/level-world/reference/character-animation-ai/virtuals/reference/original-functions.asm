
# _ZNK9Character14CanRangeAttackEv
003a4d3c: movw     r3, #0x1078
003a4d40: ldr      r3, [r0, r3]
003a4d44: cmn      r3, #1
003a4d48: beq      #0x3a4d54
003a4d4c: mov      r0, #1
003a4d50: bx       lr
003a4d54: add      r0, r0, #0x37c
003a4d58: b        #0x400014

# _ZNK9Character6IsDeadEv
003a2ed4: movw     r3, #0x1449
003a2ed8: ldrb     r0, [r0, r3]
003a2edc: bx       lr

# _ZNK10GameObject6IsDeadEv
003400ac: mov      r0, #0
003400b0: bx       lr
