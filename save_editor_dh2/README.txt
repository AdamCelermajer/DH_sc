DH2 Save Editor (Dungeon Hunter 2, Track B 1.0.2)
==================================================

Edits a hero's savegame: gold, potions, and items (see every item with its full info, add items from the catalog of all
items the game can drop, with the bonuses the game allows, remove items, change stack counts). Same editor on the
phone and on Windows. The rules come from the reconstructed original game code, the item texts and colours from the
game's own data, so edited saves only contain what the original game can produce.

What is in this folder
  dh2-trackb-0f84a5b63-saveeditor-<hash>.apk  the Track B build 0f84a5b63 with the editor added: a second app icon "DH2 Save Editor".
  DH2-Save-Editor-Windows\          the Windows app (keep the folder together): dh2-save-editor.exe + web\
  proof\                            screenshots of the checks (game and editor)

PHONE
  1. Install the APK over your Track B game (same signature, it installs as an update; your saves stay):
     adb install -r dh2-trackb-0f84a5b63-saveeditor-<hash>.apk   (or open the APK file on the phone)
  2. Best: close the game first (swipe it away from the recent apps). The game keeps the hero in memory and would
     write over the edit, so the editor never writes while the game process lives. If the game is still running when
     you tap Save, the editor asks: "The game is running. Unsaved progress in the game will be lost. Stop the game and
     save?" - Cancel leaves everything as it is; "Stop the game and save" ends the game (like swiping it away) and saves.
     It never stops the game without asking. (Stopping a process looks like a crash to Track B's "The game crashed last
     time" check; after a stop the editor ends its own process cleanly when it starts the game or when you leave it with
     nothing unsaved, so the game starts normally.)
  3. Open "DH2 Save Editor" from the app list, tap dh2_000.savegame (your hero), edit, tap Save.
  4. After saving the editor offers "Start the game".
  Backups: before every save the editor copies the file to files/editor-backups/<name>.<date> and to <name>.bak
  (the game's own backup name) in the game folder /sdcard/Android/data/com.gameloft.android.GAND.GloftD2SS/files/.
  To undo: copy a file from editor-backups back over dh2_000.savegame (adb push, or a file manager that can open
  Android/data).

WINDOWS
  Double-click DH2-Save-Editor-Windows\dh2-save-editor.exe. It opens in a Microsoft Edge app window; closing the
  window ends it. Nothing is installed.
  * Open folder: a folder that contains dh2_000.savegame (for example a copy of the phone's files folder).
  * Phone over USB (adb): USB debugging on, phone connected and accepted. The editor pulls the saves into
    phone\<serial>\ next to the exe, and on Save: checks that the game is not running on the phone, pulls a fresh
    backup to phone\<serial>\editor-backups\, copies the phone's file to <name>.bak on the phone, pushes the new
    file and reads it back to verify. adb.exe is taken from the Android SDK (ANDROID_HOME or
    %LOCALAPPDATA%\Android\Sdk\platform-tools), from PATH, or from an adb folder next to the exe. With more than one
    device connected, type the serial (adb devices) in the box.

ORGANISED LISTS
  * Inventory: the "Equipped" section comes first (the items the hero wears in the active set, each with its slot:
    Head, Torso, Hands, Waist, Feet, Right hand, Left hand, rings), then the bag, in every grouping mode.
  * Rows: the item's name and row edge in the game's rarity colour (White / Green / Blue / Purple / Gold = 0..4
    bonuses), chips for damage, armour, block and requirements (red when the hero fails them), the bonuses as chips
    coloured by category (attributes, life and mana, elemental, attack, defence, resistances, spells, loot).
  * Speed: group contents are built when a group opens, descriptions and checks are cached, the search is debounced;
    the lists stay fast on a phone with the full catalog (1170 rows) and 100 inventory items.
  * Inventory and "Add item" can be grouped by set (the game's armour sets such as Marksman or Lordly, and the
    same-name armour families such as Soldier, Marauder, Captain: names from the game's text), by slot / type, or not.
  * Every row shows the game's own stats and requirements texts (damage min-max, armour, block, level, str/dex/end/ene,
    class). Items the hero cannot equip are greyed with the reason, using the game's rule (ItemInstance::IsEquippableBy:
    level, the four attributes, class). Hero attributes = the class's starting values + points spent (from the save) +
    bonuses of the items equipped in the active set; they match the game's Stats screen. Skill/potion buffs are not
    in the save and not counted. "only items the hero can use" filters the catalog.
  * Add item in 2 taps: search (or open a set / slot group) and tap an item. It is added at once to the bag with bonuses
    pre-ticked for the hero's class: the strongest tier of each bonus, as many as the item's loot rules allow, only
    combinations the game can drop. Class preferences (web/dh2save.js, CLASS_PREFS), then the biggest values:
      Warrior (Knight, Berserker, Paladin): Strength, Endurance, Life (HP), Damage %, Armour, Defence rating, All attributes, Block
      Rogue (Assassin, Archer): Dexterity, Critical, Damage %, Attack rating, Fire/Air/Lightning/Water/Earth damage, Dodge
      Mage (Necromancer, Illusionist): Energy, Mana (MP), Mana regeneration, elemental damage, Critical, spell damage
    A toast "Added <name> - Edit bonuses" opens the edit screen to change the picks. Only the strongest tier of each bonus
    is ever shown (no weaker tiers).
  * Edit an item you have (also equipped ones): tap it, "Edit bonuses". Same picker and checks; the item keeps its row,
    equipment slot and quantity, the value is recomputed like a new drop, and a Before / After table shows the change.
    Checked in the reconstructed game: an edited equipped helm shows its new bonuses and the Stats screen changes.

EDITING RULES (from the original code)
  * Gold: 0 .. 999,999,999 (ItemInventory::m_maxGold of a player).
  * Potions: one kind (Potion0). Any whole number 0 .. 2,147,483,647: the save stores the count as a 32-bit number. A
    new hero starts with 5; the game's own pick-up cap (Special_Loot_Max_Potions) comes from skills at run time. Big
    stacks are the user's choice.
  * Rarity: the game colours an item by its number of bonuses: 0 White, 1 Green, 2 Blue, 3 Purple, 4 Gold
    (ItemInstance::GetColor, FontPalette). There are no rarity names in the game's text.
  * New items: only rows some loot table can drop (not gold, potions, quest items). Bonuses come from that loot
    entry's bonus list, the count must be one the entry's number table can roll, and bonuses that the game keeps
    apart (ItemBonusAttrMonopoly) cannot be combined (ItemInventory::_AddLootItemPowers). Value = CalcLootItemValue.
    "_Hard"/"_VeryHard" rows use the Hard/Very Hard bonus variants like the game. New items go to the bag (not
    equipped); equip them in the game. The bag holds 100 items.
  * The hero's checkpoint copy (dh2_000_single.checkpoint) gets the same inventory, so both copies agree.

LIMITATIONS
  * Level, XP, skills, quests: not edited (first version).
  * Prices: the editor shows the item value and merchant sell prices; the game's "Transmute" value depends on the
    hero's transmute bonus and is not shown.
  * Languages: the game's 8 text packs; in this data some packs hold other languages (e.g. GERMAN = Russian text).

BUILDING (developers): see tools/build.py in the save_editor_dh2 repository (one command; reads dh_noam, never
writes to it).
