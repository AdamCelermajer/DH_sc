#include "../authored_scene_subtree_v2.hpp"
#include <cassert>
#include <iostream>
int main(){using namespace dh2;scene::Scene full;scene::Node a;a.id="root-node";a.name="chosen";full.graph.push_back(a);scene::Node b;b.id="chosen-node";b.parent=0;b.translation[0]=12;b.scale[0]=3;full.graph.push_back(b);scene::Node c;c.id="child-node";c.parent=1;c.translation[1]=7;full.graph.push_back(c);scene::Node d;d.id="other-node";d.parent=0;full.graph.push_back(d);scene::Instance i{};i.node_index=2;full.instances.push_back(i);scene::Instance j{};j.node_index=3;full.instances.push_back(j);std::string error;scene::Scene selected;bool found=false;
 assert(world::authored_scene_subtree_v2(full,"chosen",selected,found,error)&&found);assert(selected.graph.size()==2&&selected.instances.size()==1&&selected.graph[0].id=="chosen-node"&&selected.graph[0].parent==-1&&selected.graph[1].parent==0);assert(selected.graph[0].translation[0]==0&&selected.graph[0].scale[0]==1&&selected.graph[1].translation[1]==7&&selected.instances[0].node_index==1);
 assert(world::authored_scene_subtree_v2(full,"absent",selected,found,error)&&!found&&selected.graph.empty());assert(!world::authored_scene_subtree_v2(full,"",selected,found,error));std::cout<<"Authored subtree source ID/suffix/DFS/TRS-reset/remap/miss PASS\n";
}
