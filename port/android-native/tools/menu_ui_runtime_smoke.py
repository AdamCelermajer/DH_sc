"""Tap the original menu/name/class/Start movie into SWAMP or debug Crypt.

API37/16KiB emulator only. Campaign profiles stay on the device: originals
are moved into a private transaction directory before launching, synthetic
profiles are archived there, and originals are restored in finally. No saves
or unfiltered Logcat are pulled. Complete NativeStartGame, campaign loading,
inventory initialization and full combat remain outside this check. Visible
development joystick and attack controls are checked for delivered input.
"""
import argparse
import hashlib
import io
import json
from pathlib import Path
import re
import shlex
import struct
import subprocess
import time
import uuid
import xml.etree.ElementTree as ET
import zipfile

from PIL import Image, ImageChops, ImageStat

PACKAGE = 'com.example.dh2'
VIEWPORT = 'DH2 native texture viewport'
CLASSES = ((0, 263, 48, 'KnightPlayerBase'),
           (1, 325, 50, 'RoguePlayerBase'),
           (2, 290, 49, 'MagePlayerBase'))
POINTS = {'main': (410, 94), 'key_a': (40, 219), 'intro_skip': (240, 160),
          'intro_skip_button': (447, 36),
          'name_confirm': (240, 126), 'class_right': (463, 190),
          'class_confirm': (240, 295), 'single_player': (410, 160),
          'next_slot': (170, 75), 'hud_attack': (425, 260),
          'hud_joystick': (70, 235), 'hud_character': (30, 26),
          'character_back': (18, 17),
          # Crossed blades on the authored CharacterMenu opens its Talent tab.
          'talent': (300, 14),
          # Center of the authored Upgrade Skill button in the API37 capture.
          'talent_upgrade_skill': (350, 300),
          # The source full-driver mapping targets the authored 480x320 stage.
          'inventory': (250, 14),
          # Sprite 439 equipment-row hit area; checked against the rendered
          # 480x320 stage, it opens InventorySheetDetails for the item list.
          'inventory_equipment_slot': (120, 68),
          # InventorySheetDetails action buttons, mapped from the retained
          # API37 capture at 2424x1080 onto its authored 480x320 stage.
          'inventory_unequip': (400, 44),
          'inventory_equip': (400, 192),
          # SWF sprite 444 (btn_Drop) is placed at (7326,4604) twips in
          # sprite 456; its visible hit panel (shape 442) centers at
          # (429,307) after the root Details offset (-69,966) twips.
          'inventory_drop': (429, 307),
          # menu_confirm2's WarningBox settles at frame 5: root menu origin
          # (134.5,67.35), WarningBox (-3.5,31), btn_yes (-8.15,65.6),
          # with the authored button hit shape centered near (154,203).
          'inventory_drop_confirm': (154, 203)}
BAD = re.compile(r'FATAL EXCEPTION|Fatal signal|Native frame failed|'
                 r'World load failed|Model draw GL error|Original menu frame failed|'
                 r'Native UI touch failure \| action=(?:DOWN|UP|MOVE|CANCEL|OTHER) \| '
                 r'Menu touch failed(?::[^\r\n]*)?|'
                 r'Player HUD (?:frame|attach) failed|Menu touch failed|'
                 r'Native Player AIS failure retained|Native Player AI timer failed|'
                 r'Start Game (?:failed|has no assigned save slot|selected slot differs)|'
                 r'Asset load failed|Required authored navigation clip absent|'
                 r'Menu touch failed(?:: SWF core busy)?')
SAFE_EVENTS = re.compile(r'(?:Surface resized to |Owned menu (?:renderer selected|navigation) \||'
                         r'Original class (?:selection updated|scene connected|scene animation) \||'
                         r'Original front(?: screen loaded| screen submitted|/HUD screen submitted) \||'
                         r'Native menu (?:profile created|slot assigned|preview selection) \||'
                         r'Authored NativeStartGame (?:request queued|continuation queued) \||'
                         r'NativeStartGame plan \||'
                         r'Native gameplay (?:class|property cache) \||'
                          r'Original gameplay menu (?:push request|selected) \||'
                          r'Inventory callback probe \||'
                          r'Inventory list callback complete \| slot -?\d+ \| rows \d+ \| player -?\d+|'
                         r'Transient debug LevelList override \|'
                         r'Menu game start \| slot \d+ \| (?:SWAMP|Crypt) \||'
                         r'Connected player HUD submitted \||'
                          r'Native Player AIS (?:initialized|retained) \||'
                          r'Native Player Save masks 2\+4 \||Native Character owner bound \||'
                         r'Source Random (?:GSInit::Update|Level::Unload) \||'
                         r'Native offline registration \||Native full PlayerInfo \||Native managed metadata \||Native WorldMap catalogue \||Native Quest (?:catalogue|startup|terminal discard) \||'
                         r'Player CharAI terminal cleanup \||Player teardown deferred(?: attempt \d+)?: |Menu return deferred: |'
                         r'Authored joystick actor update \||Authored HUD attack press \||'
                         r'Authored HUD character-menu (?:geometry|release) \|'
                         r'Gameplay HUD transition \| (?:NativeAwayFromHud|BackToHud) \|'
                         r'Native inventory click callback \| action (?:Equip|Unequip|Drop) \|'
                         r'Player position [-\d.]|Player input \|)')
def safe_failure_detail(text, prefix):
    line = next((row for row in text.splitlines() if prefix + ':' in row), '')
    detail = line.split(prefix + ':', 1)[-1].strip()
    detail = re.sub(r'(?i)(?:[a-z]:\\|/)[^\s,;)]*', '[path]', detail)
    return detail[:180]


def safe_menu_touch_failure(text):
    """Return only a tagged touch action and bounded, path-scrubbed JNI error."""
    match = re.search(r'Native UI touch failure \| action=(DOWN|UP|MOVE|CANCEL|OTHER) \| '
                      r'Menu touch failed(?:: ([^\r\n]*))?', text)
    if not match:
        return None
    detail = match.group(2) or 'native UI dispatch rejected the event'
    detail = re.sub(r'(?i)(?:[a-z]:\\|/)[^\s,;)]*', '[path]', detail)
    detail = re.sub(r'\b0x[0-9a-f]{6,}\b', '[address]', detail, flags=re.IGNORECASE)
    return {'action': match.group(1), 'detail': detail[:180]}

SOURCE_RNG_GSINIT = re.compile(r'Source Random GSInit::Update \| seed (\d+) \| sync (\d+) \| counters (\d+)/(\d+)')
SOURCE_RNG_UNLOAD = re.compile(r'Source Random Level::Unload \| seed (\d+) \| sync (\d+) \| counters (\d+)/(\d+)')
FULL_PLAYER = re.compile(r'Native full PlayerInfo \| fields (\d+) \| factory (\d+) \|'
                         r' level (-?\d+) \| class (-?\d+) \| Character660 (\d+) \|'
                         r' slot664 (-?\d+) \| counter (\d+)')
ASSIGN = re.compile(r'Native menu slot assigned \| slot (\d+) \| ordinal (\d+) \|'
                    r' manager stores (\d+) \| player stores (\d+)')
REGISTRATION = re.compile(r'Native offline registration \| entries (\d+) \| added (\d+) \|'
                          r' controllers (\d+) \| renumber (\d+) \| counter (\d+)')
MANAGED_METADATA = re.compile(r'Native managed metadata \| players (\d+) \| slot (-?\d+) \|'
                              r' published680 (\d+) \| name setters (\d+) \| class setters (\d+) \|'
                              r' level setters (\d+) \| Save (\d+) \| Character660 (\d+)')
WORLD_MAP = re.compile(r'Native WorldMap catalogue \| locations (\d+) \| lockers (\d+)')
QUEST_TABLE = re.compile(r'Native Quest catalogue \| definitions (\d+)')
QUEST_STARTUP = re.compile(r'Native Quest startup \| Save (\d+) \| Character (\d+) \|'
                           r' log b8 (\d+) \| log118 (\d+) \| constants (\d+) \| retained (\d+)')
PLAYER_SAVE_MASKS = re.compile(r'Native Player Save masks 2\+4 \| Character (\d+) \| Save (\d+) \|'
                               r' loader (\d+) \| Quest118 (\d+) \| QuestB8 (\d+) \|'
                               r' calls (\d+)/(\d+) \| retained (\d+) \| profile (\d+) \|'
                               r' mask4 requests (\d+) \|')
CHARACTER_OWNER = re.compile(r'Native Character owner bound \| object (\d+) \| Coordinator (\d+) \|'
                             r' Save (\d+) \| properties (\d+) \| inventory (\d+) \|'
                             r' item rows (\d+) \| loot rows (\d+) \| item count (\d+) \|'
                             r' equipment set (-?\d+) \| potion capacity (-?\d+) \|'
                             r' same V4 Character owner; development continuation')
START = re.compile(r'Authored NativeStartGame (?:request queued \| selected slot|continuation queued \| assigned PlayerInfo slot) (\d+) \|'
                   r' numeric difficulty (\d+) \| requested difficulty (-?\d+)')
CLASS = re.compile(r'Native gameplay class \| slot (\d+) \| class (\d+) \| preset (\w+)')
CLASS_INDEX = re.compile(r'Original class selection updated \| index (\d+) \| class (\w+)')
HUD = re.compile(r'Connected player HUD submitted \|.*?viewport (\d+) (\d+) \|'
                 r' character (\S+) \| HP (-?\d+) (-?\d+) \| MP (-?\d+) (-?\d+) \|'
                 r' XP (-?\d+) (-?\d+) \| frames (-?\d+) (-?\d+) (-?\d+) (-?\d+) (-?\d+)')


def inspect_apk(path):
    """Inspect real packaged ELF load segments without extracting assets."""
    result = []
    with zipfile.ZipFile(path) as archive:
        for name in archive.namelist():
            if not name.startswith('lib/') or not name.endswith('.so'):
                continue
            abi = name.split('/')[1]
            raw = archive.read(name)
            assert abi in ('arm64-v8a', 'x86_64') and raw[:6] == b'\x7fELF\x02\x01', 'Packaged ELF ABI differs'
            offset = struct.unpack_from('<Q', raw, 32)[0]
            size, count = struct.unpack_from('<HH', raw, 54)
            align = [struct.unpack_from('<Q', raw, offset + i * size + 48)[0]
                     for i in range(count)
                     if struct.unpack_from('<I', raw, offset + i * size)[0] == 1]
            assert align and min(align) >= 16384, 'Packaged load alignment below16KiB'
            result.append({'path': name, 'sha256': hashlib.sha256(raw).hexdigest(),
                           'minimum_load_alignment': min(align)})
    names = {abi: {Path(r['path']).name for r in result if r['path'].split('/')[1] == abi}
             for abi in ('arm64-v8a', 'x86_64')}
    assert names['arm64-v8a'] == names['x86_64'] and 'libdh2_native.so' in names['x86_64'], 'Packaged ABI library sets differ'
    return result


def stage_rectangle(width, height):
    # MenuFlash2DCamera sends the complete live driver rectangle to the SWF.
    return (0, 0, width, height)


def stage_point(bounds, surface, point):
    x0, y0, x1, y1 = bounds
    width, height = surface
    assert 0 <= x0 <= x1 <= width and 0 <= y0 <= y1 <= height, 'NativeSurface bounds outside Android surface'
    x, y, w, h = stage_rectangle(width, height)
    return (round(x0 + x + point[0] * w / 480),
            round(y0 + y + point[1] * h / 320))


def parse_focused_window(text):
    """Read focused Activity, accounting for Android 17's immersive overlay."""
    match = re.search(r'\bmCurrentFocus\s*=\s*Window\{[^\n}]*?\s'
                      r'([A-Za-z0-9_.]+)/([A-Za-z0-9_.$]+)(?:\s|\})', text)
    if match:
        package, activity = match.groups()
        if not (package == 'com.android.systemui' and
                activity.endswith('ImmersiveModeConfirmation')):
            if activity.startswith('.'):
                activity = package + activity
            return {'package': package, 'activity': activity}
    # Android 17 can leave ImmersiveModeConfirmation as mCurrentFocus while
    # MainActivity remains mFocusedApp and owns the responsive input channel.
    match = re.search(r'\bmFocusedApp\s*=\s*ActivityRecord\{[^\n}]*?\s'
                      r'([A-Za-z0-9_.]+)/([A-Za-z0-9_.$]+)(?:\s|\})', text)
    if not match:
        return None
    package, activity = match.groups()
    if activity.startswith('.'):
        activity = package + activity
    return {'package': package, 'activity': activity}


def parse_input_focused_windows(text):
    """Read only InputDispatcher's FocusedWindows section, with display IDs."""
    lines = text.splitlines()
    for index, line in enumerate(lines):
        heading = re.fullmatch(r'(\s*)FocusedWindows:\s*', line)
        if not heading:
            continue
        indentation = len(heading.group(1))
        result = []
        for entry in lines[index + 1:]:
            if not entry.strip():
                continue
            if len(entry) - len(entry.lstrip()) <= indentation:
                break
            match = re.search(r'\bdisplayId=(\d+),\s*name=.*?\b'
                              r'([A-Za-z0-9_.]+)/([A-Za-z0-9_.$]+)(?:\s|[\x27\x22}]|$)', entry)
            if match:
                display, package, activity = match.groups()
                if activity.startswith('.'):
                    activity = package + activity
                result.append({'display_id': int(display), 'package': package, 'activity': activity})
        if result:
            return result
    # Android 17's InputDispatcher dump no longer prints FocusedWindows. Pair
    # WindowManager's mFocusedApp check above with the app's normal, responsive
    # InputDispatcher connection on this single-display AVD.
    result = []
    for entry in lines:
        match = re.search(r"\bchannelName='[^']*?([A-Za-z0-9_.]+)/"
                          r"([A-Za-z0-9_.$]+)',\s*status=NORMAL,"
                          r".*\bresponsive=true\b", entry)
        if match:
            package, activity = match.groups()
            if activity.startswith('.'):
                activity = package + activity
            result.append({'display_id': 0, 'package': package, 'activity': activity})
    return result


def operation_context(command):
    # No stdout/stderr, private shell program, file path or profile content in
    # failure reports. Three command words identify the failed transport step.
    words = [word for argument in command for word in argument.split()][:3]
    return ', '.join('[path]' if '/' in word or '\\' in word or ':' in word else word
                     for word in words)


def button_rectangle(stage, bounds, surface):
    if stage == 'gameplay':
        x0, y0, _, _ = bounds
        width, height = surface
        cx, cy = x0 + width * .91, y0 + height * .87
        return (round(cx - width * .035), round(cy - height * .018),
                round(cx + width * .035), round(cy + height * .018))
    centers = {'main': POINTS['main'], 'name': POINTS['name_confirm'],
               'class': POINTS['class_confirm'], 'start': POINTS['single_player'],
               'character': POINTS['character_back'],
               'inventory': POINTS['inventory'],
               'inventory_equipment_slot': POINTS['inventory_equipment_slot'],
               'inventory_unequip': POINTS['inventory_unequip'],
               'inventory_equip': POINTS['inventory_equip'],
               'inventory_drop': POINTS['inventory_drop'],
               'inventory_drop_confirm': POINTS['inventory_drop_confirm']}
    cx, cy = stage_point(bounds, surface, centers[stage])
    _, _, w, h = stage_rectangle(*surface)
    return (round(cx - w * 35 / 480), round(cy - h * 14 / 320),
            round(cx + w * 35 / 480), round(cy + h * 14 / 320))


def verify_start(text, slot, character_class, preset, animation_table, debug_level_row=None):
    """Require a fresh actual assignment immediately before this launch."""
    starts = list(START.finditer(text))
    assert len(starts) == 1 and tuple(map(int, starts[0].groups())) == (slot, 1, 0), 'Expected one authored Normal Start request'
    assignments = [m for m in ASSIGN.finditer(text) if m.start() < starts[0].start()]
    assert assignments and tuple(map(int, assignments[-1].groups())) == (slot, 0, 1, 1), 'Canonical source Assign must precede NativeStart'
    registrations = [m for m in REGISTRATION.finditer(text) if m.start() < assignments[-1].start()]
    assert registrations, 'Source controller registration must precede authored slot assignment'
    registration = tuple(map(int, registrations[-1].groups()))
    assert registration[0] == 1 and registration[2] == 4 and registration[4] == 39, 'Canonical offline map/input/shared counter differs'
    classes = list(CLASS.finditer(text))
    assert classes and classes[-1].start() > starts[0].start(), 'Fresh gameplay class receipt missing'
    assert classes[-1].groups() == (str(slot), str(character_class), preset), 'Gameplay class differs from selected class'
    caches = re.findall(r'Native gameplay property cache \| class (\d+) \| animation table (\d+) \| max HP (-?\d+) \| max MP (-?\d+)', text)
    assert caches and tuple(map(int, caches[-1][:2])) == (character_class, animation_table), 'Live cached property2 differs from source class'
    hud = list(HUD.finditer(text))
    assert hud and hud[-1].start() > classes[-1].start(), 'Connected source HUD missing after gameplay class'
    character_identity = int(hud[-1].group(3), 0)
    values = tuple(map(int, hud[-1].groups()[3:]))
    hp, max_hp, mp, max_mp, xp, next_xp, *frames = values
    assert 0 < hp <= max_hp and 0 < mp <= max_mp and 0 <= xp < next_xp, 'Invalid live health/mana/XP source words'
    assert tuple(map(int, caches[-1][2:])) == (max_hp, max_mp), 'Live HUD maxima differ from class property cache'
    assert 0 < frames[0] < 100 and 0 < frames[1] < 100 and 0 <= frames[2] < 101, 'Original HUD health/mana/XP timelines invalid'
    assert frames == source_hud_frames(hp, max_hp, mp, max_mp, xp, next_xp), 'Original HUD frames differ from live source words'
    if debug_level_row == 23:
        assert 'Transient debug LevelList override | row 23 | campaign save unchanged' in text, 'Transient Crypt override missing'
        assert 'NativeStartGame plan | slot %d | row 23 | GOTHICUS_CRYPT_01 | source file 007_crypt_01.rule.xml' % slot in text, 'Source Crypt row 23 plan missing'
        assert 'Menu game start | slot %d | Crypt |' % slot in text, 'Actual generated Crypt start receipt missing'
    else:
        assert 'NativeStartGame plan | slot %d | row 41 | SWAMP | source file 001_swamp.mlx' % slot in text, 'Source-default row 41 plan missing'
        assert 'Menu game start | slot %d | SWAMP |' % slot in text, 'Actual source-default SWAMP start receipt missing'
    records = list(FULL_PLAYER.finditer(text))
    assert records and records[-1].start() > starts[0].start(), 'Fresh full PlayerInfo receipt missing'
    record = tuple(map(int, records[-1].groups()))
    assert record == (33, 1, 1, character_class, character_identity, slot, 45), 'Canonical record must use the metadata setters and publish its native Character owner'
    metadata = list(MANAGED_METADATA.finditer(text))
    assert metadata and starts[0].start() < metadata[-1].start() < classes[-1].start(), 'Managed metadata preparation must precede gameplay class loading'
    managed = tuple(map(int, metadata[-1].groups()))
    assert managed[:6] == (1, slot, 1, 1, 1, 1) and managed[6] > 0 and managed[7] == 0, 'Actual metadata Save publication/setters differ'
    maps = list(WORLD_MAP.finditer(text))
    quests = list(QUEST_TABLE.finditer(text))
    assert quests and int(quests[-1].group(1)) == 64, 'Actual retained Quest catalogue missing'
    quest_startup = list(QUEST_STARTUP.finditer(text))
    assert quest_startup and classes[-1].start() < quest_startup[-1].start() < hud[-1].start(), 'Actual Quest initialization must precede HUD'
    quest_values = tuple(map(int, quest_startup[-1].groups()))
    assert quest_values[0] > 0 and quest_values[0] != managed[6], 'Quest logs must belong to the distinct gameplay Save'
    assert quest_values[1] == int(hud[-1].group(3), 0), 'Quest children must use the HUD Character'
    assert quest_values[2:] == (192, 192, 1608, 0), 'Genuine per-Save Quest startup counts differ'
    save_mask_receipts = list(PLAYER_SAVE_MASKS.finditer(text))
    assert len(save_mask_receipts) == 1 and classes[-1].start() < save_mask_receipts[-1].start() < quest_startup[-1].start(), 'Exactly one Character SG_Load(2)/SG_Load(4) receipt must precede native Quest startup'
    save_mask_values = tuple(map(int, save_mask_receipts[-1].groups()))
    assert save_mask_values[:8] == (character_identity, quest_values[0], save_mask_values[2], character_identity,
                                    character_identity, 1, 1, 0) and save_mask_values[2] > 0, 'SG_Load(2) and SG_Load(4) must use the same Character, Save and both embedded Quest owners'
    assert save_mask_values[8] > 0 and save_mask_values[9] >= 9, 'SG_Load(4) must dispatch eight bound sections plus the online-state request'
    owner_bindings = list(CHARACTER_OWNER.finditer(text))
    assert owner_bindings, 'Reconstructed native Character owner publication missing'
    owner_values = tuple(map(int, owner_bindings[-1].groups()))
    assert owner_values[0] == owner_values[1] == character_identity and owner_values[2] == quest_values[0] and owner_values[3] > 0, 'PlayerInfo, Coordinator, gameplay Save and property owner identities differ'
    assert owner_values[4] > 0 and owner_values[4] not in owner_values[:4], 'V4 inventory has no distinct retained owner identity'
    assert owner_values[5:10] == (1322, 339, 0, 0, 12), 'Fresh V4 inventory differs from the canonical source table/property projection'
    assert maps and starts[0].start() < maps[-1].start() < metadata[-1].start(), 'Selected WorldMap decoder must precede metadata preparation'
    assert tuple(map(int, maps[-1].groups())) == (13, 3), 'Original WorldMap data differs'
    return {'slot': slot, 'class': character_class, 'preset': preset,
            'cached_property2': animation_table, 'assign_before_start': True,
            'numeric_difficulty': True, 'requested_difficulty': 0,
            'HP': hp, 'max_HP': max_hp, 'MP': mp, 'max_MP': max_mp,
            'XP': xp, 'next_XP': next_xp, 'HUD_frames': frames,
            'character': hud[-1].group(3),
            'full_PlayerInfo': {'fields': record[0], 'factory_registered': bool(record[1]),
                                'level': record[2], 'class': record[3], 'Character660': record[4],
                                'slot664': record[5], 'shared_counter': record[6],
                                'temporary_backing_policy': 'zero', 'role': 'local registry'},
            'offline_registration': {'entries': registration[0], 'added': registration[1],
                                     'controllers': registration[2], 'renumber': registration[3],
                                     'before_authored_assign': True},
            'managed_metadata': {'Save680': managed[6], 'published_once': True,
                                 'name_class_level_setters': list(managed[3:6]),
                                 'Character660_at_prepare': managed[7]},
            'native_character_owner': {'identity': owner_values[0],
                                       'Coordinator_identity_matches': True,
                                       'Save_matches_gameplay_Save': True,
                                       'property_owner_bound': True,
                                       'inventory_bound': True,
                                       'inventory_items_at_source_creation': owner_values[7],
                                       'inventory_table_items': owner_values[5],
                                       'inventory_loot_rows': owner_values[6],
                                       'source_AddCharacter_invoked': False},
            'world_map': {'locations': 13, 'lockers': 3},
            'quest_definitions': 64,
            'quest_startup': {'gameplay_Save': quest_values[0], 'Character': quest_values[1],
                             'log_b8': 192, 'log_118': 192, 'constant_queries': 1608},
            'source_save_masks_2_4': {'character': save_mask_values[0], 'save': save_mask_values[1],
                                      'loader': save_mask_values[2], 'embedded_quest_owners_match': True,
                                      'mask2_calls': save_mask_values[5], 'mask4_calls': save_mask_values[6],
                                      'retained': save_mask_values[7],
                                      'profile_identity': save_mask_values[8],
                                      'mask4_transport_requests': save_mask_values[9],
                                      'mask4_section_callbacks_registered': 8,
                                      'mask4_payloads_read': False}}


def source_hud_frames(hp, max_hp, mp, max_mp, xp, next_xp):
    # Match selected hud_player_values.cpp: wrapped signed*100 / signedmax,
    # HP/MP subtract1, source clamps, then HP also drives distress/hurt.
    def quotient(current, maximum):
        numerator = (current * 100) & 0xffffffff
        if numerator >= 0x80000000:
            numerator -= 0x100000000
        assert maximum, 'Source HUD denominator zero'
        result = abs(numerator) // abs(maximum)
        return -result if (numerator < 0) != (maximum < 0) else result
    hp_frame = min(99, max(0, quotient(hp, max_hp) - 1))
    mp_frame = min(99, quotient(mp, max_mp) - 1)
    xp_frame = min(99, quotient(xp, next_xp))
    return [hp_frame, mp_frame, xp_frame, hp_frame, hp_frame]


def preservation_scripts(transaction):
    """Bounded, private, reversible filename moves; no file contents pulled."""
    assert re.fullmatch(r'\.menu-ui-smoke-[0-9a-f]{32}', transaction)
    base = 'files/' + transaction
    patterns = 'files/dh2_[0-9][0-9][0-9].savegame files/dh2_[0-9][0-9][0-9].savegame.bak'
    preserve = f'''set -eu
test ! -e {base}
mkdir -p {base}/original {base}/synthetic
: > {base}/original.sha256
count=0
for file in {patterns}; do
  test -f "$file" || continue
  (cd files && sha256sum "${{file#files/}}") >> {base}/original.sha256
  mv "$file" {base}/original/
  count=$((count+1))
done
touch {base}/preserved
echo "$count"'''
    restore = f'''set -eu
test -d {base} || {{ echo 0; exit 0; }}
count=0
if test -f {base}/preserved; then
  for file in {patterns}; do
    test -f "$file" || continue
    test ! -e {base}/synthetic/"${{file##*/}}"
    mv "$file" {base}/synthetic/
    count=$((count+1))
  done
fi
for file in {base}/original/dh2_[0-9][0-9][0-9].savegame {base}/original/dh2_[0-9][0-9][0-9].savegame.bak; do
  test -f "$file" || continue
  test ! -e files/"${{file##*/}}"
  mv "$file" files/
done
if test -s {base}/original.sha256; then
  (cd files && sha256sum -c {transaction}/original.sha256 >/dev/null)
fi
touch {base}/restored
echo "$count"'''
    return preserve, restore


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('adb', 'serial'):
        parser.add_argument('--' + name, required=True)
    for name in ('apk', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--class-index', type=int, choices=(0, 1, 2),
                        help='Run one class to resolve a remaining regression risk.')
    parser.add_argument('--capture-talent-only', action='store_true',
                        help='Stop after capturing the authored Talent page; always restore device saves.')
    parser.add_argument('--stop-after-class-confirm', action='store_true',
                        help='Verify campaign publication and stop before Start or level loading; always restore device saves.')
    parser.add_argument('--stop-after-world-ready', action='store_true',
                        help='Tap Start once, capture the first World ready or failure receipt, then restore saves and stop.')
    parser.add_argument('--debug-level-row', type=int, choices=(23,),
                        help='Launch generated Crypt row 23 without changing campaign level rows.')
    parser.add_argument('--skip-intro', action='store_true',
                        help='Reveal the source-timed skip control, tap it, and verify completion.')
    args = parser.parse_args()
    selected_classes = tuple(row for row in CLASSES if args.class_index is None or row[0] == args.class_index)
    assert args.serial.startswith('emulator-'), 'Emulator only'
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=True)
    transaction = '.menu-ui-smoke-' + uuid.uuid4().hex
    preserve, restore = preservation_scripts(transaction)
    report = {'validation': 'FAIL', 'apk_sha256': hashlib.sha256(args.apk.read_bytes()).hexdigest(),
              'serial': args.serial, 'libraries': inspect_apk(args.apk),
              'scope': ('Actual original menu/name/class/Start taps, source profile creation/Assign, ' +
                        ('transient row 23 generated Crypt' if args.debug_level_row == 23 else 'source-default row 41 SWAMP') +
                        ' continuation, native Character owner, connected HUD, occupied-slot reopen and Home/resume. ' +
                        'Source _AddCharacter parity, mask-4 payload restoration, GEAR contents, full campaign/InitPost and combat remain open.'),
              'debug_level_row': args.debug_level_row,
              'private_transaction_directory': 'files/' + transaction,
              'personal_saves_pulled': False, 'cases': [], 'screenshots': []}
    events, pid, since, last_text = [], '', '', ''
    viewport_bounds = {}
    ui_flow_baselines = {}
    preservation_attempted = False
    pid_probe_totals = {'pidof_samples': 0, 'pidof_empty_or_unexpected': 0,
                        'ps_fallback_samples': 0, 'ps_exact_package_matches': 0}
    report['process_pid_probe_diagnostics'] = pid_probe_totals

    def adb(*command, allow_failure=False, binary=False):
        try:
            result = subprocess.run([args.adb, '-s', args.serial, *command], capture_output=True,
                                    text=not binary, timeout=60)
        except subprocess.TimeoutExpired:
            raise RuntimeError('ADB timeout: ' + operation_context(command)) from None
        if result.returncode and not allow_failure:
            raise RuntimeError('ADB operation failed: ' + operation_context(command))
        return result.stdout if binary else result.stdout.strip()

    def app_pid_observation(reject_pids=(), diagnostics=None):
        # pidof occasionally returns empty during API37 startup/resume even
        # though ActivityManager has the app. Verify with an exact process-name
        # row, parse locally, and retain only counters in the report.
        counters = pid_probe_totals if diagnostics is None else diagnostics

        def count(name):
            counters[name] = counters.get(name, 0) + 1
            if counters is not pid_probe_totals:
                pid_probe_totals[name] += 1

        rejected = set(reject_pids)
        count('pidof_samples')
        pidof_rows = adb('shell', 'pidof', PACKAGE,
                         allow_failure=True).split()
        observed = (pidof_rows[0] if len(pidof_rows) == 1 and
                    re.fullmatch(r'\d+', pidof_rows[0]) and
                    pidof_rows[0] not in rejected else '')
        if observed:
            return observed

        count('pidof_empty_or_unexpected')
        count('ps_fallback_samples')
        ps_output = adb('shell', 'ps', '-A', '-o', 'PID,NAME',
                        allow_failure=True)
        matches = []
        for line in ps_output.splitlines():
            fields = line.split(None, 1)
            if (len(fields) == 2 and re.fullmatch(r'\d+', fields[0]) and
                    fields[1].strip() == PACKAGE and fields[0] not in rejected):
                matches.append(fields[0])
        if len(matches) > 1:
            raise AssertionError('Multiple exact package process PIDs observed')
        if len(matches) == 1:
            count('ps_exact_package_matches')
            return matches[0]
        return ''

    def require_stable_app_pid(expected_pid, label):
        deadline = time.monotonic() + 5
        previous, stable = '', 0
        while time.monotonic() < deadline:
            observed = app_pid_observation()
            if observed == expected_pid:
                stable = stable + 1 if observed == previous else 1
                previous = observed
                if stable >= 2:
                    return
            else:
                previous, stable = '', 0
            time.sleep(.2)
        raise AssertionError('App process did not remain stable at ' + label)

    def private_script(script):
        # adb shell joins arguments, so quote the complete sh program once.
        return adb('shell', 'run-as ' + PACKAGE + ' sh -c ' + shlex.quote(script))

    def logs():
        nonlocal last_text
        assert app_pid_observation() == pid, 'App process exited or changed'
        last_text = adb('logcat', '-d', '-T', since, '--pid=' + pid, '-v', 'brief')
        touch_failure = safe_menu_touch_failure(last_text)
        if touch_failure:
            receipt = report.get('last_tap_receipt')
            if receipt:
                receipt['jni_failure'] = touch_failure
            raise AssertionError('Native/UI failure: Menu touch failed | action=' +
                                 touch_failure['action'] + ' | ' + touch_failure['detail'])
        failure = BAD.search(last_text)
        # Keep only the fixed diagnostic label matched by BAD. Never persist
        # the complete Logcat line, which may contain paths or player data.
        if failure:
            label = failure.group(0)
            diagnostic = label
            if label == 'World load failed':
                # The native loader's exception is fixed engine/build context,
                # useful for debugging; strip any paths before saving it.
                diagnostic += ': ' + safe_failure_detail(last_text, 'World load failed')
            elif label == 'Original menu frame failed':
                # Keep the first source/UI readiness failure actionable while
                # excluding machine paths from the saved smoke report.
                diagnostic += ': ' + safe_failure_detail(last_text, label)
            raise AssertionError('Native/UI failure: ' + diagnostic)
        return last_text

    def wait(predicate, label, seconds=45):
        deadline = time.monotonic() + seconds
        while time.monotonic() < deadline:
            text = logs()
            if predicate(text):
                return text
            time.sleep(.2)
        raise AssertionError('Timed out: ' + label)

    def collect_events():
        # Persist only the explicit nonpersonal source event allowlist.
        events.extend(line[line.index(match.group(0)):]
                      for line in last_text.splitlines()
                      if (match := SAFE_EVENTS.search(line)))

    def wait_focus(label):
        deadline = time.monotonic() + 25
        while time.monotonic() < deadline:
            focused = parse_focused_window(adb('shell', 'dumpsys', 'window'))
            input_windows = parse_input_focused_windows(adb('shell', 'dumpsys', 'input'))
            expected = {'package': PACKAGE, 'activity': PACKAGE + '.MainActivity'}
            expected_input = {'display_id': 0, **expected}
            if focused == expected and expected_input in input_windows:
                report.setdefault('focused_window_receipts', []).append(
                    {'phase': label, **focused, 'InputDispatcher_display_id': 0})
                return
            logs()
            time.sleep(.2)
        raise AssertionError('Timed out: MainActivity input focus at ' + label)

    def launch():
        nonlocal pid, since
        if pid:
            collect_events()
        old_pid = app_pid_observation()
        old_pids = {old_pid} if old_pid else set()
        adb('shell', 'am', 'force-stop', PACKAGE)
        deadline = time.monotonic() + 25
        while app_pid_observation():
            assert time.monotonic() < deadline, 'Old app process did not retire before fresh launch'
            time.sleep(.2)
        since = adb('shell', "date '+%m-%d %H:%M:%S.000'")
        # API37's synchronous Activity launch wait can hang after the app has
        # rendered. Prove startup from a fresh PID, actual submitted movie and
        # both window/input focus below instead of depending on that wait.
        command = ['shell', 'am', 'start', '-n', PACKAGE + '/.MainActivity']
        if args.skip_intro:
            command += ['--ez', 'skip_intro', 'true']
        if args.debug_level_row is not None:
            command += ['--ei', 'debug_level_row', str(args.debug_level_row)]
        reply = adb(*command)
        assert 'Starting: Intent' in reply and 'Error:' not in reply and 'Activity not started' not in reply, 'Fresh launcher start failed'
        deadline = time.monotonic() + 25
        pid, candidate, stable = '', '', 0
        poll = {'pidof_samples': 0, 'pidof_empty_or_unexpected': 0,
                'ps_fallback_samples': 0, 'ps_exact_package_matches': 0,
                'stable_exact_package_samples_required': 2,
                'sample_interval_ms': 200}
        while stable < 2 and time.monotonic() < deadline:
            observed = app_pid_observation(old_pids, poll)
            if not observed:
                candidate, stable = '', 0
            else:
                stable = stable + 1 if observed == candidate else 1
                candidate = observed
            time.sleep(.2)
        report.setdefault('fresh_process_poll_diagnostics', []).append(poll)
        if stable != 2:
            # A crash can remove the app PID before logs() gets its first
            # chance to inspect it. Retain only a fixed BAD-pattern label.
            startup_log = adb('logcat', '-d', '-T', since, '-v', 'brief')
            failure = BAD.search(startup_log)
            if failure:
                raise AssertionError('Native/UI failure: ' + failure.group(0))
            raise AssertionError('Stable fresh app process unavailable')
        pid = candidate
        report.setdefault('fresh_process_receipts', []).append({'pid': pid, 'stable_observations': stable})
        if args.skip_intro:
            report.setdefault('cinematic_receipts', []).append('Explicit startup skip extra applied')
        # The native startup now follows GSInit order: the loading/splash movie
        # is rendered under the intro first, and the main menu is loaded only
        # after the intro finishes or the user skips it. Waiting for `main`
        # before pressing Skip deadlocks the smoke against that intended flow.
        text = wait(lambda t: 'Original front screen submitted | screen main' in t,
                    'authored main movie after opening cinematic')
        assert 'Original front screen submitted | screen main' in text, 'Original movie not submitted'
        # Title music is gated on the authored main movie being ready. Wait
        # for both before checking pixels or sending the first menu tap.
        text = wait(lambda t: 'Original title music started after opening flow' in t,
                    'opening cinematic completion and title music', 70)
        report.setdefault('cinematic_receipts', []).append(
            'Original title music started after opening flow')
        wait_focus('fresh launch')

    def viewport(text=None):
        surface_rows = re.findall(r'Surface resized to (\d+) x (\d+)', text if text is not None else logs())
        assert surface_rows, 'Actual NativeSurface dimensions unavailable'
        surface = tuple(map(int, surface_rows[-1]))
        key = (pid, surface)
        if key in viewport_bounds:
            return viewport_bounds[key], surface
        # This fixed full-screen NativeSurface keeps its bounds until a source
        # resize or process change. Repeated accessibility idle waits during
        # the authored class camera transition are unreliable on API37.
        adb('shell', 'uiautomator', 'dump', '/sdcard/dh2-menu-smoke-window.xml')
        document = adb('shell', 'cat', '/sdcard/dh2-menu-smoke-window.xml')
        root = ET.fromstring(document)
        node = next((n for n in root.iter('node') if n.get('content-desc') == VIEWPORT), None)
        assert node is not None, 'NativeSurface bounds unavailable'
        bounds = tuple(map(int, re.findall(r'\d+', node.get('bounds', ''))))
        assert len(bounds) == 4, 'Malformed NativeSurface bounds'
        stage_point(bounds, surface, (240, 160))
        viewport_bounds[key] = bounds
        return bounds, surface

    def image():
        raw = adb('exec-out', 'screencap', '-p', binary=True)
        assert raw.startswith(b'\x89PNG'), 'Screenshot PNG unavailable'
        return raw, Image.open(io.BytesIO(raw)).convert('RGB')

    def visual_ready(stage, allow_animated=False):
        # Wait for the next actual button to settle. In gameplay this is the
        # opaque Android Attack overlay, so Android's resume animation must
        # settle too. Never send a timed input burst while it is changing.
        bounds, surface = viewport()
        roi = button_rectangle(stage, bounds, surface)
        deadline, previous, stable = time.monotonic() + (3 if allow_animated else 20), None, 0
        while time.monotonic() < deadline:
            logs()
            _, picture = image()
            crop = picture.crop(roi).resize((70, 28))
            if previous is not None:
                difference = sum(ImageStat.Stat(ImageChops.difference(crop, previous)).mean) / 3
                stable = stable + 1 if difference < 2.5 else 0
                if stable >= 3:
                    assert max(ImageStat.Stat(crop).stddev) > 3, 'Authored button pixels are blank'
                    report.setdefault('visual_readiness_receipts', []).append(
                        {'stage': stage, 'stable_frame_comparisons': stable, 'ROI': list(roi)})
                    return
            previous = crop
            time.sleep(.2)
        if allow_animated:
            report.setdefault('animated_visual_regions', []).append(stage)
            return
        raise AssertionError('Authored ' + stage + ' button did not visually settle')

    def screenshot(name):
        raw, _ = image()
        path = out / (name + '.png')
        path.write_bytes(raw)
        report['screenshots'].append({'file': path.name, 'sha256': hashlib.sha256(raw).hexdigest()})

    def tap(name):
        wait_focus('tap ' + name)
        bounds, surface = viewport()
        x, y = stage_point(bounds, surface, POINTS[name])
        receipt = {
            'button': name,
            'stage_point': list(POINTS[name]),
            'surface_point': [x, y],
            'bounds': list(bounds),
            'surface': list(surface),
            'expected_actions': ['DOWN', 'UP'],
            'adb_injection_completed': False,
        }
        report['last_tap_receipt'] = receipt
        adb('shell', 'input', '-d', '0', 'tap', str(x), str(y))
        receipt['adb_injection_completed'] = True

    def menu_after_tap(button, name):
        offset = len(logs())
        tap(button)
        wait(lambda t: 'Owned menu navigation | push ' + name + ' |' in t[offset:], 'authored ' + name + ' transition')

    def select_slot(slot):
        # A fresh original movie starts current_slot0. Observe each genuine
        # GetSlot/preview result as the authored arrow changes that field.
        for target in range(1, slot + 1):
            offset = len(logs())
            tap('next_slot')
            wait(lambda t: 'Native menu preview selection | slot %d |' % target in t[offset:], 'authored slot arrow')
            visual_ready('main')

    def start(slot, character_class, preset, animation_table):
        before = len(logs())
        tap('single_player')
        row = args.debug_level_row or 41
        name, source_file, report_name = (("GOTHICUS_CRYPT_01", "007_crypt_01.rule.xml", "Crypt")
                                          if row == 23 else ("SWAMP", "001_swamp.mlx", "SWAMP"))
        text = wait(lambda t: ('NativeStartGame plan | slot %d | row %d | %s | source file %s' % (slot, row, name, source_file) in t[before:] and
                               'Menu game start | slot %d | %s |' % (slot, report_name) in t[before:] and
                               HUD.search(t[before:]) is not None), 'NativeStartGame and connected player HUD', 70)
        fresh_seed_events = SOURCE_RNG_GSINIT.findall(text[before:])
        assert len(fresh_seed_events) == 1 and fresh_seed_events[0][1] == '0', 'Fresh game must apply exactly one source GSInit seed and clear sync seed'
        assert not SOURCE_RNG_UNLOAD.search(text[before:]), 'Fresh game start must not apply Level::Unload seed'
        return verify_start(text[before:], slot, character_class, preset, animation_table, args.debug_level_row)

    def back_main():
        wait_focus('Back from gameplay')
        visual_ready('gameplay')
        wait_focus('Back from settled gameplay')
        offset = len(logs())
        adb('shell', 'input', '-d', '0', 'keyevent', 'KEYCODE_BACK')
        text = wait(lambda t: ('Original front screen submitted | screen main' in t[offset:] or
                               'Original front/HUD screen submitted | screen main' in t[offset:]),
                    'normal Back return to menu')
        unload_seed_events = SOURCE_RNG_UNLOAD.findall(text[offset:])
        assert len(unload_seed_events) == 1 and unload_seed_events[0][1] == '0', 'Back-to-menu must apply exactly one source Level::Unload seed and clear sync seed'
        assert not SOURCE_RNG_GSINIT.search(text[offset:]), 'Back-to-menu must not apply the fresh-game seed'
        assert 'Native Quest terminal discard | destroyed b8 192 | log118 192 | source log vectors empty' in text[offset:], 'Back must destroy both canonical Quest logs before returning to the menu'
        visual_ready('main')

    def gameplay_controls():
        # Operate the actual original SWF controls. The Android overlays are
        # hidden in gameplay; hit regions are the source-authored joystick and
        # attack shapes exercised by authored_gameplay_hud_v1's host fixture.
        wait_focus('Gameplay controls')
        bounds, surface = viewport()
        cx, cy = stage_point(bounds, surface, POINTS['hud_joystick'])
        ex, ey = stage_point(bounds, surface, (70, 215))
        prior = len(logs())
        adb('shell', 'input', '-d', '0', 'swipe', str(cx), str(cy), str(ex), str(ey), '900')
        pattern = re.compile(r'Authored joystick actor update \| trace \d+ \|.*?delta (-?[\d.]+) (-?[\d.]+) \| result phase (\d+)')
        text = wait(lambda t: pattern.search(t[prior:]) is not None, 'authored joystick actor update')
        deltas = [(float(x), float(y), int(phase)) for x, y, phase in pattern.findall(text[prior:])]
        moved = next((row for row in deltas if abs(row[0])+abs(row[1]) > 1e-5), None)
        assert moved is not None, 'Authored joystick produced no physical movement'
        prior = len(logs())
        tap('hud_attack')
        text = wait(lambda t: 'Authored HUD attack press |' in t[prior:], 'authored attack button delivery')
        response = re.findall(r'Authored HUD attack press \| ([^\r\n]+)', text[prior:])[-1]
        assert response in ('Attacking', 'Walk closer to an enemy', 'Attack is cooling down', 'Attack is already in progress'), 'Attack control rejected available player'
        report['gameplay_controls'] = {'authored_movement_delta': list(moved[:2]),
            'actor_update_phase': moved[2], 'attack_response': response, 'full_combat_verified': False}
        screenshot('0-gameplay-after-controls')

    def character_menu_back_callback():
        # The source HUD portrait is btn_charactermenu. Its authored click
        # opens CharacterMenu; the sheet's btnBack calls NativeBackToHud.
        # Assert each receipt in order and require a subsequent live HUD frame
        # before allowing the rest of the launch/reopen smoke to continue.
        before = len(logs())
        tap('hud_character')
        opened = wait(lambda t: ('Original gameplay menu push request | requested menu_CharacterMenu |' in t[before:] and
                                 'Gameplay HUD transition | NativeAwayFromHud |' in t[before:]),
                      'authored Character/Skills menu open')
        assert opened.index('Original gameplay menu push request | requested menu_CharacterMenu |', before) < opened.index(
            'Gameplay HUD transition | NativeAwayFromHud |', before), 'CharacterMenu must open before the source deactivates HUD input'
        screenshot('0-character-menu')
        visual_ready('character')
        # Enter the authored Talent page by touch and require its first skill
        # detail read to come from the active Save/Player skill owner.
        talent_at = len(logs())
        tap('talent')
        if args.capture_talent_only:
            talent_menu = wait(lambda t: re.search(
                r'Original gameplay menu selected \| previous [^\r\n]*'
                r'\| current menu_SkillTreeSheetNew \| depth \d+', t[talent_at:]) is not None,
                'authored Talent page')
            menu_receipt = re.search(
                r'Original gameplay menu selected \| previous [^\r\n]*'
                r'\| current menu_SkillTreeSheetNew \| depth \d+', talent_menu[talent_at:])
            visual_ready('character')
            screenshot('0-character-menu-talent-capture')
            before_train = len(logs())
            tap('talent_upgrade_skill')
            train = wait(lambda t: re.search(
                r'NativeSkillsTrainSkill result \| index (\d+) \| test 0 \| source (\d+) '
                r'\| points (-?\d+) \| readback 1 \| level (-?\d+) \| skill (-?\d+) \| error ',
                t[before_train:]) is not None,
                'Upgrade Skill click and canonical skill Save readback')
            train_receipt = re.search(
                r'NativeSkillsTrainSkill result \| index (\d+) \| test 0 \| source (\d+) '
                r'\| points (-?\d+) \| readback 1 \| level (-?\d+) \| skill (-?\d+) \| error ',
                train[before_train:])
            assert train_receipt and int(train_receipt.group(2)) != 0 and \
                   int(train_receipt.group(3)) == 0 and int(train_receipt.group(4)) == 1, \
                'Upgrade Skill did not consume one point and raise the saved rank to one'
            report['talent_training'] = {
                'authored_button_point_480x320': list(POINTS['talent_upgrade_skill']),
                'receipt': train_receipt.group(0),
                'skill_list_index': int(train_receipt.group(1)),
                'source_result': int(train_receipt.group(2)),
                'points_after_training': int(train_receipt.group(3)),
                'saved_skill_level': int(train_receipt.group(4)),
                'skill_id': int(train_receipt.group(5)),
                'source_Save_and_Player_skill_owner_readback': True,
            }
            screenshot('0-character-menu-talent-trained')
            report['talent_capture_only'] = {
                'authored_tab_point_480x320': list(POINTS['talent']),
                'menu_receipt': menu_receipt.group(0),
                'detail_callback_wait_skipped': True,
                'single_upgrade_skill_tap_verified': True,
            }
            return
        talent_read = wait(lambda t: re.search(
            r'Talent details callback \| Character (0x[0-9a-f]+) \| list index (\d+) '
            r'\| skill id (-?\d+) \| level (-?\d+) \| slot (-?\d+) '
            r'\| source Save and Player skill owner', t[talent_at:]) is not None,
            'authored Talent tab and canonical skill detail callback')
        talent_receipt = re.search(
            r'Talent details callback \| Character (0x[0-9a-f]+) \| list index (\d+) '
            r'\| skill id (-?\d+) \| level (-?\d+) \| slot (-?\d+) '
            r'\| source Save and Player skill owner', talent_read[talent_at:])
        assert talent_receipt and int(talent_receipt.group(1), 16), \
            'Talent callback did not resolve through the active Character'
        visual_ready('character')
        screenshot('0-character-menu-talent')
        before_train = len(logs())
        tap('talent_upgrade_skill')
        trained = wait(lambda t: re.search(
            r'NativeSkillsTrainSkill result \| index (\d+) \| test 0 \| source (\d+) '
            r'\| points (-?\d+) \| readback 1 \| level (-?\d+) \| skill (-?\d+) \| error ',
            t[before_train:]) is not None,
            'Upgrade Skill click and canonical skill Save readback')
        train_receipt = re.search(
            r'NativeSkillsTrainSkill result \| index (\d+) \| test 0 \| source (\d+) '
            r'\| points (-?\d+) \| readback 1 \| level (-?\d+) \| skill (-?\d+) \| error ',
            trained[before_train:])
        assert train_receipt and int(train_receipt.group(2)) != 0 and \
               int(train_receipt.group(3)) >= 0 and int(train_receipt.group(4)) == int(talent_receipt.group(4)) + 1 and \
               int(train_receipt.group(5)) == int(talent_receipt.group(3)), \
            'Upgrade Skill did not raise the selected canonical skill exactly one rank'
        report['talent_ui'] = {
            'authored_tab_point_480x320': list(POINTS['talent']),
            'detail_receipt': talent_receipt.group(0),
            'skill_list_index': int(talent_receipt.group(2)),
            'skill_id': int(talent_receipt.group(3)),
            'saved_level_before_training': int(talent_receipt.group(4)),
            'saved_level_after_training': int(train_receipt.group(4)),
            'equipped_slot': int(talent_receipt.group(5)),
            'read_from_canonical_Save_and_Player_skill_owner': True,
            'training_receipt': train_receipt.group(0),
            'skill_training_click_changed_canonical_rank': True,
        }
        # The bag icon opens the authored equipment/paper-doll screen. Its
        # head slot then pushes InventorySheetDetails, whose onPush populates
        # the item list through NativeInvGetItemsListForSlot. Keep the two
        # source menu transitions distinct in the receipts.
        _, stats_surface = image()
        before_inventory = len(logs())
        tap('inventory')
        selected_main = wait(
            lambda t: re.search(r'Original gameplay menu selected \| previous [^\r\n]*'
                                r'\| current menu_InventorySheetMain \| depth \d+',
                                t[before_inventory:]) is not None,
            'authored InventorySheetMain')
        main_receipt = re.search(
            r'Original gameplay menu selected \| previous [^\r\n]*'
            r'\| current menu_InventorySheetMain \| depth \d+',
            selected_main[before_inventory:])
        assert main_receipt, 'InventorySheetMain selection receipt missing'
        visual_ready('inventory')
        screenshot('0-character-menu-inventory-main')

        before_details = len(logs())
        visual_ready('inventory_equipment_slot')
        tap('inventory_equipment_slot')
        inventory_callback = re.compile(
            r'Inventory list callback complete \| slot (-?\d+) \| rows (\d+) \| player (-?\d+)')
        selected_details = wait(
            lambda t: (re.search(r'Original gameplay menu selected \| previous [^\r\n]*'
                                 r'\| current menu_InventorySheetDetails \| depth \d+',
                                 t[before_details:]) is not None and
                       inventory_callback.search(t[before_details:]) is not None),
            'authored InventorySheetDetails and native inventory list callback')
        details_receipt = re.search(
            r'Original gameplay menu selected \| previous [^\r\n]*'
            r'\| current menu_InventorySheetDetails \| depth \d+',
            selected_details[before_details:])
        assert details_receipt, 'InventorySheetDetails selection receipt missing'
        callback_receipt = inventory_callback.search(selected_details[before_details:])
        assert callback_receipt, 'Native inventory list callback receipt missing'
        callback_slot, callback_rows, callback_player = map(int, callback_receipt.groups())
        assert callback_player == 0, 'Inventory callback did not use the local Player index'
        visual_ready('character')
        bounds, surface = viewport(selected_details)
        body_top_left = stage_point(bounds, surface, (150, 35))
        body_bottom_right = stage_point(bounds, surface, (470, 315))
        body_roi = (body_top_left[0], body_top_left[1],
                    body_bottom_right[0], body_bottom_right[1])
        _, details_surface = image()
        body = details_surface.crop(body_roi)
        body_stddev = max(ImageStat.Stat(body).stddev)
        body_change = sum(ImageStat.Stat(ImageChops.difference(
            body, stats_surface.crop(body_roi))).mean) / 3
        assert body_stddev > 3, 'Inventory details body rendered blank pixels'
        assert body_change > 1, 'Inventory details body did not visibly change from Stats'
        screenshot('0-character-menu-inventory-details')
        report['inventory_ui'] = {
            'authored_tab_point_480x320': list(POINTS['inventory']),
            'main_selected_menu': 'menu_InventorySheetMain',
            'main_selection_receipt': main_receipt.group(0),
            'authored_equipment_slot_point_480x320': list(POINTS['inventory_equipment_slot']),
            'details_selected_menu': 'menu_InventorySheetDetails',
            'details_selection_receipt': details_receipt.group(0),
            'list_callback_receipt': callback_receipt.group(0),
            'list_callback_receipt_verified': True,
            'list_callback_data_verified': False,
            'callback_player_index': callback_player,
            'callback_slot': callback_slot,
            'returned_row_count': callback_rows,
            'body_roi_pixels': list(body_roi),
            'body_pixel_stddev_max': round(body_stddev, 2),
            'mean_pixel_change_from_stats': round(body_change, 2),
        }

        # The upper-right item summary is populated by NativeInvGetEquipedItem
        # from the canonical equipment set. Toggle the authored Unequip/Equip
        # buttons, then pop and reopen Details so its source onPush callbacks
        # render the authoritative state again even if the movie does not
        # refresh its current panel immediately after the action.
        bounds, surface = viewport(selected_details)
        summary_a = stage_point(bounds, surface, (250, 54))
        summary_b = stage_point(bounds, surface, (470, 100))
        summary_roi = (summary_a[0], summary_a[1], summary_b[0], summary_b[1])

        def summary_crop(picture):
            return picture.crop(summary_roi).resize((220, 46))

        def summary_delta(left, right):
            return sum(ImageStat.Stat(ImageChops.difference(
                summary_crop(left), summary_crop(right))).mean) / 3

        assert callback_rows > 0, 'Cannot exercise Equip/Unequip with an empty authored inventory list'
        initial_equipped = details_surface.copy()

        inventory_callback = re.compile(
            r'Inventory list callback complete \| slot (-?\d+) \| rows (\d+) \| player (-?\d+)')

        def exercise_equipment_action(button, action, baseline):
            # These authored buttons refresh the list in-place. Do not leave
            # Details between actions: its Back returns to CharacterMenu, and
            # the still-selected bag tab then acts as an authored exit toggle.
            before = len(logs())
            visual_ready(button)
            tap(button)
            action_callback = re.compile(
                r'Native inventory click callback \| action (Equip|Unequip) \| slot (-?\d+) '
                r'\| item (-?\d+) \| player (-?\d+) \| Character (0x[0-9a-f]+) '
                r'\| canonical V4 result 1')
            text = wait(lambda t: (inventory_callback.search(t[before:]) is not None and
                                   action_callback.search(t[before:]) is not None),
                        action + ' authored Details list refresh')
            receipt = inventory_callback.search(text[before:])
            action_receipt = action_callback.search(text[before:])
            assert receipt and int(receipt.group(2)) > 0 and int(receipt.group(3)) == 0, \
                action + ' Details refresh lost the local inventory item row'
            assert action_receipt and action_receipt.group(1) == action and \
                   int(action_receipt.group(4)) == 0 and int(action_receipt.group(5), 16), \
                action + ' did not reach the canonical V4 character callback'

            deadline = time.monotonic() + 10
            previous = None
            stable = 0
            last_picture = None
            delta = 0.0
            while time.monotonic() < deadline:
                logs()
                _, picture = image()
                delta = summary_delta(baseline, picture)
                crop = summary_crop(picture)
                if delta > 1.0 and previous is not None:
                    frame_delta = sum(ImageStat.Stat(
                        ImageChops.difference(crop, previous)).mean) / 3
                    stable = stable + 1 if frame_delta < 2.5 else 0
                    if stable >= 2:
                        last_picture = picture
                        break
                previous = crop
                last_picture = picture
                time.sleep(.2)
            assert delta > 1.0 and stable >= 2, \
                action + ' did not update the authored equipped-item summary'
            return last_picture, receipt.group(0), action_receipt.group(0), delta

        unequipped_surface, unequipped_list, unequipped_action, unequipped_delta = exercise_equipment_action(
            'inventory_unequip', 'Unequip', initial_equipped)
        screenshot('0-character-menu-inventory-unequipped')

        equipped_surface, equipped_list, equipped_action, equip_delta = exercise_equipment_action(
            'inventory_equip', 'Equip', unequipped_surface)
        initial_restore_delta = summary_delta(initial_equipped, equipped_surface)
        assert initial_restore_delta < 8.0, \
            'Authored Equip did not restore the original equipped-item summary'
        screenshot('0-character-menu-inventory-equipped')
        report['inventory_equipment_actions'] = {
            'unequip_button_stage_point': list(POINTS['inventory_unequip']),
            'equip_button_stage_point': list(POINTS['inventory_equip']),
            'equipped_item_summary_roi': list(summary_roi),
            'initial_details_selection': details_receipt.group(0),
            'unequip_list_refresh': unequipped_list,
            'unequip_native_callback': unequipped_action,
            'unequip_summary_pixel_delta': round(unequipped_delta, 2),
            'equip_list_refresh': equipped_list,
            'equip_native_callback': equipped_action,
            'equip_summary_pixel_delta': round(equip_delta, 2),
            're_equip_summary_delta_from_initial': round(initial_restore_delta, 2),
            'verified_by_in_place_authored_equipped_item_render': True,
        }
        # Leave the synthetic profile in a changed but valid equipment state.
        # The occupied-slot relaunch below must render this same canonical
        # Save state, proving the click mutation survived profile reload.
        persisted_unequipped_surface, persisted_unequipped_list, persisted_unequipped_action, _ = \
            exercise_equipment_action('inventory_unequip', 'Unequip', equipped_surface)
        ui_flow_baselines[slot] = {
            'equipped': summary_crop(initial_equipped).copy(),
            'unequipped': summary_crop(persisted_unequipped_surface).copy(),
            'skill': {
                'index': int(train_receipt.group(1)),
                'id': int(train_receipt.group(5)),
                'level': int(train_receipt.group(4)),
            },
        }
        report['inventory_equipment_actions'].update({
            'final_unequip_list_refresh': persisted_unequipped_list,
            'final_unequip_native_callback': persisted_unequipped_action,
            'final_state_left_unequipped_for_reload_check': True,
        })
        callback_at = len(logs())
        report['character_menu_back_callback'] = {
            'character_menu_opened_by_authored_hud_tap': True,
            'native_away_from_hud_seen_before_menu': True,
            'inventory_details_back_to_main_seen': False,
            'native_back_to_hud_callback_seen': False,
            'hud_visual_ready_after_callback': False,
        }
        try:
            tap('character_back')
        except AssertionError as exc:
            if 'Native/UI failure: Menu touch failed' not in str(exc):
                raise
            report['character_menu_back_callback']['callback_failure'] = str(exc)
            raise
        first_back = wait(
            lambda t: ('Gameplay HUD transition | BackToHud | active 1 |' in t[callback_at:] or
                       re.search(r'Original gameplay menu selected \| previous [^\r\n]*'
                                 r'\| current menu_InventorySheetMain \| depth \d+',
                                 t[callback_at:]) is not None),
            'authored InventoryDetails back transition')
        if 'Gameplay HUD transition | BackToHud | active 1 |' not in first_back[callback_at:]:
            report['character_menu_back_callback']['inventory_details_back_to_main_seen'] = True
            tap('character_back')
        returned = wait(lambda t: 'Gameplay HUD transition | BackToHud | active 1 |' in t[callback_at:],
                        'authored NativeBackToHud callback and HUD activation')
        callback = returned.index('Gameplay HUD transition | BackToHud | active 1 |', callback_at)
        # HUD frames are only logged when the status projection changes; a
        # steady HP/MP frame has no receipt. Verify the restored HUD surface
        # visually after the activation callback instead of requiring a log
        # that the renderer intentionally does not emit every frame.
        visual_ready('gameplay')
        report['character_menu_back_callback'].update({
            'native_back_to_hud_callback_seen': True,
            'hud_visual_ready_after_callback': True,
        })
        screenshot('0-gameplay-after-character-menu-back')

    def android_system_back_returns_to_hud():
        # Exercise MainActivity's Android Back handler while a gameplay-owned
        # menu is open. This deliberately sends KEYCODE_BACK through adb; it
        # does not tap the SWF's btnBack and must retain the active level.
        before = len(logs())
        tap('hud_character')
        opened = wait(lambda t: ('Original gameplay menu push request | requested menu_CharacterMenu |' in t[before:] and
                                 'Gameplay HUD transition | NativeAwayFromHud |' in t[before:]),
                      'gameplay menu open before Android system Back')
        assert opened.index('Original gameplay menu push request | requested menu_CharacterMenu |', before) < opened.index(
            'Gameplay HUD transition | NativeAwayFromHud |', before), \
            'Gameplay menu must acquire ownership before Android system Back'
        visual_ready('character')
        back_at = len(logs())
        wait_focus('Android system Back from gameplay menu')
        adb('shell', 'input', '-d', '0', 'keyevent', 'KEYCODE_BACK')
        returned = wait(
            lambda t: 'Gameplay HUD transition | BackToHud | active 1 |' in t[back_at:],
            'Android system Back restores the live gameplay HUD')
        receipt = re.search(
            r'Gameplay HUD transition \| BackToHud \| active 1 \| prior menu depth (\d+) \| reset pending 1',
            returned[back_at:])
        assert receipt and int(receipt.group(1)) > 0, \
            'Android Back did not return ownership from the open gameplay menu'
        assert not re.search(r'Menu game start \|', returned[back_at:]), \
            'Android Back unexpectedly started/reloaded a level'
        assert 'Original front screen submitted | screen main' not in returned[back_at:] and \
            'Original front/HUD screen submitted | screen main' not in returned[back_at:], \
            'Android Back returned to the front menu instead of the active level HUD'
        assert not SOURCE_RNG_UNLOAD.search(returned[back_at:]), \
            'Android Back unexpectedly unloaded the active level'
        visual_ready('gameplay')
        report['android_system_back'] = {
            'sent_adb_keyevent': 'KEYCODE_BACK',
            'gameplay_menu_opened_before_keyevent': True,
            'back_to_hud_owner_receipt': receipt.group(0),
            'active_level_retained_no_start_or_unload_receipt': True,
            'hud_visual_ready_after_keyevent': True,
        }

    def android_system_back_dismisses_nested_menu():
        # Back from an authored child (inventory or confirmation overlay)
        # must reveal its parent; only Back from the root CharacterMenu exits
        # to the gameplay HUD.
        before = len(logs())
        tap('hud_character')
        wait(lambda t: ('Original gameplay menu push request | requested menu_CharacterMenu |' in t[before:] and
                        'Gameplay HUD transition | NativeAwayFromHud |' in t[before:]),
             'CharacterMenu open before nested Back')
        tap('inventory')
        child = wait(lambda t: re.search(
            r'Original gameplay menu selected \| previous [^\r\n]*'
            r'\| current menu_InventorySheetMain \| depth \d+', t[before:]) is not None,
            'Inventory child open before nested Back')
        wait_focus('Android system Back from nested gameplay menu')
        adb('shell', 'input', '-d', '0', 'keyevent', 'KEYCODE_BACK')
        returned = wait(lambda t: 'Gameplay menu Back | dismissed menu_InventorySheetMain | '
                                  'current menu_CharacterMenu | depth 1 | HUD remains inactive' in t[before:],
                        'nested Back reveals CharacterMenu')
        assert 'Gameplay HUD transition | BackToHud |' not in returned[before:], \
            'Nested Back exited gameplay instead of returning to its parent menu'
        visual_ready('character')
        report['nested_menu_back'] = {
            'child': 'menu_InventorySheetMain',
            'parent': 'menu_CharacterMenu',
            'parent_visible_after_back': True,
            'HUD_remained_inactive': True,
            'receipt': 'Gameplay menu Back | dismissed menu_InventorySheetMain | current menu_CharacterMenu | depth 1 | HUD remains inactive',
        }
        tap('character_back')
        wait(lambda t: 'Gameplay HUD transition | BackToHud | active 1 |' in t[before:],
             'root CharacterMenu Back restores gameplay HUD')
        visual_ready('gameplay')
        screenshot('0-gameplay-after-android-system-back')

    failure = None
    try:
        report['api'] = int(adb('shell', 'getprop', 'ro.build.version.sdk'))
        report['page_size'] = int(adb('shell', 'getconf', 'PAGE_SIZE'))
        report['abi'] = adb('shell', 'getprop', 'ro.product.cpu.abi')
        assert (report['api'], report['page_size'], report['abi']) == (37, 16384, 'x86_64'), 'API37/16KiB x86_64 required'
        adb('shell', 'am', 'force-stop', PACKAGE, allow_failure=True)
        assert 'Success' in adb('install', '-r', str(args.apk.resolve())), 'APK install failed'
        adb('shell', 'am', 'force-stop', PACKAGE)
        preservation_attempted = True
        report['original_files_preserved'] = int(private_script(preserve))
        for index, character_class, animation_table, preset in selected_classes:
            # The original menu reveals another empty slot only after a
            # profile exists. An isolated class check uses its first empty
            # slot; class selection and slot ordinal are separate values.
            slot = index if args.class_index is None else 0
            launch()
            visual_ready('main')
            screenshot('%d-main-initial' % index)
            select_slot(slot)
            screenshot('%d-main-empty' % index)
            menu_after_tap('main', 'menu_EnterName')
            visual_ready('name')
            screenshot('%d-name-empty' % index)
            # One real A-key touch creates a deliberately synthetic name.
            # Wait for the field/button pixels after it, then confirm.
            tap('key_a')
            visual_ready('name')
            screenshot('%d-name-synthetic' % index)
            offset = len(logs())
            tap('name_confirm')
            text = wait(lambda t: ('Owned menu navigation | push menu_SelectClass |' in t[offset:] and
                                   CLASS_INDEX.search(t[offset:]) is not None), 'authored class menu')
            current, _ = CLASS_INDEX.findall(text[offset:])[-1]
            current = int(current)
            assert current <= index, 'Unexpected retained class index; cannot use right-arrow progression'
            while current < index:
                prior = len(logs())
                tap('class_right')
                text = wait(lambda t: any(int(m[0]) == current + 1 for m in CLASS_INDEX.findall(t[prior:])), 'fresh class-index update')
                current, actual_preset = CLASS_INDEX.findall(text[prior:])[-1]
                current = int(current)
                assert CLASSES[current][3] == actual_preset, 'Authored class index/preset mismatch'
                visual_ready('class', allow_animated=True)
            visual_ready('class', allow_animated=True)
            screenshot('%d-class-%s' % (index, preset))
            offset = len(logs())
            tap('class_confirm')
            text = wait(lambda t: ('Native menu profile created | slot %d | class %d |' % (slot, character_class) in t[offset:] and
                                   'Owned menu navigation | push menu_StartGame |' in t[offset:]), 'source profile creation and real Start movie')
            assert len(re.findall(r'Native menu profile created \|', text)) == 1, 'Profile creation replayed in fresh process'
            if args.stop_after_class_confirm:
                report['new_campaign_publication'] = {
                    'profile_created': True,
                    'slot': slot,
                    'class_index': character_class,
                    'start_menu_opened': True,
                }
                report['validation'] = 'PASS'
                report['scope'] = 'Menu publication only: original menu, name, class confirmation, exclusive profile publication, and Start menu opened. No Start tap or level load.'
                break
            if args.stop_after_world_ready:
                before_start = len(logs())
                tap('single_player')
                row = args.debug_level_row or 41
                level_name = 'GOTHICUS_CRYPT_01' if row == 23 else 'SWAMP'
                source_file = '007_crypt_01.rule.xml' if row == 23 else '001_swamp.mlx'
                world = wait(lambda t: 'World ready |' in t[before_start:],
                             'first World ready after the authored Start tap', 70)
                plan = re.search(r'NativeStartGame plan \| slot (\d+) \| row (\d+) \| ([^|]+) \| source file ([^|]+)',
                                 world[before_start:])
                ready = re.search(r'World ready \| rooms (\d+) \| visual draws (\d+) \| navigation triangles (\d+)',
                                  world[before_start:])
                assert plan and int(plan.group(1)) == slot and int(plan.group(2)) == row and \
                       plan.group(3).strip() == level_name and plan.group(4).strip() == source_file, \
                    'Start did not resolve the intended first level/source row'
                assert ready, 'World ready receipt was incomplete'
                report['first_level_entry'] = {
                    'start_taps': 1,
                    'level_row': row,
                    'level_name': level_name,
                    'source_file': source_file,
                    'world_ready_receipt': ready.group(0),
                    'native_hud_or_combat_checked': False,
                }
                report['validation'] = 'PASS'
                report['scope'] = 'Bounded first-level entry: one authored Start tap and first World ready receipt only; no HUD, controls, or combat suite.'
                break
            visual_ready('start')
            screenshot('%d-start-new' % index)
            created = start(slot, character_class, preset, animation_table)
            screenshot('%d-gameplay-new' % index)
            if index == 0:
                if not args.capture_talent_only:
                    gameplay_controls()
                character_menu_back_callback()
                if args.capture_talent_only:
                    report['validation'] = 'PASS'
                    report['scope'] = ('Capture-only: authored menu/name/class/Start path, gameplay CharacterMenu, ' +
                                       'Talent tab selection and screenshot. No skill-row callback or Back behavior tested.')
                    break
                android_system_back_returns_to_hud()
                android_system_back_dismisses_nested_menu()
            back_main()
            screenshot('%d-main-back' % index)
            # Reopen occupied after process restart, which resets Info664 and
            # proves that the real Start button supplies its assignment.
            launch()
            visual_ready('main')
            select_slot(slot)
            menu_after_tap('main', 'menu_StartGame')
            visual_ready('start')
            screenshot('%d-start-occupied' % index)
            assert not re.search(r'Native menu profile created \|', logs()), 'Occupied slot was recreated'
            reopened = start(slot, character_class, preset, animation_table)
            screenshot('%d-gameplay-occupied' % index)
            if slot in ui_flow_baselines:
                # Re-enter real authored tabs after loading the occupied
                # profile. Detail callbacks must return the trained rank;
                # the inventory's visible equipped summary must match the
                # saved unequipped state rather than its pre-click baseline.
                ui_before = len(logs())
                tap('hud_character')
                wait(lambda t: ('Original gameplay menu push request | requested menu_CharacterMenu |' in t[ui_before:] and
                                'Gameplay HUD transition | NativeAwayFromHud |' in t[ui_before:]),
                     'CharacterMenu reopened from occupied profile HUD')
                talent_before = len(logs())
                tap('talent')
                persisted_talent = wait(lambda t: re.search(
                    r'Talent details callback \| Character (0x[0-9a-f]+) \| list index (\d+) '
                    r'\| skill id (-?\d+) \| level (-?\d+) \| slot (-?\d+) '
                    r'\| source Save and Player skill owner', t[talent_before:]) is not None,
                    'occupied-profile Talent callback from canonical Save/Player skill owner')
                talent_match = re.search(
                    r'Talent details callback \| Character (0x[0-9a-f]+) \| list index (\d+) '
                    r'\| skill id (-?\d+) \| level (-?\d+) \| slot (-?\d+) '
                    r'\| source Save and Player skill owner', persisted_talent[talent_before:])
                expected_skill = ui_flow_baselines[slot]['skill']
                assert talent_match and int(talent_match.group(1), 16) and \
                       int(talent_match.group(2)) == expected_skill['index'] and \
                       int(talent_match.group(3)) == expected_skill['id'] and \
                       int(talent_match.group(4)) == expected_skill['level'], \
                    'Occupied-slot reload lost the trained skill rank or changed its canonical identity'
                inventory_before = len(logs())
                tap('inventory')
                wait(lambda t: re.search(
                    r'Original gameplay menu selected \| previous [^\r\n]*'
                    r'\| current menu_InventorySheetMain \| depth \d+', t[inventory_before:]) is not None,
                    'occupied-profile Inventory tab')
                list_before = len(logs())
                tap('inventory_equipment_slot')
                persisted_inventory = wait(lambda t: re.search(
                    r'Original gameplay menu selected \| previous [^\r\n]*'
                    r'\| current menu_InventorySheetDetails \| depth \d+', t[list_before:]) is not None and
                    re.search(r'Inventory list callback complete \| slot (-?\d+) \| rows (\d+) \| player (-?\d+)',
                              t[list_before:]) is not None,
                    'occupied-profile inventory list callback')
                list_match = re.search(
                    r'Inventory list callback complete \| slot (-?\d+) \| rows (\d+) \| player (-?\d+)',
                    persisted_inventory[list_before:])
                assert list_match and int(list_match.group(1)) == callback_slot and \
                       int(list_match.group(2)) > 0 and int(list_match.group(3)) == 0, \
                    'Occupied-slot reload changed the canonical inventory slot/player or lost item rows'
                bounds, _ = viewport(persisted_inventory)
                current_picture = image()[1]
                summary_a = stage_point(bounds, current_picture.size, (250, 54))
                summary_b = stage_point(bounds, current_picture.size, (470, 100))
                current_summary = current_picture.crop((summary_a[0], summary_a[1], summary_b[0], summary_b[1])).resize((220, 46))
                baseline = ui_flow_baselines[slot]
                restored_delta = sum(ImageStat.Stat(ImageChops.difference(
                    current_summary, baseline['unequipped'])).mean) / 3
                equipped_delta = sum(ImageStat.Stat(ImageChops.difference(
                    current_summary, baseline['equipped'])).mean) / 3
                assert restored_delta < 8.0 and equipped_delta > 1.0, \
                    'Occupied-slot reload did not preserve the changed unequipped equipment display'
                report['occupied_profile_ui_reload'] = {
                    'talent_detail_receipt': talent_match.group(0),
                    'persisted_skill_rank_matches_pre_restart': True,
                    'inventory_list_receipt': list_match.group(0),
                    'inventory_rows_and_local_player_restored': True,
                    'unequipped_summary_delta_from_saved_state': round(restored_delta, 2),
                    'unequipped_summary_delta_from_equipped_state': round(equipped_delta, 2),
                    'equipment_state_survived_profile_reload': True,
                }
                # Exercise the authored Drop button only at coordinates
                # recovered from dqcharmenu_droid.swf. If the click opens the
                # source confirmation sheet, wait for its animated yes button
                # to settle before tapping the grounded hit-area center.
                drop_before = len(logs())
                visual_ready('inventory_drop')
                tap('inventory_drop')
                confirm_menu = re.compile(
                    r'Original gameplay menu selected \| previous [^\r\n]*'
                    r'\| current menu_confirm2 \| depth \d+')
                drop_action = re.compile(
                    r'Native inventory click callback \| action Drop \| item (-?\d+) '
                    r'\| player 0 \| Character (0x[0-9a-f]+) \| canonical V4/world result 1')
                first_drop_phase = wait(
                    lambda t: drop_action.search(t[drop_before:]) is not None or
                              confirm_menu.search(t[drop_before:]) is not None,
                    'authored Drop action or source confirmation sheet', 8)
                if drop_action.search(first_drop_phase[drop_before:]) is None:
                    visual_ready('inventory_drop_confirm')
                    tap('inventory_drop_confirm')
                dropped = wait(
                    lambda t: drop_action.search(t[drop_before:]) is not None,
                    'Drop click receipt from canonical V4/world-item owner')
                drop_receipt = drop_action.search(dropped[drop_before:])
                assert drop_receipt and int(drop_receipt.group(1)) >= 0 and \
                       int(drop_receipt.group(2), 16) == int(reopened['character'], 16), \
                    'Drop receipt did not identify the reopened canonical Character/item'
                post_drop_list = [m for m in inventory_callback.finditer(dropped[drop_before:])]
                if post_drop_list:
                    post_drop_rows = int(post_drop_list[-1].group(2))
                    assert post_drop_rows < int(list_match.group(2)), \
                        'Drop callback did not reduce the authored inventory list rows'
                else:
                    post_drop_rows = None
                report['inventory_drop_action'] = {
                    'drop_button_stage_point_480x320': list(POINTS['inventory_drop']),
                    'confirm_button_stage_point_480x320': list(POINTS['inventory_drop_confirm']),
                    'coordinate_source': 'dqcharmenu_droid.swf sprite 456/444/442 and menu_confirm2 frame 5',
                    'native_callback_receipt': drop_receipt.group(0),
                    'canonical_character_matches_reopened_profile': True,
                    'fresh_inventory_rows_after_drop': post_drop_rows,
                    'verified_v4_drop_and_world_item_result': True,
                }
                back_before = len(logs())
                tap('character_back')
                first_back = wait(lambda t: ('Gameplay HUD transition | BackToHud | active 1 |' in t[back_before:] or
                                              re.search(r'Gameplay menu Back \| dismissed menu_InventorySheetDetails',
                                                        t[back_before:]) is not None),
                                  'occupied-profile details Back')
                if 'Gameplay HUD transition | BackToHud | active 1 |' not in first_back[back_before:]:
                    tap('character_back')
                    second_back = wait(lambda t: 'Gameplay HUD transition | BackToHud | active 1 |' in t[back_before:],
                                       'occupied-profile menu Back to gameplay HUD')
                    assert second_back, 'Occupied-profile menu did not return to gameplay HUD'
                visual_ready('gameplay')
            home_pid = pid
            wait_focus('Home from gameplay')
            adb('shell', 'input', '-d', '0', 'keyevent', 'KEYCODE_HOME')
            deadline = time.monotonic() + 15
            while True:
                activity = adb('shell', 'dumpsys', 'activity', 'activities')
                resumed = [line for line in activity.splitlines()
                           if 'mResumedActivity:' in line or 'topResumedActivity=' in line]
                if resumed and all(PACKAGE not in line for line in resumed):
                    break
                assert time.monotonic() < deadline, 'Actual Home/background transition missing'
                time.sleep(.2)
            # am start brings the existing Activity forward without force-stop.
            before = len(logs())
            assert 'Status: ok' in adb('shell', 'am', 'start', '-W', '-n', PACKAGE + '/.MainActivity'), 'Resume failed'
            text = wait(lambda t: HUD.search(t[before:]) is not None, 'connected HUD after Home/resume', 70)
            assert not SOURCE_RNG_GSINIT.search(text[before:]) and not SOURCE_RNG_UNLOAD.search(text[before:]), 'Home/resume must retain the source RNG session without lifecycle reseeding'
            wait_focus('Home/resume')
            assert pid == home_pid, 'Home/resume replaced process'
            require_stable_app_pid(home_pid, 'Home/resume')
            classes = CLASS.findall(text)
            assert classes[-1] == (str(slot), str(character_class), preset), 'Home/resume changed class'
            resumed_hud = HUD.findall(text[before:])[-1]
            assert resumed_hud[2] == reopened['character'], 'Home/resume changed canonical Character'
            assert tuple(map(int, (resumed_hud[4], resumed_hud[6], resumed_hud[8]))) == (reopened['max_HP'], reopened['max_MP'], reopened['next_XP']), 'Home/resume changed cached maxima'
            resumed_quests = QUEST_STARTUP.findall(text[before:])
            assert resumed_quests and tuple(map(int, resumed_quests[-1])) == (reopened['quest_startup']['gameplay_Save'], reopened['quest_startup']['Character'], 192, 192, 1608, 1), 'Home/resume must retain the same Quest owner and Save'
            resumed_masks = PLAYER_SAVE_MASKS.findall(text[before:])
            assert len(resumed_masks) == 1 and tuple(map(int, resumed_masks[-1])) == (
                reopened['source_save_masks_2_4']['character'], reopened['source_save_masks_2_4']['save'],
                reopened['source_save_masks_2_4']['loader'], reopened['source_save_masks_2_4']['character'],
                reopened['source_save_masks_2_4']['character'], 1, 1, 1,
                reopened['source_save_masks_2_4']['profile_identity'],
                reopened['source_save_masks_2_4']['mask4_transport_requests']), 'Home/resume must retain the source Save association without replaying SG_Load(2) or SG_Load(4)'
            screenshot('%d-gameplay-resumed' % index)
            back_main()
            screenshot('%d-main-final' % index)
            report['cases'].append({'class_index': index, 'created': created, 'occupied_after_restart': reopened,
                                    'normal_back_to_main': True, 'Home_resume_same_class_and_character': True,
                                    'Home_resume_same_Quest_owner': True,
                                    'Home_resume_same_Player_Save_association_without_mask2_or_mask4_replay': True})
        checked_slots = ([slot] if args.stop_after_class_confirm or args.stop_after_world_ready else
                         [row[0] for row in selected_classes] if args.class_index is None else [0])
        private_script('set -eu\n' + '\n'.join('test -f files/dh2_%03d.savegame' % slot for slot in checked_slots))
        report['synthetic_primary_profiles_created'] = (1 if args.stop_after_class_confirm or args.stop_after_world_ready else len(selected_classes))
        report['validation'] = 'PASS'
    except Exception as exc:
        # Exception messages are deliberately our own fixed assertions; never
        # persist raw adb stdout/stderr, names, profiles or complete Logcat.
        failure = exc
        report['failure'] = {'type': type(exc).__name__, 'message': str(exc) if isinstance(exc, (AssertionError, RuntimeError)) else 'Smoke operation failed'}
    finally:
        if pid:
            collect_events()
        try:
            if preservation_attempted:
                adb('shell', 'am', 'force-stop', PACKAGE)
                report['synthetic_files_archived'] = int(private_script(restore))
                report['original_files_restored_and_device_checksums_verified'] = True
            else:
                report['personal_files_touched'] = False
                adb('shell', 'am', 'force-stop', PACKAGE, allow_failure=True)
        except Exception:
            report['validation'] = 'FAIL'
            report['original_files_restored_and_device_checksums_verified'] = False
            if preservation_attempted:
                report['restoration_failure'] = 'Private transaction retained on device; stop app and recover originals there before further tests.'
            else:
                report['cleanup_failure'] = 'App force-stop failed before profile preservation; no personal files touched.'
            failure = failure or RuntimeError('On-device campaign restoration failed')
        (out / 'menu-source-events.log').write_text('\n'.join(events) + '\n', encoding='utf-8')
        gsinit_events = SOURCE_RNG_GSINIT.findall('\n'.join(events))
        unload_events = SOURCE_RNG_UNLOAD.findall('\n'.join(events))
        expected_lifecycle_events = 2 * len(selected_classes)
        stop_before_level = args.capture_talent_only or args.stop_after_class_confirm
        report['source_random_lifecycle'] = {
            'GSInit_seed_events': len(gsinit_events),
            'Level_Unload_seed_events': len(unload_events),
            'expected_each': (expected_lifecycle_events if not stop_before_level else None),
            'capture_ended_before_level_unload_check': stop_before_level,
            'home_resume_reseeded': False,
            'synchronized_seed_zero': all(row[1] == '0' for row in gsinit_events + unload_events),
        }
        if args.stop_after_world_ready and report.get('validation') == 'PASS':
            report['source_random_lifecycle']['expected_GSInit_seed_events'] = 1
            report['source_random_lifecycle']['expected_Level_Unload_seed_events'] = 0
            report['source_random_lifecycle']['capture_ended_before_level_unload_check'] = True
            if len(gsinit_events) != 1 or len(unload_events) != 0 or \
                    not report['source_random_lifecycle']['synchronized_seed_zero']:
                report['validation'] = 'FAIL'
                report['failure'] = {'type': 'AssertionError', 'message': 'First-level entry RNG seed event did not match one zero-seed GSInit before unload'}
                failure = failure or RuntimeError('First-level entry RNG lifecycle verification failed')
        elif not stop_before_level and report.get('validation') == 'PASS' and (
                len(gsinit_events) != expected_lifecycle_events or
                len(unload_events) != expected_lifecycle_events or
                not report['source_random_lifecycle']['synchronized_seed_zero']):
            report['validation'] = 'FAIL'
            report['failure'] = {'type': 'AssertionError', 'message': 'Source RNG lifecycle event totals or synchronized seed values differ'}
            failure = failure or RuntimeError('Source RNG lifecycle verification failed')
        report['source_events_sha256'] = hashlib.sha256((out / 'menu-source-events.log').read_bytes()).hexdigest()
        (out / 'menu-ui-runtime-smoke.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({'validation': report['validation'], 'cases': len(report['cases']),
                      'restored': report.get('original_files_restored_and_device_checksums_verified', False),
                      'report': str(out / 'menu-ui-runtime-smoke.json')}))
    if failure:
        raise SystemExit(1)


if __name__ == '__main__':
    main()
