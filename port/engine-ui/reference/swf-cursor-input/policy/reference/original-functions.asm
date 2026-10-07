
# _ZN7gameswf15sprite_instance22can_handle_mouse_eventEv
0077f188: push     {r4, lr}
0077f18c: ldr      r3, [r0]
0077f190: mov      r4, r0
0077f194: mov      lr, pc
0077f198: ldr      pc, [r3, #0x170]
0077f19c: cmp      r0, #0
0077f1a0: beq      #0x77f1b8
0077f1a4: ldrb     r3, [r4, #0x9c]
0077f1a8: cmp      r3, #0
0077f1ac: beq      #0x77f1bc
0077f1b0: mov      r0, #1
0077f1b4: pop      {r4, pc}
0077f1b8: pop      {r4, pc}
0077f1bc: mov      r0, r4
0077f1c0: pop      {r4, lr}
0077f1c4: b        #0x7a8cd8

# _Z38sprite_can_handle_mouse_event_callbackPN7gameswf9characterE
007a8cd8: b        #0x7a8c6c
