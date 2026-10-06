#include "../module_visual_mesh_box_v2.hpp"
#include <cstring>
#include <iostream>
float f(unsigned w){float x;std::memcpy(&x,&w,4);return x;}
int main(){unsigned count=0;std::string e;
{std::vector<dh2::world::ModuleVisualMeshV2> m;
std::array<float,16> root{f(3211036528u),f(3212101018u),f(0u),f(0u),f(1037744983u),f(3209727738u),f(0u),f(0u),f(0u),f(0u),f(3216041514u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{0u,0u,0u,0u,0u,0u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3266422448u),f(3264122857u),f(3264158843u),f(1118877892u),f(1115695427u),f(1114287037u)},{f(1056964608u),f(3212836864u),f(3221225472u)},false});
m.push_back({{f(3241088593u),f(3262988438u),f(3262968863u),f(1108744078u),f(1106047261u),f(1084910471u)},{f(3212836864u),f(3212836864u),f(0u)},false});
std::array<float,16> root{f(3208757737u),f(3136925410u),f(0u),f(0u),f(1054399721u),f(1055865921u),f(0u),f(0u),f(0u),f(0u),f(1066023866u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3260339313u,3249429884u,0u,1112855665u,1101946236u,0u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3267705034u),f(3251444220u),f(3258952714u),f(1115282398u),f(1110009113u),f(1113795109u)},{f(0u),f(3212836864u),f(0u)},false});
m.push_back({{f(3267044268u),f(3256604593u),f(3255922096u),f(1110220228u),f(1085403272u),f(1115597349u)},{f(3221225472u),f(1077936128u),f(3212836864u)},true});
m.push_back({{f(3257176405u),f(3259566260u),f(3258620562u),f(1088231384u),f(1116881525u),f(1117882253u)},{f(1065353216u),f(0u),f(0u)},false});
std::array<float,16> root{f(1050964134u),f(3212202055u),f(0u),f(0u),f(3201188965u),f(1071929410u),f(0u),f(0u),f(0u),f(0u),f(3211713086u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3264061892u,3279597439u,3258623390u,1116578244u,1132113791u,1111139742u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3250621457u),f(3258532496u),f(3267703081u),f(1099791347u),f(1108252372u),f(1103619682u)},{f(1056964608u),f(3212836864u),f(3212836864u)},true});
m.push_back({{f(3264604013u),f(3263932912u),f(3255306851u),f(1118621825u),f(1115824982u),f(1107721515u)},{f(1065353216u),f(1065353216u),f(1077936128u)},false});
m.push_back({{f(3256433338u),f(3256505514u),f(3239369841u),f(1116141713u),f(1110024584u),f(1118439767u)},{f(0u),f(0u),f(1065353216u)},false});
m.push_back({{f(3232038136u),f(3265922162u),f(3257479205u),f(1115161266u),f(1115411253u),f(1116524821u)},{f(3221225472u),f(1077936128u),f(1056964608u)},true});
std::array<float,16> root{f(1069026807u),f(1056993550u),f(0u),f(0u),f(1057756825u),f(1072145045u),f(0u),f(0u),f(0u),f(0u),f(1069240322u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3272111803u,3284889902u,3257200195u,1124628155u,1137406254u,1109716547u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3264610156u),f(3261076211u),f(3262969016u),f(1104721768u),f(1089283832u),f(1112273546u)},{f(1077936128u),f(3212836864u),f(3221225472u)},false});
m.push_back({{f(3263928486u),f(3265340183u),f(3247566950u),f(1117927092u),f(1119652258u),f(1088930661u)},{f(1056964608u),f(1077936128u),f(1077936128u)},false});
m.push_back({{f(3263301716u),f(3260551307u),f(3258734717u),f(1082025102u),f(1109841483u),f(1087646945u)},{f(0u),f(1056964608u),f(1077936128u)},true});
m.push_back({{f(3267756343u),f(3267062516u),f(3266754047u),f(1108510615u),f(1108757536u),f(1107905071u)},{f(1077936128u),f(3221225472u),f(1056964608u)},false});
m.push_back({{f(3255625895u),f(3264691565u),f(3258765235u),f(1111193388u),f(1112910207u),f(1119655445u)},{f(3212836864u),f(1065353216u),f(1077936128u)},false});
std::array<float,16> root{f(1057727517u),f(3211925132u),f(0u),f(0u),f(1064421622u),f(3198420687u),f(0u),f(0u),f(0u),f(0u),f(1052221914u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3249852914u,3237303824u,3253154027u,1102369266u,1089820176u,1105670379u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3265272485u),f(3257374397u),f(3263879800u),f(1087515774u),f(1114599342u),f(1104630497u)},{f(1077936128u),f(1077936128u),f(1056964608u)},false});
std::array<float,16> root{f(1071818555u),f(3203701658u),f(0u),f(0u),f(3206384486u),f(3211303117u),f(0u),f(0u),f(0u),f(0u),f(3206114297u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3272090464u,3276325178u,3244749809u,1124606816u,1128841530u,1097266161u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3255488938u),f(3265276248u),f(3257701614u),f(1109392246u),f(1118133269u),f(1115760928u)},{f(1056964608u),f(3212836864u),f(1065353216u)},true});
m.push_back({{f(3264396791u),f(3265468201u),f(3258079985u),f(1116306933u),f(1109190064u),f(1112070955u)},{f(0u),f(3221225472u),f(3212836864u)},false});
std::array<float,16> root{f(1062661430u),f(3211935500u),f(0u),f(0u),f(3205148896u),f(3203375536u),f(0u),f(0u),f(0u),f(0u),f(1067118284u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3262058988u,3248706922u,3263325314u,1114575340u,1101223274u,1115841666u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3259515751u),f(3266700143u),f(3267186965u),f(1118679432u),f(1112966668u),f(1118894023u)},{f(1056964608u),f(3212836864u),f(1056964608u)},false});
m.push_back({{f(3252387479u),f(3265245149u),f(3251636280u),f(1112761469u),f(1116854527u),f(1096005024u)},{f(0u),f(3212836864u),f(3221225472u)},false});
m.push_back({{f(3213548804u),f(3263442522u),f(3259628829u),f(1084544360u),f(1112280920u),f(1119772647u)},{f(1077936128u),f(3212836864u),f(1065353216u)},true});
std::array<float,16> root{f(3217835673u),f(3210105685u),f(0u),f(0u),f(1057524052u),f(1058556582u),f(0u),f(0u),f(0u),f(0u),f(1066172091u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3258491010u,3257575289u,3265259494u,1111007362u,1110091641u,1117775846u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3262417973u),f(3246775215u),f(3241258228u),f(1117421309u),f(1104983211u),f(1120022064u)},{f(3212836864u),f(0u),f(0u)},false});
m.push_back({{f(3264457167u),f(3266306034u),f(3266100884u),f(1101725813u),f(1082558581u),f(1100434745u)},{f(1077936128u),f(1056964608u),f(1056964608u)},true});
m.push_back({{f(3258627909u),f(3263630021u),f(3259964810u),f(1115622629u),f(1119737003u),f(1118679579u)},{f(1065353216u),f(1056964608u),f(3212836864u)},false});
m.push_back({{f(3260531658u),f(3240562733u),f(3264867329u),f(1115808715u),f(1118724633u),f(1106345217u)},{f(1077936128u),f(1065353216u),f(1065353216u)},false});
std::array<float,16> root{f(3193817917u),f(1055454556u),f(0u),f(0u),f(1058677035u),f(3220257076u),f(0u),f(0u),f(0u),f(0u),f(3187808722u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3246948720u,3249309867u,3226863268u,1099465072u,1101826219u,1079379620u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3253287742u),f(3256011675u),f(3266082249u),f(1112471013u),f(1118518293u),f(1091176525u)},{f(3212836864u),f(3221225472u),f(1065353216u)},true});
m.push_back({{f(3250241244u),f(3266134195u),f(3260295562u),f(1106690290u),f(1104566118u),f(1083660411u)},{f(3221225472u),f(3221225472u),f(1056964608u)},false});
m.push_back({{f(3262293798u),f(3267796829u),f(3232193148u),f(1092802763u),f(1101573391u),f(1118274574u)},{f(0u),f(0u),f(1077936128u)},false});
m.push_back({{f(3267808758u),f(3243144458u),f(3263384185u),f(1119351072u),f(1115162038u),f(1101335533u)},{f(1065353216u),f(1065353216u),f(3221225472u)},true});
m.push_back({{f(3261378739u),f(3267625796u),f(3235242301u),f(1083857785u),f(1100181053u),f(1108814468u)},{f(1065353216u),f(1077936128u),f(3221225472u)},false});
std::array<float,16> root{f(3188256499u),f(3198804630u),f(0u),f(0u),f(3205993240u),f(1050115479u),f(0u),f(0u),f(0u),f(0u),f(1067130238u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3255558999u,3248838364u,3261458201u,1108075351u,1101354716u,1113974553u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3248208331u),f(3263400961u),f(3264110604u),f(1108121710u),f(1099616522u),f(1118311310u)},{f(3212836864u),f(3212836864u),f(1077936128u)},false});
std::array<float,16> root{f(1069833005u),f(1038373347u),f(0u),f(0u),f(1063803982u),f(1065431760u),f(0u),f(0u),f(0u),f(0u),f(1066055242u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3265215208u,3258186792u,3279704850u,1117731560u,1110703144u,1132221202u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3265317818u),f(3267578528u),f(3254699391u),f(1114397404u),f(1099912675u),f(1115502235u)},{f(1065353216u),f(0u),f(3212836864u)},false});
m.push_back({{f(3250536751u),f(3261852543u),f(3256663862u),f(1107722282u),f(1114843784u),f(1098967247u)},{f(0u),f(1077936128u),f(1065353216u)},true});
std::array<float,16> root{f(3214340071u),f(1036595416u),f(0u),f(0u),f(1064719675u),f(3214883790u),f(0u),f(0u),f(0u),f(0u),f(3214056255u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3274497920u,3277815539u,3254606632u,1127014272u,1130331891u,1107122984u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3267567788u),f(3265197086u),f(3255767690u),f(1111594083u),f(1118647499u),f(1102629958u)},{f(3221225472u),f(0u),f(1065353216u)},true});
m.push_back({{f(3263639718u),f(3248428724u),f(3244939913u),f(1116735196u),f(1095323348u),f(1118414710u)},{f(0u),f(1065353216u),f(0u)},false});
m.push_back({{f(3226088532u),f(3256341632u),f(3265316872u),f(1102465592u),f(1108530856u),f(1119059717u)},{f(1056964608u),f(3212836864u),f(1065353216u)},false});
std::array<float,16> root{f(3187107289u),f(1051058477u),f(0u),f(0u),f(3207997581u),f(1069927565u),f(0u),f(0u),f(0u),f(0u),f(1066067682u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3247246815u,3258787622u,3254748888u,1099763167u,1111303974u,1107265240u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3236611091u),f(3253690314u),f(3258679882u),f(1083857464u),f(1107997910u),f(1115429987u)},{f(0u),f(1077936128u),f(1056964608u)},false});
m.push_back({{f(3263145741u),f(3250680342u),f(3230782354u),f(1092340246u),f(1085793721u),f(1120068538u)},{f(1077936128u),f(1056964608u),f(1077936128u)},false});
m.push_back({{f(3266066979u),f(3267175625u),f(3262575920u),f(1117690875u),f(1113311053u),f(1119562869u)},{f(0u),f(0u),f(0u)},true});
m.push_back({{f(3266388261u),f(3252171013u),f(3260382608u),f(1114827073u),f(1106291082u),f(1101942398u)},{f(0u),f(3212836864u),f(1077936128u)},false});
std::array<float,16> root{f(3205918447u),f(1055672910u),f(0u),f(0u),f(3204587077u),f(1054941447u),f(0u),f(0u),f(0u),f(0u),f(3199979150u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{0u,0u,0u,0u,0u,0u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3267324678u),f(3249011813u),f(3246539310u),f(1118873160u),f(1101729480u),f(1116215999u)},{f(1056964608u),f(3221225472u),f(3212836864u)},false});
m.push_back({{f(3266051328u),f(3236061375u),f(3267587729u),f(1103437006u),f(1117239454u),f(1109020270u)},{f(1065353216u),f(3212836864u),f(3212836864u)},true});
m.push_back({{f(3261866798u),f(3263912520u),f(3255422369u),f(1117642543u),f(1106892298u),f(1109877099u)},{f(3212836864u),f(0u),f(3221225472u)},false});
m.push_back({{f(3248451592u),f(3266693048u),f(3264955735u),f(1110534895u),f(1118530664u),f(1100891978u)},{f(1077936128u),f(0u),f(1065353216u)},false});
m.push_back({{f(3246782817u),f(3251462422u),f(3261309899u),f(1113592860u),f(1096526462u),f(1104910411u)},{f(0u),f(3212836864u),f(1056964608u)},true});
std::array<float,16> root{f(3213414536u),f(1063203340u),f(0u),f(0u),f(3192205873u),f(3215730739u),f(0u),f(0u),f(0u),f(0u),f(3199141479u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3261365700u,3262701439u,3236344822u,1113882052u,1115217791u,1088861174u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3262022199u),f(3259765535u),f(3255291854u),f(1110721979u),f(1110587750u),f(1117987954u)},{f(3221225472u),f(1056964608u),f(1056964608u)},true});
std::array<float,16> root{f(1043171019u),f(3212019025u),f(0u),f(0u),f(1060464571u),f(1068049185u),f(0u),f(0u),f(0u),f(0u),f(1058631644u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3209652680u,3271764504u,3247078230u,1062169032u,1124280856u,1099594582u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3265627319u),f(3263737720u),f(3245897832u),f(1114334149u),f(1115592429u),f(1119059834u)},{f(3221225472u),f(0u),f(0u)},false});
m.push_back({{f(3244860082u),f(3266900527u),f(3260002619u),f(1090861010u),f(1075281667u),f(1100488315u)},{f(0u),f(1056964608u),f(3212836864u)},false});
std::array<float,16> root{f(3196607312u),f(1055402876u),f(0u),f(0u),f(3210655573u),f(3218133351u),f(0u),f(0u),f(0u),f(0u),f(3220967084u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3248817372u,3256531111u,0u,1101333724u,1109047463u,0u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3259899489u),f(3256086488u),f(3261948104u),f(1118431198u),f(1097722111u),f(1115761940u)},{f(3212836864u),f(1065353216u),f(3221225472u)},false});
m.push_back({{f(3264710494u),f(3266002268u),f(3265780972u),f(1119717152u),f(1085288272u),f(1087739766u)},{f(3221225472u),f(3212836864u),f(1056964608u)},true});
m.push_back({{f(3264960379u),f(3267757798u),f(3241858725u),f(1068258920u),f(1115304077u),f(1112804184u)},{f(3212836864u),f(1065353216u),f(3212836864u)},false});
std::array<float,16> root{f(3215434650u),f(1061294340u),f(0u),f(0u),f(1048860738u),f(3215839016u),f(0u),f(0u),f(0u),f(0u),f(3200964048u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3277033322u,3263614645u,3239023684u,1129549674u,1116130997u,1091540036u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3257523402u),f(3256489884u),f(3256233487u),f(1115931760u),f(1108751357u),f(1090379390u)},{f(3221225472u),f(3221225472u),f(1077936128u)},true});
m.push_back({{f(3256554592u),f(3252301528u),f(3257303107u),f(1117808570u),f(1115712366u),f(1119231194u)},{f(3212836864u),f(1056964608u),f(0u)},false});
m.push_back({{f(3267723776u),f(3238374114u),f(3244267840u),f(1110352479u),f(1114880240u),f(1118212003u)},{f(1077936128u),f(3221225472u),f(1056964608u)},false});
m.push_back({{f(3264472791u),f(3260048515u),f(3262026873u),f(1111060844u),f(1100012976u),f(1117900844u)},{f(1056964608u),f(1056964608u),f(3212836864u)},true});
std::array<float,16> root{f(3215644939u),f(3197430066u),f(0u),f(0u),f(3185926342u),f(3218771319u),f(0u),f(0u),f(0u),f(0u),f(3214916458u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3257430767u,3256540529u,3265937957u,1109947119u,1109056881u,1118454309u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3265291940u),f(3264884505u),f(3242725498u),f(1067988085u),f(1088350068u),f(1120129796u)},{f(0u),f(1077936128u),f(3212836864u)},false});
m.push_back({{f(3241571924u),f(3225681604u),f(3252843086u),f(1075153837u),f(1092347611u),f(1116810029u)},{f(1056964608u),f(1065353216u),f(1056964608u)},false});
m.push_back({{f(3234558853u),f(3264884784u),f(3263499669u),f(1108923670u),f(1111266356u),f(1108124907u)},{f(1077936128u),f(3221225472u),f(0u)},true});
m.push_back({{f(3262461921u),f(3267313974u),f(3265050043u),f(1109616787u),f(1098694620u),f(1116596661u)},{f(1077936128u),f(1065353216u),f(3221225472u)},false});
m.push_back({{f(3263365090u),f(3259951620u),f(3265523633u),f(1116297537u),f(1115480123u),f(1113572862u)},{f(1056964608u),f(1065353216u),f(0u)},false});
std::array<float,16> root{f(1069447003u),f(3211979560u),f(0u),f(0u),f(1044743387u),f(1055316009u),f(0u),f(0u),f(0u),f(0u),f(3187287050u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3264676885u,3270436230u,0u,1117193237u,1122952582u,0u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3253951912u),f(3249327646u),f(3267132818u),f(1107034357u),f(1103033271u),f(1116702867u)},{f(1077936128u),f(3212836864u),f(3212836864u)},false});
std::array<float,16> root{f(1066699045u),f(1063669506u),f(0u),f(0u),f(1062589036u),f(1064370451u),f(0u),f(0u),f(0u),f(0u),f(1065124382u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3266418342u,3262685483u,3265511272u,1118934694u,1115201835u,1118027624u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3257897185u),f(3258549395u),f(3267559462u),f(1119866252u),f(1109681199u),f(1104201600u)},{f(1065353216u),f(0u),f(0u)},true});
m.push_back({{f(3267686215u),f(3216184042u),f(3256890522u),f(1112887115u),f(1101303914u),f(1111261108u)},{f(1077936128u),f(0u),f(0u)},false});
std::array<float,16> root{f(3216495902u),f(1021591840u),f(0u),f(0u),f(1042864140u),f(1072496841u),f(0u),f(0u),f(0u),f(0u),f(1061098501u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3267937528u,3220785333u,0u,1120453880u,1073301685u,0u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3257467684u),f(3249429741u),f(3260651699u),f(1118906330u),f(1103190864u),f(1093099554u)},{f(3212836864u),f(1077936128u),f(0u)},false});
m.push_back({{f(3247194096u),f(3244062228u),f(3264379246u),f(1103581011u),f(1120075184u),f(1098934993u)},{f(1056964608u),f(1065353216u),f(0u)},false});
m.push_back({{f(3252053222u),f(3213118854u),f(3238901711u),f(1117387661u),f(1079366016u),f(1103538861u)},{f(1077936128u),f(1077936128u),f(1056964608u)},true});
std::array<float,16> root{f(1072634393u),f(1060470110u),f(0u),f(0u),f(1060112288u),f(3214170616u),f(0u),f(0u),f(0u),f(0u),f(1057552530u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3281234516u,3268249847u,3230676275u,1133750868u,1120766199u,1083192627u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3258546656u),f(3243214986u),f(3265686871u),f(1099752280u),f(1120006340u),f(1074420633u)},{f(3221225472u),f(3221225472u),f(3221225472u)},false});
m.push_back({{f(3255666746u),f(3258992017u),f(3255858858u),f(1095861883u),f(1114042381u),f(1113183055u)},{f(1065353216u),f(3212836864u),f(1077936128u)},true});
m.push_back({{f(3239736604u),f(3255555236u),f(3264505560u),f(1101360773u),f(1117195831u),f(1100625573u)},{f(3212836864u),f(1065353216u),f(1065353216u)},false});
m.push_back({{f(3259755265u),f(3267866305u),f(3249423165u),f(1117593230u),f(1104587705u),f(1112182433u)},{f(1077936128u),f(3212836864u),f(3221225472u)},false});
std::array<float,16> root{f(3217523552u),f(1063688624u),f(0u),f(0u),f(1061672058u),f(3218596318u),f(0u),f(0u),f(0u),f(0u),f(1065392519u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3265144482u,3269336280u,3272113841u,1117660834u,1121852632u,1124630193u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3229735402u),f(3256049462u),f(3264780824u),f(1099089239u),f(1107580343u),f(1112019969u)},{f(0u),f(3212836864u),f(1077936128u)},true});
m.push_back({{f(3258622892u),f(3266762251u),f(3262677880u),f(1117643198u),f(1118981208u),f(1111341896u)},{f(1056964608u),f(0u),f(0u)},false});
m.push_back({{f(3255723191u),f(3251911469u),f(3265548424u),f(1109739499u),f(1120221997u),f(1117942229u)},{f(0u),f(1065353216u),f(1065353216u)},false});
m.push_back({{f(3259202818u),f(3252702581u),f(3255744322u),f(1117787910u),f(1097381599u),f(1115222313u)},{f(0u),f(3221225472u),f(0u)},true});
m.push_back({{f(3252672060u),f(3260064136u),f(3252825018u),f(1116583529u),f(1100711547u),f(1096903744u)},{f(3212836864u),f(3221225472u),f(3212836864u)},false});
std::array<float,16> root{f(1072416530u),f(1043265258u),f(0u),f(0u),f(3186380930u),f(1057377571u),f(0u),f(0u),f(0u),f(0u),f(1071359260u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3228791552u,3247069994u,3282211454u,1081307904u,1099586346u,1134727806u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3251901753u),f(3253800322u),f(3265563502u),f(1115531295u),f(1099054253u),f(1116578421u)},{f(1065353216u),f(0u),f(3212836864u)},false});
std::array<float,16> root{f(1070723195u),f(1062289644u),f(0u),f(0u),f(3187021566u),f(1044296948u),f(0u),f(0u),f(0u),f(0u),f(3218234723u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3264445800u,3256025618u,3271268694u,1116962152u,1108541970u,1123785046u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3257631978u),f(3247067271u),f(3266358296u),f(1099552826u),f(1119798252u),f(1106105383u)},{f(1056964608u),f(3221225472u),f(1065353216u)},false});
m.push_back({{f(3267224984u),f(3228901569u),f(3266395681u),f(1089204555u),f(1113988545u),f(1118220984u)},{f(3212836864u),f(3221225472u),f(1077936128u)},true});
std::array<float,16> root{f(3209776972u),f(3198829414u),f(0u),f(0u),f(3206344015u),f(3220761726u),f(0u),f(0u),f(0u),f(0u),f(1058162012u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3265192636u,3272103661u,3272827753u,1117708988u,1124620013u,1125344105u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3264679696u),f(3265811535u),f(3260473208u),f(1110556940u),f(1111469471u),f(1114819310u)},{f(1056964608u),f(1065353216u),f(1065353216u)},true});
m.push_back({{f(3253728786u),f(3256923088u),f(3231484802u),f(1117493558u),f(1117256779u),f(1112492369u)},{f(1065353216u),f(3221225472u),f(1065353216u)},false});
m.push_back({{f(3264702036u),f(3239935549u),f(3260690052u),f(1115719118u),f(1105564471u),f(1095118924u)},{f(3212836864u),f(3212836864u),f(3221225472u)},false});
std::array<float,16> root{f(1049054253u),f(1017382224u),f(0u),f(0u),f(3211960158u),f(3190864546u),f(0u),f(0u),f(0u),f(0u),f(1068102192u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3260721544u,3240936084u,3264735547u,1113237896u,1093452436u,1117251899u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3263751020u),f(3266147391u),f(3249132667u),f(1104888139u),f(1113198115u),f(1116795383u)},{f(1056964608u),f(0u),f(3212836864u)},false});
m.push_back({{f(3256745667u),f(3266494662u),f(3265576671u),f(1108948542u),f(1106837061u),f(1111229448u)},{f(1056964608u),f(0u),f(1065353216u)},false});
m.push_back({{f(3241970482u),f(3264487753u),f(3263967029u),f(1064633031u),f(1117008560u),f(1117833581u)},{f(3212836864u),f(3221225472u),f(1077936128u)},true});
m.push_back({{f(3250246510u),f(3261208287u),f(3231440195u),f(1112537680u),f(1113817287u),f(1119507382u)},{f(1077936128u),f(1056964608u),f(1056964608u)},false});
std::array<float,16> root{f(3207397655u),f(1060072152u),f(0u),f(0u),f(3200832411u),f(3201919125u),f(0u),f(0u),f(0u),f(0u),f(1067442108u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3262754575u,3261739579u,3280795673u,1115270927u,1114255931u,1133312025u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3241134970u),f(3265761345u),f(3267688090u),f(1118778154u),f(1107501395u),f(1104322514u)},{f(1065353216u),f(0u),f(3212836864u)},false});
m.push_back({{f(3243187689u),f(3248711771u),f(3257663042u),f(1119024625u),f(1118789081u),f(1090950786u)},{f(1077936128u),f(1077936128u),f(3212836864u)},true});
m.push_back({{f(3264573077u),f(3255900861u),f(3253301626u),f(1065823638u),f(1117452948u),f(1112128099u)},{f(1056964608u),f(3221225472u),f(3212836864u)},false});
m.push_back({{f(3252762044u),f(3263880525u),f(3243578273u),f(1112028433u),f(1115654294u),f(1113287799u)},{f(3212836864u),f(1056964608u),f(1077936128u)},false});
m.push_back({{f(3261413579u),f(3266901974u),f(3259695878u),f(1088692793u),f(1118781893u),f(1087551480u)},{f(3212836864u),f(3212836864u),f(3212836864u)},true});
std::array<float,16> root{f(1065829157u),f(1048858346u),f(0u),f(0u),f(1050589473u),f(1064047094u),f(0u),f(0u),f(0u),f(0u),f(3205916677u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3277103198u,3275570643u,3244887318u,1129619550u,1128086995u,1097403670u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3260153001u),f(3266767910u),f(3266531739u),f(1099683827u),f(1113319473u),f(1101097501u)},{f(1065353216u),f(1065353216u),f(1065353216u)},true});
std::array<float,16> root{f(1070200693u),f(3190932292u),f(0u),f(0u),f(3212364906u),f(3212771353u),f(0u),f(0u),f(0u),f(0u),f(3198289789u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3246340994u,3265135507u,3247114587u,1098857346u,1117651859u,1099630939u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3209837851u),f(3257866364u),f(3218653672u),f(1074958120u),f(1100962040u),f(1118589825u)},{f(3212836864u),f(1056964608u),f(0u)},false});
m.push_back({{f(3253884658u),f(3254908261u),f(3265561184u),f(1117982385u),f(1120282228u),f(1111140755u)},{f(3221225472u),f(1077936128u),f(3212836864u)},false});
std::array<float,16> root{f(3204303274u),f(1064708599u),f(0u),f(0u),f(3201779814u),f(1071368094u),f(0u),f(0u),f(0u),f(0u),f(3209423367u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3265555109u,3282611567u,0u,1118071461u,1135127919u,0u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3246560328u),f(3258556240u),f(3264198304u),f(1114798865u),f(1108333849u),f(1108688201u)},{f(3212836864u),f(3221225472u),f(0u)},false});
m.push_back({{f(3239574327u),f(3255471054u),f(3251114577u),f(1107487505u),f(1113107196u),f(1090436732u)},{f(3221225472u),f(0u),f(1065353216u)},true});
m.push_back({{f(3264178365u),f(3252778061u),f(3234383696u),f(1114228004u),f(1099142098u),f(1119773910u)},{f(3221225472u),f(1065353216u),f(3221225472u)},false});
std::array<float,16> root{f(1024052565u),f(1018310615u),f(0u),f(0u),f(3199527673u),f(3196985538u),f(0u),f(0u),f(0u),f(0u),f(1057223373u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3216365896u,3211469916u,3238523494u,1068882248u,1063986268u,1091039846u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3260157619u),f(3262977540u),f(3265064538u),f(1117929356u),f(1118945671u),f(1109431677u)},{f(3212836864u),f(0u),f(1077936128u)},true});
m.push_back({{f(3256919198u),f(3267674142u),f(3253200424u),f(1104880758u),f(1120122626u),f(1102478868u)},{f(0u),f(1077936128u),f(1077936128u)},false});
m.push_back({{f(3215317361u),f(3265808618u),f(3231708167u),f(1105542955u),f(1104444460u),f(1082324934u)},{f(0u),f(0u),f(1065353216u)},false});
m.push_back({{f(3265177910u),f(3263711002u),f(3264698175u),f(1114979933u),f(1094059204u),f(1114709901u)},{f(0u),f(3212836864u),f(1065353216u)},true});
std::array<float,16> root{f(1070647821u),f(3189282148u),f(0u),f(0u),f(1035963095u),f(3208232803u),f(0u),f(0u),f(0u),f(0u),f(1056899318u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{0u,0u,3266394515u,0u,0u,1118910867u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3263243624u),f(3265765888u),f(3266103143u),f(1119319384u),f(1120305542u),f(1116040616u)},{f(1077936128u),f(1077936128u),f(0u)},false});
m.push_back({{f(3263798131u),f(3258740700u),f(3267519199u),f(1106928644u),f(1114596295u),f(1095815987u)},{f(1056964608u),f(3212836864u),f(0u)},false});
m.push_back({{f(3261943024u),f(3254440120u),f(3250768987u),f(1114639840u),f(1105524452u),f(1103753915u)},{f(3221225472u),f(1056964608u),f(3212836864u)},true});
m.push_back({{f(3263756914u),f(3259078483u),f(3249136979u),f(1100101999u),f(1114713630u),f(1117983918u)},{f(0u),f(1065353216u),f(1056964608u)},false});
m.push_back({{f(3240834454u),f(3197602397u),f(3264652179u),f(1118455625u),f(1113480099u),f(1076490498u)},{f(3212836864u),f(1065353216u),f(1077936128u)},false});
std::array<float,16> root{f(1060051012u),f(3199209418u),f(0u),f(0u),f(3196325340u),f(1072955742u),f(0u),f(0u),f(0u),f(0u),f(3218500590u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3265985296u,3263904914u,3257280385u,1118501648u,1116421266u,1109796737u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3256777417u),f(3263385989u),f(3265966424u),f(1101926636u),f(1108489952u),f(1119154887u)},{f(3212836864u),f(1065353216u),f(3221225472u)},false});
std::array<float,16> root{f(3212153031u),f(3198156318u),f(0u),f(0u),f(1055329015u),f(1063249189u),f(0u),f(0u),f(0u),f(0u),f(1071387485u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3260153924u,3260622323u,3281462738u,1112670276u,1113138675u,1133979090u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3263479039u),f(3265214313u),f(3252982153u),f(1103410074u),f(1101413857u),f(1109001011u)},{f(0u),f(1056964608u),f(0u)},true});
m.push_back({{f(3261116825u),f(3258962127u),f(3259103964u),f(1115811211u),f(1113048659u),f(1111420426u)},{f(0u),f(3212836864u),f(3221225472u)},false});
std::array<float,16> root{f(3219787959u),f(1063540211u),f(0u),f(0u),f(1050755902u),f(3174751096u),f(0u),f(0u),f(0u),f(0u),f(3166613118u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3237802912u,3214052512u,0u,1090319264u,1066568864u,0u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3212594614u),f(3264851896u),f(3259871809u),f(1115742257u),f(1097585231u),f(1115694058u)},{f(3221225472u),f(0u),f(3221225472u)},false});
m.push_back({{f(3253465890u),f(3247606613u),f(3247768818u),f(1112317507u),f(1118423659u),f(1089084996u)},{f(1056964608u),f(1077936128u),f(3212836864u)},false});
m.push_back({{f(3265574425u),f(3260889497u),f(3266381892u),f(1118194536u),f(1117045382u),f(1103599499u)},{f(1065353216u),f(3212836864u),f(1065353216u)},true});
std::array<float,16> root{f(1071736051u),f(3212181256u),f(0u),f(0u),f(3209801499u),f(1072483495u),f(0u),f(0u),f(0u),f(0u),f(1073568379u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3276198908u,3276241449u,3269498257u,1128715260u,1128757801u,1122014609u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3261221316u),f(3244235949u),f(3265997811u),f(1108263633u),f(1091221894u),f(1107863115u)},{f(1077936128u),f(1065353216u),f(1065353216u)},false});
m.push_back({{f(3250097074u),f(3255766615u),f(3243595630u),f(1116121688u),f(1106322814u),f(1099114228u)},{f(0u),f(3221225472u),f(3212836864u)},true});
m.push_back({{f(3267768797u),f(3265374160u),f(3265107868u),f(1075187315u),f(1107323791u),f(1104842513u)},{f(0u),f(0u),f(1056964608u)},false});
m.push_back({{f(3260343756u),f(3265621756u),f(3253945966u),f(1108765836u),f(1088599079u),f(1120033770u)},{f(1077936128u),f(1065353216u),f(3212836864u)},false});
std::array<float,16> root{f(3217139837u),f(3212832425u),f(0u),f(0u),f(1034517831u),f(1057393917u),f(0u),f(0u),f(0u),f(0u),f(3213578611u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3232664154u,3255472002u,3246484749u,1085180506u,1107988354u,1099001101u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3263912395u),f(3243069365u),f(3266020315u),f(1110714401u),f(1118718840u),f(1106301670u)},{f(0u),f(1065353216u),f(1056964608u)},true});
m.push_back({{f(3265337647u),f(3266269434u),f(3264969677u),f(1109159406u),f(1107418499u),f(1108093126u)},{f(0u),f(0u),f(1056964608u)},false});
m.push_back({{f(3255967470u),f(3257214754u),f(3229010838u),f(1118456429u),f(1094034372u),f(1096319096u)},{f(3212836864u),f(0u),f(1077936128u)},false});
m.push_back({{f(3267376229u),f(3260272396u),f(3242864736u),f(1104598562u),f(1117004851u),f(1105370686u)},{f(1065353216u),f(3212836864u),f(1065353216u)},true});
m.push_back({{f(3266268379u),f(3267788432u),f(3246624279u),f(1103223259u),f(1105732772u),f(1103869645u)},{f(3212836864u),f(1056964608u),f(1065353216u)},false});
std::array<float,16> root{f(1065707762u),f(3209231186u),f(0u),f(0u),f(3193386880u),f(3203028030u),f(0u),f(0u),f(0u),f(0u),f(1026981182u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3260434449u,3264104992u,3217754454u,1112950801u,1116621344u,1070270806u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3265001317u),f(3224039362u),f(3257101947u),f(1118549963u),f(1110136491u),f(1119253001u)},{f(1056964608u),f(3221225472u),f(0u)},false});
std::array<float,16> root{f(3218831914u),f(3201200905u),f(0u),f(0u),f(3205753065u),f(1058768938u),f(0u),f(0u),f(0u),f(0u),f(3204662309u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3257910372u,3257968330u,0u,1110426724u,1110484682u,0u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3255690236u),f(3256991670u),f(3252056729u),f(1101393483u),f(1106445688u),f(1107103780u)},{f(3221225472u),f(1077936128u),f(1056964608u)},false});
m.push_back({{f(3256614666u),f(3256485043u),f(3255796195u),f(1116215614u),f(1116282325u),f(1111256585u)},{f(3221225472u),f(3221225472u),f(1065353216u)},true});
std::array<float,16> root{f(1050909681u),f(1057743945u),f(0u),f(0u),f(1055106255u),f(1068843028u),f(0u),f(0u),f(0u),f(0u),f(3211594803u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3265502793u,3276937608u,3256462968u,1118019145u,1129453960u,1108979320u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3264350158u),f(3263829246u),f(3265248025u),f(1108489558u),f(1082633339u),f(1119350697u)},{f(1056964608u),f(1065353216u),f(1065353216u)},true});
m.push_back({{f(3265211528u),f(3266120591u),f(3260689512u),f(1115549241u),f(1117293054u),f(1119342915u)},{f(3221225472u),f(0u),f(3221225472u)},false});
m.push_back({{f(3240815657u),f(3262479780u),f(3256169688u),f(1095531115u),f(1111458078u),f(1103568240u)},{f(3221225472u),f(1077936128u),f(1056964608u)},false});
std::array<float,16> root{f(3218860743u),f(1057512909u),f(0u),f(0u),f(3206710445u),f(1051490515u),f(0u),f(0u),f(0u),f(0u),f(3216575713u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3263997235u,3252123745u,3271060400u,1116513587u,1104640097u,1123576752u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3241793846u),f(3255370821u),f(3238643722u),f(1119084799u),f(1116831797u),f(1110876559u)},{f(1077936128u),f(3212836864u),f(1056964608u)},false});
m.push_back({{f(3260283851u),f(3249353796u),f(3266881490u),f(1119085997u),f(1109336952u),f(1059316802u)},{f(1065353216u),f(1065353216u),f(1077936128u)},false});
m.push_back({{f(3261195479u),f(3267552627u),f(3250493409u),f(1119293582u),f(1099865107u),f(1116674986u)},{f(3212836864u),f(1077936128u),f(1056964608u)},true});
m.push_back({{f(3264185870u),f(3249598026u),f(3260539476u),f(1113034132u),f(1116745989u),f(1117885566u)},{f(1077936128u),f(3212836864u),f(1056964608u)},false});
std::array<float,16> root{f(3196393702u),f(1055131598u),f(0u),f(0u),f(1025829156u),f(3213850333u),f(0u),f(0u),f(0u),f(0u),f(3220049912u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3251738577u,3278029498u,3258016747u,1104254929u,1130545850u,1110533099u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3264924738u),f(3236864857u),f(3215338267u),f(1113383166u),f(1112807134u),f(1110977503u)},{f(3212836864u),f(3221225472u),f(1065353216u)},false});
m.push_back({{f(3267506886u),f(3266858996u),f(3257118101u),f(1109321590u),f(1117442112u),f(1084310574u)},{f(3212836864u),f(3221225472u),f(0u)},true});
m.push_back({{f(3263523646u),f(3264803160u),f(3253635991u),f(1099433420u),f(1111088789u),f(1086535642u)},{f(3221225472u),f(1065353216u),f(1056964608u)},false});
m.push_back({{f(3267785990u),f(3225699889u),f(3260730102u),f(1095994657u),f(1119598133u),f(1111532028u)},{f(1056964608u),f(1056964608u),f(3221225472u)},false});
m.push_back({{f(3261484726u),f(3238346452u),f(3244807079u),f(1115296959u),f(1107909493u),f(1117232169u)},{f(1065353216u),f(1077936128u),f(1065353216u)},true});
std::array<float,16> root{f(1053898964u),f(3203627564u),f(0u),f(0u),f(3206054135u),f(3215933386u),f(0u),f(0u),f(0u),f(0u),f(1063019230u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3243862478u,3270008002u,3256579886u,1096378830u,1122524354u,1109096238u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3246168699u),f(3259409114u),f(3257651530u),f(1098908606u),f(1117744595u),f(1118018478u)},{f(3221225472u),f(0u),f(1077936128u)},true});
std::array<float,16> root{f(1056240880u),f(1060504599u),f(0u),f(0u),f(1055596018u),f(3207747174u),f(0u),f(0u),f(0u),f(0u),f(3214033665u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3245561987u,3249852835u,3277182505u,1098078339u,1102369187u,1129698857u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3266518972u),f(3267370235u),f(3264918508u),f(1109224267u),f(1119498916u),f(1112568846u)},{f(1065353216u),f(0u),f(1056964608u)},false});
m.push_back({{f(3257231801u),f(3256090301u),f(3261299063u),f(1119051907u),f(1111332746u),f(1100498421u)},{f(1056964608u),f(3212836864u),f(1056964608u)},false});
std::array<float,16> root{f(3216521028u),f(1040852853u),f(0u),f(0u),f(1060379105u),f(3213277722u),f(0u),f(0u),f(0u),f(0u),f(3211088093u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3267456903u,3239121293u,3253203108u,1119973255u,1091637645u,1105719460u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3264287582u),f(3260987974u),f(3241741550u),f(1119975300u),f(1109616369u),f(1118983658u)},{f(0u),f(1077936128u),f(3212836864u)},false});
m.push_back({{f(3263973829u),f(3266903835u),f(3266408010u),f(1120092005u),f(1050623985u),f(1108976617u)},{f(1065353216u),f(3212836864u),f(3221225472u)},true});
m.push_back({{f(3245889789u),f(3261731899u),f(3267449918u),f(1114847926u),f(1111349535u),f(1103929271u)},{f(1056964608u),f(3212836864u),f(0u)},false});
std::array<float,16> root{f(3208687268u),f(3205096335u),f(0u),f(0u),f(3202762617u),f(1044927523u),f(0u),f(0u),f(0u),f(0u),f(3216116664u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3257470906u,3260615384u,3274757199u,1109987258u,1113131736u,1127273551u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3243580621u),f(3256569025u),f(3230905949u),f(1113546889u),f(1109336531u),f(1118436186u)},{f(0u),f(0u),f(3221225472u)},true});
m.push_back({{f(3264300320u),f(3257403358u),f(3225634083u),f(1114997596u),f(1115029438u),f(1118483662u)},{f(3221225472u),f(3221225472u),f(3221225472u)},false});
m.push_back({{f(3263518019u),f(3257020743u),f(3266723586u),f(1110259878u),f(1115046852u),f(1113302066u)},{f(3221225472u),f(3212836864u),f(0u)},false});
m.push_back({{f(3240063877u),f(3263608891u),f(3256787749u),f(1110715311u),f(1120262470u),f(1112576863u)},{f(3221225472u),f(0u),f(1077936128u)},true});
std::array<float,16> root{f(3160051455u),f(1062280777u),f(0u),f(0u),f(3206940858u),f(3203635681u),f(0u),f(0u),f(0u),f(0u),f(1068109639u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{0u,0u,3275158550u,0u,0u,1127674902u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3263741028u),f(3267161889u),f(3256761336u),f(1119143977u),f(1119137363u),f(1119197534u)},{f(1056964608u),f(3212836864u),f(3212836864u)},false});
m.push_back({{f(3264855979u),f(3265935043u),f(3231151135u),f(1117349618u),f(1092294403u),f(1119787070u)},{f(3212836864u),f(3212836864u),f(0u)},false});
m.push_back({{f(3265066693u),f(3267082326u),f(3265985257u),f(1073314368u),f(1118576292u),f(1118580276u)},{f(3221225472u),f(1077936128u),f(1077936128u)},true});
m.push_back({{f(3258796257u),f(3255559095u),f(3224515798u),f(1119306643u),f(1101068387u),f(1086642932u)},{f(1056964608u),f(1056964608u),f(1077936128u)},false});
m.push_back({{f(3260975910u),f(3256295742u),f(3266829610u),f(1098751040u),f(1107969683u),f(1118706557u)},{f(3221225472u),f(3221225472u),f(1065353216u)},false});
std::array<float,16> root{f(1064703543u),f(3205001667u),f(0u),f(0u),f(1060862658u),f(1067661169u),f(0u),f(0u),f(0u),f(0u),f(1049502542u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3270551516u,3284238346u,3264144921u,1123067868u,1136754698u,1116661273u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3264642166u),f(3249809113u),f(3254464788u),f(1111675844u),f(1109893586u),f(1118732510u)},{f(1077936128u),f(1065353216u),f(3212836864u)},false});
std::array<float,16> root{f(3217243493u),f(3205931285u),f(0u),f(0u),f(3198760947u),f(3216850897u),f(0u),f(0u),f(0u),f(0u),f(3220624043u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3281198774u,3273459322u,3269774184u,1133715126u,1125975674u,1122290536u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3257058194u),f(3260248527u),f(3264058886u),f(1103704726u),f(1119744708u),f(1113855395u)},{f(3221225472u),f(1056964608u),f(3221225472u)},true});
m.push_back({{f(3265821767u),f(3247335003u),f(3263758476u),f(1103340845u),f(1117529214u),f(1114937043u)},{f(1056964608u),f(0u),f(1077936128u)},false});
std::array<float,16> root{f(1050790225u),f(1059965686u),f(0u),f(0u),f(3205825046u),f(1068457634u),f(0u),f(0u),f(0u),f(0u),f(1070225009u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3257484523u,3233679552u,3276409659u,1110000875u,1086195904u,1128926011u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3267746776u),f(3261362802u),f(3262040598u),f(1093796168u),f(1110050793u),f(1101915020u)},{f(1056964608u),f(1056964608u),f(0u)},false});
m.push_back({{f(3256480112u),f(3263080192u),f(3261810635u),f(1117540218u),f(1091563728u),f(1091103568u)},{f(1056964608u),f(3221225472u),f(1065353216u)},false});
m.push_back({{f(3267086085u),f(3264969920u),f(3256612273u),f(1109537239u),f(1116863809u),f(1103612686u)},{f(3212836864u),f(3212836864u),f(1065353216u)},true});
std::array<float,16> root{f(3220740813u),f(3162573971u),f(0u),f(0u),f(1060850073u),f(1055611866u),f(0u),f(0u),f(0u),f(0u),f(1072376376u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3264664610u,3255195972u,3261794414u,1117180962u,1107712324u,1114310766u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3265645375u),f(3266447281u),f(3255884948u),f(1107174821u),f(1116251133u),f(1117216869u)},{f(3212836864u),f(1065353216u),f(1065353216u)},false});
m.push_back({{f(3248328221u),f(3260980141u),f(3242680952u),f(1114985361u),f(1104322022u),f(1116089505u)},{f(3212836864u),f(1065353216u),f(1077936128u)},true});
m.push_back({{f(3258343611u),f(3254193685u),f(3247698867u),f(1111505045u),f(1115531385u),f(1112850188u)},{f(0u),f(1065353216u),f(1077936128u)},false});
m.push_back({{f(3265293970u),f(3231481491u),f(3259590477u),f(1112262696u),f(1093039609u),f(1098249414u)},{f(1077936128u),f(1065353216u),f(1077936128u)},false});
std::array<float,16> root{f(3207914234u),f(1059706838u),f(0u),f(0u),f(1033822816u),f(1064627221u),f(0u),f(0u),f(0u),f(0u),f(3208797811u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3254678188u,3242550888u,3266654565u,1107194540u,1095067240u,1119170917u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3264768621u),f(3266044646u),f(3260827424u),f(1113364587u),f(1115843412u),f(1109525180u)},{f(1077936128u),f(1065353216u),f(0u)},true});
m.push_back({{f(3260006256u),f(3266962898u),f(3244670682u),f(1116902111u),f(1116441408u),f(1107747175u)},{f(1056964608u),f(1056964608u),f(1056964608u)},false});
m.push_back({{f(3267541359u),f(3259344837u),f(3252835032u),f(1112634250u),f(1113744995u),f(1116251415u)},{f(1077936128u),f(3221225472u),f(0u)},false});
m.push_back({{f(3263741548u),f(3257060986u),f(3237420782u),f(1119991532u),f(1106031343u),f(1118258333u)},{f(3212836864u),f(3212836864u),f(1065353216u)},true});
m.push_back({{f(3267139435u),f(3260955254u),f(3265901267u),f(1119613065u),f(1108114936u),f(1116305974u)},{f(3212836864u),f(3221225472u),f(3221225472u)},false});
std::array<float,16> root{f(3186475254u),f(1060298462u),f(0u),f(0u),f(3193297562u),f(3214192617u),f(0u),f(0u),f(0u),f(0u),f(1063955741u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3256525112u,3259467058u,3257368170u,1109041464u,1111983410u,1109884522u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3241812290u),f(3267743549u),f(3263662339u),f(1120029702u),f(1117185263u),f(1112769043u)},{f(3212836864u),f(1056964608u),f(3212836864u)},false});
std::array<float,16> root{f(1067354429u),f(3157302186u),f(0u),f(0u),f(1045474699u),f(1033136845u),f(0u),f(0u),f(0u),f(0u),f(3210435465u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3261722468u,3228544894u,3259941024u,1114238820u,1081061246u,1112457376u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3263265494u),f(3266680463u),f(3264952854u),f(1097270420u),f(1103943557u),f(1116270595u)},{f(3212836864u),f(3221225472u),f(1056964608u)},false});
m.push_back({{f(3256870100u),f(3264588421u),f(3240257891u),f(1119521943u),f(1102768094u),f(1119470595u)},{f(3221225472u),f(0u),f(3221225472u)},true});
std::array<float,16> root{f(1072260078u),f(3186655980u),f(0u),f(0u),f(3212589098u),f(1059155987u),f(0u),f(0u),f(0u),f(0u),f(3221181877u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3279091102u,3246022491u,3276638235u,1131607454u,1098538843u,1129154587u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3213696063u),f(3267027687u),f(3254833045u),f(1109236504u),f(1109869479u),f(1116602681u)},{f(3212836864u),f(3221225472u),f(1056964608u)},true});
m.push_back({{f(3267186130u),f(3254582096u),f(3263370174u),f(1112105144u),f(1113953749u),f(1103737279u)},{f(1065353216u),f(1077936128u),f(3212836864u)},false});
m.push_back({{f(3263698716u),f(3254502277u),f(3250505588u),f(1116964676u),f(1109435709u),f(1119298042u)},{f(0u),f(3221225472u),f(3212836864u)},false});
std::array<float,16> root{f(3182647475u),f(1054290426u),f(0u),f(0u),f(1043876772u),f(1063597841u),f(0u),f(0u),f(0u),f(0u),f(3209137296u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3249835830u,3271663237u,3248547064u,1102352182u,1124179589u,1101063416u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3251949698u),f(3255016619u),f(3256246973u),f(1100826881u),f(1119818021u),f(1118696034u)},{f(1056964608u),f(1077936128u),f(0u)},false});
m.push_back({{f(3261243938u),f(3265837054u),f(3267854031u),f(1112855026u),f(1105714874u),f(1116066419u)},{f(1056964608u),f(1065353216u),f(0u)},false});
m.push_back({{f(3264636347u),f(3263888983u),f(3202113846u),f(1081799304u),f(1109267050u),f(1119971307u)},{f(3212836864u),f(0u),f(3221225472u)},true});
m.push_back({{f(3256681054u),f(3266809781u),f(3257162674u),f(1112016884u),f(1100398427u),f(1116960682u)},{f(1077936128u),f(0u),f(3221225472u)},false});
std::array<float,16> root{f(3219940287u),f(3181876010u),f(0u),f(0u),f(3202835948u),f(3191163708u),f(0u),f(0u),f(0u),f(0u),f(1069173402u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3264355885u,3226413548u,3272433212u,1116872237u,1078929900u,1124949564u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3253766392u),f(3239502739u),f(3264256526u),f(1109042158u),f(1116886087u),f(1111389612u)},{f(1056964608u),f(0u),f(0u)},false});
m.push_back({{f(3267569736u),f(3244379429u),f(3251008603u),f(1115914809u),f(1099610173u),f(1098461437u)},{f(1065353216u),f(1056964608u),f(1077936128u)},true});
m.push_back({{f(3259043212u),f(3251398454u),f(3221378953u),f(1087566440u),f(1119248788u),f(1118690807u)},{f(1065353216u),f(1065353216u),f(0u)},false});
m.push_back({{f(3238863467u),f(3256170054u),f(3262082543u),f(1110673842u),f(1107978669u),f(1104298056u)},{f(1056964608u),f(0u),f(3221225472u)},false});
m.push_back({{f(3265951032u),f(3265971793u),f(3251051068u),f(1119469357u),f(1117228509u),f(1103499271u)},{f(1077936128u),f(1065353216u),f(1065353216u)},true});
std::array<float,16> root{f(3217997858u),f(1054429729u),f(0u),f(0u),f(3204390734u),f(1072679012u),f(0u),f(0u),f(0u),f(0u),f(3205125645u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3287012788u,3280219322u,3254971683u,1139529140u,1132735674u,1107488035u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3199168771u),f(3254726352u),f(3258266207u),f(1120059530u),f(1114897077u),f(1115426550u)},{f(0u),f(3212836864u),f(1056964608u)},true});
std::array<float,16> root{f(3199688052u),f(1064844353u),f(0u),f(0u),f(1041666233u),f(3219323467u),f(0u),f(0u),f(0u),f(0u),f(1069117720u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3235547564u,3265575204u,3256675278u,1088063916u,1118091556u,1109191630u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3249672001u),f(3263776875u),f(3263933333u),f(1092684760u),f(1117414956u),f(1105244754u)},{f(3212836864u),f(1065353216u),f(1077936128u)},false});
m.push_back({{f(3260493083u),f(3250782861u),f(3266560509u),f(1066328543u),f(1095403270u),f(1117529352u)},{f(3221225472u),f(1077936128u),f(1077936128u)},false});
std::array<float,16> root{f(1057455635u),f(1048560474u),f(0u),f(0u),f(1060427556u),f(3220018312u),f(0u),f(0u),f(0u),f(0u),f(3212987911u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3258611312u,3272511995u,3279960902u,1111127664u,1125028347u,1132477254u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3249875987u),f(3263384105u),f(3250239419u),f(1113597982u),f(1107232960u),f(1113318311u)},{f(1056964608u),f(0u),f(1065353216u)},false});
m.push_back({{f(3266577187u),f(3258272432u),f(3265319716u),f(1096564519u),f(1103522365u),f(1119521449u)},{f(1056964608u),f(1056964608u),f(1056964608u)},true});
m.push_back({{f(3250183665u),f(3267540134u),f(3236861687u),f(1112475207u),f(1074324952u),f(1118958263u)},{f(1056964608u),f(3212836864u),f(3221225472u)},false});
std::array<float,16> root{f(3216961183u),f(3210904965u),f(0u),f(0u),f(1058874145u),f(3206425603u),f(0u),f(0u),f(0u),f(0u),f(1038550294u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3252649689u,3255248453u,3231497285u,1105166041u,1107764805u,1084013637u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3263856487u),f(3254901384u),f(3263669995u),f(1120398465u),f(1100363946u),f(1107937476u)},{f(3212836864u),f(3212836864u),f(1065353216u)},true});
m.push_back({{f(3255559324u),f(3265442982u),f(3243646371u),f(1089776999u),f(1085573048u),f(1110035454u)},{f(1065353216u),f(1077936128u),f(0u)},false});
m.push_back({{f(3252481934u),f(3258205760u),f(3267038387u),f(1102139139u),f(1117715428u),f(1117606376u)},{f(3221225472u),f(0u),f(0u)},false});
m.push_back({{f(3259126208u),f(3243381674u),f(3254426909u),f(1101813711u),f(1118933592u),f(1118976805u)},{f(1056964608u),f(3221225472u),f(1056964608u)},true});
std::array<float,16> root{f(3206774263u),f(1052265506u),f(0u),f(0u),f(3204623569u),f(1071727891u),f(0u),f(0u),f(0u),f(0u),f(3216736296u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3197697744u,3255126127u,3265568781u,1050214096u,1107642479u,1118085133u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3233935938u),f(3255623530u),f(3267471619u),f(1094056856u),f(1115745517u),f(1118136870u)},{f(1056964608u),f(1065353216u),f(1065353216u)},false});
m.push_back({{f(3261494829u),f(3251915340u),f(3257178463u),f(1053992841u),f(1098606394u),f(1120378774u)},{f(1056964608u),f(0u),f(1056964608u)},false});
m.push_back({{f(3251695104u),f(3267621746u),f(3255850467u),f(1115046078u),f(1107768860u),f(1107666377u)},{f(3212836864u),f(1056964608u),f(3212836864u)},true});
m.push_back({{f(3265467183u),f(3257410895u),f(3250012988u),f(1095983472u),f(1120340769u),f(1117538819u)},{f(3212836864u),f(1077936128u),f(3212836864u)},false});
m.push_back({{f(3264872586u),f(3260270963u),f(3258521508u),f(1101907315u),f(1109511135u),f(1118054461u)},{f(1065353216u),f(3212836864u),f(1056964608u)},false});
std::array<float,16> root{f(3214220298u),f(3206829310u),f(0u),f(0u),f(3196192033u),f(3215351675u),f(0u),f(0u),f(0u),f(0u),f(3205852900u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3257585778u,3245006466u,3248636901u,1110102130u,1097522818u,1101153253u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
{std::vector<dh2::world::ModuleVisualMeshV2> m;
m.push_back({{f(3251192483u),f(3261949143u),f(3225474492u),f(1087074879u),f(1112854443u),f(1108406028u)},{f(3221225472u),f(1077936128u),f(1065353216u)},false});
std::array<float,16> root{f(1065144539u),f(1062252173u),f(0u),f(0u),f(3205641632u),f(3216982669u),f(0u),f(0u),f(0u),f(0u),f(1063159030u),f(0u),f(1148829696u),f(3277717504u),f(1113325568u),f(1065353216u)};std::array<float,6> out{};unsigned gold[]{3271497546u,3280665128u,3246945387u,1124013898u,1133181480u,1099461739u};
if(!dh2::world::module_visual_mesh_box_v2(m,root,out,e)||std::memcmp(out.data(),gold,24)){std::cerr<<"Mismatch "<<count<<" "<<e<<"\n";return 1;}++count;}
std::cout<<"Module CalcMeshBox original PASS "<<count<<"\n";}
