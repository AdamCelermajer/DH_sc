_ZN18SimpleTypePropertyISt6vectorI7Point3DIfESaIS2_EEE10FromStringEPvPKc
003f8fc8 push     {r4, r5, r6, lr}
003f8fcc ldr      r4, [r0, #4]
003f8fd0 sub      sp, sp, #0x10
003f8fd4 add      r5, sp, #4
003f8fd8 add      r4, r1, r4
003f8fdc mov      r3, #0
003f8fe0 mov      r1, r5
003f8fe4 mov      r0, r4
003f8fe8 mov      r6, r2
003f8fec str      r3, [sp, #0xc]
003f8ff0 str      r3, [sp, #4]
003f8ff4 str      r3, [sp, #8]
003f8ff8 bl       #0x3f8c40 ; _ZNSt6vectorI7Point3DIfESaIS1_EEaSERKS3_
003f8ffc mov      r0, r5
003f9000 bl       #0x34611c ; _ZNSt6vectorI7Point3DIfESaIS1_EED1Ev
003f9004 mov      r0, r6
003f9008 mov      r1, r4
003f900c bl       #0x30f010 ; _Z8StrToObjPKcRSt6vectorI7Point3DIfESaIS3_EE
003f9010 add      sp, sp, #0x10
003f9014 pop      {r4, r5, r6, pc}
_ZN18SimpleTypePropertyISt6vectorI7Point3DIfESaIS2_EEE17SetToDefaultValueEPv
003f8eb4 mov      r3, r0
003f8eb8 ldr      r0, [r0, #4]
003f8ebc add      r0, r1, r0
003f8ec0 add      r1, r3, #0x20
003f8ec4 b        #0x3f8c40
