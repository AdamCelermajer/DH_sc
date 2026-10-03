"""Hash official Box2D source provenance and mark every native/fork alteration."""
import argparse,hashlib,json,struct
from pathlib import Path
from elftools.elf.elffile import ELFFile
ROOT=Path(__file__).resolve().parents[1]
def hash_file(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--archive',type=Path,required=True);p.add_argument('--upstream',type=Path,required=True);a=p.parse_args()
 expected='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80';assert hash_file(a.engine)==expected
 archive_sha1=hashlib.sha1(a.archive.read_bytes()).hexdigest();assert archive_sha1=='27a1a0bd08c81bbf661fa008645ee5c538bb2767'
 with a.engine.open('rb') as stream:
  elf=ELFFile(stream);symbol=next(s for s in elf.get_section_by_name('.symtab').iter_symbols() if s.name=='b2_version');address=symbol['st_value'];segment=next(s for s in elf.iter_segments() if s['p_type']=='PT_LOAD' and s['p_vaddr']<=address<s['p_vaddr']+s['p_filesz']);stream.seek(segment['p_offset']+address-segment['p_vaddr']);version=struct.unpack('<3i',stream.read(12))
 assert version==(2,0,1);vendor=ROOT/'box2d-2.0.1';rows=[]
 for file in sorted(vendor.rglob('*')):
  if not file.is_file():continue
  relative=file.relative_to(vendor);original=a.upstream/relative;assert original.is_file();upstream_hash=hash_file(original);native_hash=hash_file(file);rows.append({'file':relative.as_posix(),'upstream_sha256':upstream_hash,'native_sha256':native_hash,'modified':upstream_hash!=native_hash})
 modified=[row['file'] for row in rows if row['modified']];assert modified==['Source/Common/b2Settings.cpp','Source/Common/b2Settings.h']
 result={'source_project':'Box2D','source_release':'2.0.1','primary_archive_index':'https://code.google.com/archive/p/box2d/downloads','primary_download_index_json':'https://storage.googleapis.com/google-code-archive/v2/code.google.com/box2d/downloads-page-2.json','primary_source_archive':'https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/box2d/Box2D_v2.0.1.zip','published_archive_sha1':archive_sha1,'archive_sha256':hash_file(a.archive),'original_engine_sha256':expected,'original_version_symbol_address':hex(address),'original_version':list(version),'vendor_files':len(rows),'modified_files':modified,'alterations':{'Source/Common/b2Settings.cpp':'64-bit aligned allocator header instead of original +4; preserves allocator size header and byte accounting with native header width.','Source/Common/b2Settings.h':'DH2 binary fork capacity:2048 proxies and 16384 pairs; official release has512/4096. Recovered original constructor loops and broadphase layout.'},'license_file_sha256':hash_file(vendor/'License.txt'),'license':'Original zlib/libpng license notices preserved; auxiliary Fixed.h retains its original MIT notice.','files':rows}
 (ROOT/'reference/provenance.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k!='files'}))
if __name__=='__main__':main()
