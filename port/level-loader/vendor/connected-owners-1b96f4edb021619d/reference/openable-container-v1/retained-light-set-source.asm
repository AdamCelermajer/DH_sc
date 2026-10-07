_ZN12VisualObject13ApplyLightSetEv
00471214 ldr      ip, [pc, #0x28]
00471218 ldr      r3, [pc, #0x28]
0047121c str      r4, [sp, #-4]!
00471220 add      ip, pc, ip
00471224 ldr      r2, [ip, r3]
00471228 ldr      r1, [r0, #0x40]
0047122c ldr      r3, [r0, #8]
00471230 ldr      r4, [r2, #0x10]
00471234 add      r2, r0, #0x44
00471238 ldr      r0, [r4, #0x1c]
0047123c ldm      sp!, {r4}
00471240 b        #0x3549a0
00471244 subseq   r3, r2, r0, ror r8
00471248 strdeq   r3, r4, [r0], -r4
