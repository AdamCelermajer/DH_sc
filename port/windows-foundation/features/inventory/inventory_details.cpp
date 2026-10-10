#include "inventory_details.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
namespace dh::foundation::inventory {
namespace {
bool prefix(const std::string& path,const char* base){return path.compare(0,std::char_traits<char>::length(base),base)==0;}
// Details is pushed as its own menu over the still-open InventorySheetMain
// (authored-actions.txt NativePushMenu "menu_InventorySheetDetails"), so the two
// full-stage background plates of the main sheet (shapes 396 at depth 34 and 436 at
// depth 177) remain visible under it. Every other main-sheet layer is still replaced.
// B056b: plate 34 is NOT kept: it draws the grey metal lip across the left column below Auto-equip (stage y 46..60) that the
// original Details page does not have (Part 1 t=372/t=512: the area around Auto-equip is dark brown, no band). Only the dark
// brown frame plate 177 stays under the Details art.
bool details_replaces_main(const std::string& path){
 if(path=="menu_InventorySheetMain/177") return false;
 return prefix(path,"menu_InventorySheetMain/");
}
// Plates 34 and 177 also carry the carved frame of the main avatar column: scroll-ornament quads at its corners
// (177 holds the outer four) and thin pillars along its sides. Those sample atlas u 0.25..0.33, v 0.20..0.43. The
// damask base (u >= 0.34) and the top button band (u >= 0.55) sample elsewhere. The original Details frame shows the
// damask base but none of the carved frame (Part 1 t=336, t=342, t=372; no ornaments or pillars in those frames).
bool carved_frame_vertex(const HudGeometryVertex& v){return v.u>=0.25f&&v.u<=0.33f&&v.v>=0.20f&&v.v<=0.43f;}
bool carved_frame_triangle(const HudGeometryVertex* t){return carved_frame_vertex(t[0])&&carved_frame_vertex(t[1])&&carved_frame_vertex(t[2]);}
void drop_carved_frame(std::vector<HudGeometryBatch>& batches){
 for(auto& batch:batches){
  if(batch.role!="menu_InventorySheetMain/34"&&batch.role!="menu_InventorySheetMain/177") continue;
  std::vector<HudGeometryVertex> kept;
  for(std::size_t i=0;i+2<batch.triangles.size();i+=3) if(!carved_frame_triangle(&batch.triangles[i])) kept.insert(kept.end(),batch.triangles.begin()+std::ptrdiff_t(i),batch.triangles.begin()+std::ptrdiff_t(i+3));
  batch.triangles.swap(kept);
 }
 batches.erase(std::remove_if(batches.begin(),batches.end(),[](const auto& batch){return batch.triangles.empty()&&(batch.role=="menu_InventorySheetMain/34"||batch.role=="menu_InventorySheetMain/177");}),batches.end());
}
bool contains(const std::vector<HudGeometryVertex>& triangles,float x,float y){
 auto edge=[](const auto& a,const auto& b,float xx,float yy){return (b.x-a.x)*(yy-a.y)-(b.y-a.y)*(xx-a.x);};
 for(std::size_t i=0;i+2<triangles.size();i+=3){const auto&a=triangles[i];const auto&b=triangles[i+1];const auto&c=triangles[i+2];if(std::abs(edge(a,b,c.x,c.y))<1e-6f)continue;const auto aa=edge(a,b,x,y),bb=edge(b,c,x,y),cc=edge(c,a,x,y);if((aa>=0&&bb>=0&&cc>=0)||(aa<=0&&bb<=0&&cc<=0))return true;}return false;
}
// P14 EQUIP / B045: the authored row hit list is only the bottom border sliver (3.3 units high) of each list row, so a
// click on the row body missed and fell through to main-sheet slots. A row's hit area is its visible art (the selected
// highlight and the border together), as for the other MovieClip buttons; that box is used instead of the sliver.
bool row_contains(const DetailRowArt& row, float x, float y) {
    float x0 = 0, x1 = 0, y0 = 0, y1 = 0;
    bool any = false;
    const auto grow = [&](const std::vector<HudGeometryVertex>& triangles) {
        for (const auto& v : triangles) {
            if (!any) { x0 = x1 = v.x; y0 = y1 = v.y; any = true; continue; }
            x0 = std::min(x0, v.x); x1 = std::max(x1, v.x); y0 = std::min(y0, v.y); y1 = std::max(y1, v.y);
        }
    };
    for (const auto& batch : row.unselected.batches) grow(batch.triangles);
    for (const auto& batch : row.selected.batches) grow(batch.triangles);
    if (!any) return contains(row.hit, x, y);
    return x >= x0 && x <= x1 && y >= y0 && y <= y1;
}
// Slot rail: role "menu_InventorySheetDetails/SideList/btn_TypeN/..." belongs to icon N (InvSlotId N).
bool rail_role_slot(const std::string& role,unsigned& slot){
 static const std::string base="menu_InventorySheetDetails/SideList/btn_Type";
 if(role.size()<base.size()+2||role.compare(0,base.size(),base)!=0||role[base.size()+1]!='/')return false;
 const char digit=role[base.size()];if(digit<'0'||digit>'9')return false;slot=unsigned(digit-'0');return true;
}
// Icons with authored normal-state art (SideList/btn_TypeN/<not Highlight>). Reference Part 1 t=372 and the B046 capture:
// a non-selected icon is dark (its normal art) and only the selected icon shows its bright Highlight art (CheckIcons).
std::array<bool,10> rail_normal_art(const std::vector<HudGeometryBatch>& batches){
 std::array<bool,10> normal{};
 for(const auto& batch:batches){unsigned slot=0;if(rail_role_slot(batch.role,slot)&&batch.role.find("/Highlight/")==std::string::npos)normal[slot]=true;}
 return normal;
}
// Hide a Highlight batch unless its icon is selected. Icons 0,1,2,5,6 export only their bright Highlight art (no normal
// art), so hiding it would remove the icon; those stay drawn. Their dark normal art is missing from the export (open gap).
bool rail_highlight_hidden(const std::string& role,unsigned selected,const std::array<bool,10>& normal){
 unsigned slot=0;
 return rail_role_slot(role,slot)&&role.find("/Highlight/")!=std::string::npos&&slot!=selected&&normal[slot];
}
// B056: the original Details page shows a grey equipped-item panel (upper right), an orange selected-item panel (lower
// right) and one continuous leaf-damask list panel (left). The exported v1.0.2 Details display list has none of them
// (Q-report 1.3), but their art is in the shipped textures: the vertical-gradient grey and orange blocks sit in
// MenusGraphics_droid (atlas px x 361..421 / 428..488, y 262..391 / 262..403), and the crisp damask picture is
// MenuGraphics02.tga (px x 90..473, y 150..620, mirror-tiled, see mirrored_damask). Reference:
// Part 1 t=512 (user shot b056-REFERENCE). Panels span the black dividers (shape 453: vertical x 217.5..220.2,
// horizontal y 177.4..180.1) to the stage edge; the list panel is stage x 33.5..217.5, y 67..296.
// The damask batch is drawn with the MenuGraphics02 atlas: see details_list_damask_role() and the host draw loop.
HudGeometryBatch textured_panel(const char* role,float x0,float y0,float x1,float y1,float u0,float v0,float u1,float v1){
 HudGeometryBatch batch;batch.role=role;
 batch.triangles={{x0,y0,u0,v0},{x1,y0,u1,v0},{x1,y1,u1,v1},{x0,y0,u0,v0},{x1,y1,u1,v1},{x0,y1,u0,v1}};
 return batch;
}
// Mirror-tiles the damask picture region over the list panel (0.34 stage px per texel: the reference motif pitch is ~33 stage px,
// the picture's own pitch ~85 texels), so the motif stays continuous at every tile edge.
HudGeometryBatch mirrored_damask(float x0,float y0,float x1,float y1){
 constexpr float atlas=1024.f,texel=.34f,tu0=90.f/atlas,tu1=473.f/atlas,tv0=150.f/atlas,tv1=620.f/atlas;
 const float tw=(tu1-tu0)*atlas*texel,th=(tv1-tv0)*atlas*texel;
 HudGeometryBatch batch;batch.role=details_list_damask_role();
 for(int j=0;y0+float(j)*th<y1;++j)for(int i=0;x0+float(i)*tw<x1;++i){
  const float xa=x0+float(i)*tw,xb=std::min(x1,xa+tw),ya=y0+float(j)*th,yb=std::min(y1,ya+th);
  const float fx=(xb-xa)/tw,fy=(yb-ya)/th;
  const float ua=(i&1)?tu1:tu0,ub=(i&1)?tu1-(tu1-tu0)*fx:tu0+(tu1-tu0)*fx;
  const float va=(j&1)?tv1:tv0,vb=(j&1)?tv1-(tv1-tv0)*fy:tv0+(tv1-tv0)*fy;
  batch.triangles.insert(batch.triangles.end(),{{xa,ya,ua,va},{xb,ya,ub,va},{xb,yb,ub,vb},{xa,ya,ua,va},{xb,yb,ub,vb},{xa,yb,ua,vb}});
 }
 return batch;
}
// The MenuGraphics02 picture is much darker than the original list panel (reference mean ~(44,39,27), left edge ~(28,22,14), picture
// ~(10,6,3)); the fixed-function overlay cannot tint above 1.0, so a translucent warm plate in vertical strips (darker at the rail
// side, lighter at the divider) lifts it while the motif stays visible.
// B056b: the lift is calibrated to the reference. Reference pixel means of the text-free list area (video 8:32 frame, 8 columns over the
// list width, stage x = 45.4/68.2/.../205) fall smoothly from ~(12,9,4) at the rail side to ~(52,48,37) near the divider, hue getting
// greyer to the right. The picture itself averages ~(10,6,3); each strip is drawn at alpha .55 so the leaf motif keeps ~45% of its
// own contrast (the reference motif is soft), and the strip colour is solved from target = a*colour + (1-a)*picture.
void list_damask_lift(std::vector<character_menu::MenuSolidBatch>& out){
 constexpr int strips=24;constexpr float x0=33.5f,x1=217.5f,y0=67.f,y1=296.f,alpha=.55f;
 constexpr float centres[8]={45.4f,68.2f,91.f,113.8f,136.6f,159.4f,182.2f,205.f};
 constexpr float target[3][8]={{12,22,31,38,46,51,52,49},{9,17,25,33,41,46,48,46},{4,10,16,23,28,33,37,37}};
 constexpr float picture[3]={10.f,6.f,3.f};
 const auto sample=[&](int channel,float x){
  if(x<=centres[0])return std::max(2.f,target[channel][0]+(x-centres[0])*(target[channel][1]-target[channel][0])/(centres[1]-centres[0]));
  for(int k=0;k<7;++k)if(x<=centres[k+1])return target[channel][k]+(x-centres[k])/(centres[k+1]-centres[k])*(target[channel][k+1]-target[channel][k]);
  return target[channel][7];
 };
 for(int i=0;i<strips;++i){
  const float xa=x0+(x1-x0)*float(i)/float(strips),xb=x0+(x1-x0)*float(i+1)/float(strips),mid=(xa+xb)*.5f;
  character_menu::MenuSolidBatch solid;solid.after_bitmap_role=details_list_damask_role();solid.geometry.role="menu_InventorySheetDetails/b056_list_damask_lift";
  for(int c=0;c<3;++c)solid.rgba[std::size_t(c)]=std::clamp((sample(c,mid)-(1.f-alpha)*picture[c])/alpha,0.f,255.f)/255.f;
  solid.rgba[3]=alpha;
  solid.geometry.triangles={{xa,y0,0,0},{xb,y0,0,0},{xb,y1,0,0},{xa,y0,0,0},{xb,y1,0,0},{xa,y1,0,0}};
  out.push_back(std::move(solid));
 }
}
// B056b: thin lighter outline of the list frame with the rounded outer corners (reference: rounded dark frame, top-left of the upper
// part and bottom-left of the lower part, straight edges at the orange row; Part 1 t=372/t=512). Drawn right after the damask.
void list_frame_outline(std::vector<character_menu::MenuSolidBatch>& out){
 constexpr float x0=33.5f,x1=217.5f,top=67.f,upper_bottom=177.4f,lower_top=211.f,bottom=296.f,r=6.f,w=.8f;
 const auto line=[&](float xa,float ya,float xb,float yb){
  const float dx=xb-xa,dy=yb-ya,len=std::sqrt(dx*dx+dy*dy);if(len<1e-4f)return;
  const float nx=-dy/len*w*.5f,ny=dx/len*w*.5f;
  character_menu::MenuSolidBatch solid;solid.after_bitmap_role=details_list_damask_role();solid.geometry.role="menu_InventorySheetDetails/b056b_list_frame";
  solid.rgba={.38f,.33f,.25f,.6f};
  solid.geometry.triangles={{xa+nx,ya+ny,0,0},{xb+nx,yb+ny,0,0},{xb-nx,yb-ny,0,0},{xa+nx,ya+ny,0,0},{xb-nx,yb-ny,0,0},{xa-nx,ya-ny,0,0}};
  out.push_back(std::move(solid));
 };
 const auto arc=[&](float cx,float cy,float a0){
  constexpr int steps=6;float px=cx+r*std::cos(a0),py=cy+r*std::sin(a0);
  for(int s=1;s<=steps;++s){const float a=a0+1.5707963f*float(s)/float(steps),qx=cx+r*std::cos(a),qy=cy+r*std::sin(a);line(px,py,qx,qy);px=qx;py=qy;}
 };
 line(x0+r,top,x1,top);line(x0,top+r,x0,upper_bottom);arc(x0+r,top+r,3.1415927f);
 line(x0,lower_top,x0,bottom-r);line(x0+r,bottom,x1,bottom);arc(x0+r,bottom-r,1.5707963f);
}
struct BoxF{float x0=1e9f,y0=1e9f,x1=-1e9f,y1=-1e9f;bool any()const{return x1>=x0&&y1>=y0;}};
void grow(BoxF& b,const std::vector<HudGeometryVertex>& t){for(const auto& v:t){b.x0=std::min(b.x0,v.x);b.y0=std::min(b.y0,v.y);b.x1=std::max(b.x1,v.x);b.y1=std::max(b.y1,v.y);}}
// B056b: the dark (unselected) rail icons for slots 0,1,2,5,6. The v1.0.2 export wired only the bright Highlight art for them, but
// the dark art is in the atlas: top-right of MenusGraphics_droid, the row of dim silhouettes (torso x704..746 y1..38, sword
// x750..785, shield x786..820, ring x821..851 around y 1..33, which both ring slots share). The exported dark icons of the other
// slots (e.g. Type3 boots uv x729..768 y37..75) use exactly the Highlight cell size, so each missing cell is centred on its
// silhouette with the Highlight cell size and drawn on the Highlight quad's screen rectangle.
struct RailDarkCell{unsigned slot;float cx,cy;};
constexpr RailDarkCell rail_dark_cells[]={{0,725.f,19.5f},{1,767.5f,17.5f},{2,802.8f,19.4f},{5,835.9f,15.5f},{6,835.9f,15.5f}};
void rail_dark_art(const std::vector<HudGeometryBatch>& art,const std::array<bool,10>& normal,std::vector<HudGeometryBatch>& out){
 constexpr float atlas=1024.f;
 for(const auto& cell:rail_dark_cells){
  if(normal[cell.slot])continue;
  BoxF sb;float u0=1e9f,v0=1e9f,u1=-1e9f,v1=-1e9f;bool found=false;
  for(const auto& batch:art){unsigned s=0;if(!rail_role_slot(batch.role,s)||s!=cell.slot||batch.role.find("/Highlight/")==std::string::npos)continue;
   found=true;grow(sb,batch.triangles);for(const auto& v:batch.triangles){u0=std::min(u0,v.u);u1=std::max(u1,v.u);v0=std::min(v0,v.v);v1=std::max(v1,v.v);}}
  if(!found||!sb.any())continue;
  const float w=(u1-u0)*atlas,h=(v1-v0)*atlas;
  float cu0=cell.cx-w*.5f,cu1=cell.cx+w*.5f,cv0=cell.cy-h*.5f,cv1=cell.cy+h*.5f,sy0=sb.y0;
  if(cv0<0.f){sy0+=(0.f-cv0)/h*(sb.y1-sb.y0);cv0=0.f;} // the shield/ring cells reach above the atlas edge: clip, keep the scale
  HudGeometryBatch batch;batch.role="menu_InventorySheetDetails/SideList/btn_Type"+std::to_string(cell.slot)+"/b056b_dark";
  const float a=cu0/atlas,b=cu1/atlas,c=cv0/atlas,d=cv1/atlas;
  batch.triangles={{sb.x0,sy0,a,c},{sb.x1,sy0,b,c},{sb.x1,sb.y1,b,d},{sb.x0,sy0,a,c},{sb.x1,sb.y1,b,d},{sb.x0,sb.y1,a,d}};
  out.push_back(std::move(batch));
 }
}
// B056b: equipped sword glyph of a list row: the header EquipedSwordIcon quads scaled about their centre and moved to (44.3, y+1).
void equipped_row_glyph(const std::vector<HudGeometryBatch>& art,float cy,std::vector<HudGeometryBatch>& out){
 for(const auto& batch:art){if(!prefix(batch.role,"menu_InventorySheetDetails/EquipedSwordIcon/"))continue;
  BoxF b;grow(b,batch.triangles);if(!b.any())continue;const float k=.78f,cx=(b.x0+b.x1)*.5f,mx=(b.y0+b.y1)*.5f;
  HudGeometryBatch glyph=batch;glyph.role="menu_InventorySheetDetails/list/b056b_equipped_glyph";
  for(auto& v:glyph.triangles){v.x=44.3f+(v.x-cx)*k;v.y=cy+1.f+(v.y-mx)*k;}
  out.push_back(std::move(glyph));}
}
// B056b: row separator glow. The exported separator (row art "/4", 3.3 units high) is dim and muddy; the reference (8:32) shows a thin
// light line, ~1 unit thick, peak ~(122,116,97) over a ~(50,45,30) list, strongest at the middle and fading to both ends.
void row_separator_glow(const std::vector<HudGeometryBatch>& batches,std::vector<character_menu::MenuSolidBatch>& out){
 for(const auto& batch:batches){
  const std::string& r=batch.role;if(r.size()<2||r.compare(r.size()-2,2,"/4")!=0||r.find("/list/")==std::string::npos)continue;
  BoxF b;grow(b,batch.triangles);if(!b.any())continue;
  const float cy=(b.y0+b.y1)*.5f,width=b.x1-b.x0;constexpr int segs=10;
  for(int i=0;i<segs;++i){
   const float t0=float(i)/float(segs),t1=float(i+1)/float(segs),tm=(t0+t1)*.5f,fade=std::pow(std::sin(3.1415927f*tm),.8f);
   for(int layer=0;layer<2;++layer){
    const float half=layer==0?.55f:1.3f;
    character_menu::MenuSolidBatch solid;solid.after_bitmap_role=r;solid.geometry.role="menu_InventorySheetDetails/list/b056b_separator";
    solid.rgba={.49f,.46f,.38f,(layer==0?.85f:.22f)*fade};
    const float xa=b.x0+width*t0,xb=b.x0+width*t1;
    solid.geometry.triangles={{xa,cy-half,0,0},{xb,cy-half,0,0},{xb,cy+half,0,0},{xa,cy-half,0,0},{xb,cy+half,0,0},{xa,cy+half,0,0}};
    out.push_back(std::move(solid));
   }
  }
 }
}
// B056b: VALUE box icon (item icon + gold coin stack) at the right end of the dark box; atlas cell x450..482 y480..511 (the sword
// over gold coins). Reference (video 8:32): icon ~30 x 27 stage units, right edge 2 units inside the box, bottom 19 above the end of the box art (which runs behind the Transmute button), i.e. just above the button.
HudGeometryBatch value_box_icon(const BoxF& box){
 constexpr float atlas=1024.f,w=30.f,h=27.f;
 const float x1=box.x1-2.f,y1=box.y1-19.f,x0=x1-w,y0=y1-h,u0=450.f/atlas,u1=483.f/atlas,v0=480.f/atlas,v1=512.f/atlas;
 return textured_panel("menu_InventorySheetDetails/btn_GAMEPLAYMENUS_TRANSMUTE2/b056b_value_icon",x0,y0,x1,y1,u0,v0,u1,v1);
}
void details_panel_art(std::vector<HudGeometryBatch>& out){
 constexpr float atlas=1024.f;
 out.push_back(textured_panel("menu_InventorySheetDetails/b056_grey_panel",220.2f,37.7f,480.f,177.4f,361.f/atlas,262.f/atlas,421.f/atlas,391.f/atlas));
 out.push_back(textured_panel("menu_InventorySheetDetails/b056_orange_panel",220.2f,180.1f,480.f,320.f,428.f/atlas,262.f/atlas,488.f/atlas,403.f/atlas));
 out.push_back(mirrored_damask(33.5f,67.f,217.5f,296.f));
}
// Soft ground shadow under the avatar: concentric dark ellipses clipped to the avatar pane bottom edge (pane 219.27..322.49 x 66.26..260.79).
void avatar_shadow(std::vector<character_menu::MenuSolidBatch>& out,const std::string& after){
 constexpr float cx=270.9f,cy=259.f,rx=51.f,ry=24.f,clip=260.79f,left=219.27f;
 constexpr int rings=12;constexpr int segs=28;
 for(int r=0;r<rings;++r){
  const float k=1.f-float(r)/float(rings);
  character_menu::MenuSolidBatch solid;solid.after_bitmap_role=after;solid.rgba={0.f,0.f,0.f,.055f};solid.geometry.role="menu_InventorySheetDetails/b056_avatar_shadow";
  const auto point=[&](int s){const float a=6.2831853f*float(s)/float(segs);return std::array<float,2>{std::max(left,cx+rx*k*std::cos(a)),std::min(clip,cy+ry*k*std::sin(a))};};
  for(int s=0;s<segs;++s){const auto a=point(s),b=point(s+1);solid.geometry.triangles.push_back({cx,std::min(clip,cy),0,0});solid.geometry.triangles.push_back({a[0],a[1],0,0});solid.geometry.triangles.push_back({b[0],b[1],0,0});}
  out.push_back(std::move(solid));
 }
}
const InventoryItem* owned_item(const CharacterState& owner,const std::string& id){const auto found=std::find_if(owner.inventory.begin(),owner.inventory.end(),[&](const auto& item){return item.instance_id==id;});return found==owner.inventory.end()?nullptr:&*found;}
std::size_t focus(const std::vector<equipment_menu::OwnedSelection>& rows,const std::string& selected){const auto at=std::find_if(rows.begin(),rows.end(),[&](const auto& row){return row.instance_id==selected;});return at==rows.end()?0:std::size_t(at-rows.begin());}
bool name(const DetailBindings& bindings,const CharacterState& owner,const dh2::data::ItemTable& table,const std::string& id,std::string& value,std::string& error){
 const auto* owned=owned_item(owner,id);if(!owned){error="Detail item selection no longer owned";return false;}const auto* record=dh2::data::item(table,dh2::data::item_id(table,owned->definition_id));if(!record||!bindings.item_name){error="Required original item name provider unavailable";return false;}return bindings.item_name(*owned,*record,value,error);
}
}
bool DetailsPresenter::candidates(std::vector<equipment_menu::OwnedSelection>& rows,std::string& error)const{return selection_.view_for_selected_slot(rows,error);}
bool DetailsPresenter::open(unsigned slot,std::string& error){if(!selection_.select_slot(slot,error))return false;std::vector<equipment_menu::OwnedSelection> rows;if(!candidates(rows,error))return false;if(!rows.empty()&&std::none_of(rows.begin(),rows.end(),[&](const auto& row){return row.instance_id==selection_.selected_instance();}))if(!selection_.select_instance(rows.front().instance_id,error))return false;open_=true;error.clear();return true;}
// Index of the selected row in the candidate list (GenerateInventoryListItems Index); 0 when nothing matches.
std::size_t DetailsPresenter::selected_index()const{std::vector<equipment_menu::OwnedSelection> rows;std::string error;if(!candidates(rows,error))return 0;return focus(rows,selection_.selected_instance());}
// After Drop/Transmute removed the selected row the original regenerates the list at the same Index; keep the nearest row.
bool DetailsPresenter::reselect_near(std::size_t index,std::string& error){std::vector<equipment_menu::OwnedSelection> rows;if(!candidates(rows,error))return false;if(rows.empty())return selection_.select_slot(selection_.selected_slot(),error);return selection_.select_instance(rows[std::min(index,rows.size()-1)].instance_id,error);}
bool DetailsPresenter::frame(const DetailBindings& b,character_menu::Frame& output,std::string& error)const{
 if(!open_){error="Original inventory details panel is closed";return false;}if(!b.symbol){error="Required detail StringManager symbol provider unavailable";return false;}
 std::vector<equipment_menu::OwnedSelection> rows;if(!candidates(rows,error))return false;
 const auto current=focus(rows,selection_.selected_instance());const auto& art=original_inventory_details();auto next=output;auto rail_normal=rail_normal_art(art.panel.batches);std::vector<HudGeometryBatch> rail_dark;rail_dark_art(art.panel.batches,rail_normal,rail_dark);for(const auto& batch:rail_dark){unsigned s=0;if(rail_role_slot(batch.role,s))rail_normal[s]=true;}
 std::string equipped_id,equipped_name;for(const auto& row:rows)if(row.equipped){equipped_id=row.instance_id;break;}
 next.art.batches.erase(std::remove_if(next.art.batches.begin(),next.art.batches.end(),[](const auto& batch){return details_replaces_main(batch.role)||prefix(batch.role,"menu_InventorySheetDetails/");}),next.art.batches.end());
 next.text.erase(std::remove_if(next.text.begin(),next.text.end(),[](const auto& value){return details_replaces_main(value.field.path)||prefix(value.field.path,"menu_InventorySheetDetails/");}),next.text.end());
 next.solids.erase(std::remove_if(next.solids.begin(),next.solids.end(),[](const auto& value){return details_replaces_main(value.geometry.role)||prefix(value.geometry.role,"menu_InventorySheetDetails/");}),next.solids.end());
 drop_carved_frame(next.art.batches);
// B056: grey/orange panels, list damask (below the Details art) and the avatar ground shadow (after the orange panel, before the avatar).
 details_panel_art(next.art.batches);list_damask_lift(next.solids);list_frame_outline(next.solids);next.art.batches.insert(next.art.batches.end(),rail_dark.begin(),rail_dark.end());avatar_shadow(next.solids,"menu_InventorySheetDetails/b056_orange_panel");
  const auto* transmute_variant=rows.empty()?nullptr:&(rows[current].equipped?art.text_states.transmute_disabled:art.text_states.transmute_idle);
  const bool has_transmute_variant=transmute_variant&&!transmute_variant->fields.empty();
  if(has_transmute_variant){
   bool inserted=false;
   for(const auto& batch:art.panel.batches){
    if(prefix(batch.role,"menu_InventorySheetDetails/btn_GAMEPLAYMENUS_TRANSMUTE2/")){
     if(!inserted){next.art.batches.insert(next.art.batches.end(),transmute_variant->batches.begin(),transmute_variant->batches.end());inserted=true;}
     continue;
    }
    if(equipped_id.empty()&&prefix(batch.role,"menu_InventorySheetDetails/EquipedSwordIcon/"))continue;
    if(rail_highlight_hidden(batch.role,selection_.selected_slot(),rail_normal))continue;
    next.art.batches.push_back(batch);
   }
   if(!inserted)next.art.batches.insert(next.art.batches.end(),transmute_variant->batches.begin(),transmute_variant->batches.end());
   for(const auto& solid:art.panel.solids)if(!prefix(solid.geometry.role,"menu_InventorySheetDetails/btn_GAMEPLAYMENUS_TRANSMUTE2/"))next.solids.push_back(solid);
   next.solids.insert(next.solids.end(),transmute_variant->solids.begin(),transmute_variant->solids.end());
  }else{
   for(const auto& batch:art.panel.batches){if(equipped_id.empty()&&prefix(batch.role,"menu_InventorySheetDetails/EquipedSwordIcon/"))continue;
    if(rail_highlight_hidden(batch.role,selection_.selected_slot(),rail_normal))continue;next.art.batches.push_back(batch);}
   next.solids.insert(next.solids.end(),art.panel.solids.begin(),art.panel.solids.end());
  }
 // B056b: VALUE box icon (item icon + coin stack) next to the value text, only where the idle Transmute variant draws the box.
 if(has_transmute_variant){BoxF value_box;for(const auto& batch:transmute_variant->batches)if(batch.role=="menu_InventorySheetDetails/btn_GAMEPLAYMENUS_TRANSMUTE2/4")grow(value_box,batch.triangles);if(value_box.any())next.art.batches.push_back(value_box_icon(value_box));}
 // Drop is not offered for an equipped selection: the original hides btn_Drop on the ItemEquipped path of
 // displaySelectedItemInfos (authored-actions.txt ~0001cbbe-0001cc09). Part 1 t=336 (Torso) and t=342 (Hands) are
 // equipped and show no Drop; t=372 (Feet, unequipped) shows Drop. The flag also covers the Drop label text below.
 const bool drop_available=rows.empty()||!rows[current].equipped;
 // B056b: the dark normal art of the rail icons without exported normal art is drawn from the atlas (rail_dark_art); the B056 dim plate is gone.
 if(!drop_available)next.art.batches.erase(std::remove_if(next.art.batches.begin(),next.art.batches.end(),[](const auto& batch){return prefix(batch.role,"menu_InventorySheetDetails/btn_Drop/");}),next.art.batches.end());
 std::string selected_name;if(!rows.empty()&&!name(b,owner_,table_,rows[current].instance_id,selected_name,error))return false;
 if(!equipped_id.empty()&&!name(b,owner_,table_,equipped_id,equipped_name,error))return false;
 for(const auto& field:art.panel.text_fields){std::string value;const auto& path=field.path;
  if(has_transmute_variant&&prefix(path,"menu_InventorySheetDetails/btn_GAMEPLAYMENUS_TRANSMUTE2/"))continue;
  if(!drop_available&&prefix(path,"menu_InventorySheetDetails/btn_Drop/"))continue;
  if(prefix(path,"menu_InventorySheetDetails/SelectedItemName/"))value=selected_name;
  else if(prefix(path,"menu_InventorySheetDetails/EquipedItemName/"))value=equipped_name;
  else if(prefix(path,"menu_InventorySheetDetails/category_title/")){if(!b.symbol("GAMEPLAYMENUS_CATEGORY_"+std::to_string(selection_.selected_slot()),value,error))return false;}
  else if(path.find("/player_gold/")!=std::string::npos)value=std::to_string(owner_.gold);
  else {const char* symbol=nullptr;
   if(path.find("/TotalGoldText/")!=std::string::npos)symbol="MENU_TOTAL_GOLD";
   else if(path.find("/btn_EquipItem/")!=std::string::npos)symbol="GLOBAL_EQUIP";
   else if(path.find("/btn_Unequip/")!=std::string::npos)symbol="GLOBAL_UNEQUIP";
   else if(path.find("/btn_Drop/")!=std::string::npos)symbol="GAMEPLAYMENUS_INVENTORY_DROP";
   else if(path.find("/btn_AutoEquip/")!=std::string::npos)symbol="GAMEPLAYMENUS_AUTOEQUIP";
   else if(path.find("/btn_GAMEPLAYMENUS_TRANSMUTE2/")!=std::string::npos)symbol="GAMEPLAYMENUS_TRANSMUTE2";
   if(symbol){if(!b.symbol(symbol,value,error))return false;}
   else if(b.item_details){const auto* owned=(path.find("/EquipedItem")!=std::string::npos)?owned_item(owner_,equipped_id):(!rows.empty()?owned_item(owner_,rows[current].instance_id):nullptr);
    if(owned&&(path.find("/ItemInfo")!=std::string::npos||path.find("/ItemReq")!=std::string::npos||path.find("/EquipedItemInfo")!=std::string::npos||path.find("/EquipedItemReq")!=std::string::npos))if(!b.item_details(path,*owned,value,error))return false;}
  }
  if(!value.empty())next.text.push_back({field,std::move(value)});
 }
 if(has_transmute_variant){
  const auto& selected=rows[current];
   const auto& fields=transmute_variant->fields;
  const auto* owned=owned_item(owner_,selected.instance_id);
  if(!owned){error="Detail item selection no longer owned";return false;}
  for(const auto& field:fields){std::string value;const auto& path=field.path;const char* symbol=nullptr;
   if(path=="menu_InventorySheetDetails/btn_GAMEPLAYMENUS_TRANSMUTE2/ButtonName/text")symbol="GAMEPLAYMENUS_TRANSMUTE2";
   else if(path=="menu_InventorySheetDetails/btn_GAMEPLAYMENUS_TRANSMUTE2/ValueText/value")symbol="MENU_VALUE_TITLE";
   else if(path=="menu_InventorySheetDetails/btn_GAMEPLAYMENUS_TRANSMUTE2/ValueBox/value"){
    if(b.transmute_value&&!b.transmute_value(*owned,value,error))return false;
   }
   if(symbol&&!b.symbol(symbol,value,error))return false;
   if(!value.empty())next.text.push_back({field,std::move(value)});
  }
 }
 for(const auto& row_art:art.rows){const auto index=static_cast<std::int64_t>(current)+row_art.relative_index;if(index<0||std::size_t(index)>=rows.size())continue;const auto& row=rows[std::size_t(index)];const auto* owned=owned_item(owner_,row.instance_id);if(!owned){error="Original list item disappeared";return false;}std::string title;if(!name(b,owner_,table_,row.instance_id,title,error))return false;const auto& item_art=row_art.relative_index==0?row_art.selected:row_art.unselected;
  next.art.batches.insert(next.art.batches.end(),item_art.batches.begin(),item_art.batches.end());
  next.solids.insert(next.solids.end(),item_art.solids.begin(),item_art.solids.end());
  // B056b: the equipped row shows the small sword glyph left of its name (reference 8:32 and t=372: 'Useless Blade' / 'Ceremonial Boots').
  // The exported row art has none; the glyph is the header EquipedSwordIcon art at ~78% scale, centred on the row text.
  if(row_art.relative_index!=0)row_separator_glow(item_art.batches,next.solids);
  if(row.equipped)for(const auto& field:item_art.text_fields)if(field.path.find("/Host")!=std::string::npos){const auto& m=field.matrix;const auto& lb=field.local_bounds;equipped_row_glyph(art.panel.batches,m[1]*(lb[0]+lb[1])*.5f+m[3]*(lb[2]+lb[3])*.5f+m[5],next.art.batches);}
  for(const auto& field:item_art.text_fields){if(field.path.find("/Host")!=std::string::npos)next.text.push_back({field,title});else if(field.path.find("/Number")!=std::string::npos&&details_row_shows_count(owned->quantity))next.text.push_back({field,std::to_string(owned->quantity)});}
  // Original rows show no digit for a single item (authored GenerateInventoryListItems clears Number; Part 1 t=336/t=372 show none).
  // Stacks keep their count. The equipped-row glyph that the reference shows in this field is not reproduced (see the B042 report).
 }
 output=std::move(next);error.clear();return true;
}
bool DetailsPresenter::release(float x,float y,DetailAction& action,std::string& error){
 action=DetailAction::none;if(!open_||!std::isfinite(x)||!std::isfinite(y)){error.clear();return true;}
 const auto& art=original_inventory_details();
 // Rail icon btn_TypeN: on_focus_in sets InvSlotId = N and refreshes the Details (authored-actions.txt 0001dc00-0001dc9a).
 // open() is the same refresh the main sheet slot hit uses: the slot's equipped item, else the first candidate row.
 if(const int rail=details_rail_slot_at(art,x,y);rail>=0){if(!open(unsigned(rail),error))return false;action=DetailAction::slot;error.clear();return true;}
 std::vector<equipment_menu::OwnedSelection> rows;if(!candidates(rows,error))return false;const auto current=focus(rows,selection_.selected_instance());
 // Transmute is disabled and Drop hidden by displaySelectedItemInfos for an equipped selection (authored-actions.txt 0001cbbe-0001cc44): no command is produced.
 for(const auto& hit:art.actions)if(contains(hit.triangles,x,y)){action=hit.action;if((action==DetailAction::transmute||action==DetailAction::drop)&&(rows.empty()||rows[current].equipped)){action=DetailAction::none;error.clear();return true;}
  // btn_left/btn_right: ClassChangeUp/ClassChangeDown (authored-actions.txt 0001d9ea-0001daeb): InvSlotId -1 with wrap to 9, or +1 with wrap to 0.
  if(action==DetailAction::previous||action==DetailAction::next){const unsigned slot=selection_.selected_slot();if(slot>9){action=DetailAction::none;error.clear();return true;}
   const unsigned target=action==DetailAction::previous?(slot>0?slot-1:9u):(slot<9?slot+1:0u);if(!open(target,error))return false;}
  error.clear();return true;}
 for(const auto& row:art.rows)if(row_contains(row,x,y)){const auto index=static_cast<std::int64_t>(current)+row.relative_index;if(index>=0&&std::size_t(index)<rows.size()){if(!selection_.select_instance(rows[std::size_t(index)].instance_id,error))return false;action=DetailAction::select;}error.clear();return true;}
 error.clear();return true;
}
}

namespace dh::foundation::inventory {
bool details_row_hit(const DetailRowArt& row, float x, float y) { return row_contains(row, x, y); }

bool details_rail_box(const DetailArt& art, unsigned slot, DetailRailBox& output) {
    bool any = false;
    output = {};
    for (const auto& batch : art.panel.batches) {
        unsigned batch_slot = 0;
        if (!rail_role_slot(batch.role, batch_slot) || batch_slot != slot) continue;
        for (const auto& v : batch.triangles) {
            if (!any) { output = {v.x, v.y, v.x, v.y}; any = true; continue; }
            output.x0 = std::min(output.x0, v.x); output.x1 = std::max(output.x1, v.x);
            output.y0 = std::min(output.y0, v.y); output.y1 = std::max(output.y1, v.y);
        }
    }
    return any;
}

std::vector<HudGeometryBatch> details_rail_dark_art(const DetailArt& art) {
    std::vector<HudGeometryBatch> out;
    rail_dark_art(art.panel.batches, rail_normal_art(art.panel.batches), out);
    return out;
}

int details_rail_slot_at(const DetailArt& art, float x, float y) {
    if (!std::isfinite(x) || !std::isfinite(y)) return -1;
    for (unsigned slot = 0; slot < 10; ++slot) {
        DetailRailBox box{};
        if (!details_rail_box(art, slot, box)) continue;
        if (x >= box.x0 && x <= box.x1 && y >= box.y0 && y <= box.y1) return int(slot);
    }
    return -1;
}
}  // namespace dh::foundation::inventory
