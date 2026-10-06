_ZN8Animator7_CBAnimEPN6glitch5scene19ITimelineControllerEPv
00366294 mov      r3, r0
00366298 mov      r0, r1
0036629c mov      r1, r3
003662a0 b        #0x366224
_ZN8Animator17_HandleAnimEndingEPN6glitch5scene19ITimelineControllerE
00366224 push     {r4, r5, r6, lr}
00366228 subs     r4, r1, #0
0036622c mov      r5, r0
00366230 beq      #0x366288
00366234 mov      r1, #0x44000000
00366238 add      r1, r1, #0x7a0000
0036623c ldr      r0, [r4, #0x1c]
00366240 bl       #0x30ed6c
00366244 bl       #0x30e4cc
00366248 mov      r1, #0x44000000
0036624c mov      r6, r0
00366250 add      r1, r1, #0x7a0000
00366254 ldr      r0, [r4, #0x2c]
00366258 bl       #0x30ed6c
0036625c bl       #0x30e4cc
00366260 ldr      r3, [r4, #4]
00366264 rsb      r0, r3, r0
00366268 cmp      r0, r6
0036626c movge    r3, #0
00366270 movlt    r3, #1
00366274 cmp      r0, #0
00366278 movlt    r3, #0
0036627c cmp      r3, #0
00366280 rsbne    r3, r0, r6
00366284 str      r3, [r5, #0x68]
00366288 mov      r3, #1
0036628c strb     r3, [r5, #0x88]
00366290 pop      {r4, r5, r6, pc}
_ZN14AnimApplicator13CheckCallbackEPN6glitch5scene19ITimelineControllerE
0036440c push     {r4, lr}
00364410 ldrb     r3, [r0, #0x30]
00364414 mov      r4, r0
00364418 cmp      r3, #0
0036441c beq      #0x364440
00364420 ldr      r3, [r0, #0x34]
00364424 cmp      r3, #0
00364428 beq      #0x364440
0036442c mov      r0, r1
00364430 ldr      r1, [r4, #0x38]
00364434 blx      r3
00364438 mov      r3, #0
0036443c strb     r3, [r4, #0x30]
00364440 pop      {r4, pc}
