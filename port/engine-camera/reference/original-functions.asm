; Selected exact ARM listing ranges recovered from the supplied APK.
; Annotated disassembly is for analysis and is not assembler-ready source.
; APK SHA-256: 32c2d027b585a42547311cd95da6a3975fdb3174e663e513d42a7f49d1a4c200
; ELF SHA-256: 36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80

; ===== CAMERA =====
; Evidence package domain: camera
; ELF VA 0x0035bec0, file offset 0x0035bec0, range size 8, SHA-256 d33acb3b11fc0edff4e8bc374c40ecdf2bf83f2d13de1e16c034fec3a5812971
; Index name: glitch::scene::ICameraSceneNode::isOrthogonal() const
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_ICameraSceneNode-ccfaf55908f4-001.asm
; FUNCTION 0x0035bec0, declared_size=8, range_size=8, mode=arm
; class-group: glitch::scene::ICameraSceneNode
; alias: _ZNK6glitch5scene16ICameraSceneNode12isOrthogonalEv
; demangled: glitch::scene::ICameraSceneNode::isOrthogonal() const
; decoder-mode: arm
0035bec0  34 01 d0 e5                                      ldrb r0, [r0, #0x134]
0035bec4  1e ff 2f e1                                      bx lr
; Evidence package domain: camera
; ELF VA 0x00581f50, file offset 0x00581f50, range size 8, SHA-256 b8fbfc167c5e7f38d951aacdf1a1b7b07b4b1d92b31083b0ef150b9e2e3fface
; Index name: glitch::scene::CCameraSceneNode::getProjectionMatrix() const
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x00581f50, declared_size=8, range_size=8, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZNK6glitch5scene16CCameraSceneNode19getProjectionMatrixEv
; demangled: glitch::scene::CCameraSceneNode::getProjectionMatrix() const
; decoder-mode: arm
00581f50  9d 0f 80 e2                                      add r0, r0, #0x274
00581f54  1e ff 2f e1                                      bx lr
; Evidence package domain: camera
; ELF VA 0x00581f58, file offset 0x00581f58, range size 8, SHA-256 4bd2e26363256d8e4e7dd6185c8874997ea6b62e5828d33ae1a47a1930a3f152
; Index name: glitch::scene::CCameraSceneNode::getViewMatrix() const
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x00581f58, declared_size=8, range_size=8, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZNK6glitch5scene16CCameraSceneNode13getViewMatrixEv
; demangled: glitch::scene::CCameraSceneNode::getViewMatrix() const
; decoder-mode: arm
00581f58  7b 0f 80 e2                                      add r0, r0, #0x1ec
00581f5c  1e ff 2f e1                                      bx lr
; Evidence package domain: camera
; ELF VA 0x00581fe4, file offset 0x00581fe4, range size 28, SHA-256 6f9de9475f4f9a6278cda29880f85abe9878260192ecc19b9602485dda5aab04
; Index name: glitch::scene::CCameraSceneNode::setTarget(glitch::core::vector3d<float> const&)
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x00581fe4, declared_size=28, range_size=28, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZN6glitch5scene16CCameraSceneNode9setTargetERKNS_4core8vector3dIfEE
; demangled: glitch::scene::CCameraSceneNode::setTarget(glitch::core::vector3d<float> const&)
; decoder-mode: arm
00581fe4  00 30 91 e5                                      ldr r3, [r1]
00581fe8  38 31 80 e5                                      str r3, [r0, #0x138]
00581fec  04 30 91 e5                                      ldr r3, [r1, #4]
00581ff0  3c 31 80 e5                                      str r3, [r0, #0x13c]
00581ff4  08 30 91 e5                                      ldr r3, [r1, #8]
00581ff8  40 31 80 e5                                      str r3, [r0, #0x140]
00581ffc  1e ff 2f e1                                      bx lr
; Evidence package domain: camera
; ELF VA 0x00582000, file offset 0x00582000, range size 8, SHA-256 bb63f94cf3db0bd399d8f255ad66835a8ccb27ae84f36f56fbb06b73ed1d5820
; Index name: glitch::scene::CCameraSceneNode::getTarget() const
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x00582000, declared_size=8, range_size=8, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZNK6glitch5scene16CCameraSceneNode9getTargetEv
; demangled: glitch::scene::CCameraSceneNode::getTarget() const
; decoder-mode: arm
00582000  4e 0f 80 e2                                      add r0, r0, #0x138
00582004  1e ff 2f e1                                      bx lr
; Evidence package domain: camera
; ELF VA 0x00582008, file offset 0x00582008, range size 28, SHA-256 ae230c37b671ee42f4456c92e524274be3446e1993a6635ff252801b78ba28e4
; Index name: glitch::scene::CCameraSceneNode::setUpVector(glitch::core::vector3d<float> const&)
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x00582008, declared_size=28, range_size=28, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZN6glitch5scene16CCameraSceneNode11setUpVectorERKNS_4core8vector3dIfEE
; demangled: glitch::scene::CCameraSceneNode::setUpVector(glitch::core::vector3d<float> const&)
; decoder-mode: arm
00582008  00 30 91 e5                                      ldr r3, [r1]
0058200c  44 31 80 e5                                      str r3, [r0, #0x144]
00582010  04 30 91 e5                                      ldr r3, [r1, #4]
00582014  48 31 80 e5                                      str r3, [r0, #0x148]
00582018  08 30 91 e5                                      ldr r3, [r1, #8]
0058201c  4c 31 80 e5                                      str r3, [r0, #0x14c]
00582020  1e ff 2f e1                                      bx lr
; Evidence package domain: camera
; ELF VA 0x00582024, file offset 0x00582024, range size 8, SHA-256 42988e395c2803c08d1950fd6301ba34bcd61fffcacf76b58db3d6de74bc67a9
; Index name: glitch::scene::CCameraSceneNode::getUpVector() const
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x00582024, declared_size=8, range_size=8, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZNK6glitch5scene16CCameraSceneNode11getUpVectorEv
; demangled: glitch::scene::CCameraSceneNode::getUpVector() const
; decoder-mode: arm
00582024  51 0f 80 e2                                      add r0, r0, #0x144
00582028  1e ff 2f e1                                      bx lr
; Evidence package domain: camera
; ELF VA 0x0058202c, file offset 0x0058202c, range size 8, SHA-256 a5eafc36f627aec1cd302b6af11f627534652c80c8f85a51204fa02e0dee828a
; Index name: glitch::scene::CCameraSceneNode::getNearValue() const
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x0058202c, declared_size=8, range_size=8, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZNK6glitch5scene16CCameraSceneNode12getNearValueEv
; demangled: glitch::scene::CCameraSceneNode::getNearValue() const
; decoder-mode: arm
0058202c  5c 01 90 e5                                      ldr r0, [r0, #0x15c]
00582030  1e ff 2f e1                                      bx lr
; Evidence package domain: camera
; ELF VA 0x00582034, file offset 0x00582034, range size 8, SHA-256 5b27900d814741f101691acb0dafa30057ce6a93c990b755ce0f1f307b2b7c1b
; Index name: glitch::scene::CCameraSceneNode::getFarValue() const
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x00582034, declared_size=8, range_size=8, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZNK6glitch5scene16CCameraSceneNode11getFarValueEv
; demangled: glitch::scene::CCameraSceneNode::getFarValue() const
; decoder-mode: arm
00582034  60 01 90 e5                                      ldr r0, [r0, #0x160]
00582038  1e ff 2f e1                                      bx lr
; Evidence package domain: camera
; ELF VA 0x0058203c, file offset 0x0058203c, range size 8, SHA-256 27adca1bd53a42b11acb6e62589f114f5278586dd7fd8e8816ff47c8669c6c0d
; Index name: glitch::scene::CCameraSceneNode::getAspectRatio() const
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x0058203c, declared_size=8, range_size=8, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZNK6glitch5scene16CCameraSceneNode14getAspectRatioEv
; demangled: glitch::scene::CCameraSceneNode::getAspectRatio() const
; decoder-mode: arm
0058203c  58 01 90 e5                                      ldr r0, [r0, #0x158]
00582040  1e ff 2f e1                                      bx lr
; Evidence package domain: camera
; ELF VA 0x00582044, file offset 0x00582044, range size 8, SHA-256 faf590b327b25a75edd3f0fe4b99d5b1f894b58cf3d8821664b5a9a761984586
; Index name: glitch::scene::CCameraSceneNode::getFOV() const
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x00582044, declared_size=8, range_size=8, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZNK6glitch5scene16CCameraSceneNode6getFOVEv
; demangled: glitch::scene::CCameraSceneNode::getFOV() const
; decoder-mode: arm
00582044  54 01 90 e5                                      ldr r0, [r0, #0x154]
00582048  1e ff 2f e1                                      bx lr
; Evidence package domain: camera
; ELF VA 0x0058204c, file offset 0x0058204c, range size 8, SHA-256 ba0df33c88fb7336cd5ddf699b8c518eabfbcf48fd0ff1fd08553661328d88e6
; Index name: glitch::scene::CCameraSceneNode::getFarToInifinity()
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x0058204c, declared_size=8, range_size=8, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZN6glitch5scene16CCameraSceneNode17getFarToInifinityEv
; demangled: glitch::scene::CCameraSceneNode::getFarToInifinity()
; decoder-mode: arm
0058204c  64 01 d0 e5                                      ldrb r0, [r0, #0x164]
00582050  1e ff 2f e1                                      bx lr
; Evidence package domain: camera
; ELF VA 0x00582054, file offset 0x00582054, range size 24, SHA-256 63f49633645549a5ba8d67e90803f3ea990fc70aa9c8d6d100ad0f70e49591c8
; Index name: glitch::scene::CCameraSceneNode::setNearValue(float)
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x00582054, declared_size=24, range_size=24, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZN6glitch5scene16CCameraSceneNode12setNearValueEf
; demangled: glitch::scene::CCameraSceneNode::setNearValue(float)
; decoder-mode: arm
00582054  10 40 2d e9                                      push {r4, lr}
00582058  5c 11 80 e5                                      str r1, [r0, #0x15c]
0058205c  00 30 90 e5                                      ldr r3, [r0]
00582060  0f e0 a0 e1                                      mov lr, pc
00582064  58 f1 93 e5                                      ldr pc, [r3, #0x158]
00582068  10 80 bd e8                                      pop {r4, pc}
; Evidence package domain: camera
; ELF VA 0x0058206c, file offset 0x0058206c, range size 24, SHA-256 296dd59e5c39c8ddbc78970bdb05a8cb18635b228c2b5fba5f86b5de63896f79
; Index name: glitch::scene::CCameraSceneNode::setFarValue(float)
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x0058206c, declared_size=24, range_size=24, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZN6glitch5scene16CCameraSceneNode11setFarValueEf
; demangled: glitch::scene::CCameraSceneNode::setFarValue(float)
; decoder-mode: arm
0058206c  10 40 2d e9                                      push {r4, lr}
00582070  60 11 80 e5                                      str r1, [r0, #0x160]
00582074  00 30 90 e5                                      ldr r3, [r0]
00582078  0f e0 a0 e1                                      mov lr, pc
0058207c  58 f1 93 e5                                      ldr pc, [r3, #0x158]
00582080  10 80 bd e8                                      pop {r4, pc}
; Evidence package domain: camera
; ELF VA 0x00582084, file offset 0x00582084, range size 24, SHA-256 306af1741ec1545243b628983bb441069a6e9fdc6bf8edd993da7c8df312c8bc
; Index name: glitch::scene::CCameraSceneNode::setAspectRatio(float)
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x00582084, declared_size=24, range_size=24, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZN6glitch5scene16CCameraSceneNode14setAspectRatioEf
; demangled: glitch::scene::CCameraSceneNode::setAspectRatio(float)
; decoder-mode: arm
00582084  10 40 2d e9                                      push {r4, lr}
00582088  58 11 80 e5                                      str r1, [r0, #0x158]
0058208c  00 30 90 e5                                      ldr r3, [r0]
00582090  0f e0 a0 e1                                      mov lr, pc
00582094  58 f1 93 e5                                      ldr pc, [r3, #0x158]
00582098  10 80 bd e8                                      pop {r4, pc}
; Evidence package domain: camera
; ELF VA 0x0058209c, file offset 0x0058209c, range size 24, SHA-256 695bf027165e799297a35c095c0ba244141ec7cbd7c466e0186e0026ba9f6644
; Index name: glitch::scene::CCameraSceneNode::setFOV(float)
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x0058209c, declared_size=24, range_size=24, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZN6glitch5scene16CCameraSceneNode6setFOVEf
; demangled: glitch::scene::CCameraSceneNode::setFOV(float)
; decoder-mode: arm
0058209c  10 40 2d e9                                      push {r4, lr}
005820a0  54 11 80 e5                                      str r1, [r0, #0x154]
005820a4  00 30 90 e5                                      ldr r3, [r0]
005820a8  0f e0 a0 e1                                      mov lr, pc
005820ac  58 f1 93 e5                                      ldr pc, [r3, #0x158]
005820b0  10 80 bd e8                                      pop {r4, pc}
; Evidence package domain: camera
; ELF VA 0x005820b4, file offset 0x005820b4, range size 24, SHA-256 4f73583ebe69df66963292597689b7a763e689f2547ad1248c91afc7625927d6
; Index name: glitch::scene::CCameraSceneNode::setMAG(float)
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x005820b4, declared_size=24, range_size=24, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZN6glitch5scene16CCameraSceneNode6setMAGEf
; demangled: glitch::scene::CCameraSceneNode::setMAG(float)
; decoder-mode: arm
005820b4  10 40 2d e9                                      push {r4, lr}
005820b8  50 11 80 e5                                      str r1, [r0, #0x150]
005820bc  00 30 90 e5                                      ldr r3, [r0]
005820c0  0f e0 a0 e1                                      mov lr, pc
005820c4  58 f1 93 e5                                      ldr pc, [r3, #0x158]
005820c8  10 80 bd e8                                      pop {r4, pc}
; Evidence package domain: camera
; ELF VA 0x005820cc, file offset 0x005820cc, range size 24, SHA-256 22d928e7195f393b9c08ba9d0d8d52e55a81b1e2d934dcbc02cd0e7f07836be4
; Index name: glitch::scene::CCameraSceneNode::setFarToInifinity(bool)
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x005820cc, declared_size=24, range_size=24, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZN6glitch5scene16CCameraSceneNode17setFarToInifinityEb
; demangled: glitch::scene::CCameraSceneNode::setFarToInifinity(bool)
; decoder-mode: arm
005820cc  10 40 2d e9                                      push {r4, lr}
005820d0  64 11 c0 e5                                      strb r1, [r0, #0x164]
005820d4  00 30 90 e5                                      ldr r3, [r0]
005820d8  0f e0 a0 e1                                      mov lr, pc
005820dc  58 f1 93 e5                                      ldr pc, [r3, #0x158]
005820e0  10 80 bd e8                                      pop {r4, pc}
; Evidence package domain: camera
; ELF VA 0x005820e4, file offset 0x005820e4, range size 76, SHA-256 e7f1a9b25b8a791521cc3cb0d78cce7737eb6f1362f52fdf58018ceb8a49ac27
; Index name: glitch::scene::CCameraSceneNode::render(void*)
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x005820e4, declared_size=76, range_size=76, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZN6glitch5scene16CCameraSceneNode6renderEPv
; demangled: glitch::scene::CCameraSceneNode::render(void*)
; decoder-mode: arm
005820e4  70 40 2d e9                                      push {r4, r5, r6, lr}
005820e8  10 31 90 e5                                      ldr r3, [r0, #0x110]
005820ec  00 50 a0 e1                                      mov r5, r0
005820f0  14 40 93 e5                                      ldr r4, [r3, #0x14]
005820f4  00 00 54 e3                                      cmp r4, #0
005820f8  0b 00 00 0a                                      beq #0x58212c
005820fc  04 00 a0 e1                                      mov r0, r4
00582100  02 10 a0 e3                                      mov r1, #2
00582104  9d 2f 85 e2                                      add r2, r5, #0x274
00582108  00 30 94 e5                                      ldr r3, [r4]
0058210c  0f e0 a0 e1                                      mov lr, pc
00582110  6c f0 93 e5                                      ldr pc, [r3, #0x6c]
00582114  04 00 a0 e1                                      mov r0, r4
00582118  7b 2f 85 e2                                      add r2, r5, #0x1ec
0058211c  00 30 94 e5                                      ldr r3, [r4]
00582120  00 10 a0 e3                                      mov r1, #0
00582124  0f e0 a0 e1                                      mov lr, pc
00582128  6c f0 93 e5                                      ldr pc, [r3, #0x6c]
0058212c  70 80 bd e8                                      pop {r4, r5, r6, pc}
; Evidence package domain: camera
; ELF VA 0x00582348, file offset 0x00582348, range size 100, SHA-256 b6fd332ddc7b0d8c9171549fd68ea03f41f44b80e49fd48652685e5c3efa52c2
; Index name: glitch::scene::SViewFrustum::setTransformState(glitch::video::E_TRANSFORMATION_STATE)
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_SViewFrustum-60298c5f0871-001.asm
; FUNCTION 0x00582348, declared_size=100, range_size=100, mode=arm
; class-group: glitch::scene::SViewFrustum
; alias: _ZN6glitch5scene12SViewFrustum17setTransformStateENS_5video22E_TRANSFORMATION_STATEE
; demangled: glitch::scene::SViewFrustum::setTransformState(glitch::video::E_TRANSFORMATION_STATE)
; decoder-mode: arm
00582348  00 00 51 e3                                      cmp r1, #0
0058234c  70 40 2d e9                                      push {r4, r5, r6, lr}
00582350  00 30 a0 e1                                      mov r3, r0
00582354  0c 00 00 1a                                      bne #0x58238c
00582358  84 50 80 e2                                      add r5, r0, #0x84
0058235c  65 4f 80 e2                                      add r4, r0, #0x194
00582360  43 1f 80 e2                                      add r1, r0, #0x10c
00582364  05 20 a0 e1                                      mov r2, r5
00582368  15 0e 80 e2                                      add r0, r0, #0x150
0058236c  b8 31 fa eb                                      bl #0x40ea54
00582370  05 10 a0 e1                                      mov r1, r5
00582374  04 00 a0 e1                                      mov r0, r4
00582378  41 20 a0 e3                                      mov r2, #0x41
0058237c  39 31 f6 eb                                      bl #0x30e868
00582380  04 00 a0 e1                                      mov r0, r4
00582384  70 40 bd e8                                      pop {r4, r5, r6, lr}
00582388  da ff ff ea                                      b #0x5822f8
0058238c  01 00 51 e3                                      cmp r1, #1
00582390  00 00 00 0a                                      beq #0x582398
00582394  70 80 bd e8                                      pop {r4, r5, r6, pc}
00582398  c8 20 80 e2                                      add r2, r0, #0xc8
0058239c  15 1e 83 e2                                      add r1, r3, #0x150
005823a0  76 0f 80 e2                                      add r0, r0, #0x1d8
005823a4  70 40 bd e8                                      pop {r4, r5, r6, lr}
005823a8  c6 ff ff ea                                      b #0x5822c8
; Evidence package domain: camera
; ELF VA 0x005823ac, file offset 0x005823ac, range size 40, SHA-256 af217974639cbf6a4a2a9c6d6d64233d3b88b9f9814fe91e1c004c266ab4e504
; Index name: glitch::scene::CCameraSceneNode::setProjectionMatrix(glitch::core::CMatrix4<float> const&, bool)
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x005823ac, declared_size=40, range_size=40, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZN6glitch5scene16CCameraSceneNode19setProjectionMatrixERKNS_4core8CMatrix4IfEEb
; demangled: glitch::scene::CCameraSceneNode::setProjectionMatrix(glitch::core::CMatrix4<float> const&, bool)
; decoder-mode: arm
005823ac  10 40 2d e9                                      push {r4, lr}
005823b0  00 40 a0 e1                                      mov r4, r0
005823b4  34 21 c0 e5                                      strb r2, [r0, #0x134]
005823b8  41 20 a0 e3                                      mov r2, #0x41
005823bc  9d 0f 80 e2                                      add r0, r0, #0x274
005823c0  28 31 f6 eb                                      bl #0x30e868
005823c4  5a 0f 84 e2                                      add r0, r4, #0x168
005823c8  02 10 a0 e3                                      mov r1, #2
005823cc  10 40 bd e8                                      pop {r4, lr}
005823d0  dc ff ff ea                                      b #0x582348
; Evidence package domain: camera
; ELF VA 0x005826c0, file offset 0x005826c0, range size 580, SHA-256 5270c5a55f1d81173b2f3acc0bfb3633ab7bf4001fbb99878cd740582362e66a
; Index name: glitch::scene::SViewFrustum::setFrom(glitch::core::CMatrix4<float> const&)
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_SViewFrustum-60298c5f0871-001.asm
; FUNCTION 0x005826c0, declared_size=580, range_size=580, mode=arm
; class-group: glitch::scene::SViewFrustum
; alias: _ZN6glitch5scene12SViewFrustum7setFromERKNS_4core8CMatrix4IfEE
; demangled: glitch::scene::SViewFrustum::setFrom(glitch::core::CMatrix4<float> const&)
; decoder-mode: arm
005826c0  f0 47 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, lr}
005826c4  01 50 a0 e1                                      mov r5, r1
005826c8  00 60 a0 e1                                      mov r6, r0
005826cc  00 10 91 e5                                      ldr r1, [r1]
005826d0  0c 00 95 e5                                      ldr r0, [r5, #0xc]
005826d4  32 31 f6 eb                                      bl #0x30eba4
005826d8  2c 00 86 e5                                      str r0, [r6, #0x2c]
005826dc  10 10 95 e5                                      ldr r1, [r5, #0x10]
005826e0  1c 00 95 e5                                      ldr r0, [r5, #0x1c]
005826e4  2e 31 f6 eb                                      bl #0x30eba4
005826e8  30 00 86 e5                                      str r0, [r6, #0x30]
005826ec  20 10 95 e5                                      ldr r1, [r5, #0x20]
005826f0  2c 00 95 e5                                      ldr r0, [r5, #0x2c]
005826f4  2a 31 f6 eb                                      bl #0x30eba4
005826f8  34 00 86 e5                                      str r0, [r6, #0x34]
005826fc  30 10 95 e5                                      ldr r1, [r5, #0x30]
00582700  3c 00 95 e5                                      ldr r0, [r5, #0x3c]
00582704  26 31 f6 eb                                      bl #0x30eba4
00582708  38 00 86 e5                                      str r0, [r6, #0x38]
0058270c  00 10 95 e5                                      ldr r1, [r5]
00582710  0c 00 95 e5                                      ldr r0, [r5, #0xc]
00582714  24 2f f6 eb                                      bl #0x30e3ac
00582718  3c 00 86 e5                                      str r0, [r6, #0x3c]
0058271c  10 10 95 e5                                      ldr r1, [r5, #0x10]
00582720  1c 00 95 e5                                      ldr r0, [r5, #0x1c]
00582724  20 2f f6 eb                                      bl #0x30e3ac
00582728  40 00 86 e5                                      str r0, [r6, #0x40]
0058272c  20 10 95 e5                                      ldr r1, [r5, #0x20]
00582730  2c 00 95 e5                                      ldr r0, [r5, #0x2c]
00582734  1c 2f f6 eb                                      bl #0x30e3ac
00582738  44 00 86 e5                                      str r0, [r6, #0x44]
0058273c  30 10 95 e5                                      ldr r1, [r5, #0x30]
00582740  3c 00 95 e5                                      ldr r0, [r5, #0x3c]
00582744  18 2f f6 eb                                      bl #0x30e3ac
00582748  48 00 86 e5                                      str r0, [r6, #0x48]
0058274c  04 10 95 e5                                      ldr r1, [r5, #4]
00582750  0c 00 95 e5                                      ldr r0, [r5, #0xc]
00582754  14 2f f6 eb                                      bl #0x30e3ac
00582758  5c 00 86 e5                                      str r0, [r6, #0x5c]
0058275c  14 10 95 e5                                      ldr r1, [r5, #0x14]
00582760  1c 00 95 e5                                      ldr r0, [r5, #0x1c]
00582764  10 2f f6 eb                                      bl #0x30e3ac
00582768  60 00 86 e5                                      str r0, [r6, #0x60]
0058276c  24 10 95 e5                                      ldr r1, [r5, #0x24]
00582770  2c 00 95 e5                                      ldr r0, [r5, #0x2c]
00582774  0c 2f f6 eb                                      bl #0x30e3ac
00582778  64 00 86 e5                                      str r0, [r6, #0x64]
0058277c  34 10 95 e5                                      ldr r1, [r5, #0x34]
00582780  3c 00 95 e5                                      ldr r0, [r5, #0x3c]
00582784  08 2f f6 eb                                      bl #0x30e3ac
00582788  68 00 86 e5                                      str r0, [r6, #0x68]
0058278c  04 10 95 e5                                      ldr r1, [r5, #4]
00582790  0c 00 95 e5                                      ldr r0, [r5, #0xc]
00582794  02 31 f6 eb                                      bl #0x30eba4
00582798  4c 00 86 e5                                      str r0, [r6, #0x4c]
0058279c  14 10 95 e5                                      ldr r1, [r5, #0x14]
005827a0  1c 00 95 e5                                      ldr r0, [r5, #0x1c]
005827a4  fe 30 f6 eb                                      bl #0x30eba4
005827a8  50 00 86 e5                                      str r0, [r6, #0x50]
005827ac  24 10 95 e5                                      ldr r1, [r5, #0x24]
005827b0  2c 00 95 e5                                      ldr r0, [r5, #0x2c]
005827b4  fa 30 f6 eb                                      bl #0x30eba4
005827b8  54 00 86 e5                                      str r0, [r6, #0x54]
005827bc  34 10 95 e5                                      ldr r1, [r5, #0x34]
005827c0  3c 00 95 e5                                      ldr r0, [r5, #0x3c]
005827c4  f6 30 f6 eb                                      bl #0x30eba4
005827c8  58 00 86 e5                                      str r0, [r6, #0x58]
005827cc  08 10 95 e5                                      ldr r1, [r5, #8]
005827d0  0c 00 95 e5                                      ldr r0, [r5, #0xc]
005827d4  f4 2e f6 eb                                      bl #0x30e3ac
005827d8  0c 00 86 e5                                      str r0, [r6, #0xc]
005827dc  18 10 95 e5                                      ldr r1, [r5, #0x18]
005827e0  00 90 a0 e1                                      mov sb, r0
005827e4  1c 00 95 e5                                      ldr r0, [r5, #0x1c]
005827e8  ef 2e f6 eb                                      bl #0x30e3ac
005827ec  10 00 86 e5                                      str r0, [r6, #0x10]
005827f0  28 10 95 e5                                      ldr r1, [r5, #0x28]
005827f4  00 a0 a0 e1                                      mov sl, r0
005827f8  2c 00 95 e5                                      ldr r0, [r5, #0x2c]
005827fc  ea 2e f6 eb                                      bl #0x30e3ac
00582800  14 00 86 e5                                      str r0, [r6, #0x14]
00582804  38 10 95 e5                                      ldr r1, [r5, #0x38]
00582808  00 80 a0 e1                                      mov r8, r0
0058280c  3c 00 95 e5                                      ldr r0, [r5, #0x3c]
00582810  e5 2e f6 eb                                      bl #0x30e3ac
00582814  18 00 86 e5                                      str r0, [r6, #0x18]
00582818  08 30 95 e5                                      ldr r3, [r5, #8]
0058281c  06 40 a0 e1                                      mov r4, r6
00582820  00 70 a0 e3                                      mov r7, #0
00582824  1c 30 86 e5                                      str r3, [r6, #0x1c]
00582828  18 30 95 e5                                      ldr r3, [r5, #0x18]
0058282c  20 30 86 e5                                      str r3, [r6, #0x20]
00582830  28 30 95 e5                                      ldr r3, [r5, #0x28]
00582834  24 30 86 e5                                      str r3, [r6, #0x24]
00582838  38 30 95 e5                                      ldr r3, [r5, #0x38]
0058283c  28 30 86 e5                                      str r3, [r6, #0x28]
00582840  02 00 00 ea                                      b #0x582850
00582844  0c 90 94 e5                                      ldr sb, [r4, #0xc]
00582848  10 a0 94 e5                                      ldr sl, [r4, #0x10]
0058284c  14 80 94 e5                                      ldr r8, [r4, #0x14]
00582850  09 10 a0 e1                                      mov r1, sb
00582854  09 00 a0 e1                                      mov r0, sb
00582858  43 31 f6 eb                                      bl #0x30ed6c
0058285c  0a 10 a0 e1                                      mov r1, sl
00582860  00 50 a0 e1                                      mov r5, r0
00582864  0a 00 a0 e1                                      mov r0, sl
00582868  3f 31 f6 eb                                      bl #0x30ed6c
0058286c  00 10 a0 e1                                      mov r1, r0
00582870  05 00 a0 e1                                      mov r0, r5
00582874  ca 30 f6 eb                                      bl #0x30eba4
00582878  08 10 a0 e1                                      mov r1, r8
0058287c  00 50 a0 e1                                      mov r5, r0
00582880  08 00 a0 e1                                      mov r0, r8
00582884  38 31 f6 eb                                      bl #0x30ed6c
00582888  00 10 a0 e1                                      mov r1, r0
0058288c  05 00 a0 e1                                      mov r0, r5
00582890  c3 30 f6 eb                                      bl #0x30eba4
00582894  22 2e f6 eb                                      bl #0x30e124
00582898  00 10 a0 e1                                      mov r1, r0
0058289c  fe 05 a0 e3                                      mov r0, #0x3f800000
005828a0  fb 30 f6 eb                                      bl #0x30ec94
005828a4  02 51 80 e2                                      add r5, r0, #0x80000000
005828a8  05 10 a0 e1                                      mov r1, r5
005828ac  0c 00 94 e5                                      ldr r0, [r4, #0xc]
005828b0  2d 31 f6 eb                                      bl #0x30ed6c
005828b4  05 10 a0 e1                                      mov r1, r5
005828b8  0c 00 84 e5                                      str r0, [r4, #0xc]
005828bc  10 00 94 e5                                      ldr r0, [r4, #0x10]
005828c0  29 31 f6 eb                                      bl #0x30ed6c
005828c4  05 10 a0 e1                                      mov r1, r5
005828c8  10 00 84 e5                                      str r0, [r4, #0x10]
005828cc  14 00 94 e5                                      ldr r0, [r4, #0x14]
005828d0  25 31 f6 eb                                      bl #0x30ed6c
005828d4  05 10 a0 e1                                      mov r1, r5
005828d8  14 00 84 e5                                      str r0, [r4, #0x14]
005828dc  18 00 94 e5                                      ldr r0, [r4, #0x18]
005828e0  21 31 f6 eb                                      bl #0x30ed6c
005828e4  01 70 87 e2                                      add r7, r7, #1
005828e8  06 00 57 e3                                      cmp r7, #6
005828ec  18 00 84 e5                                      str r0, [r4, #0x18]
005828f0  10 40 84 e2                                      add r4, r4, #0x10
005828f4  d2 ff ff 1a                                      bne #0x582844
005828f8  06 00 a0 e1                                      mov r0, r6
005828fc  f0 47 bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, lr}
00582900  b3 fe ff ea                                      b #0x5823d4
; Evidence package domain: camera
; ELF VA 0x00582904, file offset 0x00582904, range size 68, SHA-256 8f9d88ba462ced6ead123459d5d96711003eca42f7a6815f1a50e5be86f98e31
; Index name: glitch::scene::CCameraSceneNode::recalculateViewArea()
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x00582904, declared_size=68, range_size=68, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZN6glitch5scene16CCameraSceneNode19recalculateViewAreaEv
; demangled: glitch::scene::CCameraSceneNode::recalculateViewArea()
; decoder-mode: arm
00582904  10 40 2d e9                                      push {r4, lr}
00582908  10 d0 4d e2                                      sub sp, sp, #0x10
0058290c  00 40 a0 e1                                      mov r4, r0
00582910  00 10 a0 e1                                      mov r1, r0
00582914  04 00 8d e2                                      add r0, sp, #4
00582918  18 52 00 eb                                      bl #0x597180
0058291c  04 10 9d e5                                      ldr r1, [sp, #4]
00582920  08 20 9d e5                                      ldr r2, [sp, #8]
00582924  0c 30 9d e5                                      ldr r3, [sp, #0xc]
00582928  5a 0f 84 e2                                      add r0, r4, #0x168
0058292c  68 11 84 e5                                      str r1, [r4, #0x168]
00582930  6c 21 84 e5                                      str r2, [r4, #0x16c]
00582934  70 31 84 e5                                      str r3, [r4, #0x170]
00582938  ae 1f 84 e2                                      add r1, r4, #0x2b8
0058293c  5f ff ff eb                                      bl #0x5826c0
00582940  10 d0 8d e2                                      add sp, sp, #0x10
00582944  10 80 bd e8                                      pop {r4, pc}
; Evidence package domain: camera
; ELF VA 0x00582c88, file offset 0x00582c88, range size 264, SHA-256 b671d8fd7da7789ff165f09e9ab0e55f6b9976f60921642f89f6e13937dee4c5
; Index name: glitch::core::CMatrix4<float> glitch::core::buildProjectionMatrixPerspectiveFov<float>(float, float, float, float)
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_core_CMatrix4_float_glitch_core-451577a2d7ed-001.asm
; FUNCTION 0x00582c88, declared_size=264, range_size=264, mode=arm
; class-group: glitch::core::CMatrix4<float> glitch::core
; alias: _ZN6glitch4core35buildProjectionMatrixPerspectiveFovIfEENS0_8CMatrix4IT_EES3_S3_S3_S3_
; demangled: glitch::core::CMatrix4<float> glitch::core::buildProjectionMatrixPerspectiveFov<float>(float, float, float, float)
; decoder-mode: arm
00582c88  f0 47 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, lr}
00582c8c  00 40 a0 e1                                      mov r4, r0
00582c90  01 00 a0 e1                                      mov r0, r1
00582c94  03 50 a0 e1                                      mov r5, r3
00582c98  02 60 a0 e1                                      mov r6, r2
00582c9c  00 2f f6 eb                                      bl #0x30e8a4
00582ca0  ff 35 a0 e3                                      mov r3, #0x3fc00000
00582ca4  00 20 a0 e3                                      mov r2, #0
00582ca8  02 36 83 e2                                      add r3, r3, #0x200000
00582cac  80 2f f6 eb                                      bl #0x30eab4
00582cb0  11 2e f6 eb                                      bl #0x30e4fc
00582cb4  01 30 a0 e1                                      mov r3, r1
00582cb8  ff 15 a0 e3                                      mov r1, #0x3fc00000
00582cbc  00 20 a0 e1                                      mov r2, r0
00582cc0  03 16 81 e2                                      add r1, r1, #0x300000
00582cc4  00 00 a0 e3                                      mov r0, #0
00582cc8  9c 2d f6 eb                                      bl #0x30e340
00582ccc  00 80 a0 e1                                      mov r8, r0
00582cd0  06 00 a0 e1                                      mov r0, r6
00582cd4  01 90 a0 e1                                      mov sb, r1
00582cd8  f1 2e f6 eb                                      bl #0x30e8a4
00582cdc  00 20 a0 e1                                      mov r2, r0
00582ce0  01 30 a0 e1                                      mov r3, r1
00582ce4  08 00 a0 e1                                      mov r0, r8
00582ce8  09 10 a0 e1                                      mov r1, sb
00582cec  93 2d f6 eb                                      bl #0x30e340
00582cf0  6a 2e f6 eb                                      bl #0x30e6a0
00582cf4  00 60 a0 e3                                      mov r6, #0
00582cf8  00 00 84 e5                                      str r0, [r4]
00582cfc  09 10 a0 e1                                      mov r1, sb
00582d00  08 00 a0 e1                                      mov r0, r8
00582d04  04 60 84 e5                                      str r6, [r4, #4]
00582d08  08 60 84 e5                                      str r6, [r4, #8]
00582d0c  0c 60 84 e5                                      str r6, [r4, #0xc]
00582d10  10 60 84 e5                                      str r6, [r4, #0x10]
00582d14  61 2e f6 eb                                      bl #0x30e6a0
00582d18  20 70 9d e5                                      ldr r7, [sp, #0x20]
00582d1c  05 10 a0 e1                                      mov r1, r5
00582d20  14 00 84 e5                                      str r0, [r4, #0x14]
00582d24  18 60 84 e5                                      str r6, [r4, #0x18]
00582d28  1c 60 84 e5                                      str r6, [r4, #0x1c]
00582d2c  20 60 84 e5                                      str r6, [r4, #0x20]
00582d30  24 60 84 e5                                      str r6, [r4, #0x24]
00582d34  07 00 a0 e1                                      mov r0, r7
00582d38  9b 2d f6 eb                                      bl #0x30e3ac
00582d3c  00 80 a0 e1                                      mov r8, r0
00582d40  08 10 a0 e1                                      mov r1, r8
00582d44  07 00 a0 e1                                      mov r0, r7
00582d48  d1 2f f6 eb                                      bl #0x30ec94
00582d4c  02 51 85 e2                                      add r5, r5, #0x80000000
00582d50  fe 35 a0 e3                                      mov r3, #0x3f800000
00582d54  2c 30 84 e5                                      str r3, [r4, #0x2c]
00582d58  28 00 84 e5                                      str r0, [r4, #0x28]
00582d5c  07 10 a0 e1                                      mov r1, r7
00582d60  30 60 84 e5                                      str r6, [r4, #0x30]
00582d64  34 60 84 e5                                      str r6, [r4, #0x34]
00582d68  05 00 a0 e1                                      mov r0, r5
00582d6c  fe 2f f6 eb                                      bl #0x30ed6c
00582d70  08 10 a0 e1                                      mov r1, r8
00582d74  c6 2f f6 eb                                      bl #0x30ec94
00582d78  00 30 a0 e3                                      mov r3, #0
00582d7c  38 00 84 e5                                      str r0, [r4, #0x38]
00582d80  40 30 c4 e5                                      strb r3, [r4, #0x40]
00582d84  3c 60 84 e5                                      str r6, [r4, #0x3c]
00582d88  04 00 a0 e1                                      mov r0, r4
00582d8c  f0 87 bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, pc}
; Evidence package domain: camera
; ELF VA 0x00582d90, file offset 0x00582d90, range size 212, SHA-256 978c27da3bbdf35d952b5a6661dad84220b9ca64144bbc34c9d181c539189528
; Index name: glitch::core::CMatrix4<float> glitch::core::buildProjectionMatrixPerspectiveFovInfinity<float>(float, float, float)
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_core_CMatrix4_float_glitch_core-451577a2d7ed-001.asm
; FUNCTION 0x00582d90, declared_size=212, range_size=212, mode=arm
; class-group: glitch::core::CMatrix4<float> glitch::core
; alias: _ZN6glitch4core43buildProjectionMatrixPerspectiveFovInfinityIfEENS0_8CMatrix4IT_EES3_S3_S3_
; demangled: glitch::core::CMatrix4<float> glitch::core::buildProjectionMatrixPerspectiveFovInfinity<float>(float, float, float)
; decoder-mode: arm
00582d90  70 43 2d e9                                      push {r4, r5, r6, r8, sb, lr}
00582d94  00 40 a0 e1                                      mov r4, r0
00582d98  01 00 a0 e1                                      mov r0, r1
00582d9c  02 60 a0 e1                                      mov r6, r2
00582da0  03 50 a0 e1                                      mov r5, r3
00582da4  be 2e f6 eb                                      bl #0x30e8a4
00582da8  ff 35 a0 e3                                      mov r3, #0x3fc00000
00582dac  00 20 a0 e3                                      mov r2, #0
00582db0  02 36 83 e2                                      add r3, r3, #0x200000
00582db4  3e 2f f6 eb                                      bl #0x30eab4
00582db8  cf 2d f6 eb                                      bl #0x30e4fc
00582dbc  01 30 a0 e1                                      mov r3, r1
00582dc0  ff 15 a0 e3                                      mov r1, #0x3fc00000
00582dc4  00 20 a0 e1                                      mov r2, r0
00582dc8  03 16 81 e2                                      add r1, r1, #0x300000
00582dcc  00 00 a0 e3                                      mov r0, #0
00582dd0  5a 2d f6 eb                                      bl #0x30e340
00582dd4  00 80 a0 e1                                      mov r8, r0
00582dd8  06 00 a0 e1                                      mov r0, r6
00582ddc  01 90 a0 e1                                      mov sb, r1
00582de0  af 2e f6 eb                                      bl #0x30e8a4
00582de4  00 20 a0 e1                                      mov r2, r0
00582de8  01 30 a0 e1                                      mov r3, r1
00582dec  08 00 a0 e1                                      mov r0, r8
00582df0  09 10 a0 e1                                      mov r1, sb
00582df4  51 2d f6 eb                                      bl #0x30e340
00582df8  28 2e f6 eb                                      bl #0x30e6a0
00582dfc  00 60 a0 e3                                      mov r6, #0
00582e00  00 00 84 e5                                      str r0, [r4]
00582e04  09 10 a0 e1                                      mov r1, sb
00582e08  08 00 a0 e1                                      mov r0, r8
00582e0c  04 60 84 e5                                      str r6, [r4, #4]
00582e10  08 60 84 e5                                      str r6, [r4, #8]
00582e14  0c 60 84 e5                                      str r6, [r4, #0xc]
00582e18  10 60 84 e5                                      str r6, [r4, #0x10]
00582e1c  1f 2e f6 eb                                      bl #0x30e6a0
00582e20  02 51 85 e2                                      add r5, r5, #0x80000000
00582e24  fe 35 a0 e3                                      mov r3, #0x3f800000
00582e28  00 20 a0 e3                                      mov r2, #0
00582e2c  14 00 84 e5                                      str r0, [r4, #0x14]
00582e30  2c 30 84 e5                                      str r3, [r4, #0x2c]
00582e34  38 50 84 e5                                      str r5, [r4, #0x38]
00582e38  40 20 c4 e5                                      strb r2, [r4, #0x40]
00582e3c  3c 60 84 e5                                      str r6, [r4, #0x3c]
00582e40  18 60 84 e5                                      str r6, [r4, #0x18]
00582e44  1c 60 84 e5                                      str r6, [r4, #0x1c]
00582e48  20 60 84 e5                                      str r6, [r4, #0x20]
00582e4c  24 60 84 e5                                      str r6, [r4, #0x24]
00582e50  28 30 84 e5                                      str r3, [r4, #0x28]
00582e54  30 60 84 e5                                      str r6, [r4, #0x30]
00582e58  34 60 84 e5                                      str r6, [r4, #0x34]
00582e5c  04 00 a0 e1                                      mov r0, r4
00582e60  70 83 bd e8                                      pop {r4, r5, r6, r8, sb, pc}
; Evidence package domain: camera
; ELF VA 0x00582e64, file offset 0x00582e64, range size 1052, SHA-256 05925cbfa6ca7d5408a7f003567c7d20474d3e42c362dd386786b719772de499
; Index name: glitch::core::CMatrix4<float> glitch::core::buildCameraLookAtMatrix<float>(glitch::core::vector3d<float> const&, glitch::core::vector3d<float> const&, glitch::core::vector3d<float> const&)
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_core_CMatrix4_float_glitch_core-451577a2d7ed-001.asm
; FUNCTION 0x00582e64, declared_size=1052, range_size=1052, mode=arm
; class-group: glitch::core::CMatrix4<float> glitch::core
; alias: _ZN6glitch4core23buildCameraLookAtMatrixIfEENS0_8CMatrix4IT_EERKNS0_8vector3dIS3_EES8_S8_
; demangled: glitch::core::CMatrix4<float> glitch::core::buildCameraLookAtMatrix<float>(glitch::core::vector3d<float> const&, glitch::core::vector3d<float> const&, glitch::core::vector3d<float> const&)
; decoder-mode: arm
00582e64  f0 4f 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00582e68  01 50 a0 e1                                      mov r5, r1
00582e6c  14 d0 4d e2                                      sub sp, sp, #0x14
00582e70  00 10 91 e5                                      ldr r1, [r1]
00582e74  00 40 a0 e1                                      mov r4, r0
00582e78  00 00 92 e5                                      ldr r0, [r2]
00582e7c  02 60 a0 e1                                      mov r6, r2
00582e80  03 a0 a0 e1                                      mov sl, r3
00582e84  48 2d f6 eb                                      bl #0x30e3ac
00582e88  04 10 95 e5                                      ldr r1, [r5, #4]
00582e8c  00 80 a0 e1                                      mov r8, r0
00582e90  04 00 96 e5                                      ldr r0, [r6, #4]
00582e94  44 2d f6 eb                                      bl #0x30e3ac
00582e98  08 10 95 e5                                      ldr r1, [r5, #8]
00582e9c  00 70 a0 e1                                      mov r7, r0
00582ea0  08 00 96 e5                                      ldr r0, [r6, #8]
00582ea4  40 2d f6 eb                                      bl #0x30e3ac
00582ea8  08 10 a0 e1                                      mov r1, r8
00582eac  00 60 a0 e1                                      mov r6, r0
00582eb0  08 00 a0 e1                                      mov r0, r8
00582eb4  ac 2f f6 eb                                      bl #0x30ed6c
00582eb8  07 10 a0 e1                                      mov r1, r7
00582ebc  00 90 a0 e1                                      mov sb, r0
00582ec0  07 00 a0 e1                                      mov r0, r7
00582ec4  a8 2f f6 eb                                      bl #0x30ed6c
00582ec8  00 10 a0 e1                                      mov r1, r0
00582ecc  09 00 a0 e1                                      mov r0, sb
00582ed0  33 2f f6 eb                                      bl #0x30eba4
00582ed4  06 10 a0 e1                                      mov r1, r6
00582ed8  00 90 a0 e1                                      mov sb, r0
00582edc  06 00 a0 e1                                      mov r0, r6
00582ee0  a1 2f f6 eb                                      bl #0x30ed6c
00582ee4  00 10 a0 e1                                      mov r1, r0
00582ee8  09 00 a0 e1                                      mov r0, sb
00582eec  2c 2f f6 eb                                      bl #0x30eba4
00582ef0  00 10 a0 e3                                      mov r1, #0
00582ef4  00 90 a0 e1                                      mov sb, r0
00582ef8  23 2c f6 eb                                      bl #0x30df8c
00582efc  00 00 50 e3                                      cmp r0, #0
00582f00  11 00 00 1a                                      bne #0x582f4c
00582f04  09 00 a0 e1                                      mov r0, sb
00582f08  85 2c f6 eb                                      bl #0x30e124
00582f0c  00 10 a0 e1                                      mov r1, r0
00582f10  fe 05 a0 e3                                      mov r0, #0x3f800000
00582f14  5e 2f f6 eb                                      bl #0x30ec94
00582f18  00 90 a0 e1                                      mov sb, r0
00582f1c  09 10 a0 e1                                      mov r1, sb
00582f20  08 00 a0 e1                                      mov r0, r8
00582f24  90 2f f6 eb                                      bl #0x30ed6c
00582f28  09 10 a0 e1                                      mov r1, sb
00582f2c  00 80 a0 e1                                      mov r8, r0
00582f30  07 00 a0 e1                                      mov r0, r7
00582f34  8c 2f f6 eb                                      bl #0x30ed6c
00582f38  09 10 a0 e1                                      mov r1, sb
00582f3c  00 70 a0 e1                                      mov r7, r0
00582f40  06 00 a0 e1                                      mov r0, r6
00582f44  88 2f f6 eb                                      bl #0x30ed6c
00582f48  00 60 a0 e1                                      mov r6, r0
00582f4c  04 20 9a e5                                      ldr r2, [sl, #4]
00582f50  06 00 a0 e1                                      mov r0, r6
00582f54  08 b0 9a e5                                      ldr fp, [sl, #8]
00582f58  02 11 82 e2                                      add r1, r2, #0x80000000
00582f5c  04 20 8d e5                                      str r2, [sp, #4]
00582f60  81 2f f6 eb                                      bl #0x30ed6c
00582f64  0b 10 a0 e1                                      mov r1, fp
00582f68  00 90 a0 e1                                      mov sb, r0
00582f6c  07 00 a0 e1                                      mov r0, r7
00582f70  7d 2f f6 eb                                      bl #0x30ed6c
00582f74  00 10 a0 e1                                      mov r1, r0
00582f78  09 00 a0 e1                                      mov r0, sb
00582f7c  08 2f f6 eb                                      bl #0x30eba4
00582f80  02 11 8b e2                                      add r1, fp, #0x80000000
00582f84  00 90 a0 e1                                      mov sb, r0
00582f88  08 00 a0 e1                                      mov r0, r8
00582f8c  76 2f f6 eb                                      bl #0x30ed6c
00582f90  00 b0 9a e5                                      ldr fp, [sl]
00582f94  00 a0 a0 e1                                      mov sl, r0
00582f98  06 00 a0 e1                                      mov r0, r6
00582f9c  0b 10 a0 e1                                      mov r1, fp
00582fa0  71 2f f6 eb                                      bl #0x30ed6c
00582fa4  00 10 a0 e1                                      mov r1, r0
00582fa8  0a 00 a0 e1                                      mov r0, sl
00582fac  fc 2e f6 eb                                      bl #0x30eba4
00582fb0  02 11 8b e2                                      add r1, fp, #0x80000000
00582fb4  00 a0 a0 e1                                      mov sl, r0
00582fb8  07 00 a0 e1                                      mov r0, r7
00582fbc  6a 2f f6 eb                                      bl #0x30ed6c
00582fc0  04 20 9d e5                                      ldr r2, [sp, #4]
00582fc4  00 b0 a0 e1                                      mov fp, r0
00582fc8  08 00 a0 e1                                      mov r0, r8
00582fcc  02 10 a0 e1                                      mov r1, r2
00582fd0  65 2f f6 eb                                      bl #0x30ed6c
00582fd4  00 10 a0 e1                                      mov r1, r0
00582fd8  0b 00 a0 e1                                      mov r0, fp
00582fdc  f0 2e f6 eb                                      bl #0x30eba4
00582fe0  09 10 a0 e1                                      mov r1, sb
00582fe4  00 b0 a0 e1                                      mov fp, r0
00582fe8  09 00 a0 e1                                      mov r0, sb
00582fec  5e 2f f6 eb                                      bl #0x30ed6c
00582ff0  0a 10 a0 e1                                      mov r1, sl
00582ff4  00 30 a0 e1                                      mov r3, r0
00582ff8  0a 00 a0 e1                                      mov r0, sl
00582ffc  04 30 8d e5                                      str r3, [sp, #4]
00583000  59 2f f6 eb                                      bl #0x30ed6c
00583004  04 30 9d e5                                      ldr r3, [sp, #4]
00583008  00 10 a0 e1                                      mov r1, r0
0058300c  03 00 a0 e1                                      mov r0, r3
00583010  e3 2e f6 eb                                      bl #0x30eba4
00583014  0b 10 a0 e1                                      mov r1, fp
00583018  00 30 a0 e1                                      mov r3, r0
0058301c  0b 00 a0 e1                                      mov r0, fp
00583020  04 30 8d e5                                      str r3, [sp, #4]
00583024  50 2f f6 eb                                      bl #0x30ed6c
00583028  04 30 9d e5                                      ldr r3, [sp, #4]
0058302c  00 10 a0 e1                                      mov r1, r0
00583030  03 00 a0 e1                                      mov r0, r3
00583034  da 2e f6 eb                                      bl #0x30eba4
00583038  00 10 a0 e3                                      mov r1, #0
0058303c  08 00 8d e5                                      str r0, [sp, #8]
00583040  d1 2b f6 eb                                      bl #0x30df8c
00583044  00 00 50 e3                                      cmp r0, #0
00583048  14 00 00 1a                                      bne #0x5830a0
0058304c  08 00 9d e5                                      ldr r0, [sp, #8]
00583050  33 2c f6 eb                                      bl #0x30e124
00583054  00 10 a0 e1                                      mov r1, r0
00583058  fe 05 a0 e3                                      mov r0, #0x3f800000
0058305c  0c 2f f6 eb                                      bl #0x30ec94
00583060  00 30 a0 e1                                      mov r3, r0
00583064  03 10 a0 e1                                      mov r1, r3
00583068  09 00 a0 e1                                      mov r0, sb
0058306c  04 30 8d e5                                      str r3, [sp, #4]
00583070  3d 2f f6 eb                                      bl #0x30ed6c
00583074  04 30 9d e5                                      ldr r3, [sp, #4]
00583078  00 90 a0 e1                                      mov sb, r0
0058307c  0a 00 a0 e1                                      mov r0, sl
00583080  03 10 a0 e1                                      mov r1, r3
00583084  38 2f f6 eb                                      bl #0x30ed6c
00583088  04 30 9d e5                                      ldr r3, [sp, #4]
0058308c  00 a0 a0 e1                                      mov sl, r0
00583090  0b 00 a0 e1                                      mov r0, fp
00583094  03 10 a0 e1                                      mov r1, r3
00583098  33 2f f6 eb                                      bl #0x30ed6c
0058309c  00 b0 a0 e1                                      mov fp, r0
005830a0  02 11 87 e2                                      add r1, r7, #0x80000000
005830a4  0b 00 a0 e1                                      mov r0, fp
005830a8  2f 2f f6 eb                                      bl #0x30ed6c
005830ac  0a 10 a0 e1                                      mov r1, sl
005830b0  00 30 a0 e1                                      mov r3, r0
005830b4  06 00 a0 e1                                      mov r0, r6
005830b8  04 30 8d e5                                      str r3, [sp, #4]
005830bc  2a 2f f6 eb                                      bl #0x30ed6c
005830c0  04 30 9d e5                                      ldr r3, [sp, #4]
005830c4  00 10 a0 e1                                      mov r1, r0
005830c8  03 00 a0 e1                                      mov r0, r3
005830cc  b4 2e f6 eb                                      bl #0x30eba4
005830d0  02 11 86 e2                                      add r1, r6, #0x80000000
005830d4  08 00 8d e5                                      str r0, [sp, #8]
005830d8  09 00 a0 e1                                      mov r0, sb
005830dc  22 2f f6 eb                                      bl #0x30ed6c
005830e0  08 10 a0 e1                                      mov r1, r8
005830e4  00 30 a0 e1                                      mov r3, r0
005830e8  0b 00 a0 e1                                      mov r0, fp
005830ec  04 30 8d e5                                      str r3, [sp, #4]
005830f0  1d 2f f6 eb                                      bl #0x30ed6c
005830f4  04 30 9d e5                                      ldr r3, [sp, #4]
005830f8  00 10 a0 e1                                      mov r1, r0
005830fc  03 00 a0 e1                                      mov r0, r3
00583100  a7 2e f6 eb                                      bl #0x30eba4
00583104  02 11 88 e2                                      add r1, r8, #0x80000000
00583108  0c 00 8d e5                                      str r0, [sp, #0xc]
0058310c  0a 00 a0 e1                                      mov r0, sl
00583110  15 2f f6 eb                                      bl #0x30ed6c
00583114  09 10 a0 e1                                      mov r1, sb
00583118  00 30 a0 e1                                      mov r3, r0
0058311c  07 00 a0 e1                                      mov r0, r7
00583120  04 30 8d e5                                      str r3, [sp, #4]
00583124  10 2f f6 eb                                      bl #0x30ed6c
00583128  04 30 9d e5                                      ldr r3, [sp, #4]
0058312c  00 10 a0 e1                                      mov r1, r0
00583130  03 00 a0 e1                                      mov r0, r3
00583134  9a 2e f6 eb                                      bl #0x30eba4
00583138  00 20 a0 e3                                      mov r2, #0
0058313c  00 10 a0 e3                                      mov r1, #0
00583140  2c 20 84 e5                                      str r2, [r4, #0x2c]
00583144  00 90 84 e5                                      str sb, [r4]
00583148  40 10 c4 e5                                      strb r1, [r4, #0x40]
0058314c  08 10 9d e5                                      ldr r1, [sp, #8]
00583150  0c 20 84 e5                                      str r2, [r4, #0xc]
00583154  08 80 84 e5                                      str r8, [r4, #8]
00583158  10 a0 84 e5                                      str sl, [r4, #0x10]
0058315c  04 10 84 e5                                      str r1, [r4, #4]
00583160  0c 10 9d e5                                      ldr r1, [sp, #0xc]
00583164  1c 20 84 e5                                      str r2, [r4, #0x1c]
00583168  18 70 84 e5                                      str r7, [r4, #0x18]
0058316c  24 00 84 e5                                      str r0, [r4, #0x24]
00583170  14 10 84 e5                                      str r1, [r4, #0x14]
00583174  20 b0 84 e5                                      str fp, [r4, #0x20]
00583178  28 60 84 e5                                      str r6, [r4, #0x28]
0058317c  00 30 a0 e1                                      mov r3, r0
00583180  00 10 95 e5                                      ldr r1, [r5]
00583184  09 00 a0 e1                                      mov r0, sb
00583188  04 30 8d e5                                      str r3, [sp, #4]
0058318c  f6 2e f6 eb                                      bl #0x30ed6c
00583190  04 10 95 e5                                      ldr r1, [r5, #4]
00583194  00 90 a0 e1                                      mov sb, r0
00583198  0a 00 a0 e1                                      mov r0, sl
0058319c  f2 2e f6 eb                                      bl #0x30ed6c
005831a0  00 10 a0 e1                                      mov r1, r0
005831a4  09 00 a0 e1                                      mov r0, sb
005831a8  7d 2e f6 eb                                      bl #0x30eba4
005831ac  08 10 95 e5                                      ldr r1, [r5, #8]
005831b0  00 a0 a0 e1                                      mov sl, r0
005831b4  0b 00 a0 e1                                      mov r0, fp
005831b8  eb 2e f6 eb                                      bl #0x30ed6c
005831bc  00 10 a0 e1                                      mov r1, r0
005831c0  0a 00 a0 e1                                      mov r0, sl
005831c4  76 2e f6 eb                                      bl #0x30eba4
005831c8  02 01 80 e2                                      add r0, r0, #0x80000000
005831cc  30 00 84 e5                                      str r0, [r4, #0x30]
005831d0  00 10 95 e5                                      ldr r1, [r5]
005831d4  08 00 9d e5                                      ldr r0, [sp, #8]
005831d8  e3 2e f6 eb                                      bl #0x30ed6c
005831dc  04 10 95 e5                                      ldr r1, [r5, #4]
005831e0  00 a0 a0 e1                                      mov sl, r0
005831e4  0c 00 9d e5                                      ldr r0, [sp, #0xc]
005831e8  df 2e f6 eb                                      bl #0x30ed6c
005831ec  00 10 a0 e1                                      mov r1, r0
005831f0  0a 00 a0 e1                                      mov r0, sl
005831f4  6a 2e f6 eb                                      bl #0x30eba4
005831f8  04 30 9d e5                                      ldr r3, [sp, #4]
005831fc  08 10 95 e5                                      ldr r1, [r5, #8]
00583200  00 a0 a0 e1                                      mov sl, r0
00583204  03 00 a0 e1                                      mov r0, r3
00583208  d7 2e f6 eb                                      bl #0x30ed6c
0058320c  00 10 a0 e1                                      mov r1, r0
00583210  0a 00 a0 e1                                      mov r0, sl
00583214  62 2e f6 eb                                      bl #0x30eba4
00583218  02 01 80 e2                                      add r0, r0, #0x80000000
0058321c  34 00 84 e5                                      str r0, [r4, #0x34]
00583220  00 10 95 e5                                      ldr r1, [r5]
00583224  08 00 a0 e1                                      mov r0, r8
00583228  cf 2e f6 eb                                      bl #0x30ed6c
0058322c  04 10 95 e5                                      ldr r1, [r5, #4]
00583230  00 80 a0 e1                                      mov r8, r0
00583234  07 00 a0 e1                                      mov r0, r7
00583238  cb 2e f6 eb                                      bl #0x30ed6c
0058323c  00 10 a0 e1                                      mov r1, r0
00583240  08 00 a0 e1                                      mov r0, r8
00583244  56 2e f6 eb                                      bl #0x30eba4
00583248  08 10 95 e5                                      ldr r1, [r5, #8]
0058324c  00 70 a0 e1                                      mov r7, r0
00583250  06 00 a0 e1                                      mov r0, r6
00583254  c4 2e f6 eb                                      bl #0x30ed6c
00583258  00 10 a0 e1                                      mov r1, r0
0058325c  07 00 a0 e1                                      mov r0, r7
00583260  4f 2e f6 eb                                      bl #0x30eba4
00583264  fe 35 a0 e3                                      mov r3, #0x3f800000
00583268  02 01 80 e2                                      add r0, r0, #0x80000000
0058326c  38 00 84 e5                                      str r0, [r4, #0x38]
00583270  3c 30 84 e5                                      str r3, [r4, #0x3c]
00583274  04 00 a0 e1                                      mov r0, r4
00583278  14 d0 8d e2                                      add sp, sp, #0x14
0058327c  f0 8f bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
; Evidence package domain: camera
; ELF VA 0x00583280, file offset 0x00583280, range size 692, SHA-256 36148fa669a57933611fbaf108ef01b64bc24911ad9534366667801d7fa9f4f6
; Index name: glitch::scene::CCameraSceneNode::recalculateMatrices()
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x00583280, declared_size=692, range_size=692, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZN6glitch5scene16CCameraSceneNode19recalculateMatricesEv
; demangled: glitch::scene::CCameraSceneNode::recalculateMatrices()
; decoder-mode: arm
00583280  f0 4f 2d e9                                      push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00583284  6c d0 4d e2                                      sub sp, sp, #0x6c
00583288  00 40 a0 e1                                      mov r4, r0
0058328c  5c b0 8d e2                                      add fp, sp, #0x5c
00583290  0b 00 a0 e1                                      mov r0, fp
00583294  04 10 a0 e1                                      mov r1, r4
00583298  b8 4f 00 eb                                      bl #0x597180
0058329c  38 01 94 e5                                      ldr r0, [r4, #0x138]
005832a0  5c 10 9d e5                                      ldr r1, [sp, #0x5c]
005832a4  40 2c f6 eb                                      bl #0x30e3ac
005832a8  60 10 9d e5                                      ldr r1, [sp, #0x60]
005832ac  00 90 a0 e1                                      mov sb, r0
005832b0  3c 01 94 e5                                      ldr r0, [r4, #0x13c]
005832b4  3c 2c f6 eb                                      bl #0x30e3ac
005832b8  64 10 9d e5                                      ldr r1, [sp, #0x64]
005832bc  00 a0 a0 e1                                      mov sl, r0
005832c0  40 01 94 e5                                      ldr r0, [r4, #0x140]
005832c4  38 2c f6 eb                                      bl #0x30e3ac
005832c8  44 51 94 e5                                      ldr r5, [r4, #0x144]
005832cc  48 71 94 e5                                      ldr r7, [r4, #0x148]
005832d0  4c 61 94 e5                                      ldr r6, [r4, #0x14c]
005832d4  00 80 a0 e1                                      mov r8, r0
005832d8  09 10 a0 e1                                      mov r1, sb
005832dc  09 00 a0 e1                                      mov r0, sb
005832e0  50 50 8d e5                                      str r5, [sp, #0x50]
005832e4  54 70 8d e5                                      str r7, [sp, #0x54]
005832e8  58 60 8d e5                                      str r6, [sp, #0x58]
005832ec  9e 2e f6 eb                                      bl #0x30ed6c
005832f0  0a 10 a0 e1                                      mov r1, sl
005832f4  00 30 a0 e1                                      mov r3, r0
005832f8  0a 00 a0 e1                                      mov r0, sl
005832fc  00 30 8d e5                                      str r3, [sp]
00583300  99 2e f6 eb                                      bl #0x30ed6c
00583304  00 30 9d e5                                      ldr r3, [sp]
00583308  00 10 a0 e1                                      mov r1, r0
0058330c  03 00 a0 e1                                      mov r0, r3
00583310  23 2e f6 eb                                      bl #0x30eba4
00583314  08 10 a0 e1                                      mov r1, r8
00583318  00 30 a0 e1                                      mov r3, r0
0058331c  08 00 a0 e1                                      mov r0, r8
00583320  00 30 8d e5                                      str r3, [sp]
00583324  90 2e f6 eb                                      bl #0x30ed6c
00583328  00 30 9d e5                                      ldr r3, [sp]
0058332c  00 10 a0 e1                                      mov r1, r0
00583330  03 00 a0 e1                                      mov r0, r3
00583334  1a 2e f6 eb                                      bl #0x30eba4
00583338  00 10 a0 e3                                      mov r1, #0
0058333c  04 00 8d e5                                      str r0, [sp, #4]
00583340  11 2b f6 eb                                      bl #0x30df8c
00583344  00 00 50 e3                                      cmp r0, #0
00583348  14 00 00 1a                                      bne #0x5833a0
0058334c  04 00 9d e5                                      ldr r0, [sp, #4]
00583350  73 2b f6 eb                                      bl #0x30e124
00583354  00 10 a0 e1                                      mov r1, r0
00583358  fe 05 a0 e3                                      mov r0, #0x3f800000
0058335c  4c 2e f6 eb                                      bl #0x30ec94
00583360  00 50 a0 e1                                      mov r5, r0
00583364  05 10 a0 e1                                      mov r1, r5
00583368  09 00 a0 e1                                      mov r0, sb
0058336c  7e 2e f6 eb                                      bl #0x30ed6c
00583370  05 10 a0 e1                                      mov r1, r5
00583374  00 90 a0 e1                                      mov sb, r0
00583378  0a 00 a0 e1                                      mov r0, sl
0058337c  7a 2e f6 eb                                      bl #0x30ed6c
00583380  05 10 a0 e1                                      mov r1, r5
00583384  00 a0 a0 e1                                      mov sl, r0
00583388  08 00 a0 e1                                      mov r0, r8
0058338c  76 2e f6 eb                                      bl #0x30ed6c
00583390  50 50 9d e5                                      ldr r5, [sp, #0x50]
00583394  54 70 9d e5                                      ldr r7, [sp, #0x54]
00583398  58 60 9d e5                                      ldr r6, [sp, #0x58]
0058339c  00 80 a0 e1                                      mov r8, r0
005833a0  05 10 a0 e1                                      mov r1, r5
005833a4  05 00 a0 e1                                      mov r0, r5
005833a8  6f 2e f6 eb                                      bl #0x30ed6c
005833ac  07 10 a0 e1                                      mov r1, r7
005833b0  00 30 a0 e1                                      mov r3, r0
005833b4  07 00 a0 e1                                      mov r0, r7
005833b8  00 30 8d e5                                      str r3, [sp]
005833bc  6a 2e f6 eb                                      bl #0x30ed6c
005833c0  00 30 9d e5                                      ldr r3, [sp]
005833c4  00 10 a0 e1                                      mov r1, r0
005833c8  03 00 a0 e1                                      mov r0, r3
005833cc  f4 2d f6 eb                                      bl #0x30eba4
005833d0  06 10 a0 e1                                      mov r1, r6
005833d4  00 30 a0 e1                                      mov r3, r0
005833d8  06 00 a0 e1                                      mov r0, r6
005833dc  00 30 8d e5                                      str r3, [sp]
005833e0  61 2e f6 eb                                      bl #0x30ed6c
005833e4  00 30 9d e5                                      ldr r3, [sp]
005833e8  00 10 a0 e1                                      mov r1, r0
005833ec  03 00 a0 e1                                      mov r0, r3
005833f0  eb 2d f6 eb                                      bl #0x30eba4
005833f4  00 10 a0 e3                                      mov r1, #0
005833f8  04 00 8d e5                                      str r0, [sp, #4]
005833fc  e2 2a f6 eb                                      bl #0x30df8c
00583400  00 00 50 e3                                      cmp r0, #0
00583404  13 00 00 1a                                      bne #0x583458
00583408  04 00 9d e5                                      ldr r0, [sp, #4]
0058340c  44 2b f6 eb                                      bl #0x30e124
00583410  00 10 a0 e1                                      mov r1, r0
00583414  fe 05 a0 e3                                      mov r0, #0x3f800000
00583418  1d 2e f6 eb                                      bl #0x30ec94
0058341c  50 10 9d e5                                      ldr r1, [sp, #0x50]
00583420  00 60 a0 e1                                      mov r6, r0
00583424  50 2e f6 eb                                      bl #0x30ed6c
00583428  54 10 9d e5                                      ldr r1, [sp, #0x54]
0058342c  00 50 a0 e1                                      mov r5, r0
00583430  06 00 a0 e1                                      mov r0, r6
00583434  50 50 8d e5                                      str r5, [sp, #0x50]
00583438  4b 2e f6 eb                                      bl #0x30ed6c
0058343c  58 10 9d e5                                      ldr r1, [sp, #0x58]
00583440  00 70 a0 e1                                      mov r7, r0
00583444  06 00 a0 e1                                      mov r0, r6
00583448  54 70 8d e5                                      str r7, [sp, #0x54]
0058344c  46 2e f6 eb                                      bl #0x30ed6c
00583450  00 60 a0 e1                                      mov r6, r0
00583454  58 00 8d e5                                      str r0, [sp, #0x58]
00583458  09 00 a0 e1                                      mov r0, sb
0058345c  05 10 a0 e1                                      mov r1, r5
00583460  41 2e f6 eb                                      bl #0x30ed6c
00583464  07 10 a0 e1                                      mov r1, r7
00583468  00 90 a0 e1                                      mov sb, r0
0058346c  0a 00 a0 e1                                      mov r0, sl
00583470  3d 2e f6 eb                                      bl #0x30ed6c
00583474  00 10 a0 e1                                      mov r1, r0
00583478  09 00 a0 e1                                      mov r0, sb
0058347c  c8 2d f6 eb                                      bl #0x30eba4
00583480  06 10 a0 e1                                      mov r1, r6
00583484  00 70 a0 e1                                      mov r7, r0
00583488  08 00 a0 e1                                      mov r0, r8
0058348c  36 2e f6 eb                                      bl #0x30ed6c
00583490  00 10 a0 e1                                      mov r1, r0
00583494  07 00 a0 e1                                      mov r0, r7
00583498  c1 2d f6 eb                                      bl #0x30eba4
0058349c  bd 17 03 e3                                      movw r1, #0x37bd
005834a0  02 61 c0 e3                                      bic r6, r0, #0x80000000
005834a4  86 15 43 e3                                      movt r1, #0x3586
005834a8  06 00 a0 e1                                      mov r0, r6
005834ac  bc 2d f6 eb                                      bl #0x30eba4
005834b0  fe 15 a0 e3                                      mov r1, #0x3f800000
005834b4  fe 2b f6 eb                                      bl #0x30e4b4
005834b8  00 00 50 e3                                      cmp r0, #0
005834bc  0b 00 00 0a                                      beq #0x5834f0
005834c0  bd 17 03 e3                                      movw r1, #0x37bd
005834c4  86 15 43 e3                                      movt r1, #0x3586
005834c8  06 00 a0 e1                                      mov r0, r6
005834cc  b6 2b f6 eb                                      bl #0x30e3ac
005834d0  fe 15 a0 e3                                      mov r1, #0x3f800000
005834d4  34 2d f6 eb                                      bl #0x30e9ac
005834d8  00 00 50 e3                                      cmp r0, #0
005834dc  03 00 00 0a                                      beq #0x5834f0
005834e0  05 00 a0 e1                                      mov r0, r5
005834e4  3f 14 a0 e3                                      mov r1, #0x3f000000
005834e8  ad 2d f6 eb                                      bl #0x30eba4
005834ec  50 00 8d e5                                      str r0, [sp, #0x50]
005834f0  0c 50 8d e2                                      add r5, sp, #0xc
005834f4  50 30 8d e2                                      add r3, sp, #0x50
005834f8  0b 10 a0 e1                                      mov r1, fp
005834fc  05 00 a0 e1                                      mov r0, r5
00583500  4e 2f 84 e2                                      add r2, r4, #0x138
00583504  56 fe ff eb                                      bl #0x582e64
00583508  05 10 a0 e1                                      mov r1, r5
0058350c  41 20 a0 e3                                      mov r2, #0x41
00583510  7b 0f 84 e2                                      add r0, r4, #0x1ec
00583514  d3 2c f6 eb                                      bl #0x30e868
00583518  5a 0f 84 e2                                      add r0, r4, #0x168
0058351c  00 10 a0 e3                                      mov r1, #0
00583520  88 fb ff eb                                      bl #0x582348
00583524  04 00 a0 e1                                      mov r0, r4
00583528  f5 fc ff eb                                      bl #0x582904
0058352c  6c d0 8d e2                                      add sp, sp, #0x6c
00583530  f0 8f bd e8                                      pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
; Evidence package domain: camera
; ELF VA 0x00583534, file offset 0x00583534, range size 104, SHA-256 e289962e0a84226603aea182d051b5816affb9f8a808fc8d176ddda02d7a3b4f
; Index name: glitch::scene::CCameraSceneNode::onRegisterSceneNode()
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x00583534, declared_size=104, range_size=104, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZN6glitch5scene16CCameraSceneNode19onRegisterSceneNodeEv
; demangled: glitch::scene::CCameraSceneNode::onRegisterSceneNode()
; decoder-mode: arm
00583534  30 40 2d e9                                      push {r4, r5, lr}
00583538  00 50 a0 e1                                      mov r5, r0
0058353c  1c d0 4d e2                                      sub sp, sp, #0x1c
00583540  4e ff ff eb                                      bl #0x583280
00583544  10 01 95 e5                                      ldr r0, [r5, #0x110]
00583548  e4 30 90 e5                                      ldr r3, [r0, #0xe4]
0058354c  03 00 55 e1                                      cmp r5, r3
00583550  02 00 00 0a                                      beq #0x583560
00583554  01 00 a0 e3                                      mov r0, #1
00583558  1c d0 8d e2                                      add sp, sp, #0x1c
0058355c  30 80 bd e8                                      pop {r4, r5, pc}
00583560  00 20 90 e5                                      ldr r2, [r0]
00583564  00 30 a0 e3                                      mov r3, #0
00583568  18 40 8d e2                                      add r4, sp, #0x18
0058356c  24 c0 92 e5                                      ldr ip, [r2, #0x24]
00583570  04 30 24 e5                                      str r3, [r4, #-4]!
00583574  02 21 e0 e3                                      mvn r2, #0x80000000
00583578  08 20 8d e5                                      str r2, [sp, #8]
0058357c  00 30 8d e5                                      str r3, [sp]
00583580  04 30 8d e5                                      str r3, [sp, #4]
00583584  05 10 a0 e1                                      mov r1, r5
00583588  04 20 a0 e1                                      mov r2, r4
0058358c  3c ff 2f e1                                      blx ip
00583590  04 00 a0 e1                                      mov r0, r4
00583594  93 35 f6 eb                                      bl #0x310be8
00583598  ed ff ff ea                                      b #0x583554
; Evidence package domain: camera
; ELF VA 0x0058359c, file offset 0x0058359c, range size 176, SHA-256 cef1ec4431e1d3725edd2e3b2d65933bf25a713f30456d0e75f4409dded1a783
; Index name: glitch::core::CMatrix4<float> glitch::core::buildProjectionMatrixOrtho<float>(float, float, float, float)
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_core_CMatrix4_float_glitch_core-451577a2d7ed-001.asm
; FUNCTION 0x0058359c, declared_size=176, range_size=176, mode=arm
; class-group: glitch::core::CMatrix4<float> glitch::core
; alias: _ZN6glitch4core26buildProjectionMatrixOrthoIfEENS0_8CMatrix4IT_EES3_S3_S3_S3_
; demangled: glitch::core::CMatrix4<float> glitch::core::buildProjectionMatrixOrtho<float>(float, float, float, float)
; decoder-mode: arm
0058359c  f0 41 2d e9                                      push {r4, r5, r6, r7, r8, lr}
005835a0  00 40 a0 e1                                      mov r4, r0
005835a4  01 01 a0 e3                                      mov r0, #0x40000000
005835a8  03 50 a0 e1                                      mov r5, r3
005835ac  02 70 a0 e1                                      mov r7, r2
005835b0  b7 2d f6 eb                                      bl #0x30ec94
005835b4  00 60 a0 e3                                      mov r6, #0
005835b8  00 00 84 e5                                      str r0, [r4]
005835bc  07 10 a0 e1                                      mov r1, r7
005835c0  04 60 84 e5                                      str r6, [r4, #4]
005835c4  08 60 84 e5                                      str r6, [r4, #8]
005835c8  0c 60 84 e5                                      str r6, [r4, #0xc]
005835cc  10 60 84 e5                                      str r6, [r4, #0x10]
005835d0  01 01 a0 e3                                      mov r0, #0x40000000
005835d4  ae 2d f6 eb                                      bl #0x30ec94
005835d8  18 70 9d e5                                      ldr r7, [sp, #0x18]
005835dc  05 10 a0 e1                                      mov r1, r5
005835e0  14 00 84 e5                                      str r0, [r4, #0x14]
005835e4  18 60 84 e5                                      str r6, [r4, #0x18]
005835e8  1c 60 84 e5                                      str r6, [r4, #0x1c]
005835ec  20 60 84 e5                                      str r6, [r4, #0x20]
005835f0  24 60 84 e5                                      str r6, [r4, #0x24]
005835f4  07 00 a0 e1                                      mov r0, r7
005835f8  6b 2b f6 eb                                      bl #0x30e3ac
005835fc  00 10 a0 e1                                      mov r1, r0
00583600  fe 05 a0 e3                                      mov r0, #0x3f800000
00583604  a2 2d f6 eb                                      bl #0x30ec94
00583608  07 10 a0 e1                                      mov r1, r7
0058360c  28 00 84 e5                                      str r0, [r4, #0x28]
00583610  34 60 84 e5                                      str r6, [r4, #0x34]
00583614  2c 60 84 e5                                      str r6, [r4, #0x2c]
00583618  30 60 84 e5                                      str r6, [r4, #0x30]
0058361c  05 00 a0 e1                                      mov r0, r5
00583620  61 2b f6 eb                                      bl #0x30e3ac
00583624  00 10 a0 e1                                      mov r1, r0
00583628  05 00 a0 e1                                      mov r0, r5
0058362c  98 2d f6 eb                                      bl #0x30ec94
00583630  00 30 a0 e3                                      mov r3, #0
00583634  40 30 c4 e5                                      strb r3, [r4, #0x40]
00583638  fe 35 a0 e3                                      mov r3, #0x3f800000
0058363c  38 00 84 e5                                      str r0, [r4, #0x38]
00583640  3c 30 84 e5                                      str r3, [r4, #0x3c]
00583644  04 00 a0 e1                                      mov r0, r4
00583648  f0 81 bd e8                                      pop {r4, r5, r6, r7, r8, pc}
; Evidence package domain: camera
; ELF VA 0x0058364c, file offset 0x0058364c, range size 232, SHA-256 d578be4165d11e2b3b7331f48ef8d00a4d8bdb13cff48fea93097a1a0ee0172d
; Index name: glitch::scene::CCameraSceneNode::recalculateProjectionMatrix()
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CCameraSceneNode-0bd806afa97e-001.asm
; FUNCTION 0x0058364c, declared_size=232, range_size=232, mode=arm
; class-group: glitch::scene::CCameraSceneNode
; alias: _ZN6glitch5scene16CCameraSceneNode27recalculateProjectionMatrixEv
; demangled: glitch::scene::CCameraSceneNode::recalculateProjectionMatrix()
; decoder-mode: arm
0058364c  70 40 2d e9                                      push {r4, r5, r6, lr}
00583650  d8 d0 4d e2                                      sub sp, sp, #0xd8
00583654  00 30 90 e5                                      ldr r3, [r0]
00583658  00 40 a0 e1                                      mov r4, r0
0058365c  0f e0 a0 e1                                      mov lr, pc
00583660  50 f1 93 e5                                      ldr pc, [r3, #0x150]
00583664  00 00 50 e3                                      cmp r0, #0
00583668  1e 00 00 1a                                      bne #0x5836e8
0058366c  64 31 d4 e5                                      ldrb r3, [r4, #0x164]
00583670  00 00 53 e3                                      cmp r3, #0
00583674  10 00 00 1a                                      bne #0x5836bc
00583678  60 c1 94 e5                                      ldr ip, [r4, #0x160]
0058367c  0c 50 8d e2                                      add r5, sp, #0xc
00583680  54 11 94 e5                                      ldr r1, [r4, #0x154]
00583684  58 21 94 e5                                      ldr r2, [r4, #0x158]
00583688  5c 31 94 e5                                      ldr r3, [r4, #0x15c]
0058368c  05 00 a0 e1                                      mov r0, r5
00583690  00 c0 8d e5                                      str ip, [sp]
00583694  7b fd ff eb                                      bl #0x582c88
00583698  05 10 a0 e1                                      mov r1, r5
0058369c  9d 0f 84 e2                                      add r0, r4, #0x274
005836a0  41 20 a0 e3                                      mov r2, #0x41
005836a4  6f 2c f6 eb                                      bl #0x30e868
005836a8  5a 0f 84 e2                                      add r0, r4, #0x168
005836ac  02 10 a0 e3                                      mov r1, #2
005836b0  24 fb ff eb                                      bl #0x582348
005836b4  d8 d0 8d e2                                      add sp, sp, #0xd8
005836b8  70 80 bd e8                                      pop {r4, r5, r6, pc}
005836bc  50 50 8d e2                                      add r5, sp, #0x50
005836c0  05 00 a0 e1                                      mov r0, r5
005836c4  54 11 94 e5                                      ldr r1, [r4, #0x154]
005836c8  58 21 94 e5                                      ldr r2, [r4, #0x158]
005836cc  5c 31 94 e5                                      ldr r3, [r4, #0x15c]
005836d0  ae fd ff eb                                      bl #0x582d90
005836d4  05 10 a0 e1                                      mov r1, r5
005836d8  9d 0f 84 e2                                      add r0, r4, #0x274
005836dc  41 20 a0 e3                                      mov r2, #0x41
005836e0  60 2c f6 eb                                      bl #0x30e868
005836e4  ef ff ff ea                                      b #0x5836a8
005836e8  50 01 94 e5                                      ldr r0, [r4, #0x150]
005836ec  94 50 8d e2                                      add r5, sp, #0x94
005836f0  00 10 a0 e1                                      mov r1, r0
005836f4  2a 2d f6 eb                                      bl #0x30eba4
005836f8  58 11 94 e5                                      ldr r1, [r4, #0x158]
005836fc  00 60 a0 e1                                      mov r6, r0
00583700  99 2d f6 eb                                      bl #0x30ed6c
00583704  60 c1 94 e5                                      ldr ip, [r4, #0x160]
00583708  00 10 a0 e1                                      mov r1, r0
0058370c  5c 31 94 e5                                      ldr r3, [r4, #0x15c]
00583710  06 20 a0 e1                                      mov r2, r6
00583714  05 00 a0 e1                                      mov r0, r5
00583718  00 c0 8d e5                                      str ip, [sp]
0058371c  9e ff ff eb                                      bl #0x58359c
00583720  05 10 a0 e1                                      mov r1, r5
00583724  9d 0f 84 e2                                      add r0, r4, #0x274
00583728  41 20 a0 e3                                      mov r2, #0x41
0058372c  4d 2c f6 eb                                      bl #0x30e868
00583730  dc ff ff ea                                      b #0x5836a8

; ===== LIGHT =====
; Evidence package domain: light
; ELF VA 0x005aaa40, file offset 0x005aaa40, range size 68, SHA-256 33830f9852bafa4c5cee56a9baf4b43f2ea667ff8323c16a9b8da2c60c641c33
; Index name: glitch::video::IVideoDriver::addDynamicLight(boost::intrusive_ptr<glitch::video::CLight> const&)
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_video_IVideoDriver-128257112762-001.asm
; FUNCTION 0x005aaa40, declared_size=68, range_size=68, mode=arm
; class-group: glitch::video::IVideoDriver
; alias: _ZN6glitch5video12IVideoDriver15addDynamicLightERKN5boost13intrusive_ptrINS0_6CLightEEE
; demangled: glitch::video::IVideoDriver::addDynamicLight(boost::intrusive_ptr<glitch::video::CLight> const&)
; decoder-mode: arm
005aaa40  10 40 2d e9                                      push {r4, lr}
005aaa44  00 40 a0 e1                                      mov r4, r0
005aaa48  ba 23 d0 e1                                      ldrh r2, [r0, #0x3a]
005aaa4c  bc 03 d0 e1                                      ldrh r0, [r0, #0x3c]
005aaa50  02 00 50 e1                                      cmp r0, r2
005aaa54  09 00 00 9a                                      bls #0x5aaa80
005aaa58  b8 c3 d4 e1                                      ldrh ip, [r4, #0x38]
005aaa5c  01 30 a0 e1                                      mov r3, r1
005aaa60  e4 00 94 e5                                      ldr r0, [r4, #0xe4]
005aaa64  0c 20 82 e0                                      add r2, r2, ip
005aaa68  72 10 ff e6                                      uxth r1, r2
005aaa6c  00 20 a0 e3                                      mov r2, #0
005aaa70  05 4f 00 eb                                      bl #0x5be68c
005aaa74  ba 33 d4 e1                                      ldrh r3, [r4, #0x3a]
005aaa78  01 30 83 e2                                      add r3, r3, #1
005aaa7c  ba 33 c4 e1                                      strh r3, [r4, #0x3a]
005aaa80  10 80 bd e8                                      pop {r4, pc}

; Evidence package domain: light
; ELF VA 0x00583c18, file offset 0x00583c18, range size 216, SHA-256 63b9fd64eee2f7c4031afbec74dde485656905811adb74dbae3f1bf7e2c55a1b
; Index name: glitch::scene::CLightSceneNode::doLightRecalc()
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CLightSceneNode-bf70b6dba9f9-001.asm
; FUNCTION 0x00583c18, declared_size=216, range_size=216, mode=arm
; class-group: glitch::scene::CLightSceneNode
; alias: _ZN6glitch5scene15CLightSceneNode13doLightRecalcEv
; demangled: glitch::scene::CLightSceneNode::doLightRecalc()
; decoder-mode: arm
00583c18  70 40 2d e9                                      push {r4, r5, r6, lr}
00583c1c  34 21 90 e5                                      ldr r2, [r0, #0x134]
00583c20  00 40 a0 e1                                      mov r4, r0
00583c24  b8 35 d2 e1                                      ldrh r3, [r2, #0x58]
00583c28  01 00 53 e3                                      cmp r3, #1
00583c2c  19 00 00 ca                                      bgt #0x583c98
00583c30  40 50 92 e5                                      ldr r5, [r2, #0x40]
00583c34  02 11 e0 e3                                      mvn r1, #0x80000000
00583c38  02 15 41 e2                                      sub r1, r1, #0x800000
00583c3c  05 00 a0 e1                                      mov r0, r5
00583c40  d1 28 f6 eb                                      bl #0x30df8c
00583c44  00 00 50 e3                                      cmp r0, #0
00583c48  21 00 00 1a                                      bne #0x583cd4
00583c4c  05 10 a0 e1                                      mov r1, r5
00583c50  05 00 a0 e1                                      mov r0, r5
00583c54  44 2c f6 eb                                      bl #0x30ed6c
00583c58  3f 14 a0 e3                                      mov r1, #0x3f000000
00583c5c  42 2c f6 eb                                      bl #0x30ed6c
00583c60  02 31 80 e2                                      add r3, r0, #0x80000000
00583c64  44 31 84 e5                                      str r3, [r4, #0x144]
00583c68  48 01 84 e5                                      str r0, [r4, #0x148]
00583c6c  4c 01 84 e5                                      str r0, [r4, #0x14c]
00583c70  50 01 84 e5                                      str r0, [r4, #0x150]
00583c74  3c 31 84 e5                                      str r3, [r4, #0x13c]
00583c78  40 31 84 e5                                      str r3, [r4, #0x140]
00583c7c  04 00 a0 e1                                      mov r0, r4
00583c80  01 10 a0 e3                                      mov r1, #1
00583c84  44 4d 00 eb                                      bl #0x59719c
00583c88  34 31 94 e5                                      ldr r3, [r4, #0x134]
00583c8c  b8 35 d3 e1                                      ldrh r3, [r3, #0x58]
00583c90  38 31 84 e5                                      str r3, [r4, #0x138]
00583c94  70 80 bd e8                                      pop {r4, r5, r6, pc}
00583c98  02 00 53 e3                                      cmp r3, #2
00583c9c  0a 00 00 1a                                      bne #0x583ccc
00583ca0  00 30 a0 e3                                      mov r3, #0
00583ca4  44 31 80 e5                                      str r3, [r0, #0x144]
00583ca8  48 31 80 e5                                      str r3, [r0, #0x148]
00583cac  4c 31 80 e5                                      str r3, [r0, #0x14c]
00583cb0  50 31 80 e5                                      str r3, [r0, #0x150]
00583cb4  3c 31 80 e5                                      str r3, [r0, #0x13c]
00583cb8  40 31 80 e5                                      str r3, [r0, #0x140]
00583cbc  00 10 a0 e3                                      mov r1, #0
00583cc0  35 4d 00 eb                                      bl #0x59719c
00583cc4  34 31 94 e5                                      ldr r3, [r4, #0x134]
00583cc8  b8 35 d3 e1                                      ldrh r3, [r3, #0x58]
00583ccc  38 31 84 e5                                      str r3, [r4, #0x138]
00583cd0  70 80 bd e8                                      pop {r4, r5, r6, pc}
00583cd4  04 00 a0 e1                                      mov r0, r4
00583cd8  00 10 a0 e3                                      mov r1, #0
00583cdc  2e 4d 00 eb                                      bl #0x59719c
00583ce0  34 31 94 e5                                      ldr r3, [r4, #0x134]
00583ce4  b8 35 d3 e1                                      ldrh r3, [r3, #0x58]
00583ce8  38 31 84 e5                                      str r3, [r4, #0x138]
00583cec  70 80 bd e8                                      pop {r4, r5, r6, pc}
; Evidence package domain: light
; ELF VA 0x00583cf0, file offset 0x00583cf0, range size 40, SHA-256 c09a79d31b19eee1bdd446c8f1e9a1d3a28a80da5afe7cd9a8b66b06f6dd12d4
; Index name: glitch::scene::CLightSceneNode::getBoundingBox() const
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CLightSceneNode-bf70b6dba9f9-001.asm
; FUNCTION 0x00583cf0, declared_size=40, range_size=40, mode=arm
; class-group: glitch::scene::CLightSceneNode
; alias: _ZNK6glitch5scene15CLightSceneNode14getBoundingBoxEv
; demangled: glitch::scene::CLightSceneNode::getBoundingBox() const
; decoder-mode: arm
00583cf0  10 40 2d e9                                      push {r4, lr}
00583cf4  34 31 90 e5                                      ldr r3, [r0, #0x134]
00583cf8  38 21 90 e5                                      ldr r2, [r0, #0x138]
00583cfc  00 40 a0 e1                                      mov r4, r0
00583d00  b8 35 d3 e1                                      ldrh r3, [r3, #0x58]
00583d04  03 00 52 e1                                      cmp r2, r3
00583d08  00 00 00 0a                                      beq #0x583d10
00583d0c  c1 ff ff eb                                      bl #0x583c18
00583d10  4f 0f 84 e2                                      add r0, r4, #0x13c
00583d14  10 80 bd e8                                      pop {r4, pc}
; Evidence package domain: light
; ELF VA 0x00583d18, file offset 0x00583d18, range size 28, SHA-256 bb2b02b0c864b54e913036e067d389ee0d8ce7ec6add32f623101e44f21977e7
; Index name: glitch::scene::CLightSceneNode::render(void*)
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CLightSceneNode-bf70b6dba9f9-001.asm
; FUNCTION 0x00583d18, declared_size=28, range_size=28, mode=arm
; class-group: glitch::scene::CLightSceneNode
; alias: _ZN6glitch5scene15CLightSceneNode6renderEPv
; demangled: glitch::scene::CLightSceneNode::render(void*)
; decoder-mode: arm
00583d18  10 31 90 e5                                      ldr r3, [r0, #0x110]
00583d1c  00 10 a0 e1                                      mov r1, r0
00583d20  14 00 93 e5                                      ldr r0, [r3, #0x14]
00583d24  00 00 50 e3                                      cmp r0, #0
00583d28  1e ff 2f 01                                      bxeq lr
00583d2c  4d 1f 81 e2                                      add r1, r1, #0x134
00583d30  42 9b 00 ea                                      b #0x5aaa40
; Evidence package domain: light
; ELF VA 0x00583d34, file offset 0x00583d34, range size 84, SHA-256 24f3646a99bfb1bfbd5d0b761ab614f98a43b503e23eba138079a9d522ce2c1e
; Index name: glitch::scene::CLightSceneNode::onRegisterSceneNode()
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_scene_CLightSceneNode-bf70b6dba9f9-001.asm
; FUNCTION 0x00583d34, declared_size=84, range_size=84, mode=arm
; class-group: glitch::scene::CLightSceneNode
; alias: _ZN6glitch5scene15CLightSceneNode19onRegisterSceneNodeEv
; demangled: glitch::scene::CLightSceneNode::onRegisterSceneNode()
; decoder-mode: arm
00583d34  30 40 2d e9                                      push {r4, r5, lr}
00583d38  00 10 a0 e1                                      mov r1, r0
00583d3c  10 01 90 e5                                      ldr r0, [r0, #0x110]
00583d40  1c d0 4d e2                                      sub sp, sp, #0x1c
00583d44  00 20 a0 e3                                      mov r2, #0
00583d48  00 30 90 e5                                      ldr r3, [r0]
00583d4c  18 40 8d e2                                      add r4, sp, #0x18
00583d50  01 50 a0 e3                                      mov r5, #1
00583d54  24 c0 93 e5                                      ldr ip, [r3, #0x24]
00583d58  04 20 24 e5                                      str r2, [r4, #-4]!
00583d5c  02 31 e0 e3                                      mvn r3, #0x80000000
00583d60  0c 00 8d e9                                      stmib sp, {r2, r3}
00583d64  02 30 a0 e1                                      mov r3, r2
00583d68  00 50 8d e5                                      str r5, [sp]
00583d6c  04 20 a0 e1                                      mov r2, r4
00583d70  3c ff 2f e1                                      blx ip
00583d74  04 00 a0 e1                                      mov r0, r4
00583d78  9a 33 f6 eb                                      bl #0x310be8
00583d7c  05 00 a0 e1                                      mov r0, r5
00583d80  1c d0 8d e2                                      add sp, sp, #0x1c
00583d84  30 80 bd e8                                      pop {r4, r5, pc}
; Evidence package domain: light
; ELF VA 0x0059fc00, file offset 0x0059fc00, range size 64, SHA-256 6d1f1233c07b40e938aa98e97982ae523acedfa42db1f739184c45a88b00462a
; Index name: glitch::video::CLight::setAbsoluteTransformation(glitch::core::CMatrix4<float> const&)
; Source listing: ../recovery/dh2-reconstruction/recovered/native/assembly/libDungeonHunter2.so/glitch_video_CLight-909492ef9a46-001.asm
; FUNCTION 0x0059fc00, declared_size=64, range_size=64, mode=arm
; class-group: glitch::video::CLight
; alias: _ZN6glitch5video6CLight25setAbsoluteTransformationERKNS_4core8CMatrix4IfEE
; demangled: glitch::video::CLight::setAbsoluteTransformation(glitch::core::CMatrix4<float> const&)
; decoder-mode: arm
0059fc00  10 40 2d e9                                      push {r4, lr}
0059fc04  54 30 d0 e5                                      ldrb r3, [r0, #0x54]
0059fc08  00 00 53 e3                                      cmp r3, #0
0059fc0c  04 00 00 1a                                      bne #0x59fc24
0059fc10  41 20 a0 e3                                      mov r2, #0x41
0059fc14  50 00 90 e5                                      ldr r0, [r0, #0x50]
0059fc18  12 bb f5 eb                                      bl #0x30e868
0059fc1c  01 00 a0 e3                                      mov r0, #1
0059fc20  10 80 bd e8                                      pop {r4, pc}
0059fc24  10 00 9f e5                                      ldr r0, [pc, #0x10]
0059fc28  03 10 a0 e3                                      mov r1, #3
0059fc2c  00 00 8f e0                                      add r0, pc, r0
0059fc30  1a ac 01 eb                                      bl #0x60aca0
0059fc34  00 00 a0 e3                                      mov r0, #0
0059fc38  10 80 bd e8                                      pop {r4, pc}
; mapping-symbol data/literal pool
0059fc3c  1c ff 33 00                                      .byte 0x1c, 0xff, 0x33, 0x00
