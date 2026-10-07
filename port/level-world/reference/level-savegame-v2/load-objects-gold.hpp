#pragma once
struct LoadObjectsGoldenV2{const char* name;bool missing,initializing;unsigned consume,loads,cursor;const unsigned char* bytes;unsigned byte_size;};
static const unsigned char load_obj_bytes_0[]={1,0,0,0,18,0,0,0,73,103,110,111,114,101,100,83,111,117,114,99,101,82,111,108,101,0,4,0,0,0,77,111,98,0,7,0,0,0,8,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7};
static const unsigned char load_obj_bytes_1[]={1,0,0,0,18,0,0,0,73,103,110,111,114,101,100,83,111,117,114,99,101,82,111,108,101,0,18,0,0,0,80,108,97,121,101,114,67,104,97,114,97,99,116,101,114,95,48,0,7,0,0,0,8,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7};
static const unsigned char load_obj_bytes_2[]={1,0,0,0,18,0,0,0,73,103,110,111,114,101,100,83,111,117,114,99,101,82,111,108,101,0,4,0,0,0,77,111,98,0,7,0,0,0,8,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7};
static const unsigned char load_obj_bytes_3[]={1,0,0,0,18,0,0,0,73,103,110,111,114,101,100,83,111,117,114,99,101,82,111,108,101,0,18,0,0,0,80,108,97,121,101,114,67,104,97,114,97,99,116,101,114,95,48,0,7,0,0,0,8,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7};
static const unsigned char load_obj_bytes_4[]={1,0,0,0,18,0,0,0,73,103,110,111,114,101,100,83,111,117,114,99,101,82,111,108,101,0,4,0,0,0,77,111,98,0,7,0,0,0,8,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7};
static const unsigned char load_obj_bytes_5[]={1,0,0,0,18,0,0,0,73,103,110,111,114,101,100,83,111,117,114,99,101,82,111,108,101,0,18,0,0,0,80,108,97,121,101,114,67,104,97,114,97,99,116,101,114,95,48,0,7,0,0,0,8,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7};
static const LoadObjectsGoldenV2 load_objects_gold_v2[]={
{"Mob",false,false,0,1,54,load_obj_bytes_0,54},
{"PlayerCharacter_0",false,false,4,1,68,load_obj_bytes_1,68},
{"Mob",true,false,0,0,54,load_obj_bytes_2,54},
{"PlayerCharacter_0",false,false,8,1,68,load_obj_bytes_3,68},
{"Mob",false,false,4,1,54,load_obj_bytes_4,54},
{"PlayerCharacter_0",false,true,8,0,0,load_obj_bytes_5,68},
};
