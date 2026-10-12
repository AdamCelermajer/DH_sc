exec(open('C:/Users/adamc/AppData/Local/Temp/dh2-lightset-discover-v53.py').read().split('for row in rows:')[0])
vals={'scene_reference':txt(0x40b154+8+w(0x40b3b0)),'SyncData_debug_switch':txt(0x40a94c+8+w(0x40aa90))};(r/'lightpoint-literals-v53.json').write_text(json.dumps(vals,indent=2)+'\n');print(vals)
