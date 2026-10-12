# Fixed filename source close continuation

FilenameRootRouteV65 now uses one retained close continuation for normal copy/open/close loading and cancellation. Previously cancellation returned complete whenever a pending/failed provider had cleared opened.actual_owner; normal loading could pass empty metadata into the next close. The unfinished close prefix could therefore be dropped before authentic unload.

The continuation pins the SAME independent IStream owner across pending, failure and exceptions. Cleared native identity remains cleared; it never reconstructs a handle after native destruction. Provider replacement fails explicitly. Completion is recorded before recursion guards, so a completed native close is not replayed during cancellation retry. Source loading errors remain sticky while explicit cancellation can finish the retained close prefix.

Changed active private filename_root_source_v65.hpp and aligned the existing coherent native-file candidate copy. Main needs this source correction in its root sibling. Comprehensive milestone fixtures must cover pending/failure after metadata clearing, exception, replacement, recursive close and completed close without replay. No compiler, test, engine or emulator job was run. Whole-level cleanup/render acceptance remains unverified.
