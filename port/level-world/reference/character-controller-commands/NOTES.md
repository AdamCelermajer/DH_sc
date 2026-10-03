# Character controller command reconstruction

The complete `v2Controller::Cmd_LookAt(object)` (`4052bc`),
`Cmd_MoveTo(object)` (`405540`) and `Cmd_Stop` (`40559c`) execute the same
gates: a nonzero forced byte bypasses global blocked and local locked bytes;
otherwise either block suppresses the command. The manifest binds the complete
functions, Character adjustment thunks and getters to the owned original ELF.

The selected Character controllable subobject is at `+374`. Its object look
method ignores a null target; otherwise it captures its point virtual before
the target-position query and invokes it with the returned point. The four-byte
override at `3addbc` is a tail branch to `GameObject::LookAt(Point)` at `393cec`.
It is **not** an empty override. The point body remains an explicit service in
this module, alongside complete GameObject Stop, PathTo and Character RaiseEvent.
Existing heading reconstruction supplies the original angle arithmetic, but
live ownership and the complete point-call binding remain separate work.

Move and Stop query the owner's actual `IsRemotelyUpdated` virtual first. Move
checks a null target only afterward. Stop invokes complete GameObject Stop,
then synchronously raises Character event `3f` with null payload. Target position
uses cached XYZ only when both the cache pointer and cached-position byte are
nonzero. Remote status is true when the network word differs from `ffffffff`,
otherwise its raw byte determines the result.

The original-versus-O2-ARM64 corpus executes the actual controller wrappers,
Character thunks/bodies, remote getter and target getter. Its 2,456 comparisons
include raw byte gates, forced bypass, remote suppression, null targets, cache
selection and exact point words. The 2,329 ordered service requests match with
zero differences. Five malformed-call guards and eight service-failure prefixes
are checked independently. Service failures may leave preceding synchronous
effects; this is not a transactional API.

The native host audit replays the same original corpus through the actual world
shared library with identities above 4 GiB. Deeper service fixtures do not prove
full pathfinding, orientation, physical Stop, event dispatch or live AI integration.
