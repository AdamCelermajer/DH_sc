from pathlib import Path
p=Path(__file__).resolve().with_name('run_authored_fx_alignment_v39.py')
s=p.read_text().replace('alignment-v39','publication-v49').replace("authored_fx_alignment_v39.cpp'","authored_fx_publication_v49.cpp'").replace("sources+=['port/level-world/authored_fx_publication_v49.cpp']",'')
exec(compile(s,str(p),'exec'),{'__file__':str(p),'__name__':'__main__'})
