# Complete PathTo and point-facing source kernels

`original-functions.json` and `original-functions.asm` bind the owned original
ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The native implementation is `character_path_commands.hpp/.cpp`.

GameObject.PathTo at 0x3939f0 skips when owner byte84 is nonzero. It requests
FindPath when the valid owned route ring is empty, or when the XYZ squared
distance from cached target208 to the new target is strictly greater than
40000. Each float operation follows source order. The actual original
`__aeabi_fcmpgt` is executed by the comparison runner; distance exactly200
does not replace the route. NaN comparison is false, while positive infinity
can trigger replacement. Source owner26c is the unsigned request limit; zero
selects30. Native `path_nonempty` describes a valid ring, not corrupt-memory
traversal. Original FindPath remains an explicit borrowed service.

GameObject.LookAt(Point) at 0x393cec computes direction from the owner's XYZ
position and executes the complete original LookTowards at 0x393b1c. Character's
point override branches into that body. Native facing modifies only the heading
angle; it does not invent an active movement heading or overwrite direction.
The additive unchecked heading entry preserves original atan/constant branch
behavior instead of substituting atan2. Nonfinite arithmetic follows the source;
NaN payload identity is outside the comparison claim.

The original-versus-optimized ARM64 audit passes937 PathTo cases,989 point-facing
cases,84 exact ordered FindPath requests and three unavailable/failure checks.
It executes the actual original valid route-ring traversal, floating comparisons
and full LookTowards arithmetic. Alias input pointing at owner position is
covered. The gold corpus SHA256 is
`12d4e2f6eb4f77f1c69ce59a2cfb03000d7e3cfe461866a995f939afd5af70d9`.

The genuine host world-DSO replay passes the same corpus,11 caller guards and
64 real Crypt FindPath sessions under ASan/UBSan. Those additional floor-center
sessions produced14 found and50 failed paths using source limit30. They are
native composition checks, not64 original floor-pair comparisons or proof that
every destination is reachable. Failed routes are retained as failures.

The renderer connects the source point-facing path to its prototype Prince
controller. Full MoveTo/Stop ownership, original target-node/cache positioning
and complete AI/path planning remain separate integration work. Source kernels
and their bounded instruction proofs do not establish full-game/GPU parity or
physical ARM64 device behavior.
