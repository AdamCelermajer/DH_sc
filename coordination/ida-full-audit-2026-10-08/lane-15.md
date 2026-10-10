# Lane 15 - Box2D and online/network SDK static audit

Working-tree read-only audit against HEAD marker 75a7c2fe3261403e841ffbeb46734e9e4e84e75c. Only lane-15.md and lane-15.csv were written. No source edits, builds, tests, emulator or external messages. All runtime acceptance is not_run.

Coverage: **1,866 / 1,866 assigned records**, exactly one row each for libDungeonHunter2.so function numbers **26126-27991** inclusive. All 1,866 pseudocode files and assembly bodies read/extracted: 1,866 success, 0 failed, 0 absent. Exact one-instruction branch thunks: 43; no PLT import entry lies in this slice. CSV retains each body/code hash, callers/callees, data refs, source hashes and underlying unresolved disposition.

**Complete record disposition is not completed semantic parity.** SDK bodies without established current source/provider mappings remain unclear. Nontrivial physics paths retain partial status where full statement-level arithmetic/ABI/error-domain equivalence could not be established. Static xrefs do not close every indirect callback. These limits must remain in the merged audit.

| Final disposition | Records |
| --- | ---: |
| clone | 201 |
| disconnected | 1 |
| import/thunk | 43 |
| matched | 104 |
| missing | 2 |
| partial | 72 |
| unclear | 1443 |

Clone rows retain underlying status; thunks are assembly-proven wrappers and do not establish their destination implementation. Underlying statuses: disconnected=1, matched=123, missing=2, partial=72, unclear=1668.

## Findings and traced source behavior

### F15-1 - Physics source exists and is linked; full parity remains bounded

Records 26126-26297 (172) cover contact manager/island, joint factory and six joint families, allocators, TOI/circle/polygon collision/contact solver and GJK distance. Per-record source definitions are under port/physics-backend/box2d-2.0.1/Source. Getter/setter, constructor, callback and solver outlines were compared to the bodies. Long arithmetic paths remain partial; version/provenance labels were not used as complete parity evidence.

IDA #26131 (0x7ea9d0) integrates forces, clamps damping, limits linear speed 200/angular 250, initializes contacts/joints, solves velocities, stores warm-start impulses, integrates positions, optionally solves positions at 0.2 and sleeps at thresholds 0.5/0.0001/0.00012346. b2Island.cpp:138 follows that staged algorithm. #26130 uses TOI correction 0.75; #26129 reports results through listener virtual+20. NativeWorld installs real contact/boundary/filter/destruction listeners; physical_world.cpp:66 updates with world.Step(milliseconds*0.001f,10). Level-world CMake adds physics-backend and links dh2_box2d_201; backend CMake compiles vendored cpp with fast math/contraction disabled. Parent source wiring exists; complete current gameplay acceptance was not run.

Contact destruction #26126 reports removal before unlinking world/body lists and freeing. Contact Evaluate #26162/#26170 preserves manifold IDs/impulses and distinguishes Add/Persist/Remove. Factory #26142 dispatches six joint enums. Gear position #26295 chooses revolute angle versus prismatic translation, applies coupled correction, synchronizes both bodies and returns true (source linearError=0). These source-backed paths are not missing.

Allocation #26236/#26237 is an intentional native adaptation: original assembly reserves 4 bytes; b2Settings.cpp uses alignof(max_align_t) while retaining header/accounting. Native alignment is justified, but byte-count/layout observations differ. Source assertions and arbitrary invalid-domain inputs were not accepted as original shipping behavior. Historical full-world-arm64-differential.json reports 40 representative worlds/0 mismatches, not every joint branch or integrated current gameplay.

### F15-2 - Online and local matching are narrow source projections

SetIsOnlineGame #26364 (0x7fd50c) is STRB r1,[r0,#5]; BX lr, mapped to SourceOnlineLoadingOwnerV55.set_is_online_game. Application lazily retains the same byte5 facet (application_services_owner_v5.hpp:79). GetInstance #26377 constructs a 72-byte COnlineImpl; base constructors #26379/#26380 create queues/mutexes/time/sync cells. Current facet has two bytes and identity. It cannot establish SendPackets #26372, ReceivePackets #26382, Update #26389, time-sync or teardown parity; neighboring bodies stay unresolved.

Matching.Get #26485 (0x800f8c) supports local 1/Bluetooth 2/GLLive 3/4. MatchingLocalSelectionOwnerV4.get supports local identity only, mode 0 -> 1 and narrow Reset, rejecting other providers. IsServer #26419 joined-byte/member/server gates and GetMemberIdList #26474 maximum 32/stride 432/value 0x170 are represented in a local scalar array; full room/socket/NetStruct/event lifecycle is not.

CNetPlayerInfo.IsLocal #26796 (0x80f1ec) is represented in PlayerNetworkLocalOwnerV4 with SAME PlayerInfo parent backing. IsActive #26792 four-cell online gate differs from the selected derived PlayerInfo offline-true branch; generic bare-CNet parity is not claimed. Reset #26799 conditionally dirties network members while the narrow constructor successor only projects final values. NetStructMember.SetChanged #26920 (0x814f84) source helper agrees on dirty 0x1c, masks 0x14/0x10 = member 0x18, old process timestamp 0x8 and counter increment for selected PlayerInfo fields. Generic ack/history/serialization remains unresolved.

### F15-3 - Packet registration body is disconnected from production

Original ObjectManager.NetworkInitLevel #1690 (0x340be0, cross-lane) registers slot 3 through #26935 (0x815258) when online, with actual sWritePacketData/sReadPacketData/sProcessAcknowledgedPacket/sProcessLostPacket callbacks. Assembly proves92-byte slot stride, used-byte guard, four stores and bitmap update. packet_slot_registry_v88.hpp:20 implements those metadata stores and retained process identity. Scoped production search finds no register_slot caller, only unregister_slot(3) in source_campaign_release_v88.cpp:260. Registration is disconnected within current source scope. This metadata owner explicitly excludes packet payload/queue transport.

Unregister #26936 (0x8152c0) matches writes used=0, callback +0x10, then +4/+8/+0xc and bitmap clear and is connected to release. Source returns bool success while ARM returns remaining bitmap; original ObjectManager.NetworkUnInitLevel #1689 (0x340bc4) returns it. Current return-sensitive consumers are unestablished, so this remains partial.

### F15-4 - Two positive source seams lack native providers

CMessaging.SendMsg #26729 (0x80e2a4) sets destination 0xFFFFFFFF and enqueues owned CMessage; SendMsgTo #26728 routes same-member locally or sets a member mask and queues sending. Registration/factory/serializers/reliability/ack/locks/queue processors are real bodies. source_script_ui_world_v97.cpp:81 explicitly fails positive online sends with Required positive CMsgScriptCmd constructor/network SendMessage transport; renderer_player_attack_v1.inc:57 similarly requires packet queue continuation. The positive send transport is missing at these concrete source routes. Offline suppression is implemented. Other unestablished SDK bodies remain unclear, not blanket missing.

MatchingGLLive.IsHost #27257 (0x81f524) gets the live GLXPlayerMPLobby and dispatches virtual +0x3c IsMaster. LoadingMenu requires matching_is_host in states 3/4 (loading_menu_v1.cpp:86); LevelConstructor requires it on its positive nonlocal-host path. source_online_loading_menu_v135.hpp supplies only enabled, and scoped current producer search found no matching_is_host assignment beyond binding passthrough. The positive host provider is missing on that concrete seam. Offline paths are explicit; no online/lobby runtime was run.

### F15-5 - Remaining networking and GLX closure is unresolved

The remaining 1,694 records include event/room/matching state, messages, NetBitStream, NetStruct histories, CNetPlayerManager, packet reliability, receiver threads, CConnection ping/keepalive/timeout, network emulation, UDP/TCP/Bluetooth, Unicode/blob/parsing and GLX HTTP/login/friend/leaderboard/message/lobby. Every record has body triage and direct xrefs; constructors/destructors/no-op methods/thunks/byte duplicates/STLport templates are distinct. No generic out-of-scope or missing claim comes from search alone.

Traced lifecycle boundaries: Matching.Initialize registers packet callbacks; ConnectionManager.InitializeInternal registers packet 5/ping 0; TransportManagerImpl starts a joinable ReceiverThread looping ReceiverThreadExecute with 10ms sleeps until stop byte 252. Termination sets stop before joining and destroys transports under mutex. NetBitStream uses high-bit-first bit positions and low-byte-first U32 chunks. ARM float/int-return and shift conventions need assembly before recreation; no current source parity is asserted for these unresolved paths.

GLXPlayerComponent.RegisterObserver #27717 stores actual observer +4. WebComponent.Update #27821 has an 18000ms timeout and routes network/completion/error through the live request/observer; OnUpdateParse #27817 parses f/g/function-id/r/s/e before callback selection. MPLobby.mpProcessIncomingMessages #27969 switches packet type/current lobby state, dispatches retained observer and destroys packet ownership after callbacks. Connection/ConnectionLobby/Thread/GLXPlayerPing callees cross to lane16. Those owner/protocol/callback paths remain unresolved and cannot be replaced by constant flags.

## Search and cross-boundary scope

S2 searches covered current port C/C++/hpp/h/inc/CMake source using subsystem class/method tokens, original-address leads and production provider/member/slot sites. Historical reports/reference/tests/integration snapshots were excluded from producer claims. Vendored Box2D was examined explicitly and connected by CMake. All port source addresses were scanned for lane entry-address leads. A miss remains unclear except the two concrete required seams and unused registration method. Native aliases/facets mean search evidence alone is not proof of source absence.

Exported xrefs: 2,315 incoming and 5,110 outgoing cross-lane instruction references. CSV retains every direct interfunction entry-target call/jump site and data reference, with underlying source disposition applying to each edge. Indirect/vtable closure is unverified. Important inbound callers: Application.Update/LoadLevel, ObjectManager network/serialization, PlayerInfo/PlayerManager, Lua/script, MultiplayerCallback, CXPlayerManager. Outgoing lane16 protocol/thread/connection functions, other-lane physics world/body functions and libc/compiler boundaries are explicit.

## Identical emitted-body groups

Exact code SHA256 and equal whitespace-normalized executable pseudocode are both required, preventing arbitrary PC-relative same-byte branches from grouping different targets. First record is canonical; every member has its own row and underlying status. Shared emitted return sequences do not prove shared receiver ownership or source coverage.

| Code SHA256 | All assigned record numbers |
| --- | --- |
| `379bec29dccd0a93c94826144d7ef6e42fab64ef195a3b8313a16926f66f388f` | 26136, 26137, 26150, 26155, 26163, 26190, 26203, 26231, 26240, 26241, 26246, 26279, 26293, 26313, 26315, 26316, 26317, 26318, 26319, 26363, 26492, 26500, 26656, 26732, 26733, 26734, 26735, 26738, 26739, 26740, 26741, 26742, 26743, 26786, 26787, 26809, 26942, 26987, 26988, 27066, 27068, 27069, 27085, 27086, 27087, 27128, 27129, 27130, 27131, 27132, 27133, 27134, 27135, 27136, 27137, 27138, 27139, 27140, 27141, 27145, 27156, 27158, 27159, 27281, 27282, 27283, 27284, 27286, 27293, 27340, 27341, 27342, 27378, 27473, 27476, 27477, 27498, 27530, 27531, 27546, 27547, 27557, 27558, 27561, 27592, 27593, 27596, 27634, 27635, 27644, 27931, 27932, 27933, 27934, 27935, 27936, 27937 |
| `684eb0f1ae24552b214a09f468d9ead9d1ea6911426cc4483c8b89e7ebb042d3` | 26138, 26139 |
| `007f34a6c3441b0240da53f253e513b959105da2d8803255e1856aa48f48db47` | 26143, 26502, 27064, 27065, 27379, 27386, 27387, 27478, 27480 |
| `3061f8159f0e1e9ac1e655b7b925e76c39489bfd7d49fe84945b110990be794b` | 26148, 26217 |
| `6dcd5e75586fe6683156c0d559d4827b73bd48d501fa5781a1c6099aacd7c877` | 26149, 26199, 26278, 26299, 26365, 26415, 26417, 26616, 26662, 27096, 27144, 27146, 27148, 27149, 27150, 27151, 27154, 27294, 27295, 27296, 27297, 27345, 27361, 27474, 27475, 27479, 27526, 27527, 27528, 27529, 27532, 27533, 27534, 27535, 27536, 27537, 27538, 27539, 27540, 27541, 27542, 27543, 27544, 27545, 27549, 27552, 27553, 27554, 27555, 27584, 27587, 27588, 27589, 27590, 27930 |
| `403cd6b5ca0bfb178f87e11ec622a7cf167028b7ca2f2e62ef7816f9a1316fac` | 26156, 26164, 26247 |
| `2769a7361987f02cd9992286851b50892fb5d98c7b44c3a4cc7587f46a802807` | 26202, 27990 |
| `80b4228909ea142ac4d965cb0f9df2968e4ea9c4db6c18e9a0069ceca162c906` | 26218, 27830, 27858, 27885 |
| `b9548f1ce2be2b18d75b8d1b1b48faf070a3326a14a679dd0e93c97e352b0f7d` | 26223, 27829, 27857, 27985 |
| `66da709498946a188b0c2d299cbf265c8ab722ecb31e0f35e5e4f9e84b6c5b0e` | 26238, 26239 |
| `6042108c996ec8853dbec03bcaa45849453c010c3986287288ac1a338b1b92c0` | 26416, 26548, 27147 |
| `f794a96f7ae6cca84512e07d84d5433bb44a5caedd5c59ce72b1fa2cdf86e291` | 26433, 26996 |
| `f784f97224e88b07c2539cb007a7a0f937f635d59f538c9f3e1d31af77ad9831` | 26434, 26994 |
| `48066c39f31d2e9a1eef7268277f72287b41fde39a0959050f3185decdbc4de5` | 26546, 27157 |
| `505b94340fbfdd3d8e442a68bb956489c518053106e3f320e3074676d41dcf1c` | 26730, 26731 |
| `adb591a7f69f280d3116791af6e43c0d420e841d275885417c7192bd9524cbb5` | 26736, 26737 |
| `9bd61643affd205bd31a59c3e09cc3b992222765ed9dbd64298a01d6c70eb218` | 27037, 27038 |
| `d6bcdcc94c922d2220e987041d631009c591efdf5d49572703b171fc3492833b` | 27067, 27344 |
| `ae5cd4653b8d552cc544515329f444c0365b59788fe1c12c58171d8a19fac68d` | 27083, 27084 |
| `6143299c415a779d14d46483b05772d81c6b595bc189b4499503e30275041932` | 27143, 27559, 27560, 27594, 27595 |
| `1ff1708058b0b0f9e64f355a1b0c018bb88dd69c3b718f81696264598cb8d835` | 27279, 27280 |
| `4d939f78463abacbe0a8ce6acd5b3e25c1de79e8e0e04e683e6b9e9a107b6adf` | 27500, 27501 |
| `960d40c07ed0cf4e8cce1c6cbccba998d728d602349bb959e807f564feaf8244` | 27550, 27585 |
| `3cdb7a4b040d7da8de38907e72b332de347a3ff17597f54f7070e92fc3eb9668` | 27551, 27586 |
| `800ecd3a9bf4eed9516d82750fb7e3e5fd6de10e95e3819c2c40875115d9972d` | 27556, 27591 |
| `a5d1ebb661b3004e8aa73d2371157e025609734a594fa060d11bec5372cee31b` | 27715, 27716 |
| `280d9531648cae4b688eb10a394ecdee9829b6b51fe4073da227194b4c8ca2fd` | 27718, 27786 |
| `2ba7962e839653cc59757dd877095c695a1f9ddfe008e935d110f92781f2a120` | 27827, 27856, 27984 |
| `f1651055a36ad883aa66764063b48eab21f296159db5940800e03ab0d66bc81d` | 27828, 27859, 27897, 27987 |
| `c5bd4fdc6db8744b456f25d436159584f99ed90c62ea2ec69e7c87cb41db8a17` | 27852, 27980 |
| `a6857e4a4726555222ec3dd39dd62b12a175e99c3b9ca62c326f55842f3fb966` | 27853, 27981 |
| `3cf5be3cb5caff6585306fab97e30efbecfa1d8278d6cd2d74372dae0f7ed058` | 27855, 27983 |
| `83f5157b329cf8ef15195f82b1f9d56ad2d1ec16bd15e02dbd92fb2ffc9ae6f0` | 27896, 27988 |
| `c19b1897bacd43f6d38a41f1ace73d7009c5f44e36eb238efeb4f2c14c78d13a` | 27938, 27939 |

## Cited source SHA256 snapshot

Files were hashed from current working-tree bytes and rechecked at report creation. Changed files: none observed within this comparison window.

| Source file | SHA256 |
| --- | --- |
| `port/android-native/app/src/main/cpp/CMakeLists.txt` | `ee593e57786a13b37a90364667da0507d1ca4e92d56adc0dccac02b05217da45` |
| `port/android-native/app/src/main/cpp/renderer_player_attack_v1.inc` | `cecfb75d984034d3b81923550fa057d78d2086f0634afddf00b4fd46782e9aad` |
| `port/android-native/app/src/main/cpp/source_campaign_release_v88.cpp` | `d98ae5cebbd12db699de9c9e22d2bc2a621736e60624f6ffe7f7db43b3fb74b3` |
| `port/android-native/app/src/main/cpp/source_script_ui_world_v97.cpp` | `e8b1bff049b815b4dcf7daf4d15b6595c2cb3d55de5f4ca10e2a314749452fd2` |
| `port/engine-ui/loading_menu_v1.cpp` | `30ba37ebc50d0e3aedef2cb7d094ce588c4f6731c117ef0c731363c99a7fbe72` |
| `port/level-loader/level_constructor_v3.cpp` | `039c0c476f29314f7c2f87b04217b4b632575786563167143ad6ea23df00e5c6` |
| `port/level-world/CMakeLists.txt` | `7f0e6314dd7e2e7afde9fce9a579d5f75e917b2c3f039cabf15f8ef78ec434b4` |
| `port/level-world/application_services_owner_v5.hpp` | `1dae0d7d970c2aaef6eb475ce89ec4b2e4f11f685ff4797c41e1c7028d8afef1` |
| `port/level-world/gameobject_online_update_v5.cpp` | `d738cef09105103dbaa98f6c58f4763341e7fbb54664d1742bb1fdf0aeab2214` |
| `port/level-world/packet_slot_registry_v88.hpp` | `b0bb7f3c2f2f2d901212733da6ce24c37f524ef18d005c6051eee5272bc380ee` |
| `port/level-world/physical_world.cpp` | `104dd9049d1ca95ed2bfdcfb6a83748d0a34f9e6447c0c1ec015d26bcc5c0ba4` |
| `port/level-world/player_manager_owner_v1.cpp` | `6c173138000ecc63c3774151077c23ae7d59fb1bb72272a972a555949f20f45a` |
| `port/level-world/player_network_local_owner_v4.cpp` | `02870b9001f5a99da159d2a79955e8c1e3b93aeab99a99b974c8e10f0dea713a` |
| `port/level-world/player_network_local_owner_v4.hpp` | `fa8af239087e5aab4f530fe69338cf317da948c964c38bb05efcfa888d5666bf` |
| `port/level-world/source_online_loading_menu_v135.hpp` | `011628fc7216e916f42a7fcec65ae8ddadcbb85554eb374355902dfa3ee7a3b0` |
| `port/level-world/source_online_loading_owner_v55.hpp` | `e96ed204bda5782ff5b113444742cfa9d20464ca3f9367d4a0171e4457d83860` |
| `port/physics-backend/CMakeLists.txt` | `8e7041f90cf62cc78a52085cac6ddcceface17825916f493ff836561616d0d10` |
| `port/physics-backend/README.md` | `f553c0a692e044a762493eab284faf751a00c06c650136d867a5b0e3675c347e` |
| `port/physics-backend/box2d-2.0.1/Source/Collision/b2CollideCircle.cpp` | `a0ce1f621ae819514cfd26b2b9231cfd1fb301d960857256c809bd0620495113` |
| `port/physics-backend/box2d-2.0.1/Source/Collision/b2CollidePoly.cpp` | `22b2e4611e696f041298ae0b63596f2eb54159d4a039fc65036290295080dacc` |
| `port/physics-backend/box2d-2.0.1/Source/Collision/b2Distance.cpp` | `0e5e43d8a6318555994ad7dbd44e3d8d38430964ea8989991aed9c880365dc1a` |
| `port/physics-backend/box2d-2.0.1/Source/Collision/b2TimeOfImpact.cpp` | `a4b145c6773ad798844f83ee7fb621a143261a237372e1fbb82c27bb72ee6201` |
| `port/physics-backend/box2d-2.0.1/Source/Common/b2Settings.cpp` | `72cc86a675e9caff585175b36ef3eef9d83f2785bba26f5dfd0ad3433aacc6a2` |
| `port/physics-backend/box2d-2.0.1/Source/Common/b2StackAllocator.cpp` | `abbfebe42cd6d45ee5a46d8afb031c8e6560908eadaab010152afb94e3a8a8c4` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Contacts/b2CircleContact.cpp` | `ded2f2cda858750019b6859922ac5d2b0916ff1f0d388a3fab2ed9689a665c9d` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Contacts/b2CircleContact.h` | `7d5d026bc0f7e4d05bfb43f6f01cc5581b672138cd30ab5fb804a8dd42abd231` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Contacts/b2ContactSolver.cpp` | `c9c0560fe24e87b37c54268b8a90a36ce3261e61647fc76acf4b38f09b6cf65c` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Contacts/b2PolyAndCircleContact.cpp` | `0ff192b00e5a99843a0c9515b60a600540f2bdb87b0c493d7d8427798b1891fb` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Contacts/b2PolyAndCircleContact.h` | `a15c3588c089059af1d99b7733412518059e16c2dc2df6c92cd99089fb66d55e` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Contacts/b2PolyContact.cpp` | `04e0d8d2be9eb55e2374935f01d09be785959e806f49ec478700cd07f7d6955c` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Contacts/b2PolyContact.h` | `67ad4841a0f4a008722bbd1fb41b49b46bb60ea0b1d63fd92002528224acbd8a` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Joints/b2DistanceJoint.cpp` | `0853ceb8d146a5db0ec0eea208f8751f61386f41532f4edf3ac0e72a0108a34c` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Joints/b2DistanceJoint.h` | `c167e9f0362edb80c8949e8c66fb9ac267fed3628313c4b696cd055cd88ed100` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Joints/b2GearJoint.cpp` | `4a5424b398f016dc9a8c5b5538066a6c5e391e9748498e702bb5d060ad44083d` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Joints/b2GearJoint.h` | `e61df42dd9366d6f472bf18843838579863602281c336b4148f634b7f9a60fd5` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Joints/b2Joint.cpp` | `b6a439cbbf8bc4a4fe835d29b652fcfedf5affb3a8dc7786b68ed1e1a45547dd` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Joints/b2Joint.h` | `b01f4e90a0ea7044b9cadeb5c8cf91ae752e2c115cf0f764c1f45a249e4778f9` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Joints/b2MouseJoint.cpp` | `0a9d8342dfa3dc12af1876421d3c33e71b5532d14db8cba0bbf24bf80f6b1e78` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Joints/b2MouseJoint.h` | `820476f8770f51e86df059703a54244e37291cadf48be436582d2341d5921473` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Joints/b2PrismaticJoint.cpp` | `ee3981cfa0e94d82799d6eebecf81a09cd2bc137ac6f43a6c04ef5de197a9a28` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Joints/b2PrismaticJoint.h` | `0f9fc788a01c0cb02b3e959d9d2a2d6eed1012276288bff39d1dd6cd019f9081` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Joints/b2PulleyJoint.cpp` | `5cb5b0c762d2e6164c79e62316930f9d317fe4f99be8376481f08ebae157a83a` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Joints/b2PulleyJoint.h` | `3a3b2642c88952e5412c29409ea066d18b82611710e8cb926b2d83f1931731c4` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Joints/b2RevoluteJoint.cpp` | `c720e0b2cfe29d40292536913d32b2da0f74cc2d60759c5fe6e67cb735b56c33` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/Joints/b2RevoluteJoint.h` | `608269ad82b95ba822abcb65d55f1c82dcba40ed07cb35eba22236ebfc58d1ff` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/b2ContactManager.cpp` | `7c370f2d5012f921ce8b2a3ae194332792f5e773f3031e61e7c2c4f4a9dcb6a8` |
| `port/physics-backend/box2d-2.0.1/Source/Dynamics/b2Island.cpp` | `6cb95aa76e75683326a5e0a1c841d7df4501559d79d5456ef74dcc7787a0d79f` |

## Unresolved limitations

All integrated runtime cells are not_run. Historical component fixtures do not accept the current complete application, arbitrary joints or multiplayer protocols. Long-body arithmetic/ABI/error-domain parity stays partial; unestablished SDK/compiler-template mappings remain unclear, including message queues, generic serialization, room state, transport/thread/socket and GLX callbacks. Symbols annotated [clone] are not assumed to be semantically identical; only the recorded code/body groups receive a proven clone disposition. Indirect calls and data-driven protocol configuration are not fully closed. Hashes cover cited files within the comparison window, not the entire dirty tree. This lane provides complete inventory accounting and concrete findings; it does not satisfy the global semantic/runtime completion gate by itself.
