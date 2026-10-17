# CharAI constructor and native queue registration

The two original 352-byte `CharAI` constructors install the class dispatch
table, initialize the explicitly named scalar fields, three empty tree headers
and a circular list, then append the exact `this` pointer to the shared queue.
The source queue's allocation/growth helper remains an explicit provider.

The reconstruction uses native pointer widths and owned container headers. It
is not an ARM32 memory-layout overlay. Field names retain source offsets when
the full meaning is not established. The table supplied to `construct` is the
native dispatch identity; no original game binary is loaded by the app.

The constructor **does not write owner+4 or alive+0x48**. It leaves active AIS
and alternate AIS null, master/target/last/requested target null, sets byte
0x4d (Character targetability) to one, sets pause to zero and sight/sticky to
zero. Character association and actual AIS allocation/initialization belong
to later original callers. These defaults alone cannot enable monster AI.

The host runner executes both complete original ARM constructors on several
initial byte patterns, empty/populated/growth-boundary queues and registration
callback mutations. It compares every projected field and verifies all
unwritten original bytes remain unchanged. It also checks native identities
above 4 GiB, unavailable/failed callbacks and retained callback effects.
Ordinary queue writes execute the original instructions. Only the deque growth
helper is replaced by a named fixture allocation boundary.

Initialization occurs before the registration service. A missing or failing
service retains those writes; there is no rollback or fabricated registration.
The caller must keep state, services, headers and callback storage alive and
must not reenter construction on the same object or move a registered object.
