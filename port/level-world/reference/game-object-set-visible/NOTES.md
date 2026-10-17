# Visibility setters

Complete callers: GameObject::SetVisible `0x38b0f0/32`; VisualObject::SetVisible
`0x471368/104` (96 code + 8 literal bytes). GameObject captures +2d8, restores
raw +8a for nonzero input, writes +80, then calls **SyncVisibility**. VisualObject
compares raw input with root flags+11c bit0, calls **SceneManager::ForceRegister**
`0x350ee0` on mismatch, reloads root+8 and virtual+48, and forwards raw input.
Equal visibility still calls the setter; null root skips it. Owner+4 is unread.
The GOT resolves the inline Application singleton; +10 is read directly.

SyncVisibility/ForceRegister/node setter bodies remain external. ARM tests
execute both complete callers with named provider probes; source lifetime and
nonnull reached application/device remain adapter responsibilities. Port
alignment/alias/error guards preserve prior effects. Services are captured;
nested same-owner calls require separate live outputs. No native wiring.

```text
python port/level-world/tests/run_game_object_set_visible_host.py --original-elf ABSOLUTE_ELF --compiler ABSOLUTE_GXX --build-dir port/level-world/build/game-object-set-visible
```
