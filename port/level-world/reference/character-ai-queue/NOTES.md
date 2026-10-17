# Shared AI update queue

`character_ai_queue` reconstructs the original `CharAI::IncUpdateQueue`
568-byte caller (`0x3ce9b8`). Native bounded pointer storage replaces the
original STL deque allocation and block representation. It is one app-owned
queue shared by actors. `turn_globals` supplies the existing source-tested
`CharAI::IsMyTurn` reader without changing its predicates.

A positive signed timer is captured before `Application::GetDt`; subtraction
wraps in 32 bits and overwrites any callback timer mutation. A nonpositive
timer is set to 180 before querying queue size. Zero/one entries return.
Larger queues rotate the first entry before evaluating the next candidate,
then examine at most `count-1` candidates. Readiness follows source forced,
global block, lock, virtual Faerie, virtual Follower, visibility, virtual
Zonable, zoned and in-zone tests. The owner captured before IsZonable remains
the source for its two subsequent fields; the final zoned read uses a fresh
AI owner. Missing services retain prior timer/rotation effects.

The root comparison executes the original 568-byte body, genuine deque
subtraction, and original deque push/pop instructions inside one existing
allocation. Its 44 cases cover zero/one/multiple entries, signed timer edges,
raw virtual results, forced/lock/block gates, and owner/timer mutation.
GetDt and concrete Character virtual predicates are fixture providers.
Allocation boundaries, queue registration/destruction, native live ownership,
and full gameplay are outside this proof.

Run `tests/run_character_ai_queue_host.py --compiler <C++17 compiler>
--original-elf <pinned ELF> --output <proof directory>`.
