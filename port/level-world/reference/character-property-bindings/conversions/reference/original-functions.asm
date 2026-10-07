
# _ZN3sfc6script3lua5ValueC1Ei
0037ca9c: ldr      r2, [pc, #0x78]
0037caa0: ldr      ip, [pc, #0x78]
0037caa4: mov      r3, r0
0037caa8: add      r2, pc, r2
0037caac: ldr      ip, [r2, ip]
0037cab0: push     {r4, r5, r6, lr}
0037cab4: add      ip, ip, #8
0037cab8: mov      r4, r0
0037cabc: str      ip, [r3], #0xc
0037cac0: mov      r5, r1
0037cac4: mov      r0, r3
0037cac8: mov      r1, #0x10
0037cacc: str      r3, [r4, #0x1c]
0037cad0: str      r3, [r4, #0x20]
0037cad4: bl       #0x31167c
0037cad8: ldr      r2, [r4, #0x1c]
0037cadc: add      r3, r4, #0x24
0037cae0: mov      r6, #0
0037cae4: strb     r6, [r2]
0037cae8: mov      r0, r3
0037caec: str      r3, [r4, #0x64]
0037caf0: str      r3, [r4, #0x68]
0037caf4: bl       #0x37be44
0037caf8: ldr      r3, [r4, #0x64]
0037cafc: mov      r0, r5
0037cb00: str      r6, [r3]
0037cb04: bl       #0x30e964
0037cb08: mov      r1, r0
0037cb0c: mov      r0, r4
0037cb10: bl       #0x31b5e8
0037cb14: mov      r0, r4
0037cb18: pop      {r4, r5, r6, pc}
0037cb1c: rsbeq    r7, r1, r8, ror #31
0037cb20: muleq    r0, r8, r7

# _ZNK3sfc6script3lua5Value9getNumberEv
0031bbf0: push     {r4, r5, r6, lr}
0031bbf4: ldr      r3, [r0, #4]
0031bbf8: mov      r5, r0
0031bbfc: cmp      r3, #0
0031bc00: beq      #0x31bc2c
0031bc04: cmp      r3, #1
0031bc08: beq      #0x31bc38
0031bc0c: cmp      r3, #3
0031bc10: beq      #0x31bc38
0031bc14: cmp      r3, #2
0031bc18: beq      #0x31bc44
0031bc1c: cmp      r3, #7
0031bc20: beq      #0x31bc44
0031bc24: cmp      r3, #4
0031bc28: beq      #0x31bc54
0031bc2c: mov      r5, #0
0031bc30: mov      r0, r5
0031bc34: pop      {r4, r5, r6, pc}
0031bc38: ldr      r5, [r5, #8]
0031bc3c: mov      r0, r5
0031bc40: pop      {r4, r5, r6, pc}
0031bc44: ldr      r0, [r5, #0x6c]
0031bc48: bl       #0x30e2e0
0031bc4c: mov      r5, r0
0031bc50: b        #0x31bc30
0031bc54: bl       #0x84c7e0
0031bc58: ldr      r1, [r5, #0x20]
0031bc5c: mov      r4, r0
0031bc60: bl       #0x84c04c
0031bc64: mov      r0, r4
0031bc68: mvn      r1, #0
0031bc6c: bl       #0x84c450
0031bc70: mov      r5, r0
0031bc74: mov      r0, r4
0031bc78: bl       #0x85797c
0031bc7c: b        #0x31bc30
