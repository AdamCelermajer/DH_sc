
C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so:	file format elf32-littlearm

Disassembly of section .text:

003f1008 <Level::_LoadCamera()>:
  3f1008: e92d4ff0     	push	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3f100c: e3a01000     	mov	r1, #0
  3f1010: e24dd014     	sub	sp, sp, #20
  3f1014: e1a04000     	mov	r4, r0
  3f1018: e3a00028     	mov	r0, #40
  3f101c: ebfc7d53     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0xe0ab4
  3f1020: e1a06000     	mov	r6, r0
  3f1024: eb008010     	bl	0x41106c <CameraOverview::CameraOverview()> @ imm = #0x20040
  3f1028: e584612c     	str	r6, [r4, #0x12c]
  3f102c: e3a01000     	mov	r1, #0
  3f1030: e3a000a8     	mov	r0, #168
  3f1034: ebfc7d4d     	bl	0x310570 <operator new(unsigned int, MemoryHintState)> @ imm = #-0xe0acc
  3f1038: e59f5308     	ldr	r5, [pc, #0x308]        @ 0x3f1348 <Level::_LoadCamera()+0x340>
  3f103c: e1a06000     	mov	r6, r0
  3f1040: eb007bca     	bl	0x40ff70 <CameraLevel::CameraLevel()> @ imm = #0x1ef28
  3f1044: e3560000     	cmp	r6, #0
  3f1048: e5846128     	str	r6, [r4, #0x128]
  3f104c: e08f5005     	add	r5, pc, r5
  3f1050: 0a0000a6     	beq	0x3f12f0 <Level::_LoadCamera()+0x2e8> @ imm = #0x298
  3f1054: e5947038     	ldr	r7, [r4, #0x38]
  3f1058: e3570000     	cmp	r7, #0
  3f105c: 0a000077     	beq	0x3f1240 <Level::_LoadCamera()+0x238> @ imm = #0x1dc
  3f1060: e5973260     	ldr	r3, [r7, #0x260]
  3f1064: e58d300c     	str	r3, [sp, #0xc]
  3f1068: e59f32dc     	ldr	r3, [pc, #0x2dc]        @ 0x3f134c <Level::_LoadCamera()+0x344>
  3f106c: e5979278     	ldr	r9, [r7, #0x278]
  3f1070: e7953003     	ldr	r3, [r5, r3]
  3f1074: e593a000     	ldr	r10, [r3]
  3f1078: e35a0000     	cmp	r10, #0
  3f107c: 0a00002b     	beq	0x3f1130 <Level::_LoadCamera()+0x128> @ imm = #0xac
  3f1080: e59f32c8     	ldr	r3, [pc, #0x2c8]        @ 0x3f1350 <Level::_LoadCamera()+0x348>
  3f1084: e3a08000     	mov	r8, #0
  3f1088: e7953003     	ldr	r3, [r5, r3]
  3f108c: e593b000     	ldr	r11, [r3]
  3f1090: ea000002     	b	0x3f10a0 <Level::_LoadCamera()+0x98> @ imm = #0x8
  3f1094: e2888001     	add	r8, r8, #1
  3f1098: e158000a     	cmp	r8, r10
  3f109c: 0a000023     	beq	0x3f1130 <Level::_LoadCamera()+0x128> @ imm = #0x8c
  3f10a0: e79b1108     	ldr	r1, [r11, r8, lsl #2]
  3f10a4: e1a00009     	mov	r0, r9
  3f10a8: ebfc749b     	bl	0x30e31c <.plt+0x5a8>   @ imm = #-0xe2d94
  3f10ac: e3500000     	cmp	r0, #0
  3f10b0: 1afffff7     	bne	0x3f1094 <Level::_LoadCamera()+0x8c> @ imm = #-0x24
  3f10b4: e1a00006     	mov	r0, r6
  3f10b8: e5973290     	ldr	r3, [r7, #0x290]
  3f10bc: e1a02008     	mov	r2, r8
  3f10c0: e59d100c     	ldr	r1, [sp, #0xc]
  3f10c4: eb007d70     	bl	0x41068c <CameraLevel::Load(char const*, int, char const*)> @ imm = #0x1f5c0
  3f10c8: e5947038     	ldr	r7, [r4, #0x38]
  3f10cc: e5946128     	ldr	r6, [r4, #0x128]
  3f10d0: e3570000     	cmp	r7, #0
  3f10d4: 1a00001e     	bne	0x3f1154 <Level::_LoadCamera()+0x14c> @ imm = #0x78
  3f10d8: e59f3274     	ldr	r3, [pc, #0x274]        @ 0x3f1354 <Level::_LoadCamera()+0x34c>
  3f10dc: e7953003     	ldr	r3, [r5, r3]
  3f10e0: e5933000     	ldr	r3, [r3]
  3f10e4: e3530002     	cmp	r3, #2
  3f10e8: 05877000     	streq	r7, [r7]
  3f10ec: 0a000018     	beq	0x3f1154 <Level::_LoadCamera()+0x14c> @ imm = #0x60
  3f10f0: e3530001     	cmp	r3, #1
  3f10f4: 1a000016     	bne	0x3f1154 <Level::_LoadCamera()+0x14c> @ imm = #0x58
  3f10f8: e59f0258     	ldr	r0, [pc, #0x258]        @ 0x3f1358 <Level::_LoadCamera()+0x350>
  3f10fc: e59f1258     	ldr	r1, [pc, #0x258]        @ 0x3f135c <Level::_LoadCamera()+0x354>
  3f1100: e59f2258     	ldr	r2, [pc, #0x258]        @ 0x3f1360 <Level::_LoadCamera()+0x358>
  3f1104: e7950000     	ldr	r0, [r5, r0]
  3f1108: e59f3254     	ldr	r3, [pc, #0x254]        @ 0x3f1364 <Level::_LoadCamera()+0x35c>
  3f110c: e3a0cf77     	mov	r12, #476
  3f1110: e08f1001     	add	r1, pc, r1
  3f1114: e28000a8     	add	r0, r0, #168
  3f1118: e08f2002     	add	r2, pc, r2
  3f111c: e08f3003     	add	r3, pc, r3
  3f1120: e58dc000     	str	r12, [sp]
  3f1124: ebfc73b6     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xe3128
  3f1128: e5947038     	ldr	r7, [r4, #0x38]
  3f112c: ea000008     	b	0x3f1154 <Level::_LoadCamera()+0x14c> @ imm = #0x20
  3f1130: e1a00006     	mov	r0, r6
  3f1134: e5973290     	ldr	r3, [r7, #0x290]
  3f1138: e3e02000     	mvn	r2, #0
  3f113c: e59d100c     	ldr	r1, [sp, #0xc]
  3f1140: eb007d51     	bl	0x41068c <CameraLevel::Load(char const*, int, char const*)> @ imm = #0x1f544
  3f1144: e5947038     	ldr	r7, [r4, #0x38]
  3f1148: e5946128     	ldr	r6, [r4, #0x128]
  3f114c: e3570000     	cmp	r7, #0
  3f1150: 0affffe0     	beq	0x3f10d8 <Level::_LoadCamera()+0xd0> @ imm = #-0x80
  3f1154: e5970294     	ldr	r0, [r7, #0x294]
  3f1158: ebfc7601     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0xe27fc
  3f115c: e1a08000     	mov	r8, r0
  3f1160: e5970298     	ldr	r0, [r7, #0x298]
  3f1164: ebfc75fe     	bl	0x30e964 <.plt+0xbf0>   @ imm = #-0xe2808
  3f1168: e30f1877     	movw	r1, #0xf877
  3f116c: e30728e9     	movw	r2, #0x78e9
  3f1170: e1a03008     	mov	r3, r8
  3f1174: e3431edb     	movt	r1, #0x3edb
  3f1178: e3432fd5     	movt	r2, #0x3fd5
  3f117c: e58d0000     	str	r0, [sp]
  3f1180: e1a00006     	mov	r0, r6
  3f1184: e3a06000     	mov	r6, #0
  3f1188: e58d6004     	str	r6, [sp, #0x4]
  3f118c: eb007605     	bl	0x40e9a8 <CameraBase::SetData(float, float, float, float, bool)> @ imm = #0x1d814
  3f1190: e5940128     	ldr	r0, [r4, #0x128]
  3f1194: eb0078b0     	bl	0x40f45c <CameraBase::SetActive()> @ imm = #0x1e2c0
  3f1198: e59f31c8     	ldr	r3, [pc, #0x1c8]        @ 0x3f1368 <Level::_LoadCamera()+0x360>
  3f119c: e5940128     	ldr	r0, [r4, #0x128]
  3f11a0: e3a0e01c     	mov	lr, #28
  3f11a4: e7953003     	ldr	r3, [r5, r3]
  3f11a8: e5901080     	ldr	r1, [r0, #0x80]
  3f11ac: e1a02006     	mov	r2, r6
  3f11b0: e593c000     	ldr	r12, [r3]
  3f11b4: e1a03006     	mov	r3, r6
  3f11b8: e021c19e     	mla	r1, lr, r1, r12
  3f11bc: e5911010     	ldr	r1, [r1, #0x10]
  3f11c0: eb0079cf     	bl	0x40f904 <CameraLevel::PlayAnim(int, int, bool)> @ imm = #0x1e73c
  3f11c4: e59f31a0     	ldr	r3, [pc, #0x1a0]        @ 0x3f136c <Level::_LoadCamera()+0x364>
  3f11c8: e1a01006     	mov	r1, r6
  3f11cc: e3a02001     	mov	r2, #1
  3f11d0: e7957003     	ldr	r7, [r5, r3]
  3f11d4: e5948128     	ldr	r8, [r4, #0x128]
  3f11d8: e5970040     	ldr	r0, [r7, #0x40]
  3f11dc: ebfdf4a5     	bl	0x36e478 <PlayerManager::GetLocalPlayer(int, bool)> @ imm = #-0x82d6c
  3f11e0: e1a02006     	mov	r2, r6
  3f11e4: e5901660     	ldr	r1, [r0, #0x660]
  3f11e8: e1a00008     	mov	r0, r8
  3f11ec: eb0081f4     	bl	0x4119c4 <CameraTarget::SetTarget(GameObject*, int)> @ imm = #0x207d0
  3f11f0: e5970050     	ldr	r0, [r7, #0x50]
  3f11f4: e5941128     	ldr	r1, [r4, #0x128]
  3f11f8: ebfe436c     	bl	0x381fb0 <ZoomHandler::setCamera(CameraLevel*)> @ imm = #-0x6f250
  3f11fc: e5943128     	ldr	r3, [r4, #0x128]
  3f1200: e3a025fe     	mov	r2, #1065353216
  3f1204: e583208c     	str	r2, [r3, #0x8c]
  3f1208: e5943128     	ldr	r3, [r4, #0x128]
  3f120c: e3a02000     	mov	r2, #0
  3f1210: e5832088     	str	r2, [r3, #0x88]
  3f1214: e5943038     	ldr	r3, [r4, #0x38]
  3f1218: e5972010     	ldr	r2, [r7, #0x10]
  3f121c: e1530006     	cmp	r3, r6
  3f1220: e592601c     	ldr	r6, [r2, #0x1c]
  3f1224: 0a00001b     	beq	0x3f1298 <Level::_LoadCamera()+0x290> @ imm = #0x6c
  3f1228: e5931248     	ldr	r1, [r3, #0x248]
  3f122c: e1a00006     	mov	r0, r6
  3f1230: e3a02000     	mov	r2, #0
  3f1234: e28dd014     	add	sp, sp, #20
  3f1238: e8bd4ff0     	pop	{r4, r5, r6, r7, r8, r9, r10, r11, lr}
  3f123c: eafda1fd     	b	0x359a38 <SceneManager::AddSkyBoxSceneNode(char const*, char const*)> @ imm = #-0x9780c
  3f1240: e59f310c     	ldr	r3, [pc, #0x10c]        @ 0x3f1354 <Level::_LoadCamera()+0x34c>
  3f1244: e7953003     	ldr	r3, [r5, r3]
  3f1248: e5933000     	ldr	r3, [r3]
  3f124c: e3530002     	cmp	r3, #2
  3f1250: 05877000     	streq	r7, [r7]
  3f1254: 0affff81     	beq	0x3f1060 <Level::_LoadCamera()+0x58> @ imm = #-0x1fc
  3f1258: e3530001     	cmp	r3, #1
  3f125c: 1affff7f     	bne	0x3f1060 <Level::_LoadCamera()+0x58> @ imm = #-0x204
  3f1260: e59f00f0     	ldr	r0, [pc, #0xf0]         @ 0x3f1358 <Level::_LoadCamera()+0x350>
  3f1264: e59f1104     	ldr	r1, [pc, #0x104]        @ 0x3f1370 <Level::_LoadCamera()+0x368>
  3f1268: e59f2104     	ldr	r2, [pc, #0x104]        @ 0x3f1374 <Level::_LoadCamera()+0x36c>
  3f126c: e7950000     	ldr	r0, [r5, r0]
  3f1270: e59f3100     	ldr	r3, [pc, #0x100]        @ 0x3f1378 <Level::_LoadCamera()+0x370>
  3f1274: e3a0cf77     	mov	r12, #476
  3f1278: e08f1001     	add	r1, pc, r1
  3f127c: e28000a8     	add	r0, r0, #168
  3f1280: e08f2002     	add	r2, pc, r2
  3f1284: e08f3003     	add	r3, pc, r3
  3f1288: e58dc000     	str	r12, [sp]
  3f128c: ebfc735c     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xe3290
  3f1290: e5947038     	ldr	r7, [r4, #0x38]
  3f1294: eaffff71     	b	0x3f1060 <Level::_LoadCamera()+0x58> @ imm = #-0x23c
  3f1298: e59f20b4     	ldr	r2, [pc, #0xb4]         @ 0x3f1354 <Level::_LoadCamera()+0x34c>
  3f129c: e7952002     	ldr	r2, [r5, r2]
  3f12a0: e5922000     	ldr	r2, [r2]
  3f12a4: e3520002     	cmp	r2, #2
  3f12a8: 05833000     	streq	r3, [r3]
  3f12ac: 0affffdd     	beq	0x3f1228 <Level::_LoadCamera()+0x220> @ imm = #-0x8c
  3f12b0: e3520001     	cmp	r2, #1
  3f12b4: 1affffdb     	bne	0x3f1228 <Level::_LoadCamera()+0x220> @ imm = #-0x94
  3f12b8: e59f0098     	ldr	r0, [pc, #0x98]         @ 0x3f1358 <Level::_LoadCamera()+0x350>
  3f12bc: e59f10b8     	ldr	r1, [pc, #0xb8]         @ 0x3f137c <Level::_LoadCamera()+0x374>
  3f12c0: e59f20b8     	ldr	r2, [pc, #0xb8]         @ 0x3f1380 <Level::_LoadCamera()+0x378>
  3f12c4: e7950000     	ldr	r0, [r5, r0]
  3f12c8: e59f30b4     	ldr	r3, [pc, #0xb4]         @ 0x3f1384 <Level::_LoadCamera()+0x37c>
  3f12cc: e3a0cf77     	mov	r12, #476
  3f12d0: e08f1001     	add	r1, pc, r1
  3f12d4: e08f3003     	add	r3, pc, r3
  3f12d8: e28000a8     	add	r0, r0, #168
  3f12dc: e08f2002     	add	r2, pc, r2
  3f12e0: e58dc000     	str	r12, [sp]
  3f12e4: ebfc7346     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xe32e8
  3f12e8: e5943038     	ldr	r3, [r4, #0x38]
  3f12ec: eaffffcd     	b	0x3f1228 <Level::_LoadCamera()+0x220> @ imm = #-0xcc
  3f12f0: e59f305c     	ldr	r3, [pc, #0x5c]         @ 0x3f1354 <Level::_LoadCamera()+0x34c>
  3f12f4: e7953003     	ldr	r3, [r5, r3]
  3f12f8: e5933000     	ldr	r3, [r3]
  3f12fc: e3530002     	cmp	r3, #2
  3f1300: 05866000     	streq	r6, [r6]
  3f1304: 0affff52     	beq	0x3f1054 <Level::_LoadCamera()+0x4c> @ imm = #-0x2b8
  3f1308: e3530001     	cmp	r3, #1
  3f130c: 1affff50     	bne	0x3f1054 <Level::_LoadCamera()+0x4c> @ imm = #-0x2c0
  3f1310: e59f0040     	ldr	r0, [pc, #0x40]         @ 0x3f1358 <Level::_LoadCamera()+0x350>
  3f1314: e59f106c     	ldr	r1, [pc, #0x6c]         @ 0x3f1388 <Level::_LoadCamera()+0x380>
  3f1318: e59f206c     	ldr	r2, [pc, #0x6c]         @ 0x3f138c <Level::_LoadCamera()+0x384>
  3f131c: e7950000     	ldr	r0, [r5, r0]
  3f1320: e59f3068     	ldr	r3, [pc, #0x68]         @ 0x3f1390 <Level::_LoadCamera()+0x388>
  3f1324: e3a0ce9b     	mov	r12, #2480
  3f1328: e08f1001     	add	r1, pc, r1
  3f132c: e28000a8     	add	r0, r0, #168
  3f1330: e08f2002     	add	r2, pc, r2
  3f1334: e08f3003     	add	r3, pc, r3
  3f1338: e58dc000     	str	r12, [sp]
  3f133c: ebfc7330     	bl	0x30e004 <.plt+0x290>   @ imm = #-0xe3340
  3f1340: e5946128     	ldr	r6, [r4, #0x128]
  3f1344: eaffff42     	b	0x3f1054 <Level::_LoadCamera()+0x4c> @ imm = #-0x2f8
  3f1348: 44 3a 5a 00  	.word	0x005a3a44
  3f134c: e4 38 00 00  	.word	0x000038e4
  3f1350: 5c 3a 00 00  	.word	0x00003a5c
  3f1354: c0 39 00 00  	.word	0x000039c0
  3f1358: c0 19 00 00  	.word	0x000019c0
  3f135c: c8 d2 4c 00  	.word	0x004cd2c8
  3f1360: d8 4a 4d 00  	.word	0x004d4ad8
  3f1364: 3c 0b 4d 00  	.word	0x004d0b3c
  3f1368: d4 3d 00 00  	.word	0x00003dd4
  3f136c: f4 37 00 00  	.word	0x000037f4
  3f1370: 60 d1 4c 00  	.word	0x004cd160
  3f1374: 70 49 4d 00  	.word	0x004d4970
  3f1378: d4 09 4d 00  	.word	0x004d09d4
  3f137c: 08 d1 4c 00  	.word	0x004cd108
  3f1380: 14 49 4d 00  	.word	0x004d4914
  3f1384: 84 09 4d 00  	.word	0x004d0984
  3f1388: b0 d0 4c 00  	.word	0x004cd0b0
  3f138c: a8 52 4d 00  	.word	0x004d52a8
  3f1390: dc 51 4d 00  	.word	0x004d51dc
