bool SwfMovie::display_source_stage_clip_v5(const char* path,std::string&e){
 auto owner=impl_;auto connection=viewport_;if(!owner||!connection){e="Required source panel viewport connection";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){e="SWF core busy";return false;}
 auto* clip=owner->find(path);if(!clip){e="SWF clip not found";return false;}
 float r[4];if(!connection->display_rectangle(r,e))return false;
 const auto& v=connection->state().viewport;const auto& stage=owner->root->m_def->m_frame_size;
 if(v[2]<=0||v[3]<=0||!(r[1]>r[0])||!(r[3]>r[2])){e="Invalid source panel projection";return false;}
 const float sx=float(v[2])/(r[1]-r[0]),sy=float(v[3])/(r[3]-r[2]);
 const int left=std::max(v[0],int(std::ceil(v[0]+(stage.m_x_min-r[0])*sx)));
 const int right=std::min(v[0]+v[2],int(std::floor(v[0]+(stage.m_x_max-r[0])*sx)));
 const int top=std::max(v[1],int(std::ceil(v[1]+(stage.m_y_min-r[2])*sy)));
 const int bottom=std::min(v[1]+v[3],int(std::floor(v[1]+(stage.m_y_max-r[2])*sy)));
 if(right<=left||bottom<=top){e="Authored panel stage outside source viewport";return false;}
 // Derive the cropped logical rectangle from integer pixel edges. This keeps
 // the identical source-to-screen transform even at fractional stage edges.
 owner->begin_display(owner->root->m_background_color,left,top,right-left,bottom-top,
  r[0]+float(left-v[0])/sx,r[0]+float(right-v[0])/sx,
  r[2]+float(top-v[1])/sy,r[2]+float(bottom-v[1])/sy);
 clip->display();owner->end_display();return owner->finish(e);
}
