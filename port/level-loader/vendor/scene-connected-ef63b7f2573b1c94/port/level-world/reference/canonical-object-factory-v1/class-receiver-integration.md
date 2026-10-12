# Canonical class receiver composition

`CanonicalClassReceiverBindingsV1` borrows the application-owned
`CanonicalPropertyMapV1`; retain both until all canonical receivers are removed.
Its identity map owns only receiver leases and dispatch closures. It owns no HP,
inventory, script state, pose, physical body or property values.

Create `CanonicalReceiverConstructionV1` callbacks for Character,
OpenableContainer and AnimatedDecor. Each callback must allocate the real class
transport using the same World pin and fresh sole runtime. For Character this is
the renderer's derived `RetainedCharacterActorV1`, not the bare base class. For
chests it is `CanonicalOpenableContainerV1` borrowing a runtime owned by the same
retained transport. Runtime must be constructed before the chest and destroyed
after it.

In each callback use `canonical_class_receiver_v1(receiver)` to bind the actual
canonical Handle and property view. Attach `init_post`, `is_game_object`,
`position`, and `set_position` closures to genuine class methods. Do not substitute
the property vector writer for source SetPosition: it has bounds/physical/visual
continuations. Missing callbacks fail when reached. Source factory assigns
ObjectBase class-name20 before property dispatch; the property closure obtains a
fresh view each time so it sees this assignment.

Pass `bindings.services()` to `CanonicalObjectFactoryAttemptV1::execute`.
Manager duplicate-name Add returns the old registered identity; property calls
therefore reach the old receiver, as in the original. The actual duplicate
deleting destructor must first finish class teardown and call
`bindings.erased(new_identity)`; remove only the new constructor record. Normal
world removal must unpublish manager/language/target references and finish real
class teardown before erasing its retained lease. Prefix failures retain the
partially constructed receiver and must be explicitly rolled back through the
real destructor; repeated construction on the same live identity is rejected.

This composes callable owners. It does not supply absent class construction,
conditions, VisualObject/PODecor/PF, network assignment or source Debug providers,
and does not claim the all-50-Character/all-5-chest SWAMP production acceptance.

Standalone dispatcher test sources: tests/canonical_class_receiver_bindings_v1.cpp,
canonical_class_receiver_bindings_v1.cpp, canonical_property_map_v1.cpp. Its
receiver callbacks are declared fixtures; it verifies lifetime, same-schema
dispatch, duplicate result identity, preserved constructor failure prefix and
required lifecycle failure.
