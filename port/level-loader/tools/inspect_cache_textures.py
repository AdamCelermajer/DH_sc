import json,zipfile
with zipfile.ZipFile(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip') as z:
    print(json.dumps([n for n in z.namelist() if 'env_swamp' in n.lower() or ('textures/' in n and n.lower().endswith('.tga'))][:40]))
