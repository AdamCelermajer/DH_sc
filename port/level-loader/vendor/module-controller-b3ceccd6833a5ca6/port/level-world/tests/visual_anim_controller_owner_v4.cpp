#include "../visual_anim_controller_owner_v4.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::world;
struct Root {
 std::vector<VisualAnimatorBorrowV4> animators;
 std::vector<int>* trace{};
 ~Root(){trace->push_back(4);}
 static bool get(void* p,const std::vector<VisualAnimatorBorrowV4>*& out,std::string&){auto& r=*static_cast<Root*>(p);r.trace->push_back(1);out=&r.animators;return true;}
 static bool remove(void* p,std::string&){static_cast<Root*>(p)->trace->push_back(2);return true;}
};
struct Animator {
 unsigned calls{};VisualTimelineCallbackV4 complete{};VisualEventCallbackV4 event{};void* user{};
 static bool set(void* p,VisualTimelineCallbackV4 c,void* cu,VisualEventCallbackV4 e,void* eu,std::string&){auto& a=*static_cast<Animator*>(p);assert(cu==eu);a.complete=c;a.event=e;a.user=cu;++a.calls;return true;}
};
int main(){
 std::string error;std::vector<int> trace;
 std::shared_ptr<void> visual_root=std::make_shared<Root>();auto* root=static_cast<Root*>(visual_root.get());root->trace=&trace;
 std::weak_ptr<void> weak=visual_root;std::shared_ptr<void> parent_membership=visual_root;
 auto controller=std::make_unique<VisualAnimControllerOwnerV4>();
 assert(controller->construct(visual_root,reinterpret_cast<std::uintptr_t>(root),{root,Root::get,Root::remove},false,error));assert(trace==std::vector<int>{1});
 assert(controller->root_identity()==reinterpret_cast<std::uintptr_t>(root));
 // Exact VisualD1 first deletes controller, then parent remove/drop, then own
 // root drop. No detached refcounter dictates this same allocation's lifetime.
 controller.reset();trace.push_back(3);assert(!weak.expired());parent_membership.reset();assert(!weak.expired());visual_root.reset();assert(weak.expired());assert(trace==std::vector<int>({1,3,4}));
 trace.clear();visual_root=std::make_shared<Root>();root=static_cast<Root*>(visual_root.get());root->trace=&trace;
 Animator animator;root->animators={{0,nullptr,nullptr},{42,&animator,Animator::set}};
 controller=std::make_unique<VisualAnimControllerOwnerV4>();assert(controller->construct(visual_root,reinterpret_cast<std::uintptr_t>(root),{root,Root::get,Root::remove},false,error));
 assert(animator.calls==1&&animator.user==controller.get());animator.complete(7,animator.user);animator.event(nullptr,animator.user);
 assert(!controller->construct(visual_root,reinterpret_cast<std::uintptr_t>(root),{root,Root::get,Root::remove},false,error));controller.reset();visual_root.reset();
 trace.clear();visual_root=std::make_shared<Root>();root=static_cast<Root*>(visual_root.get());root->trace=&trace;
 controller=std::make_unique<VisualAnimControllerOwnerV4>();assert(controller->construct(visual_root,reinterpret_cast<std::uintptr_t>(root),{root,Root::get,Root::remove},true,error));assert(trace==std::vector<int>{2});controller.reset();visual_root.reset();
 trace.clear();visual_root=std::make_shared<Root>();root=static_cast<Root*>(visual_root.get());root->trace=&trace;
 controller=std::make_unique<VisualAnimControllerOwnerV4>();assert(!controller->construct(visual_root,reinterpret_cast<std::uintptr_t>(root),{},false,error));
 weak=visual_root;visual_root.reset();assert(!weak.expired());controller.reset();assert(weak.expired());
 std::cout<<"AnimController SAME root lifetime/callback/remove/failure-prefix PASS\n";
}
