_ZN10ItemObject11HideTooltipEv
003ebcf8 push     {r4, lr}
003ebcfc mov      r4, r0
003ebd00 ldr      r0, [r0, #0x3c8]
003ebd04 cmp      r0, #0
003ebd08 beq      #0x3ebd18
003ebd0c bl       #0x498cdc ; _ZNK14SWFAnimToolTip9IsVisibleEv
003ebd10 cmp      r0, #0
003ebd14 bne      #0x3ebd1c
003ebd18 pop      {r4, pc}
003ebd1c ldr      r0, [r4, #0x3c8]
003ebd20 mov      r1, #0x64
003ebd24 pop      {r4, lr}
003ebd28 b        #0x498d38
