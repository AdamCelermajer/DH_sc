"""Generate native lookup metadata from the recovered original registry receipt."""
import hashlib,json,pathlib
root=pathlib.Path(__file__).resolve().parents[1]
receipt=root/'reports/original-object-factories.json';data=json.loads(receipt.read_text())
assert data['validation']=='PASS' and len(data['registry'])==33
lines=['// Original ELF registry 0x95c800; addresses are provenance, never native callable pointers.',
       '// Receipt SHA-256: '+hashlib.sha256(receipt.read_bytes()).hexdigest()]
for index,row in enumerate(data['registry']):
    assert row['index']==index and row['gametype'].isascii() and row['gametype'].isalnum()
    lines.append('{"'+row['gametype']+'", '+row['factory_address']+'u},')
path=root/'original_factory_table_v1.inc';path.write_text('\n'.join(lines)+'\n')
print(path)
