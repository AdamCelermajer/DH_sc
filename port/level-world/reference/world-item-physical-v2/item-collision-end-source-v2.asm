_ZN10ItemObject15OnCollisionEndsEP9Character
003ebd2c cmp      r1, #0
003ebd30 push     {r4, lr}
003ebd34 mov      r4, r0
003ebd38 beq      #0x3ebd58
003ebd3c movw     r3, #0x14a4
003ebd40 ldr      r3, [r1, r3]
003ebd44 cmp      r0, r3
003ebd48 beq      #0x3ebd58
003ebd4c bl       #0x3ebcf8 ; _ZN10ItemObject11HideTooltipEv
003ebd50 mov      r3, #0
003ebd54 str      r3, [r4, #0x3c4]
003ebd58 pop      {r4, pc}
