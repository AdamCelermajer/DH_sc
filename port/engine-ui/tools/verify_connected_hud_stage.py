"""Read-only connected HUD source receipt; no build, device or historical rebinding."""
import argparse,hashlib,json,pathlib,zipfile

ROOT=pathlib.Path(__file__).resolve().parents[3]
CAPTURE_SHA='18ddbe441ecbbb4df714c4b63b426d321601c749331db521806639daf4989aa1'
RENDERER_SHA='4e2e915db366b03881c0b6bde21cca93b99206f20a82ab1e3d832ce51b33c32e'
CPP='port/android-native/app/src/main/cpp/model_renderer.cpp'
HPP=CPP.removesuffix('.cpp')+'.hpp'
GETTER='''PlayerHudView player_hud_view(){
 const auto& sheet=prince_combat.properties().resolved;
 return {sheet.data(),sheet.size(),reinterpret_cast<std::uintptr_t>(&prince_combat),bool(prince_combat.life().dead)};
}
'''
VIEW='''// Synchronous GL-thread borrow of the same resolved sheet used by combat.
// Loading another world revokes this view; UI must not cache the data pointer.
struct PlayerHudView {
    const std::int32_t* resolved{};
    std::size_t count{};
    std::uintptr_t character{};
    bool dead{};
};
PlayerHudView player_hud_view();
'''

def sha(data):return hashlib.sha256(data).hexdigest()
def digest(path):return sha(path.read_bytes())
def normalized(data):return data.decode('utf8').replace('\r','')
def verify_addition(old,current,addition):
    text=normalized(current)
    assert text.count(addition)==1,'exact additive block missing/repeated'
    assert text.replace(addition,'',1)==normalized(old),'unexpected existing source changes'
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--historical-capture',type=pathlib.Path,default=ROOT/'.local-inputs/native-original-hud-packaged-build-capture.zip')
    p.add_argument('--baseline-directory',type=pathlib.Path,default=ROOT/'.local-inputs/connected-hud-stage/historical-renderer')
    p.add_argument('--host',type=pathlib.Path,help='Optional current whole-feature host report; binding only, not rerun')
    p.add_argument('--core-stage',type=pathlib.Path,help='Optional current font core stage verifier PASS report')
    p.add_argument('--output',required=True,type=pathlib.Path)
    args=p.parse_args()
    assert digest(args.historical_capture)==CAPTURE_SHA,'unexpected historical capture'
    names=[CPP,HPP]
    names += ['port/android-native/app/src/main/cpp/'+n for n in
      ('native_app.cpp','original_ui_session.cpp','original_ui_session.hpp','original_ui_assets.cpp','original_ui_assets.hpp',
       'swf_gpu.cpp','swf_gpu.hpp','authored_shader_program.cpp','authored_shader_program.hpp','CMakeLists.txt')]
    names += ['port/android-native/app/src/main/java/com/example/dh2/'+n for n in ('MainActivity.java','NativeBridge.java')]
    names += ['port/engine-ui/'+n for n in ('swf_movie.cpp','swf_movie.hpp','CMakeLists.txt','gameswf_sources.cmake',
      'gameswf_font_overlay_v1.cmake','overlays/font-v1/gameswf_font.cpp')]
    for stem in ('player_status_hud','hud_player_values','swf_viewport_connection','hud_sprite_timeline','hud_sprite_core','hud_advance','hud_advance_owner'):
        names += ['port/engine-ui/'+stem+ext for ext in ('.hpp','.cpp')]
    before={n:digest(ROOT/n) for n in names}
    with zipfile.ZipFile(args.historical_capture) as z:old={n:z.read('source/'+n) for n in (CPP,HPP)}
    assert sha(old[CPP])==RENDERER_SHA,'captured renderer is not the verified baseline'
    for name,addition in ((CPP,GETTER),(HPP,VIEW)):
        verify_addition(old[name],(ROOT/name).read_bytes(),addition)
    args.baseline_directory.mkdir(parents=True,exist_ok=True)
    retained={}
    for name,data in old.items():
        target=args.baseline_directory/pathlib.Path(name).name
        if target.exists():assert target.read_bytes()==data,'historical baseline copy changed'
        else:target.write_bytes(data)
        retained[name]={'path':str(target),'sha256':sha(data)}
    report={'validation':'PASS','scope':'Source connection receipt only: exact additive read-only player sheet getter plus header view, apart from line-ending representation. No gameplay/renderer body changes; current connected app/facade/bridge source hashes bound separately. Historical source/proofs not rebound. Host report optional binding; no APK/device or full HUD callback claim.',
      'script_sha256':digest(pathlib.Path(__file__)),
      'renderer_migration':{'historical_sha256':RENDERER_SHA,'current_sha256':before[CPP],
        'historical_capture_sha256':CAPTURE_SHA,'exact_additive_getter_verified':True,
        'historical_header_sha256':sha(old[HPP]),'current_header_sha256':before[HPP],
        'exact_additive_header_view_verified':True,'line_ending_normalization':'CR bytes removed only; all other source bytes must match after removing the exact additions',
        'historical_bytes':retained},'source_sha256':before,'historical_proof_rebound':False}
    for option,label in ((args.host,'host'),(args.core_stage,'core_stage')):
        if option:
            proof=json.loads(option.read_text());assert proof['validation']=='PASS',label+' proof is not PASS'
            report[label+'_path']=str(option);report[label+'_sha256']=digest(option)
    assert before=={n:digest(ROOT/n) for n in names},'current sources changed during read-only receipt'
    args.output.parent.mkdir(parents=True,exist_ok=True)
    args.output.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({'validation':'PASS','exact_additive_getter_verified':True,'source_inputs':len(before),'historical_proof_rebound':False}))
if __name__=='__main__':main()
