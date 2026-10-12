#include "glitch-video/deps/gl_function_table.hpp"

namespace glitch::video::deps {

GLFunctionTable& activeGLFunctionTable() {
    static GLFunctionTable table;
    return table;
}

}  // namespace glitch::video::deps
