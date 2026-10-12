# Native constant backend

`script_constants.hpp/.cpp` owns named integer constants and supplies the
`kind0` design lookup used by GetPyCst. Actual original reloadData, readString,
integer readers, string construction, RB map insertion and getConstant execute
in the oracle. The allocator, libc, integer division and single-thread mutex
imports are explicit services. Four diagnostic logger/string call sites are
skipped; original Application startup and logging are not reconstructed here.

The original reads 26 ordinary cache files completely, assigning 5,608 values.
It merges entries across reloads, overwrites duplicate keys and retains omitted
old values. Zero-entry groups do not insert anything. Names are C strings after
readString: embedded NUL truncates the key. A name of 255 bytes succeeds; a name
of 256 or more consumes 255 bytes and stops the reload after prior assignments.
The source count controls completion; trailing bytes are not consumed.

The sound input is mixed: SoundBus contains string-valued records after six
integer groups. The generic original loader assigns 27 entries, consumes 994
of 1,283 bytes, and stops. This is an observed partial load, not successful
sound-format decoding. It is excluded from ordinary integer asset bundling.

The proof uses 27 cache inputs and nine additional streams, 5,969 actual
original getConstant queries and 5,646 assignments. Its chosen sorted file
order does not establish original Application load order. The native bounded
reader rejects short input instead of reproducing stale stack contents.

GetPyStruct/GetPyOID still need their array registration/getter backend; kind1
delivery fails explicitly here. Native constants and bound contexts outlive
their VMs. No live AI, APK instruction parity or physical GPU result is claimed.
