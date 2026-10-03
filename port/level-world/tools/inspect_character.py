"""Inspect the evidenced leading CharacterTable and ModelDict sections."""
import argparse,struct,json
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('folder',type=Path);a=p.parse_args()
def strings(raw):
 count=struct.unpack_from('<I',raw)[0];offset=4;out=[];assert count<10000
 for _ in range(count):
  size=struct.unpack_from('<I',raw,offset)[0];offset+=4;assert size<=4096;out.append(raw[offset:offset+size].decode());offset+=size
 return out,offset
def load(name):return (a.folder/name).read_bytes()
names,names_end=strings(load('character_properties_pyarraynames.bin'));fields,fields_end=strings(load('character_properties_pystructnames.bin'));models,model_end=strings(load('character_models_dictionary_pyarray.bin'))
raw=load('character_properties_pyarray.bin');count=struct.unpack_from('<I',raw)[0];assert count==len(names) and len(fields)==224
out={'rows':count,'fields':len(fields),'characters_section_end':4+count*len(fields)*4,'file_bytes':len(raw),'model_count':len(models),'selected':[]}
for name in ('Crypt_Skeleton','CryptSlime','WanderingPriest','DefaultFairy'):
 index=names.index(name);values=struct.unpack_from('<224i',raw,4+index*224*4);row=dict(zip(fields,values));model=row['ModelFile']
 out['selected'].append({'name':name,'row':index,'model_id':model,'model':models[model] if model>=0 else None,'properties':row})
print(json.dumps(out,indent=2))
