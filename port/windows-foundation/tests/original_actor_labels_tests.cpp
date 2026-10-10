#include "../original_actor_labels.hpp"
#include "../asset_catalog.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
void check(bool value,const std::string& error){if(!value)throw std::runtime_error(error);}
int main(int argc,char**argv){try{
 check(argc==2,"usage label-assets-root");AssetCatalog assets(argv[1]);OriginalActorLabels labels;std::string e;
 check(labels.load(assets,"original-cache/data",0,e),e);
 OriginalActorLabelInput enemy;enemy.name_id=1507566;enemy.level_raw=256;OriginalActorLabel label;
 check(labels.label(enemy,label,e),e);check(label.level=="1","Q8 level conversion");
 std::cout<<"lizard="<<label.name<<" level="<<label.level<<"\n";
 enemy.boss=true;check(labels.label(enemy,label,e)&&label.level=="??","Original boss level mask");
 enemy.name_id=-1;auto old=label;check(!labels.label(enemy,label,e)&&label.name==old.name,"Missing name must preserve output");
 enemy.player=true;enemy.saved_player_name="Source player";check(labels.label(enemy,label,e)&&label.name=="Source player","Saved player name");
 enemy.saved_player_name.clear();check(!labels.label(enemy,label,e),"Missing player save name must reject");
 check(!labels.load(assets,"missing",0,e),"Missing tables must reject");
 enemy.player=false;enemy.name_id=1507535;enemy.boss=false;check(labels.label(enemy,label,e),e);
 std::cout<<"priest="<<label.name<<"\noriginal actor label tests passed\n";
}catch(const std::exception&ex){std::cerr<<ex.what()<<"\n";return 1;}}
