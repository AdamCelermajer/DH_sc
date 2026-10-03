"""Validate and decode the complete owner's texture corpus with native C++."""
import argparse
import ctypes as c
import hashlib
import json
from pathlib import Path
import zipfile

class Description(c.Structure):
    _fields_=[(n,c.c_uint32) for n in ('kind','engine_format','base_mip','reserved','width','height','surfaces','mipmapped')]
class View(c.Structure):
    _fields_=[('payload',c.c_void_p),('payload_size',c.c_size_t)]+[(n,c.c_uint32) for n in
                ('width','height','format','alpha','top_origin','right_origin')]

def bind(path):
    dll=c.CDLL(str(path.resolve()))
    dll.dh2_texture_open.argtypes=[c.c_void_p,c.c_size_t,c.POINTER(View)]
    dll.dh2_texture_open.restype=c.c_uint32
    dll.dh2_texture_decode.argtypes=[c.POINTER(View),c.c_void_p,c.c_size_t]
    dll.dh2_texture_decode.restype=c.c_uint32
    dll.dh2_pvr_describe.argtypes=[c.c_void_p,c.c_size_t,c.POINTER(Description)]
    dll.dh2_pvr_describe.restype=c.c_bool
    return dll

def main():
    p=argparse.ArgumentParser()
    p.add_argument('--cache',type=Path,required=True)
    p.add_argument('--library',type=Path,required=True)
    p.add_argument('--report',type=Path,required=True)
    p.add_argument('--preview-dir',type=Path)
    a=p.parse_args(); dll=bind(a.library); rows=[]; formats={}
    previews={'skybox_wind.tga','godparticle_face.tga','fx_smoke_03.tga','menugraphics03.tga'}
    with zipfile.ZipFile(a.cache) as archive:
        for entry in archive.infolist():
            if not entry.filename.endswith('.tga'): continue
            raw=archive.read(entry); data=c.create_string_buffer(raw); v=View()
            error=dll.dh2_texture_open(data,len(raw),c.byref(v))
            assert error==0,(entry.filename,error)
            out=c.create_string_buffer(v.width*v.height*4)
            assert dll.dh2_texture_decode(c.byref(v),out,len(out))==0
            assert dll.dh2_texture_decode(c.byref(v),out,len(out)-1)==6
            if not v.alpha: assert set(out.raw[3::4])=={255}
            formats[str(v.format)]=formats.get(str(v.format),0)+1
            row={'path':entry.filename,'input_sha256':hashlib.sha256(raw).hexdigest(),
                 'width':v.width,'height':v.height,'format':v.format,'alpha':v.alpha,
                 'rgba_sha256':hashlib.sha256(out.raw).hexdigest()}
            if v.format<2:
                desc=Description();assert dll.dh2_pvr_describe(data,len(raw),c.byref(desc))
                row['engine_description']={name:getattr(desc,name) for name,_ in Description._fields_}
            if a.preview_dir and Path(entry.filename).name in previews:
                a.preview_dir.mkdir(parents=True,exist_ok=True)
                rgba=out.raw
                rgb=bytes(channel for i in range(0,len(rgba),4) for channel in rgba[i:i+3])
                (a.preview_dir/(Path(entry.filename).stem+'.ppm')).write_bytes(f'P6\n{v.width} {v.height}\n255\n'.encode()+rgb)
            rows.append(row)
    report={'cache_sha256':hashlib.sha256(a.cache.read_bytes()).hexdigest(),
            'native_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'decoded_files':len(rows),
            'formats':formats,'gpu_execution':False,'original_pixel_equivalence':False,'files':rows}
    a.report.parent.mkdir(parents=True,exist_ok=True)
    a.report.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:v for k,v in report.items() if k!='files'}))
if __name__=='__main__':main()
