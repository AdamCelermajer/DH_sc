# Same VM source protocol extensions

Narrow adaptation of Adam c3ae797 runtime/first-return/indexed-return code into the existing C VM. Legacy APIs/statuses and discarded return projection retained. Source file bytes use `loadFile()`/1024-byte reader, positive Lua status, numeric/string Error conversion and explicit unsupported object -4. Required callback marker -1001 advances epoch; first/indexed calls report -5 even when Lua pcall caught it. Load keeps source status; caller queries epoch.

Every return projects in source order before observer. Strings remain stack-rooted and are borrowed only through synchronous observer; pointers stay full width. Observer cannot throw/destroy/reenter VM. No new VM/Include/object provider or complete original body credit. Source owner/player provider wiring remains pending.

Run `python port/adam-script-runtime/tests/run_source_compatibility_host.py --compiler <g++> --original-elf <ELF>`; real Lua host semantics, original ranges pinned only.
