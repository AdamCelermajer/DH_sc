// B062/B066 diagnostic: portable, unclamped per-frame / per-phase timing (std::chrono only).
// Enabled by the DH_PERF environment variable (any non-empty value other than "0"); when off, every call is a
// single predictable branch. Output is plain text lines ("Perf ..."), one per second plus spike lines, written to stdout
// AND appended to a log file: DH_PERF_LOG=<path> (default "dh-perf.log" in the working directory).
// B066 adds: per-pass GPU-side counters (draw calls, triangles, texture binds, state changes, immediate-mode vertices,
// client-array bytes the driver may copy, culled ranges, VBO draws), a separate swap phase, and swap-interval info.
#pragma once
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace dh::foundation::perf {

enum Phase : int {
    poll, input, audio_pre, sim_update, post_update, camera, fx_update, fx_render_prep, ctext, fx_prepare, scene_build, world_draw, fx_draw, hud_ui, fx_drain, swap_present, sleep_wait, phase_count
};
inline constexpr const char* phase_names[phase_count] = {
    "poll","input","audioPre","simUpdate","postUpdate","camera","fxUpdate","fxRenderPrep","ctext","preDraw","sceneBuild","worldDraw","fxDraw","hudUi","fxDrain","swap","sleep"};
// Alias kept for the B062 call sites that marked drain+swap together.
inline constexpr Phase fx_drain_swap = fx_drain;

struct DrawCounters {
    std::uint64_t calls=0,triangles=0,ranges=0,textureUploads=0;
    std::uint64_t texBinds=0,stateChanges=0,immediateVerts=0,immediateBatches=0,clientBytes=0,culledRanges=0,vboDraws=0;
};

class FramePerf {
public:
    static FramePerf& get(){static FramePerf instance;return instance;}
    bool enabled() const noexcept {return enabled_;}
    DrawCounters& counters() noexcept {return counters_;}
    // Free-form line (e.g. GL vendor, swap interval) to stdout and the log file.
    void note(const std::string& text) {if(enabled_)emit(text);}
    // GPU frame time from timestamp queries (collected asynchronously; may lag a few frames behind).
    void record_gpu(double elapsedMs) noexcept {
        if(!enabled_)return;
        ++gpuCount_;gpuTotal_+=elapsedMs;gpuWorst_=std::max(gpuWorst_,elapsedMs);++secondGpuCount_;secondGpuTotal_+=elapsedMs;
    }
    // Charge the time since the previous mark to `phase` (and the counters accumulated since then).
    void mark(Phase phase) noexcept {
        if(!enabled_)return;
        const auto now=clock::now();
        frame_[phase]+=ms(now-last_);last_=now;
        auto& pc=phaseCounters_[phase];
        pc.calls+=counters_.calls-markCounters_.calls;pc.triangles+=counters_.triangles-markCounters_.triangles;
        pc.texBinds+=counters_.texBinds-markCounters_.texBinds;pc.stateChanges+=counters_.stateChanges-markCounters_.stateChanges;
        pc.immediateVerts+=counters_.immediateVerts-markCounters_.immediateVerts;pc.immediateBatches+=counters_.immediateBatches-markCounters_.immediateBatches;
        pc.clientBytes+=counters_.clientBytes-markCounters_.clientBytes;pc.culledRanges+=counters_.culledRanges-markCounters_.culledRanges;
        pc.vboDraws+=counters_.vboDraws-markCounters_.vboDraws;pc.textureUploads+=counters_.textureUploads-markCounters_.textureUploads;
        markCounters_=counters_;
    }
    // Call at the top of each loop iteration (after the previous frame's final mark).
    void begin_frame() {
        if(!enabled_)return;
        const auto now=clock::now();
        if(started_) finish_frame(now);
        started_=true;last_=now;frameStart_=now;frame_.fill(0.0);frameCounters_=counters_;markCounters_=counters_;
        phaseCounters_.fill(DrawCounters{});
        if(secondStart_==clock::time_point{})secondStart_=now;
    }
    // Named sub-phase probes (RAII Probe below): total ms, call count and worst call per name, printed in the summary.
    struct ProbeStat {const char* name;double total=0,worst=0;std::uint64_t calls=0;};
    void record_probe(const char* name,double elapsedMs) {
        for(auto& p:probes_)if(p.name==name||std::string_view(p.name)==name){p.total+=elapsedMs;p.worst=std::max(p.worst,elapsedMs);++p.calls;return;}
        probes_.push_back({name,elapsedMs,elapsedMs,1});
    }
    void finish() {if(enabled_&&started_){finish_frame(clock::now());started_=false;summary();}}
    // Machine-readable end-of-run statistics for budget checks (valid after finish()).
    struct Stats {std::size_t frames=0;double avg=0,p50=0,p95=0,p99=0,max=0;double workAvg=0;std::size_t over33=0;};
    const Stats& stats() const noexcept {return stats_;}
private:
    using clock=std::chrono::steady_clock;
    static double ms(clock::duration d){return std::chrono::duration<double,std::milli>(d).count();}
    FramePerf(){
        const char* v=std::getenv("DH_PERF");enabled_=v&&*v&&*v!='0';
        if(enabled_){
            const char* path=std::getenv("DH_PERF_LOG");
            file_.open(path&&*path?path:"dh-perf.log",std::ios::out|std::ios::app);
        }
    }
    void emit(const std::string& line){std::cout<<line<<'\n';if(file_.is_open()){file_<<line<<'\n';file_.flush();}}
    void finish_frame(clock::time_point now) {
        const double total=ms(now-frameStart_);
        all_.push_back(float(total));
        // "work" = everything except the pacing sleep and the swap (which blocks on vsync): the cost that scales with CPU/GPU speed.
        workTotal_+=total-frame_[sleep_wait]-frame_[swap_present];
        ++secondFrames_;secondTotal_+=total;secondWorst_=std::max(secondWorst_,total);
        for(int i=0;i<phase_count;++i){secondPhase_[i]+=frame_[i];totalPhase_[i]+=frame_[i];
            auto& s=secondCounters_[i];const auto& p=phaseCounters_[i];
            s.calls+=p.calls;s.triangles+=p.triangles;s.texBinds+=p.texBinds;s.stateChanges+=p.stateChanges;s.immediateVerts+=p.immediateVerts;
            s.immediateBatches+=p.immediateBatches;s.clientBytes+=p.clientBytes;s.culledRanges+=p.culledRanges;s.vboDraws+=p.vboDraws;s.textureUploads+=p.textureUploads;}
        ++frameIndex_;
        if(total>33.4&&spikes_<400) {
            ++spikes_;
            std::ostringstream o;o<<"Perf spike frame="<<frameIndex_<<" ms="<<total;
            for(int i=0;i<phase_count;++i)if(frame_[i]>1.0)o<<' '<<phase_names[i]<<'='<<frame_[i];
            o<<" drawCalls="<<counters_.calls-frameCounters_.calls<<" textureUploads="<<counters_.textureUploads-frameCounters_.textureUploads;
            emit(o.str());
        }
        if(ms(now-secondStart_)>=1000.0) {
            const double n=double(secondFrames_);
            std::ostringstream o;o<<"Perf second frames="<<secondFrames_<<" avgMs="<<secondTotal_/n<<" worstMs="<<secondWorst_;
            for(int i=0;i<phase_count;++i)o<<' '<<phase_names[i]<<'='<<secondPhase_[i]/n;
            std::uint64_t calls=0,tris=0;for(const auto& c:secondCounters_){calls+=c.calls;tris+=c.triangles;}
            o<<" drawCallsPerFrame="<<double(calls)/n<<" trisPerFrame="<<double(tris)/n;
            emit(o.str());
            std::ostringstream g;g<<"Perf gl";
            static constexpr Phase passes[]={world_draw,fx_draw,hud_ui};static constexpr const char* passNames[]={"world","fx","hud"};
            for(int k=0;k<3;++k){const auto& c=secondCounters_[passes[k]];
                g<<" | "<<passNames[k]<<" calls="<<double(c.calls)/n<<" tris="<<double(c.triangles)/n<<" binds="<<double(c.texBinds)/n
                 <<" states="<<double(c.stateChanges)/n<<" imVerts="<<double(c.immediateVerts)/n<<" imBatches="<<double(c.immediateBatches)/n
                 <<" clientKB="<<double(c.clientBytes)/1024.0/n<<" vboDraws="<<double(c.vboDraws)/n<<" culled="<<double(c.culledRanges)/n;}
            std::uint64_t uploads=0;for(const auto& c:secondCounters_)uploads+=c.textureUploads;
            g<<" | textureUploads="<<uploads;
            if(secondGpuCount_)g<<" | gpuMsPerFrame="<<secondGpuTotal_/double(secondGpuCount_);
            secondGpuCount_=0;secondGpuTotal_=0;
            emit(g.str());
            secondStart_=now;secondFrames_=0;secondTotal_=0;secondWorst_=0;secondPhase_.fill(0.0);secondCounters_.fill(DrawCounters{});
        }
    }
    void summary() {
        if(all_.empty())return;
        std::vector<float> sorted=all_;std::sort(sorted.begin(),sorted.end());
        auto pct=[&](double p){return sorted[std::min(sorted.size()-1,std::size_t(p*sorted.size()))];};
        double sum=0;for(float v:sorted)sum+=v;
        for(const auto& p:probes_){std::ostringstream o;o<<"Perf probe "<<p.name<<" totalMs="<<p.total<<" calls="<<p.calls<<" avgMs="<<p.total/p.calls<<" worstMs="<<p.worst<<" perFrameMs="<<p.total/sorted.size();emit(o.str());}
        const auto over=std::size_t(std::count_if(sorted.begin(),sorted.end(),[](float v){return v>33.4f;}));
        stats_={sorted.size(),sum/sorted.size(),pct(.5),pct(.95),pct(.99),sorted.back(),workTotal_/sorted.size(),over};
        std::ostringstream o;
        o<<"Perf summary frames="<<sorted.size()<<" avgMs="<<sum/sorted.size()<<" p50="<<pct(.5)<<" p95="<<pct(.95)<<" p99="<<pct(.99)<<" maxMs="<<sorted.back()
         <<" workMs="<<workTotal_/sorted.size()<<" gpuAvgMs="<<(gpuCount_?gpuTotal_/double(gpuCount_):-1.0)<<" gpuMaxMs="<<gpuWorst_;
        for(int i=0;i<phase_count;++i)o<<' '<<phase_names[i]<<'='<<totalPhase_[i]/sorted.size();
        o<<" over33ms="<<over;
        emit(o.str());
    }
    bool enabled_=false,started_=false;
    clock::time_point last_{},frameStart_{},secondStart_{};
    std::array<double,phase_count> frame_{},secondPhase_{},totalPhase_{};
    DrawCounters counters_{},frameCounters_{},markCounters_{};
    std::array<DrawCounters,phase_count> phaseCounters_{},secondCounters_{};
    std::uint64_t frameIndex_=0,secondFrames_=0,spikes_=0;
    double secondTotal_=0,secondWorst_=0,workTotal_=0;
    std::uint64_t gpuCount_=0,secondGpuCount_=0;double gpuTotal_=0,gpuWorst_=0,secondGpuTotal_=0;
    std::vector<float> all_;
    std::vector<ProbeStat> probes_;
    std::ofstream file_;
    Stats stats_{};
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
