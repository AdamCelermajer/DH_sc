# Same retained chest callback construction

Additive successor to retained graph checkpoint81cda0afacdcc44a, archive SHA256
747e5f3894cdf9738e701e5b5c7d84acc17b52f06d02347524fa8302894b6ea8.
That1156-entry archive and its verified receipt are unchanged.

`CanonicalLevelClassDispatchInputsV1` appends optional
`openable_services_with_receiver(source, weak_slot, services, error)`.
The callback receives a shared weak publication cell before the actual
CanonicalOpenableGraphV4 C1. Bind table/visual/condition/PF callbacks to that
cell, and lock it when reached. After C1 the adapter publishes its SAME actual
graph result into the cell. RNG uses this identical cell. No second canonical
base, object, scene, property storage or Level is allocated for the callbacks.
The older `openable_services` callback remains supported when the new hook is
absent. Hook failure stops before C1; unsupported declarations still fail in
the original unfiltered walk.

For the engine-owned data cache, the intended binding is:

```cpp
input.openable_services_with_receiver =
    [actualData, actualWorld, actualRoots](const auto&, const auto& slot,
                                          auto& services, auto&) {
        services.world = actualWorld;
        services.roots = actualRoots;
        services.container.resolve_row =
            [actualData](const auto& name, auto& id, auto& row, auto& error) {
                return actualData->resolve_row(name, id, row, error);
            };
        services.container.visual_asset =
            [actualData, slot](int id, auto& error) {
                auto graph = slot->lock();
                if (!graph) {
                    error = "Required same retained Container receiver";
                    return false;
                }
                return actualData->visual_asset(graph->receiver().base(), id, error);
            };
        // Bind the remaining actual providers when available.
        return true;
    };
```

`actualData` must be the initialized engine-owned
OpenableContainerDataConnectionV11 with genuine group5 record/name extraction
and full GameObjectDict streams. This small successor supplies the binding
point only. It does not yet consume an unfrozen table provider or satisfy the
GetDataId, full Character SetPosition or later initialization boundaries.

The existing original-cache Swamp source test now verifies each pre-C1 slot
starts empty and resolves to its exact retained chest result. Host, ASAN/UBSAN
and Android5590 checks pass; both Android ABIs compile. Map/Character/RNG and
all existing failure boundaries remain unchanged. The visible APK is unchanged
and full-loader acceptance is false.

Import the CPP/HPP together and rebuild their consumers coherently, since the
construction input structure gained an appended field. Keep the shared
engine/Level headers authoritative; no vendor snapshot replacement is needed.
The full dependency graph is the frozen base archive plus these changed files.
