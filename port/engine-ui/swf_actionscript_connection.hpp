#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
namespace gameswf {struct player;struct root;struct fn_call;struct as_object;}
namespace dh2::ui {
// Retain the exact graph, including the player during shared-movie loading.
// Root is supplied only after load_file has returned its genuine root.
struct SwfAsLease {
 std::shared_ptr<void> owner;
 gameswf::player* player{};
 gameswf::root* root{};
};
class SwfAsGraph;
class SwfAsValue {
public:
 enum class Kind {undefined,null_value,boolean,number,text,object,property};
 SwfAsValue()=default;
 static SwfAsValue boolean(bool);
 static SwfAsValue number(double);
 static SwfAsValue text(const std::string&);
 static SwfAsValue null();
 Kind kind() const noexcept;
 std::uintptr_t identity() const noexcept;
private:
 struct Pin;std::shared_ptr<Pin> pin_;
 friend class SwfAsGraph;
};
struct SwfAsServices {
 void* context{};
 // Every operation, including native dispatch, must be inside the owning
 // facade's core Scope. Provider owner must be independent of the graph owner
 // and must not own this graph/its handles, which would create a lease cycle.
 std::shared_ptr<void> owner;
 bool (*within_scope)(void*,const void* graph_owner){};
 bool (*native_call)(void*,const char*,const gameswf::fn_call&,std::string&){};
 void (*failure)(void*,const std::string&){};
};
class SwfAsGraph {
public:
 SwfAsGraph()=default;~SwfAsGraph();
 SwfAsGraph(const SwfAsGraph&)=delete;SwfAsGraph& operator=(const SwfAsGraph&)=delete;
 bool bind(SwfAsLease,const SwfAsServices&,std::string&);
 bool attach_root(gameswf::root*,std::string&);
 void release() noexcept;
 bool install_native(const std::vector<std::string>& names,std::string&);
 bool root_value(SwfAsValue&,std::string&);
 bool global_value(SwfAsValue&,std::string&);
 // Borrow only during the same facade Scope; callers retain the value/graph.
 bool borrow_object(const SwfAsValue&,gameswf::as_object*&,std::string&);
 bool retain_object(gameswf::as_object*,SwfAsValue&,std::string&);
 // Actual member lookup and setter/watch execution. Member absence and the
 // setter's boolean are source results, separate from delivery. Some source
 // setters return true while leaving read-only members unchanged.
 bool find_target(const SwfAsValue&,const char*,SwfAsValue&,std::string&);
 bool get_member(const SwfAsValue&,const char*,SwfAsValue&,bool& found,std::string&);
 bool set_member(const SwfAsValue&,const char*,const SwfAsValue&,bool& accepted,std::string&);
 bool to_number(const SwfAsValue&,double&,std::string&);
 bool to_boolean(const SwfAsValue&,bool&,std::string&);
 bool to_text(const SwfAsValue&,std::string&,std::string&);
 // Environment sprite and this receiver are deliberately separate. Original
 // RenderFX uses a sprite parent's environment for non-sprite receivers.
 // Arguments retain their native tags/objects and use original reverse stack
 // order. No native callback flattens passed objects into numbers or JSON.
 bool invoke(const SwfAsValue& environment_sprite,const SwfAsValue& receiver,
             const char* name,const std::vector<SwfAsValue>& arguments,
             SwfAsValue& result,bool& callable,std::string&);
private:
 struct State;std::shared_ptr<State> state_;
 friend struct SwfAsValue::Pin;
 bool enter(std::shared_ptr<State>&,std::string&) const;
 bool value_ok(const std::shared_ptr<State>&,const SwfAsValue&,std::string&) const;
 SwfAsValue retain(const std::shared_ptr<State>&,const void*) const;
};
}
