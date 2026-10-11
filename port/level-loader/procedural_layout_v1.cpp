#include "procedural_layout_v1.hpp"
#include <algorithm>
#include <cmath>
#include <deque>
#include <stdexcept>
#include <new>
namespace dh2::loader {
namespace {
constexpr std::int8_t distributions[]{
#include "procedural_distribution_v1.inc"
};
static_assert(sizeof(distributions)==26136);
constexpr std::uint32_t opposite[]{2,3,0,1,4};
constexpr std::int32_t delta[5][2]{{0,-1},{1,0},{0,1},{-1,0},{0,0}};
constexpr std::uint32_t absent=UINT32_MAX;
struct Exit {
    std::uint32_t block{absent},index{absent};
    bool operator==(const Exit& b)const{return block==b.block&&index==b.index;}
    bool present()const{return block!=absent;}
};
struct Candidate {
    Exit exit;
    ProceduralListElementV1 element{absent,"","","",100,false};
    std::int32_t list{-1};
};
struct Tile {
    std::uint32_t block{};std::int32_t x{},y{};float height{};
        Candidate candidate;Tile* parent{};std::vector<std::unique_ptr<Tile>> children;
    // Original RemoveNeighbors/RemoveChild publishes count decrement BEFORE
    // reached recursive release. Retain this detached prefix on a checked OOM.
    std::unique_ptr<Tile> detached_cleanup;
};
struct Impl {
    const ProceduralRuleV1* source{};std::vector<std::uint32_t> path;
    std::int32_t length{},steps{},restriction{-1};std::uint32_t path_direction{4};
    std::vector<std::string> names;std::vector<std::unique_ptr<Impl>> children;
};
// SourceArray2d+8 logical storage: x/y origins, shared row width and deque rows.
// operator() ALWAYS EnsurePosition, including failed FitsInMap probes. Growth
// is columns first, then rows; removal never shrinks extents. Host deque ABI is
// not original STLport/ARM allocation byte parity.
class SourceArray2dV70 {
    std::int32_t min_x_{},min_y_{};std::size_t width_{};bool growth_failed_{};
    std::unique_ptr<std::deque<std::deque<Tile*>>> rows_=std::make_unique<std::deque<std::deque<Tile*>>>();
    static std::size_t checked(std::int64_t value){
        if(value<0||value>1000000)throw std::runtime_error("Original Array2d growth outside checked span domain");
        return static_cast<std::size_t>(value);
    }
    void area(std::size_t width,std::size_t height)const{
        if(width&&height>1000000/width)throw std::runtime_error("Original Array2d extent exceeds checked cell domain");
    }
public:
    Tile*& cell(std::int32_t x,std::int32_t y){
        if(!rows_||growth_failed_)throw std::runtime_error("Original Array2d allocation prefix unavailable");
        try {
        // Source48b578 checks x against origin and width before touching y.
        if(x<min_x_){
            const auto count=checked(std::int64_t(min_x_)-x);const auto next=checked(std::int64_t(width_)+count);
            area(next,rows_->size());
            width_=next;
            for(auto& row:*rows_)for(std::size_t i=0;i<count;++i)row.push_front(nullptr);
            min_x_=x;
        }else if(std::int64_t(x)>=std::int64_t(min_x_)+std::int64_t(width_)){
            const auto count=checked(std::int64_t(x)-min_x_-std::int64_t(width_)+1);
            const auto next=checked(std::int64_t(width_)+count);area(next,rows_->size());
            width_=next;
            for(auto& row:*rows_)for(std::size_t i=0;i<count;++i)row.push_back(nullptr);
        }
        if(y<min_y_){
            const auto count=checked(std::int64_t(min_y_)-y);area(width_,checked(std::int64_t(rows_->size())+count));
            for(std::size_t i=0;i<count;++i)rows_->push_front(std::deque<Tile*>(width_,nullptr));
            min_y_=y;
        }else if(std::int64_t(y)>=std::int64_t(min_y_)+std::int64_t(rows_->size())){
            const auto count=checked(std::int64_t(y)-min_y_-std::int64_t(rows_->size())+1);
            area(width_,checked(std::int64_t(rows_->size())+count));
            for(std::size_t i=0;i<count;++i)rows_->push_back(std::deque<Tile*>(width_,nullptr));
        }
        return rows_->at(checked(std::int64_t(y)-min_y_)).at(checked(std::int64_t(x)-min_x_));
        }catch(const std::bad_alloc&){growth_failed_=true;throw;}
    }
    bool coherent()const noexcept{return rows_&&!growth_failed_;}
    // Source487bb0 empties every row and outer deque, then origin/width zero.
    void clear()noexcept {rows_->clear();min_x_=0;min_y_=0;width_=0;}
    void destroy()noexcept {rows_.reset();min_x_=0;min_y_=0;width_=0;}
};
class Generator final:public ProceduralLayoutOwnerV70 {
    std::vector<std::string> names_;
    const std::vector<ProceduralBlockV1>* blocks_{};
    const ProceduralConnectionGraphV1* graph_{};
    std::shared_ptr<const ProceduralRulePlanV1> plan_;
    std::vector<ProceduralListDeclarationV1> lists_;
    ProceduralRandomV1 random_{0};
    ProceduralLayoutEventsV1 events_;
    SourceArray2dV70 cells_;std::unique_ptr<Tile> root_;
    ProceduralRulesV1::Borrow source_pins_;
    bool started_{},root_published_{},retiring_{},tiles_unloaded_{},rules_unloaded_{},grid_destroyed_{};
    std::uint32_t work_{},depth_{};
    void tick(){if(retiring_)return;if(++work_>1000000)throw std::runtime_error("Procedural generation work limit exceeded");}
    struct Depth {
        Generator& g;explicit Depth(Generator& v):g(v){if(++g.depth_>256)throw std::runtime_error("Procedural generation depth exceeded");}
        ~Depth(){--g.depth_;}
    };
    static std::int32_t add(std::int32_t a,std::int32_t b){
        const auto n=std::int64_t(a)+b;
        if(n<INT32_MIN||n>INT32_MAX)throw std::runtime_error("Procedural grid arithmetic exceeds int32 domain");
        return std::int32_t(n);
    }
    const ProceduralBlockV1& block(std::uint32_t index)const{return blocks_->at(graph_->selected_sources.at(index));}
    const std::string& name(std::uint32_t index)const{return names_.at(graph_->selected_sources.at(index));}
    const ProceduralExitV1& exit(Exit e)const{return block(e.block).exits.at(e.index);}
    std::int32_t find_block(const std::string& text)const{
        for(std::uint32_t i=0;i<graph_->selected_sources.size();++i)if(name(i)==text)return std::int32_t(i);
        return -1;
    }
    template<class T> void shuffle(std::vector<T>& values){
        for(std::uint32_t i=1;i<values.size();++i){++events_.random_calls;std::swap(values[i],values[random_.bounded(i+1)]);}
    }
    std::unique_ptr<Impl> initialize(const ProceduralRuleV1& rule,std::vector<std::uint32_t> path,bool parent){
        tick();ProceduralRuleInstanceV1 initial;std::string error;
        if(!initialize_procedural_rule_instance_v1(plan_,path,parent,random_,initial,error))throw std::runtime_error(error);
        if(rule.kind==ProceduralRuleKindV1::path&&!rule.id_hash&&rule.length[0]<rule.length[1])++events_.random_calls;
        auto value=std::make_unique<Impl>();value->source=&rule;value->path=std::move(path);
        value->length=initial.length;value->restriction=initial.exit_direction;value->names=std::move(initial.block_names);
        // The constructor snapshot's +0x28/+0x40 labels are not consumed.
        // Path::OneStep uses +0x28 as an integer step counter, initialized zero.
        return value;
    }
    void rectangle(Tile& tile,bool remove){
        const auto& b=block(tile.block);
        // Native PlaceInMap48ba58 and RemoveFromMap48b8f0 make no cell
        // accesses when either loop dimension is nonpositive. Such a tile is
        // still a real owned/serialized root/child, not a no-layout result.
        if(b.width<=0||b.height<=0)return;
        if(std::uint64_t(b.width)*std::uint64_t(b.height)>1000000)
            throw std::runtime_error("Procedural block footprint outside checked domain");
        // Native placement/removal: y outer, x inner. Cell access preserves
        // Array2d EnsurePosition side effects before the reached write.
        for(std::int32_t y=0;y<b.height;++y)for(std::int32_t x=0;x<b.width;++x){
            tick();auto& cell=cells_.cell(add(tile.x,x),add(tile.y,y));
            if(remove){if(cell==&tile)cell=nullptr;}
            else {if(cell)throw std::runtime_error("Original placement assertion: occupied map cell");cell=&tile;}
        }
    }
    bool fits(std::uint32_t index,std::int32_t x,std::int32_t y){
        const auto& b=block(index);
        // Native48b870/874 compares x against x+width before entering any
        // probe; a nonpositive width succeeds without querying the grid. If
        // width is positive, each x compares y against y+height likewise.
        // Preserve endpoint arithmetic checks rather than permitting native
        // signed-wrap loop behavior outside the port's existing raw domain.
        const auto end_x=add(x,b.width);if(x>=end_x)return true;
        const auto end_y=add(y,b.height);if(y>=end_y)return true;
        if(std::uint64_t(b.width)*std::uint64_t(b.height)>1000000)
            throw std::runtime_error("Procedural block footprint outside checked domain");
        // Native48b85c probes x outer/y inner. Failed probes preserve the
        // expanded grid before returning at the first occupied source cell.
        for(std::int32_t i=0;i<b.width;++i)for(std::int32_t j=0;j<b.height;++j){
            tick();if(cells_.cell(add(x,i),add(y,j)))return false;
        }
        return true;
    }
    void consume(const Candidate& candidate){
        if(candidate.list<0)return;
        auto& list=lists_.at(std::size_t(candidate.list));if(list.replacement)return;
        // Original Spawn exits the search after erasing the first matching
        // name+gameplay entry; visual is not compared.
        for(auto it=list.elements.begin();it!=list.elements.end();++it){
            if(it->block_name==candidate.element.block_name&&it->gameplay==candidate.element.gameplay){
                list.elements.erase(it);break;
            }
        }
    }
    void restore(const Candidate& candidate){
        if(candidate.list>=0){auto& list=lists_.at(std::size_t(candidate.list));if(!list.replacement)list.elements.push_back(candidate.element);}
    }
    void remove_neighbors(Tile& tile){
        // Native491928 decrements parent count before recursive removal. The
        // detached owning slot preserves that exact prefix across checked OOM.
        while(tile.detached_cleanup||!tile.children.empty()){
            if(!tile.detached_cleanup){tile.detached_cleanup=std::move(tile.children.back());tile.children.pop_back();}
            remove_neighbors(*tile.detached_cleanup);unspawn(*tile.detached_cleanup);tile.detached_cleanup.reset();
        }
    }
    void unspawn(Tile& tile){
        tick();++events_.unspawn_calls;rectangle(tile,true);remove_neighbors(tile);restore(tile.candidate);
    }
    void discard(Tile& parent,Tile* child){
        if(!parent.detached_cleanup){
            const auto found=std::find_if(parent.children.begin(),parent.children.end(),[child](const auto& p){return p.get()==child;});
            if(found==parent.children.end())throw std::runtime_error("Generated child owner unavailable");
            parent.detached_cleanup=std::move(*found);*found=std::move(parent.children.back());parent.children.pop_back();
        }else if(parent.detached_cleanup.get()!=child)throw std::runtime_error("Another generated child release prefix remains owned");
        unspawn(*parent.detached_cleanup);parent.detached_cleanup.reset();
    }
    Tile* spawn(Tile& parent,const Candidate& candidate,Exit connecting){
        tick();const auto& target=exit(candidate.exit);const auto direction=opposite[target.direction];
        auto x=add(add(parent.x,delta[direction][0]),-target.grid[0]);
        auto y=add(add(parent.y,delta[direction][1]),-target.grid[1]);float height=parent.height;
        if(connecting.present()){
            const auto& own=exit(connecting);x=add(x,own.grid[0]);y=add(y,own.grid[1]);height=height+own.height;
        }
        if(!fits(candidate.exit.block,x,y))return nullptr;
        height=height-target.height;if(!std::isfinite(height))throw std::runtime_error("Generated height outside finite domain");
        if(parent.children.size()>=8)throw std::runtime_error("Original tile child capacity exceeded");
        auto tile=std::make_unique<Tile>();tile->block=candidate.exit.block;tile->x=x;tile->y=y;tile->height=height;
        tile->candidate=candidate;tile->parent=&parent;auto* result=tile.get();parent.children.push_back(std::move(tile));
        consume(candidate);++events_.place_calls;rectangle(*result,false);return result;
    }
    void base_filter(std::vector<Candidate>& candidates){
        candidates.erase(std::remove_if(candidates.begin(),candidates.end(),[this](const auto& c){
            return name(c.exit.block).find("_start")!=std::string::npos;
        }),candidates.end());shuffle(candidates);
    }
    void add_path(Impl& impl,std::vector<Candidate>& out,Exit selected,std::size_t li){
        const auto& b=block(selected.block);
        if(b.width!=1||b.height!=1)return;
        const bool dead=add(impl.steps,1)==impl.length&&impl.source->children.empty();
        if(b.exits.size()!=(dead?1u:2u))return;
        out.push_back({selected,lists_.at(std::size_t(impl.source->list_source)).elements.at(li),impl.source->list_source});
    }
    void filter(Impl& impl,std::vector<Candidate>& candidates){
        const auto& rule=*impl.source;std::vector<Candidate> chosen;
        if(rule.kind==ProceduralRuleKindV1::path){
            if(rule.list_source>=0){
                const auto& list=lists_.at(std::size_t(rule.list_source));
                for(const auto& c:candidates){
                    if(rule.list_index!=-1){
                        if(rule.list_index<0||std::size_t(rule.list_index)>=list.elements.size())
                            throw std::runtime_error("Original Path list index outside allocated elements");
                        const auto index=std::size_t(rule.list_index);
                        if(name(c.exit.block)==list.elements[index].block_name)add_path(impl,chosen,c.exit,index);
                    }else for(std::size_t i=0;i<list.elements.size();++i)
                        if(name(c.exit.block)==list.elements[i].block_name)add_path(impl,chosen,c.exit,i);
                }
            }
        }else if(rule.kind==ProceduralRuleKindV1::force_block){
            if(rule.connect_from!=4)throw std::runtime_error("Unreconstructed forced connection index");
            for(const auto& c:candidates){
                if(rule.list_source>=0){
                    const auto& list=lists_.at(std::size_t(rule.list_source));
                    if(rule.list_index!=-1&&std::uint32_t(rule.list_index)<list.elements.size()){
                        const auto& element=list.elements[std::size_t(rule.list_index)];
                        if(element.block_name==name(c.exit.block))chosen.push_back({c.exit,element,rule.list_source});
                    }else for(const auto& element:list.elements)
                        if(element.block_name==name(c.exit.block))chosen.push_back({c.exit,element,rule.list_source});
                }else for(const auto& text:impl.names)if(text==name(c.exit.block))chosen.push_back(c);
            }
        }else if(rule.kind==ProceduralRuleKindV1::root)return;
        else chosen=candidates;
        candidates=std::move(chosen);base_filter(candidates);
    }
    std::vector<Candidate> connected(Exit own){
        std::vector<Candidate> candidates;
        for(const auto& target:graph_->connections.at(own.block).at(own.index))candidates.push_back({{target.block,target.exit},{absent,"","","",100,false},-1});
        return candidates;
    }
    bool one_step(Impl& impl,Tile& tile,Exit own,Exit incoming){
        Depth depth(*this);tick();const auto& rule=*impl.source;
        if(rule.kind==ProceduralRuleKindV1::end_path)return true;
        if(rule.kind==ProceduralRuleKindV1::path){
            ++events_.path_calls;
            if(impl.steps>=impl.length){
                Exit excluded;
                if(block(tile.block).exits.size()==2){
                    excluded={tile.block,own==Exit{tile.block,0}?1u:0u};
                }
                return step(impl,tile,excluded);
            }
            if(!own.present())throw std::runtime_error("Original Path dereferences null connecting exit");
            const auto reverse=opposite[exit(own).direction];
            if(!impl.steps&&rule.dont_go_back)impl.path_direction=reverse;
            auto available=connected(own);std::vector<Candidate> candidates;std::int32_t self=-1;
            for(std::size_t i=0;i<available.size();++i){
                if(rule.list_index==-1&&name(available[i].exit.block)==name(tile.block))self=std::int32_t(i);
                else candidates.push_back(available[i]);
            }
            filter(impl,candidates);
            if(self!=-1){
                if(rule.list_source<0)throw std::runtime_error("Original Path dereferences null self-room list");
                const auto selected=available[std::size_t(self)].exit;
                for(const auto& element:lists_.at(std::size_t(rule.list_source)).elements)
                    if(element.block_name==name(selected.block))candidates.push_back({selected,element,rule.list_source});
            }
            for(const auto& candidate:candidates){
                const auto& b=block(candidate.exit.block);
                if(impl.path_direction!=4&&std::any_of(b.exits.begin(),b.exits.end(),[&impl](const auto& e){return e.direction==impl.path_direction;})
                    &&reverse!=impl.path_direction)continue;
                auto* child=spawn(tile,candidate,own);if(!child)continue;
                if(b.exits.size()==1)return true;
                impl.steps=add(impl.steps,1);Exit next;
                if(b.exits.size()==2)next={child->block,candidate.exit.index==0?1u:0u};
                if(one_step(impl,*child,next,candidate.exit))return true;
                --impl.steps;discard(tile,child);
            }
            return false;
        }
        ++events_.rule_calls;
        if(!own.present())throw std::runtime_error("Original rule dereferences null connecting exit");
        auto candidates=connected(own);filter(impl,candidates);
        for(const auto& candidate:candidates){
            auto* child=spawn(tile,candidate,own);if(!child)continue;
            if(step(impl,*child,candidate.exit))return true;
            discard(tile,child);
        }
        (void)incoming;return false;
    }
    bool step(Impl& impl,Tile& tile,Exit incoming){
        Depth depth(*this);tick();++events_.step_calls;
        std::vector<Candidate> exits;
        for(std::uint32_t i=0;i<block(tile.block).exits.size();++i){
            const Exit e{tile.block,i};if(!(e==incoming))exits.push_back({e,{absent,"","","",100,false},-1});
        }
        shuffle(exits);const auto count=impl.source->children.size();
        if(!count||exits.empty())return true;
        if(count>=6||exits.size()>=6)throw std::runtime_error("Original distribution table indices outside captured domain");
        const auto offset=726*count+4356*exits.size();std::vector<const std::int8_t*> choices;
        std::size_t row=0;
        for(;row<121;++row){const auto* p=distributions+offset+6*row;if(p[0]<0)break;choices.push_back(p);}
        if(row==121)throw std::runtime_error("Original distribution subtable lacks bounded sentinel");
        shuffle(choices);const auto maximum=exits.size()*(exits.size()+1)/2;
        for(const auto* choice:choices){
            auto work=exits;bool success=true;
            for(std::size_t i=0;i<work.size();++i){
                if(i>=6||choice[i]<0||std::size_t(choice[i])>=count)throw std::runtime_error("Original distribution selects unavailable rule");
                if(impl.children.size()>=6)throw std::runtime_error("Original runtime rule child capacity exceeded");
                auto path=impl.path;path.push_back(std::uint32_t(choice[i]));
                auto child=initialize(impl.source->children[std::size_t(choice[i])],std::move(path),true);
                auto* active=child.get();impl.children.push_back(std::move(child));
                if(active->restriction>=0&&std::uint32_t(active->restriction)!=exit(work[i].exit).direction){
                    if(work.size()>=maximum){success=false;break;}
                    work.push_back(work[i]);
                }else if(!one_step(*active,tile,work[i].exit,incoming)){success=false;break;}
            }
            if(success)return true;
            while(!impl.children.empty())impl.children.pop_back();
            remove_neighbors(tile);
        }
        return false;
    }
    bool root(Impl& impl,std::uint32_t index,const Candidate& selected){
        tick();++events_.root_attempts;
        if(root_)throw std::runtime_error("Previous generated root prefix remains owned");
        root_=std::make_unique<Tile>();root_->block=index;root_->candidate=selected;
        ++events_.place_calls;rectangle(*root_,false);
        if(step(impl,*root_,{}))return true;
        unspawn(*root_);root_.reset();return false;
    }
    void project(const Tile& tile,ProceduralLayoutResultV1& result){
        if(result.tiles.size()>=4096)throw std::runtime_error("Generated tile count exceeds checked domain");
        const auto index=std::uint32_t(result.tiles.size());
        ProceduralLayoutTileV1 row;row.block=tile.block;row.block_source=graph_->selected_sources.at(tile.block);
        row.name=name(tile.block);row.grid={tile.x,tile.y};row.height=tile.height;
        row.list_element=tile.candidate.element;row.list_source=tile.candidate.list;result.tiles.push_back(std::move(row));
        for(const auto& child:tile.children){
            const auto child_index=std::uint32_t(result.tiles.size());project(*child,result);result.tiles[index].children.push_back(child_index);
        }
    }
public:
    bool unload_tiles(std::string& error)override {
        if(tiles_unloaded_){error.clear();return true;}
        // Original4843a0 Unspawn FIRST while block/list/rule owners are live,
        // then clear114. Checked active attempts are owned too, before114 was
        // published; do not abandon their cell/list prefix on source failure.
        if(!cells_.coherent()){error="Original Array2d allocation-failure prefix retained; native diagnostic/unwind branch unresolved";return false;}
        retiring_=true;
        try {if(root_){unspawn(*root_);root_.reset();}root_published_=false;tiles_unloaded_=true;error.clear();return true;}
        catch(const std::exception& e){error=e.what();return false;}
    }
    void unload_rule_sources()noexcept override {
        if(!tiles_unloaded_)return; // preserve referenced list/block owners
        std::vector<ProceduralListDeclarationV1> empty;lists_.swap(empty);plan_.reset();rules_unloaded_=true;
    }
    void destroy_grid()noexcept override {
        if(!tiles_unloaded_||!rules_unloaded_)return;
        cells_.destroy();source_pins_={};std::vector<std::string> empty;names_.swap(empty);grid_destroyed_=true;
    }
    bool live()const noexcept override {return !tiles_unloaded_&&!grid_destroyed_;}
    std::uintptr_t root114_identity()const noexcept override {
        return root_published_&&!tiles_unloaded_?reinterpret_cast<std::uintptr_t>(root_.get()):0;
    }
    Generator()=default; // real Array2d C1 occurs before readers/config
    Generator(const ProceduralRulePlanV1& p,const std::vector<std::string>& names,
              const std::vector<ProceduralBlockV1>& blocks,const ProceduralConnectionGraphV1& graph,std::uint32_t seed,std::uint32_t* actual_state=nullptr,ProceduralRulesV1::Borrow source={}):Generator(){
        prepare_inputs(p,names,blocks,graph,seed,actual_state,std::move(source));
    }
    void prepare_inputs(const ProceduralRulePlanV1& p,const std::vector<std::string>& names,
              const std::vector<ProceduralBlockV1>& blocks,const ProceduralConnectionGraphV1& graph,std::uint32_t seed,std::uint32_t* actual_state,ProceduralRulesV1::Borrow source){
        if(started_||tiles_unloaded_||grid_destroyed_||plan_)throw std::runtime_error("Generated body input occurrence already entered/retired");
        names_=names;blocks_=&blocks;graph_=&graph;source_pins_=std::move(source);
        plan_=std::make_shared<const ProceduralRulePlanV1>(p);lists_=p.lists.declarations;
        // Original Generate Clear precedes state+0 reseed/RootRule::Impl C1.
        cells_.clear();if(actual_state)*actual_state=seed;random_=actual_state?ProceduralRandomV1::borrow_actual_state_v68(*actual_state):ProceduralRandomV1(seed);
        if(!p.pools.empty()&&!p.room_pools_allocated_v68)throw std::runtime_error("Required original room-pool allocation before Generate seed reset");
        if(!p.lists.document||names.size()!=blocks.size()||graph.selected_sources.size()!=graph.connections.size())
            throw std::runtime_error("Procedural generation inputs unavailable/inconsistent");
        for(std::uint32_t i=0;i<graph_->selected_sources.size();++i){
            if(graph_->connections[i].size()!=block(i).exits.size())throw std::runtime_error("Connection exits inconsistent");
            for(const auto& e:block(i).exits)if(e.direction>=5||e.grid[0]==INT32_MIN||e.grid[1]==INT32_MIN)
                throw std::runtime_error("Exit arithmetic outside checked domain");
        }
    }
    ProceduralLayoutResultV1 run(std::uint32_t seed){
                if(started_||tiles_unloaded_)throw std::runtime_error("Original generated occurrence already entered; preserve failure prefix");
        started_=true;auto impl=initialize(plan_->rule,{},false);bool selected=false;
        const auto& rule=*impl->source;
        if(rule.list_source>=0){
            auto list=lists_.at(std::size_t(rule.list_source)).elements;
            if(rule.list_index!=-1&&std::uint32_t(rule.list_index)<list.size()){
                const auto element=list[std::size_t(rule.list_index)];const auto index=find_block(element.block_name);
                if(index>=0)selected=root(*impl,std::uint32_t(index),{{},element,rule.list_source});
            }else{
                shuffle(list);
                for(const auto& element:list){const auto index=find_block(element.block_name);if(index>=0){
                    selected=root(*impl,std::uint32_t(index),{{},element,rule.list_source});if(selected)break;}}
            }
        }else{
            auto values=impl->names;shuffle(values);
            for(const auto& text:values){const auto index=find_block(text);if(index>=0){selected=root(*impl,std::uint32_t(index),{});if(selected)break;}}
        }
        ProceduralLayoutResultV1 result;result.seed=seed;result.generated=bool(selected);result.final_state=random_.state();result.events=events_;
        root_published_=selected;if(selected)project(*root_,result);
        return result;
    }
};
}
bool generate_procedural_layout_v1(const ProceduralRulePlanV1& plan,
    const std::vector<std::string>& names,const std::vector<ProceduralBlockV1>& blocks,
    const ProceduralConnectionGraphV1& graph,std::uint32_t seed,ProceduralLayoutResultV1& out,std::string& error){
    try{Generator generator(plan,names,blocks,graph,seed);auto candidate=generator.run(seed);
        out=std::move(candidate);error.clear();return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
bool generate_procedural_layout_v1(ProceduralRulesV1::Borrow source,std::uint32_t seed,ProceduralLayoutResultV1& out,std::string& error){
    try{
        if(!source)throw std::runtime_error("Procedural generation source owner unavailable");
        const auto& connections=source.lists().connections();const auto& blocks=connections.blocks();
        std::vector<std::string> names;for(const auto& candidate:blocks.sources().blocks())names.push_back(candidate.name);
        ProceduralLayoutResultV1 next;
        if(!generate_procedural_layout_v1(source.plan(),names,blocks.blocks(),connections.graph(),seed,next,error))return false;
        next.source_owner=std::move(source);out=std::move(next);error.clear();return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
bool generate_procedural_layout_v68(ProceduralRulesV1::Borrow source,std::uint32_t seed,
 std::uint32_t& actual_generator_state,ProceduralLayoutResultV1& out,std::string& error){
 try {
  if(!source)throw std::runtime_error("Procedural generation source owner unavailable");
  const auto& connections=source.lists().connections();const auto& blocks=connections.blocks();
  std::vector<std::string> names;for(const auto& candidate:blocks.sources().blocks())names.push_back(candidate.name);
  // Source488154/15c writes seed+4 and state+0 before RootRule::Impl C1.
  actual_generator_state=seed;
  Generator generator(source.plan(),names,blocks.blocks(),connections.graph(),seed,&actual_generator_state);
  auto next=generator.run(seed);next.source_owner=std::move(source);out=std::move(next);error.clear();return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
bool create_procedural_layout_body_v70(std::shared_ptr<ProceduralLayoutOwnerV70>& out,std::string& error){
 if(out){error="Original generated body already constructed; retain its actual prefix";return false;}
 try {out=std::make_shared<Generator>();error.clear();return true;}
 catch(const std::exception& e){error=e.what();return false;}
}
bool generate_procedural_layout_v70(ProceduralRulesV1::Borrow source,std::uint32_t seed,
 std::uint32_t& actual_generator_state,std::shared_ptr<ProceduralLayoutOwnerV70>& actual_body,
 ProceduralLayoutResultV1& out,std::string& error){
 try {
  if(!source||!actual_body||!actual_body->live())throw std::runtime_error("Required SAME original generated body/source owners");
  // This slot is produced only by create_procedural_layout_body_v70 inside
  // SAME generator C1. No Main-supplied alternate layout implementation.
  auto body=std::static_pointer_cast<Generator>(actual_body);
  const auto& connections=source.lists().connections();const auto& blocks=connections.blocks();
  std::vector<std::string> names;for(const auto& candidate:blocks.sources().blocks())names.push_back(candidate.name);
  body->prepare_inputs(source.plan(),names,blocks.blocks(),connections.graph(),seed,&actual_generator_state,source);
  auto next=body->run(seed);next.source_owner=std::move(source);out=std::move(next);error.clear();return true;
 }catch(const std::exception& e){error=e.what();return false;}
 // The actual body remains in generator14c on success and on any reached
 // failure. Row projection neither destroys nor becomes its live Tile graph.
}
}




