#pragma once
#include "asset_catalog.hpp"
#include <functional>
#include <map>
#include <string>
#include <vector>
namespace dh::foundation {
struct OriginalCampaignCommand {
    int kind = -1;
    std::string class_name;
    std::map<unsigned,std::uint32_t> scalars;
    std::map<unsigned,std::string> strings;
    std::map<unsigned,std::vector<std::uint32_t>> arrays;
};
struct OriginalCampaignScript { int id=-1; std::string scope,name; std::vector<OriginalCampaignCommand> commands; };
struct OriginalCampaignTrigger { std::string key; std::map<std::string,std::string> attributes; };
enum class CampaignCommandPhase { execute, is_blocking, update };
struct OriginalCampaignServices {
    // Caller supplies source PM/network start admission; denied is a normal no-op.
    std::function<bool(int script,int module,bool received,bool& admitted,std::string&)> admit_start;
    // Every non Wait/ExecScript command needs an explicit source implementation.
    // A blocking command receives is_blocking then update once per invocation.
    std::function<bool(CampaignCommandPhase,const OriginalCampaignCommand&,int module,
                       bool& blocking,std::string&)> command;
    // Source RNG for ExecScript random choice, required only for nonempty arrays.
    std::function<bool(std::uint32_t bound,std::uint32_t& result,std::string&)> random;
};
class OriginalCampaignRuntime {
public:
    bool load(const AssetCatalog&,const std::string& xml,std::string& error);
    void bind(OriginalCampaignServices services) { services_=std::move(services); }
    int script_id(const std::string& name,bool include_common=true) const;
    bool start(int global_id,int module,bool received,std::string& error);
    bool tick(std::int32_t delta_ms,std::string& error);
    // Host supplies original physical inside/condition/alive eligibility facts.
    // Keys are source file + "::" + authored trigger name; no map-specific logic.
    bool trigger_contact(const std::string& key,bool inside,bool qualified,int module,std::string& error);
    // P16 HOST: adds one authored level declaration occurrence that the campaign XML did not carry.
    // Same key format as load(); an existing key is never replaced.
    bool register_trigger(const std::string& key,const std::map<std::string,std::string>& attributes,std::string& error);
    // P16 HOST: host policy after a reported failure. Abandons every running script and clears the latched
    // failure so the session keeps its other triggers. Returns the number of scripts abandoned.
    std::size_t abandon_running_scripts();
    bool running(int global_id) const;
    bool failed() const { return !failure_.empty(); }
    const std::vector<OriginalCampaignScript>& scripts() const { return scripts_; }
    const std::map<std::string,OriginalCampaignTrigger>& triggers() const { return triggers_; }
private:
    struct Context { int state=2,module=-1;std::size_t command=0;std::int32_t elapsed=0,duration=0;int child=-1; };
    struct TriggerState { bool inside=false;std::int32_t activations=0,delay=0; };
    std::vector<OriginalCampaignScript> scripts_;
    std::vector<Context> contexts_;
    std::map<std::string,OriginalCampaignTrigger> triggers_;
    std::map<std::string,TriggerState> trigger_state_;
    std::size_t common_count_=0;
    OriginalCampaignServices services_;
    std::string failure_;
    bool ticking_=false;
    bool fail(std::string message,std::string& error);
    bool execute(std::size_t id,std::int32_t dt,std::string& error);
};
}
