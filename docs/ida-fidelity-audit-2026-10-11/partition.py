import json,sys,re,collections,os
B={
'combat':r'^(CSAttack|CSSkill|CSCast|ObjectSearcher|SearchFailCache|Projectile|ProjectileManager|LaserTypeProjectile|POProjectile|CMsgAttackResult|GameObjectDamager)$',
'stats':r'^(CharProperties|CharPropTextField|PlayerStatManager|CharTimers|PyDataConstants|UserProperties|PropertyMap)$|Properties$|^ClassFunc|^(StatListTable|StatAutoAssign\w*|ClassTable|CharacterTable|Charater_Templates|GameDifficult\w*|DesignSettings\w*|SkillTable|SkillListTable|ProjectileTable|FaeryTable|FaeryListTable|AIFaction\w*|AITable)$',
'states':r'^(CharStateMachine|StateMachine|CS\w+|SlideEventCharacter|CharAnimator|AnimSetController|BlendedAnimSetController|AnimController|CharAnimTable|AnimTable)$',
'ai':r'^(CharAI|CharAIScript|CharAISkillScript|AIS\w+|PFWorld|PFFloor|PFRoom|PFObject|PFGInnerGraph|SortModuleByDistance)$',
'items':r'^(ItemInventory|ItemInstance|ItemObject|ItemManager|item|Container|SlotContainer|SortBy\w+|Item\w*Table|ItemList|ItemTypeList|ItemPowerList|ItemBonusAttrMonopoly|LootTable|MerchantTable|InventoryTable|DropTilePriorityTable|NumProbArray|rnd|Random|RewardList|Reward\w*|CMsgDropLoot|MenuMerchant|MenuCharMenu_Inv\w+|DragAndDrop)$',
'world':r'^(GameObject|ObjectManager|ObjectBase|ObjectHandle|PhysicalObject|PhysicalWorld|Door|Doors|Trigger\w*|ProjectileTrap\w*|TimerTrap\w*|ExplosiveTraps|OpenableContainers?|DestructibleContainers?|LiftableObjects?|Dragable|Dropable|Zone|ZoneEx|RoomZone|CheckpointZone|SpawnPoint|SpawnSpot|SpawnGroupManager|SpawnGroups|Decor|AnimatedDecor|PO\w+|ColBox|Floor|CRoomAttributes|Anchor\w+)$',
'quests':r'^(Quest|QuestSavegame|QuestMoveInZone|Objective\w*|ObjectiveList|Script_\w+|Condition\w*|ConditionData|ConditionList|ScriptManager|LuaScript|LuaManager|GameEventManager|GameEvent|EventManager|DialogMsg|Dialogs|DialogActors|v2Quests|v2Events|v2Conditions)$',
'level':r'^(Level|LevelSavegame|PlayerManager|PlayerSavegame|PlayerInfo|Savegame|SavegameManager|GSLevel|GSLevelMap|GS_InterruptLoading|GSEndGame|LevelConfig|Module|LevelMap|LevelList|FastTravelList|WorldMap\w*|MenuWorldMap|MenuCharMenu_Map|SceneManager|CameraLevel|CameraTarget|CameraBase|CameraOverview)$',
}
F={'items':r'^Native(Inv|Item|Merchant|Equip|Loot|Gold|Sell|Buy)','quests':r'GetNewScriptCmd|GetConditionImpl|^Native(Quest|Dialog)','stats':r'^Native(Stat|Skill|Faery|Char|Attr)','world':r'^Native(Touch|Interact)'}
out=collections.defaultdict(list)
for line in open(sys.argv[1],encoding='utf8'):
    r=json.loads(line); d=r.get('demangled') or r['name']
    m=re.match(r'(?:`[^\']*\')?([A-Za-z_]\w*)::',d); cls=m.group(1) if m else None
    if d.startswith('Structs::') or d.startswith('Arrays::'):
        mm=re.match(r'(?:Structs|Arrays)::(\w+)',d); cls=mm.group(1)
    hit=None
    for b,rx in B.items():
        if cls and re.search(rx,cls): hit=b;break
    if not hit and not cls:
        for b,rx in F.items():
            if re.search(rx,d): hit=b;break
    if not hit and cls=='Character':
        hit='character'
    if hit: out[hit].append((r['address'],int(r['end'],16)-int(r['address'],16),d,r.get('pseudocode') or ''))
os.makedirs(sys.argv[2],exist_ok=True)
for b,v in out.items():
    v.sort(key=lambda x:int(x[0],16))
    if b=='character':
        n=len(v); parts=[v[:n//3],v[n//3:2*n//3],v[2*n//3:]]
        for i,p in enumerate(parts):
            open(os.path.join(sys.argv[2],f'character{i+1}.tsv'),'w',encoding='utf8').write(''.join(f'{a}\t{s}\t{p_}\t{d}\n' for a,s,d,p_ in p)); print(f'character{i+1}',len(p),sum(x[1] for x in p))
    else:
        open(os.path.join(sys.argv[2],b+'.tsv'),'w',encoding='utf8').write(''.join(f'{a}\t{s}\t{p_}\t{d}\n' for a,s,d,p_ in v)); print(b,len(v),sum(x[1] for x in v))
