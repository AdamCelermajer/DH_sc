#include "../viewport_math.hpp"

#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace f = dh::foundation;
namespace {
void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}
void near(double a, double b) { require(std::abs(a-b)<1.0e-10,"Coordinate round-trip mismatch"); }
}

int main() {
    try {
        const auto native=f::fitViewport(1280,720,1.667752385,f::ViewportFitMode::Contain);
        require(native.x==39 && native.y==0 && native.width==1201 && native.height==720,"Native authored-aspect fit");
        const auto recording=f::fitViewport(640,360,1.667752385,f::ViewportFitMode::Contain);
        require(recording.x==20 && recording.width==600 && recording.height==360,"Recording-sized authored-aspect fit");
        const auto stretch=f::fitViewport(1280,720,1.667752385,f::ViewportFitMode::Stretch);
        require(stretch.x==0 && stretch.width==1280 && stretch.height==720,"Explicit stretch must retain whole viewport");
        const auto tall=f::fitViewport(800,1200,16.0/9.0,f::ViewportFitMode::Contain);
        require(tall.x==0 && tall.y==375 && tall.width==800 && tall.height==450,"Tall host fit");
        const auto odd=f::fitViewport(1001,800,1.0,f::ViewportFitMode::Contain);
        require(odd.x==100 && odd.width==800,"Odd pillar centering");
        for (const auto bounds : {native,recording,stretch,tall,odd}) {
            for (const f::ViewportPoint source : {f::ViewportPoint{-1,1},f::ViewportPoint{1,-1},f::ViewportPoint{0,0},f::ViewportPoint{.3,-.7},f::ViewportPoint{1.4,-1.2}}) {
                f::ViewportPoint pixel,returned;
                require(f::normalizedToViewport(source,bounds,pixel),"NDC mapping rejected");
                require(f::viewportToNormalized(pixel,bounds,returned),"Pixel mapping rejected");
                near(source.x,returned.x); near(source.y,returned.y);
            }
        }
        // Projecting equal angular displacements must use fitted bounds in both
        // axes. Full-window scaling would introduce the 6.6% horizontal stretch.
        const double fitScaleRatio=(static_cast<double>(native.width)/native.height)/1.667752385;
        require(std::abs(fitScaleRatio-1.0)<0.001,"Contain must preserve angular pixel scale within integer rounding");
        const double stretchedScaleRatio=(1280.0/720.0)/1.667752385;
        require(stretchedScaleRatio>1.065 && stretchedScaleRatio<1.067,"Expected stretched projection diagnostic");
        require(f::fitViewport(0,720,1.5,f::ViewportFitMode::Contain).width==0,"Invalid host accepted");
        require(f::fitViewport(640,360,std::numeric_limits<double>::infinity(),f::ViewportFitMode::Contain).width==0,"Invalid aspect accepted");
        f::ViewportPoint unchanged{17,23};
        require(!f::normalizedToViewport({0,0},{},unchanged),"Invalid bounds accepted");
        require(unchanged.x==17 && unchanged.y==23,"Failed mapping mutated output");
        std::cout<<"Explicit viewport fitting and coordinate round trips passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr<<error.what()<<'\n';
        return 1;
    }
}
