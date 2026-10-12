import pathlib,sys,json,hashlib,struct
R=pathlib.Path(__file__).resolve().parents[2];D=pathlib.Path(__file__).resolve().parent
sys.path.insert(0,str(R/'port/game-data/tests'))
from items_differential import Original,strings
manifest=json.loads((D/'original-functions.json').read_text());c=Original(R/'.local-inputs/libDungeonHunter2.so',manifest)
records=(R/'.local-inputs/design-settings/design_pyarray.bin').read_bytes();names=(R/'.local-inputs/design-settings/design_pyarraynames.bin').read_bytes();schema=(R/'.local-inputs/design-settings/design_pystructnames.bin').read_bytes()
c.blob=records;c.cursor=240;c.invoke(0x4bc8e0,[c.stream]);count=c.word(0x9a64ac);table=c.word(0x9a64b0);assert count==16 and c.cursor==len(records)
rows=[list(struct.unpack('<7i',c.uc.mem_read(table+i*32+4,28))) for i in range(count)]
name_blocks=strings(names);field_blocks=strings(schema);names_start=15+4+sum(4+len(x) for x in name_blocks[1])
c.blob=names;c.cursor=names_start;c.invoke(0x4b360c,[c.stream]);name_array=c.word(0x9a64b4)
def text(p):
 out=bytearray()
 while c.uc.mem_read(p+len(out),1)!=b'\0':out.extend(c.uc.mem_read(p+len(out),1))
 return out.decode()
actual_names=[text(c.word(name_array+i*4)) for i in range(count)];assert actual_names==[x.decode() for x in name_blocks[2]] and c.cursor==len(names)
out={'validation':'PASS','original_sha256':manifest['original_sha256'],'records_offset':240,'records_consumed':len(records),'names_offset':names_start,'names_consumed':len(names),'rows':[{'name':name,'fields':dict(zip([x.decode() for x in field_blocks[2]],row)),'words':row} for name,row in zip(actual_names,rows)],'input_sha256':{f'design_{kind}.bin':hashlib.sha256((R/f'.local-inputs/design-settings/design_{kind}.bin').read_bytes()).hexdigest() for kind in ('pyarray','pyarraynames','pystructnames')},'stream_reads':c.reads,'allocations':c.allocations,'scope':'Actual original GameOptionTable.read/readNames executed on canonical cache stream suffixes; caller stream+allocation services. No inferred defaults.'}
(D/'original-cache.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
