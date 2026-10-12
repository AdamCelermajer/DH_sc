#include "core-platform/stream_ops.hpp"

#include <cstdio>
#include <sstream>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    {
        std::ostringstream os;
        os << glitch::core::vector3d<float>{1.f, 2.5f, -3.f};
        CHECK(os.str() == "1,2.5,-3");
    }
    {
        std::ostringstream os;
        os << glitch::core::vector3d<int>{4, -5, 6};
        CHECK(os.str() == "4,-5,6");
    }
    {
        std::ostringstream os;
        os << glitch::video::SColor{'a', 'b', 'c', 0};
        CHECK(os.str() == "abc");
    }
    {
        std::ostringstream os;
        std::vector<Point3D<float>> pts{{1.f, 2.f, 3.f}, {4.f, 5.f, 6.f}};
        os << pts;
        CHECK(os.str() == "1,2,3;4,5,6");
    }
    {
        std::istringstream is("1280,720");
        glitch::core::dimension2d<int> d{0, 0};
        is >> d;
        CHECK(d.Width == 1280 && d.Height == 720);
    }
    {
        std::istringstream is("10,20,30");
        glitch::core::vector3d<int> v{0, 0, 0};
        is >> v;
        CHECK(v.X == 10 && v.Y == 20 && v.Z == 30);
    }
    {
        std::istringstream is("0.5,-1.5,2");
        glitch::core::vector3d<float> v{0, 0, 0};
        is >> v;
        CHECK(v.X == 0.5f && v.Y == -1.5f && v.Z == 2.f);
    }
    {
        std::istringstream is("255,128,7");
        glitch::video::SColor c{9, 9, 9, 9};
        is >> c;
        CHECK(c.R == 255 && c.G == 128 && c.B == 7 && c.A == 0);
    }
    std::printf("%s\n", failures ? "stream_ops: FAILED" : "stream_ops: ok");
    return failures != 0;
}
