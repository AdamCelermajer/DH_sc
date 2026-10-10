#include "intro_segments_v1.hpp"

#include <cstdlib>
#include <sstream>

namespace dh::foundation::startup {

const IntroSegment* IntroSegmentTable::segment_at(double movie_seconds) const noexcept {
    const IntroSegment* found = nullptr;
    for (const auto& s : segments) {
        if (s.start_seconds <= movie_seconds) found = &s;
        else break;
    }
    return found;
}

bool IntroSegmentTable::skippable_at(double movie_seconds) const noexcept {
    if (!loaded) return true;
    const IntroSegment* s = segment_at(movie_seconds);
    return s ? s->skippable : false;
}

bool parse_intro_segments(const std::string& text, IntroSegmentTable& table, std::string& error) {
    IntroSegmentTable out;
    std::istringstream in(text);
    std::string line;
    int lineNo = 0;
    while (std::getline(in, line)) {
        ++lineNo;
        const auto hash = line.find('#');
        if (hash != std::string::npos) line.erase(hash);
        std::istringstream ls(line);
        double start = 0.0;
        int flag = 0;
        if (!(ls >> start)) {
            if (line.find_first_not_of(" \t\r") == std::string::npos) continue;  // blank
            error = "intro segments line " + std::to_string(lineNo) + ": bad start time";
            return false;
        }
        if (!(ls >> flag) || (flag != 0 && flag != 1)) {
            error = "intro segments line " + std::to_string(lineNo) + ": skippable must be 0 or 1";
            return false;
        }
        IntroSegment seg;
        seg.start_seconds = start;
        seg.skippable = flag == 1;
        ls >> seg.name;
        if (out.segments.empty() ? start != 0.0 : start <= out.segments.back().start_seconds) {
            error = "intro segments line " + std::to_string(lineNo) + ": starts must ascend from 0";
            return false;
        }
        out.segments.push_back(std::move(seg));
    }
    if (out.segments.empty()) {
        error = "intro segments: no segments";
        return false;
    }
    out.loaded = true;
    table = std::move(out);
    return true;
}

} // namespace dh::foundation::startup
