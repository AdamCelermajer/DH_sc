# PlayerLightTweaker source inputs V82

SOURCE ONLY. No tests, compiler, native execution, hashes, fixtures or proof packets. V80 active/candidate callback now takes `(shared_ptr<GameObjectSceneRootRegistryV1>,float,bool,error)` and invokes0.0f,false. Its fog receipt/errors identify first actual App58 PlayerLightTweaker. SAME source ownership retained.

## Resource transport

Canonical existing input: `C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip`, entry `com.gameloft.android.GAND.GloftD2SS/files/data/tweaker/player_light.tweaker_xml` (7602bytes). The adjacent file is an unchanged source copy for Main; it is not generated defaults or a fixture. It is UTF-16LE with ff fe BOM, XML declaration, `attributes` root, nested `group` hierarchy and typed attributes with name/value/desc/range. Original constructor sprintf is `%s/data/tweaker/player_light.tweaker_xml`. Existing Root `OriginalCacheAssetsV1::read("data/tweaker/player_light.tweaker_xml",found,bytes,error)` supplies the same retained original resource; use current mounted cache/service owner, not a second archive.

Root's XmlDocumentV1/TinyXML route is the actual level parser; it is not the recovered wchar CXMLAttributesReader/CTweakable implementation and raw UTF-16LE bytes cannot be treated as its normal UTF-8 level document. No CXMLAttributesReader/CTweakable registration/parser implementation was found in current native engine-resources/engine-ui/level-world source. No alternate parser or tweaker is introduced here.

## Exact original constructor registrations consumed by Stage34

PlayerLightTweakerC1 32f2b4 first constructs SAME CTweakable/device. registerVariable(float&)32ef4c creates SMapping(type1,actual float pointer); registerVariable(SColor&)32eeb8 creates SMapping(type5,actual color pointer). Names/register calls are:

- `m_fogColor`, actual four-bytef4, color registration32f628.
- `m_fogStart`, actual float110,32f63c.
- `m_fogEnd`, actual float114,32f650.
- `m_fogDirectionMask.X/Y/Z`, actual floats11c/120/124,32f664/678/68c.

The names and source pointers must belong to SAME actual App58 owner. Direction has C1 zero stores32f350/354/358. C1 does not explicitly initialize f4/110/114. Stage34's existing source body produces its real fog values before the later original fog/tweak consumers. Preserve missing/unproduced constructor fields; do not fabricate constructor defaults.

## Important: XML values are not C1 field assignments

Authored Fog metadata includes m_fogEnabled=true; start/end=0.0; direction.X/Y/Z=0.1; camera masks=0.1; camera offsets=0.0; m_fogColor=ffffff. These are genuine resource contents, not proof that C1 writes these values into fields.

Exact CTweakable.loadXML32b6b8:

1. Stack CReadFile(filename,false)570734; null file80 skips parse and proceeds source destruction.
2. SAME device6c.filesystem34 virtual50 obtains wchar XML reader from the real CReadFile.
3. Stack CXMLAttributesReader(reader,false,NULL)570d18, stack CAttributes(actual device driver10)562b78, reader.read571920.
4. Pop groups to root, getString("Tweakable")564768, assign nonempty result to SAME CTweakable name54.
5. Local helper32b650 ONLY recursively walks getGroupsCount/setGroup/pop. CAttributes vtable973fb0+8 confirms virtual40=getGroupsCount5618f0,44=setGroup561908,3c=pop5618d8. No setValue call or registered-cell assignment appears in that helper/loadXML.
6. XML reader drop, temporary string release, CAttributesD1, XMLAttributesReaderD1 and ReadFileD1 in source order.

CTweakable.setValue32b3b4 is separate: resolves the existing registration map by exact attribute name, misses return without new mapping; type1 gets actual CAttributes.getFloat(index)virtual74 and stores one word; type5 gets CAttributes.getColor(index)virtual128, stores its four bytes preserving original returned color encoding; afterward invokes SAME tweaker virtual14/onSetValue with the registered name. Do not invoke this body from C1/loadXML where original does not. A bounded static direct-BL call scan found no direct caller of32b3b4; indirect/UI dispatch remains distinct and is not claimed absent.

Main's new retained tweaker should consume genuine C1 registration/device/resource/name/group metadata production and existing source fog stores. Merely decoding XML value attributes into a new map and applying them as defaults would change original constructor semantics. No rendering/tweaking interaction algorithm is reconstructed or replaced here.
