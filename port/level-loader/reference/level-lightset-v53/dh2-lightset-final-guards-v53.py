from pathlib import Path
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader')
p=b/'canonical_lightset_file_v53.cpp';s=p.read_text().replace('if(!input_.validate_current||!input_.validate_current(e))return fail(e.empty()?"Required current actual light-set candidate validation":e);','''if(!input_.validate_current||!input_.validate_current(e))return fail(e.empty()?"Required current actual light-set candidate validation":e);
 if(failed_)return fail(error_);
 // This adapter implements resource-open/buffer traversal only. Do not resolve
 // a filename when original LoadFile already owns an assigned stream140.
 if(input_.level->constructor_fields_v3().field140)return fail("Required actual assigned Level140 stream continuation");''').replace('std::string next;if(!input_.resolve_filename(raw,next,e))return fail(e);','std::string next;if(!input_.resolve_filename(raw,next,e))return fail(e);if(failed_)return fail(error_);').replace('if(!input_.validate_current(e))return fail(e);\n e.clear();','if(!input_.validate_current(e))return fail(e);if(failed_)return fail(error_);\n e.clear();');p.write_text(s)
p=b/'level_lightset_stage9_v53.cpp';s=p.read_text().replace('if(!services_.validate_current(e))return fail(e,e);\n if(complete_)','if(!services_.validate_current(e))return fail(e,e);if(failed_){e=error_;return LifecycleStepV36::failed;}\n if(complete_)').replace('if(!services_.validate_current(e))return fail(e,e);\n if(result==','if(!services_.validate_current(e))return fail(e,e);if(failed_){e=error_;return LifecycleStepV36::failed;}\n if(result==');p.write_text(s)
p=Path('C:/Users/adamc/AppData/Local/Temp/dh2-lightset-real-source-v53.py');s=p.read_text().replace("test=test.replace('c.handle08','c.handle')","test=test.replace('c.handle08','c.shared_handle')");p.write_text(s)
print('Unique V53 recursive failure and assigned-stream guards complete')
