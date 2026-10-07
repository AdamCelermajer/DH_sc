$a


$a


_ZNK16CharStateMachine11SM_IsScaredEb
003c034c: cmp r1, #0
003c0350: push {r4, lr}
003c0354: beq #0x3c0364
003c0358: ldr r0, [r0, #0x2c]
003c035c: ubfx r0, r0, #2, #1
003c0360: pop {r4, pc}
003c0364: bl #0x3c01ac
003c0368: cmp r0, #8
003c036c: movne r0, #0
003c0370: moveq r0, #1
003c0374: pop {r4, pc}

_ZNK16CharStateMachine12SM_IsStunnedEb
003c0378: cmp r1, #0
003c037c: push {r4, lr}
003c0380: beq #0x3c0390
003c0384: ldr r0, [r0, #0x2c]
003c0388: ubfx r0, r0, #1, #1
003c038c: pop {r4, pc}
003c0390: bl #0x3c01ac
003c0394: cmp r0, #9
003c0398: movne r0, #0
003c039c: moveq r0, #1
003c03a0: pop {r4, pc}
