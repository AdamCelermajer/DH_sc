#include "core-platform/stream_ops.hpp"

// 0x30fdd0 _ZlsRSoRKN6glitch4core8vector3dIfEE
// Partial: the separator is written as one ',' here. The listing shows a
// one-character separator literal that is not recoverable from the stripped
// binary, so the character is an assumption.
std::ostream& operator<<(std::ostream& os, const glitch::core::vector3d<float>& v) {
    os << static_cast<double>(v.X) << ',';
    os << static_cast<double>(v.Y) << ',';
    os << static_cast<double>(v.Z);
    return os;
}

// 0x30ff48 _ZlsRSoRKN6glitch4core8vector3dIiEE
// Partial: same separator assumption as the float overload.
std::ostream& operator<<(std::ostream& os, const glitch::core::vector3d<int>& v) {
    os << v.X << ',' << v.Y << ',' << v.Z;
    return os;
}

// 0x310178 _ZlsRSoRKN6glitch5video6SColorE
// Partial: the channels are written as raw bytes (put, not a number). The
// listing's two separator writes carry no visible literal, so none are written.
std::ostream& operator<<(std::ostream& os, const glitch::video::SColor& c) {
    os.put(static_cast<char>(c.R));
    os.put(static_cast<char>(c.G));
    os.put(static_cast<char>(c.B));
    return os;
}

// 0x3133e0 _ZlsRSoRKSt6vectorI7Point3DIfESaIS2_EE
// Partial: the element separator is a multi-character literal that is not
// recoverable from the listing; ", " is used here. Each element is written as
// three floats with ',' between them, like the vector3d overload.
std::ostream& operator<<(std::ostream& os, const std::vector<Point3D<float>>& v) {
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i != 0) {
            os << ", ";
        }
        os << static_cast<double>(v[i].X) << ',' << static_cast<double>(v[i].Y) << ','
           << static_cast<double>(v[i].Z);
    }
    return os;
}

// 0x31026c _ZrsRSiRN6glitch4core11dimension2dIiEE
// Reads Width, skips one separator character, then reads Height.
std::istream& operator>>(std::istream& is, glitch::core::dimension2d<int>& d) {
    int width = 0;
    int height = 0;
    char sep = 0;
    is >> width;
    is >> sep;
    is >> height;
    d.Width = width;
    d.Height = height;
    return is;
}

// 0x3102b4 _ZrsRSiRN6glitch5video6SColorE
// Reads three integers, each followed by a separator character, and stores the
// low byte of each. The alpha channel is set to zero.
std::istream& operator>>(std::istream& is, glitch::video::SColor& c) {
    int r = 0;
    int g = 0;
    int b = 0;
    char sep = 0;
    is >> r;
    is >> sep;
    is >> g;
    is >> sep;
    is >> b;
    c.A = 0;
    c.R = static_cast<unsigned char>(r);
    c.G = static_cast<unsigned char>(g);
    c.B = static_cast<unsigned char>(b);
    return is;
}

// 0x31032c _ZrsRSiRN6glitch4core8vector3dIiEE
// Reads X, skips a separator, reads Y, skips a separator, reads Z.
std::istream& operator>>(std::istream& is, glitch::core::vector3d<int>& v) {
    char sep = 0;
    is >> v.X;
    is >> sep;
    is >> v.Y;
    is >> sep;
    is >> v.Z;
    return is;
}

// 0x310380 _ZrsRSiRN6glitch4core8vector3dIfEE
// Same reading pattern as the integer overload, with float components.
std::istream& operator>>(std::istream& is, glitch::core::vector3d<float>& v) {
    char sep = 0;
    is >> v.X;
    is >> sep;
    is >> v.Y;
    is >> sep;
    is >> v.Z;
    return is;
}
