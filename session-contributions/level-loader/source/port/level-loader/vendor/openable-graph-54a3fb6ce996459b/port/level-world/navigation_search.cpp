#include "navigation_search.hpp"
#include <algorithm>
#include <cmath>
namespace {
using namespace dh2::navigation;
float add(float a,float b){volatile float result=a+b;return result;}
bool compare(const SearchEntry& a,const SearchEntry& b){return a.priority>b.priority;}
void push(SearchEntry* heap,unsigned& count,const SearchEntry& value){
 unsigned hole=count++;while(hole){const unsigned parent=(hole-1)/2;if(!compare(heap[parent],value))break;heap[hole]=heap[parent];hole=parent;}heap[hole]=value;
}
void pop(SearchEntry* heap,unsigned& count){
 // Original SGI pop_heap chooses the right child when priorities tie, then
 // pushes the last value upward from the bottom. std::priority_queue alone
 // does not specify this observable ordering across implementations.
 const SearchEntry value=heap[--count];if(!count)return;
 unsigned hole=0,child=2;
 while(child<count){if(compare(heap[child],heap[child-1]))--child;heap[hole]=heap[child];hole=child;child=2*(child+1);}
 if(child==count){heap[hole]=heap[child-1];hole=child-1;}
 while(hole){const unsigned parent=(hole-1)/2;if(!compare(heap[parent],value))break;heap[hole]=heap[parent];hole=parent;}heap[hole]=value;
}
}
extern "C" int dh2_nav_search(SearchResult* result,const SearchRequest* request,SearchWorkspace* workspace){
 if(!result||!request||!workspace||request->reserved||result->reserved||request->external_path>1||!request->graph||!request->test)return 1;
 const auto& view=*request->graph;const auto* graph=view.graph;const auto& test=*request->test;
 if(!graph||!view.offsets||(graph->node_count&&!graph->nodes)||(graph->edge_count&&(!graph->edges||!view.edge_order))||
  !test.goal||!test.edge_valid||!test.node_valid||result->path_count>result->path_capacity||(result->path_capacity&&!result->path)||
  (workspace->node_capacity&&!workspace->nodes)||(workspace->heap_capacity&&!workspace->heap))return 1;
 if(view.offsets[0]!=0||view.offsets[graph->node_count]!=graph->edge_count)return 1;
 for(unsigned i=0;i<graph->node_count;++i){
  if(graph->nodes[i].id!=i+1||view.offsets[i]>view.offsets[i+1]||view.offsets[i+1]>graph->edge_count)return 1;
  unsigned previous=0;
  for(unsigned j=view.offsets[i];j<view.offsets[i+1];++j){
   const unsigned id=view.edge_order[j];if(!id||id>graph->edge_count)return 1;const auto& edge=graph->edges[id-1];
   if(edge.from!=i+1||!edge.to||edge.to>graph->node_count||edge.to<=previous||!std::isfinite(edge.weight)||edge.weight<0)return 1;
   previous=edge.to;
  }
 }
 // Nonnegative edge weights are the original native graph producer's contract.
 // Bounded storage checks replace legacy allocation and reject before callbacks.
 const unsigned prefix=request->external_path?result->path_count:0;
 if(workspace->node_capacity<graph->node_count||std::uint64_t(workspace->heap_capacity)<std::uint64_t(graph->edge_count)+1||
  std::uint64_t(prefix)+graph->node_count>result->path_capacity)return 2;
 result->found=result->expanded=result->edges_examined=result->candidate_relaxations=result->non_goal_enqueues=0;result->path_count=prefix;
 if(!request->start||request->start>graph->node_count)return 0;
 std::fill(workspace->nodes,workspace->nodes+graph->node_count,SearchNode{});
 workspace->nodes[request->start-1]={0,0,0,1};unsigned current=request->start,remaining=request->limit,queued=0;float distance=0;
 while(true){
  if(test.goal(test.user,current)||!remaining)break;
  ++result->expanded;
  for(unsigned at=view.offsets[current-1];at<view.offsets[current];++at){
   const unsigned id=view.edge_order[at];const auto& edge=graph->edges[id-1];++result->edges_examined;
   if(!test.goal(test.user,edge.to)&&(!test.edge_valid(test.user,id)||!test.node_valid(test.user,edge.to)))continue;
   // The original routine repeats its goal predicate here and after marking.
   // Keep those calls even when this one's return value is discarded.
   test.goal(test.user,edge.to);++result->candidate_relaxations;
   const float candidate=add(edge.weight,distance);const SearchEntry entry{id,candidate,add(candidate,0)};auto& state=workspace->nodes[edge.to-1];
   if(state.seen&&state.distance<=candidate)continue;
   state={id,entry.distance,entry.priority,1};
   if(test.goal(test.user,edge.to)){while(queued)pop(workspace->heap,queued);push(workspace->heap,queued,entry);break;}
   else{++result->non_goal_enqueues;push(workspace->heap,queued,entry);}
  }
  if(!--remaining||!queued)break;
  current=graph->edges[workspace->heap[0].edge-1].to;distance=workspace->heap[0].distance;pop(workspace->heap,queued);
 }
 result->found=bool(test.goal(test.user,current));
 if(result->found){
  while(current!=request->start){const unsigned edge=workspace->nodes[current-1].edge;result->path[result->path_count++]=edge;current=graph->edges[edge-1].from;}
 }else for(unsigned i=0;i<graph->node_count;++i)if(workspace->nodes[i].seen&&workspace->nodes[i].edge)result->path[result->path_count++]=workspace->nodes[i].edge;
 // Both reconstruction branches insert before list.begin() on every step.
 // Reverse the emitted segment and place it before an external caller prefix.
 if(result->path_count>prefix){std::reverse(result->path+prefix,result->path+result->path_count);std::rotate(result->path,result->path+prefix,result->path+result->path_count);}
 return 0;
}
