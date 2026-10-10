// B062 diagnostic: portable, unclamped per-frame / per-phase timing (std::chrono only).
// Enabled by the DH_PERF environment variable (any non-empty value other than "0"); when off, every call is a
// single predictable branch. Output is plain text lines on stdout ("Perf ..."), one per second plus spike lines.
#pragma once
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string_view>
#include <vector>

namespace dh::foundation::perf {

enum Phase : int {
    poll, input, audio_pre, sim_update, post_update, camera, fx_update, fx_render_prep, ctext, fx_prepare, scene_build, world_draw, fx_draw, hud_ui, fx_drain_swap, sleep_wait, phase_count
};
inline constexpr const char* phase_names[phase_count] = {
    "poll","input","audioPre","simUpdate","postUpdate","camera","fxUpdate","fxRenderPrep","ctext","preDraw","sceneBuild","worldDraw","fxDraw","hudUi","drainSwap","sleep"};

struct DrawCounters { std::uint64_t calls=0,triangles=0,ranges=0,textureUploads=0; };

class FramePerf {
public:
    static FramePerf& get(){static FramePerf instance;return instance;}
    bool enabled() const noexcept {return enabled_;}
    DrawCounters& counters() noexcept {return counters_;}
    // Charge the time since the previous mark to `phase`.
    void mark(Phase phase) noexcept {
        if(!enabled_)return;
        const auto now=clock::now();
        frame_[phase]+=ms(now-last_);last_=now;
    }
    // Call at the top of each loop iteration (after the previous frame's final mark).
    void begin_frame() {
        if(!enabled_)return;
        const auto now=clock::now();
        if(started_) finish_frame(now);
        started_=true;last_=now;frameStart_=now;frame_.fill(0.0);frameCounters_=counters_;
        if(secondStart_==clock::time_point{})secondStart_=now;
    }
    // Named sub-phase probes (RAII Probe below): total ms, call count and worst call per name, printed in the summary.
    struct ProbeStat {const char* name;double total=0,worst=0;std::uint64_t calls=0;};
    void record_probe(const char* name,double elapsedMs) {
        for(auto& p:probes_)if(p.name==name||std::string_view(p.name)==name){p.total+=elapsedMs;p.worst=std::max(p.worst,elapsedMs);++p.calls;return;}
        probes_.push_back({name,elapsedMs,elapsedMs,1});
    }
    void finish() {if(enabled_&&started_){finish_frame(clock::now());started_=false;summary();}}
private:
    using clock=std::chrono::steady_clock;
    static double ms(clock::duration d){return std::chrono::duration<double,std::milli>(d).count();}
    FramePerf(){const char* v=std::getenv("DH_PERF");enabled_=v&&*v&&*v!='0';}
    void finish_frame(clock::time_point now) {
        const double total=ms(now-frameStart_);
        all_.push_back(float(total));
        ++secondFrames_;secondTotal_+=total;secondWorst_=std::max(secondWorst_,total);
        for(int i=0;i<phase_count;++i){secondPhase_[i]+=frame_[i];totalPhase_[i]+=frame_[i];}
        secondCalls_+=counters_.calls-frameCounters_.calls;secondTris_+=counters_.triangles-frameCounters_.triangles;
        ++frameIndex_;
        if(total>33.4&&spikes_<400) {
            ++spikes_;
            std::cout<<"Perf spike frame="<<frameIndex_<<" ms="<<total;
            for(int i=0;i<phase_count;++i)if(frame_[i]>1.0)std::cout<<' '<<phase_names[i]<<'='<<frame_[i];
            std::cout<<" drawCalls="<<counters_.calls-frameCounters_.calls<<'\n';
        }
        if(ms(now-secondStart_)>=1000.0) {
            std::cout<<"Perf second frames="<<secondFrames_<<" avgMs="<<secondTotal_/secondFrames_<<" worstMs="<<secondWorst_;
            for(int i=0;i<phase_count;++i)std::cout<<' '<<phase_names[i]<<'='<<secondPhase_[i]/secondFrames_;
            std::cout<<" drawCallsPerFrame="<<secondCalls_/secondFrames_<<" trisPerFrame="<<secondTris_/secondFrames_<<'\n';
            secondStart_=now;secondFrames_=0;secondTotal_=0;secondWorst_=0;secondPhase_.fill(0.0);secondCalls_=secondTris_=0;
        }
    }
    void summary() {
        if(all_.empty())return;
        std::vector<float> sorted=all_;std::sort(sorted.begin(),sorted.end());
        auto pct=[&](double p){return sorted[std::min(sorted.size()-1,std::size_t(p*sorted.size()))];};
        double sum=0;for(float v:sorted)sum+=v;
        for(const auto& p:probes_)std::cout<<"Perf probe "<<p.name<<" totalMs="<<p.total<<" calls="<<p.calls<<" avgMs="<<p.total/p.calls<<" worstMs="<<p.worst<<" perFrameMs="<<p.total/sorted.size()<<'\n';
        std::cout<<"Perf summary frames="<<sorted.size()<<" avgMs="<<sum/sorted.size()<<" p50="<<pct(.5)<<" p95="<<pct(.95)<<" p99="<<pct(.99)<<" maxMs="<<sorted.back();
        for(int i=0;i<phase_count;++i)std::cout<<' '<<phase_names[i]<<'='<<totalPhase_[i]/sorted.size();
        std::cout<<" over33ms="<<std::count_if(sorted.begin(),sorted.end(),[](float v){return v>33.4f;})<<'\n';
    }
    bool enabled_=false,started_=false;
    clock::time_point last_{},frameStart_{},secondStart_{};
    std::array<double,phase_count> frame_{},secondPhase_{},totalPhase_{};
    DrawCounters counters_{},frameCounters_{};
    std::uint64_t frameIndex_=0,secondFrames_=0,secondCalls_=0,secondTris_=0,spikes_=0;
    double secondTotal_=0,secondWorst_=0;
    std::vector<float> all_;
    std::vector<ProbeStat> probes_;
};

} // namespace dh::foundation::perf

namespace dh::foundation::perf {
// RAII sub-phase probe: DH_PROBE("name") times the enclosing scope when DH_PERF is on (one branch otherwise).
class Probe {
public:
    explicit Probe(const char* name) noexcept : name_(FramePerf::get().enabled() ? name : nullptr) {
        if(name_)start_=std::chrono::steady_clock::now();
    }
    ~Probe() {
        if(name_)FramePerf::get().record_probe(name_,std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-start_).count());
    }
    Probe(const Probe&)=delete;Probe& operator=(const Probe&)=delete;
private:
    const char* name_;
    std::chrono::steady_clock::time_point start_{};
};
} // namespace dh::foundation::perf
#define DH_PROBE_CAT2(a,b) a##b
#define DH_PROBE_CAT(a,b) DH_PROBE_CAT2(a,b)
#define DH_PROBE(name) ::dh::foundation::perf::Probe DH_PROBE_CAT(dh_probe_,__LINE__)(name)
