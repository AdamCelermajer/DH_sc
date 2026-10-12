from pathlib import Path
p=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/production-filesystem-adoption-v50/main_mounted_image_filesystem_v50.cpp.proposed')
s=p.read_text()
old='''  if(fs.actual_source_globals->value()!=working||!fs.require_current_renderer_owner(error)){
   if(error.empty())error="Source filesystem working-directory changed during filename scope";
   return false;
  }
'''
new='''  const auto lookup_error=error;
  if(fs.actual_source_globals->value()!=working){
   error="Source filesystem working-directory changed during filename scope";return false;
  }
  std::string owner_error;
  if(!fs.require_current_renderer_owner(owner_error)){
   error=owner_error.empty()?"Required current renderer owner after metadata probe":owner_error;return false;
  }
'''
assert s.count(old)==1;s=s.replace(old,new).replace('if(read||found||error!=stopped||','if(read||found||lookup_error!=stopped||').replace('  if(!read)return false;','  if(!read){error=lookup_error;return false;}')
p.write_text(s,newline='\n')
print('Preserved owned metadata stop diagnostic across current-owner success callback.')
