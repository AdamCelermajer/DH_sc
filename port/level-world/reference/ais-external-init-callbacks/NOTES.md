# AIS initialization callback callers

Six source bodies are reconstructed: AISExternal `OnInit` (36 bytes),
`OnInitPost` (16), `OnInitFinal` (16), and the three matching AISDefault
empty leaves (4 bytes each). This unit adds six caller bodies and no
LuaScript, skill initialization, Binder or script dependency body claims.

`OnInit` executes the actual empty default body then calls the fixed name
`OnInit`. Post and Final directly tail-call `OnInitPost` and `OnInitFinal`.
They do not call their default counterparts, consult callback flags or gate
on VFTable membership. The original LuaScript::Call overload at `0x37c514`
receives the AIS pointer and literal name with no Arguments parameter.
The owning provider must preserve alias resolution, script execution and
discarded return-value conversion. Missing or failed providers stop explicitly;
no VM, callback table, or successful no-op service is invented.

AISExternal OnInit retains the entry AIS in ARM r4 across the default call.
Each native invocation captures the entry identity. Provider mutation of its
borrowed state survives return and is read on the next invocation. All borrowed
storage and provider contexts outlive synchronous return; control records and
output are disjoint. The native error guards are port contracts, not assertions
about invalid original pointers. One owning thread; same-output reentry and
destruction during callbacks are outside the contract.

The differential runner validates the original ELF, six symbol ranges, and
three source string byte ranges. It executes 18 external caller cases across
three identities and two callback mutation modes, plus the three empty default
bodies. LuaScript::Call alone is intercepted. Host cases also check provider
failure/exception, missing service, fresh identity and eight control guards.
The report pins all six maintained source/evidence files.
