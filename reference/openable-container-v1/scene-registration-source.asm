_ZN12SceneManager13ForceRegisterEv
00350ee0 mov      r3, #1
00350ee4 strb     r3, [r0, #0x448]
00350ee8 strb     r3, [r0, #0x289]
00350eec bx       lr
_ZN6glitch5scene10ISceneNode6removeEv
0059706c push     {r4, lr}
00597070 ldr      r3, [r0, #0xec]
00597074 mov      r1, r0
00597078 cmp      r3, #0
0059707c beq      #0x597090
00597080 mov      r0, r3
00597084 ldr      r3, [r3]
00597088 mov      lr, pc
0059708c ldr      pc, [r3, #0x60]
00597090 pop      {r4, pc}
_ZN6glitch5scene10ISceneNode11removeChildEPS1_
00597004 ldr      r3, [r1, #0xec]
00597008 push     {r4, lr}
0059700c cmp      r3, r0
00597010 beq      #0x59701c
00597014 mov      r0, #0
00597018 pop      {r4, pc}
0059701c ldr      r2, [r1, #4]
00597020 add      ip, r1, #4
00597024 cmp      r2, #0
00597028 ldrne    r0, [r1, #8]
0059702c strne    r2, [r0]
00597030 strne    r0, [r2, #4]
00597034 ldr      lr, [r3, #0xf0]
00597038 mov      r2, #0
0059703c sub      r0, ip, #4
00597040 sub      lr, lr, #1
00597044 str      lr, [r3, #0xf0]
00597048 str      r2, [r1, #8]
0059704c str      r2, [r1, #4]
00597050 str      r2, [r0, #0xec]
00597054 ldr      r3, [ip, #-4]
00597058 ldr      r3, [r3, #-0xc]
0059705c add      r0, r0, r3
00597060 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00597064 mov      r0, #1
00597068 pop      {r4, pc}
