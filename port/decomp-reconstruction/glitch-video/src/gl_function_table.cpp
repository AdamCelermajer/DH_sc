#include "glitch-video/deps/gl_function_table.hpp"

#ifdef __ANDROID__
#include <GLES2/gl2.h>
#endif

namespace glitch::video::deps {

#ifdef __ANDROID__
namespace {

// The table is the port's indirection over the GL calls the original makes directly (it resolves them through the EGL/GLES libraries the
// shared object links). On Android every entry is bound to the GLES2 function of the same meaning; all argument types are 32-bit integers,
// floats or pointers, which the AArch64 calling convention passes identically, so the function pointers are reinterpreted. The two entries
// whose size parameters are pointer-sized in GLES2 (glBufferData, glBufferSubData) go through wrappers.
#define BIND(field, function) t.field = reinterpret_cast<decltype(t.field)>(function)

void bindGLES2(GLFunctionTable& t) {
    BIND(enable, glEnable);
    BIND(disable, glDisable);
    BIND(blendFunc, glBlendFunc);
    BIND(blendEquation, glBlendEquation);
    BIND(blendColor, glBlendColor);
    BIND(clearColor, glClearColor);
    BIND(createProgram, glCreateProgram);
    BIND(deleteProgram, glDeleteProgram);
    BIND(useProgram, glUseProgram);
    BIND(cullFace, glCullFace);
    BIND(depthFunc, glDepthFunc);
    BIND(depthMask, glDepthMask);
    BIND(depthRangef, glDepthRangef);
    BIND(clearDepthf, glClearDepthf);
    BIND(frontFace, glFrontFace);
    BIND(lineWidth, glLineWidth);
    BIND(polygonOffset, glPolygonOffset);
    BIND(sampleCoverage, glSampleCoverage);
    BIND(stencilFunc, glStencilFunc);
    BIND(stencilMask, glStencilMask);
    BIND(stencilOp, glStencilOp);
    BIND(clearStencil, glClearStencil);
    BIND(colorMask, glColorMask);
    BIND(scissor, glScissor);
    BIND(flush, glFlush);
    BIND(finish, glFinish);
    BIND(clear, glClear);
    BIND(createShader, glCreateShader);
    BIND(shaderSource, glShaderSource);
    BIND(compileShader, glCompileShader);
    BIND(deleteShader, glDeleteShader);
    BIND(getShaderiv, glGetShaderiv);
    BIND(getShaderInfoLog, glGetShaderInfoLog);
    BIND(attachShader, glAttachShader);
    BIND(linkProgram, glLinkProgram);
    BIND(getProgramiv, glGetProgramiv);
    BIND(getProgramInfoLog, glGetProgramInfoLog);
    BIND(getUniformLocation, glGetUniformLocation);
    BIND(getAttribLocation, glGetAttribLocation);
    BIND(getActiveUniform, glGetActiveUniform);
    BIND(getActiveAttrib, glGetActiveAttrib);
    BIND(getIntegerv, glGetIntegerv);
    BIND(getFloatv, glGetFloatv);
    BIND(hint, glHint);
    BIND(pixelStorei, glPixelStorei);
    BIND(getString, glGetString);
    BIND(getError, glGetError);
    BIND(bindBuffer, glBindBuffer);
    BIND(genBuffers, glGenBuffers);
    BIND(deleteBuffers, glDeleteBuffers);
    BIND(vertexAttribPointer, glVertexAttribPointer);
    BIND(vertexAttrib4f, glVertexAttrib4f);
    BIND(viewport, glViewport);
    BIND(activeTexture, glActiveTexture);
    BIND(bindTexture, glBindTexture);
    BIND(readPixels, glReadPixels);
    BIND(bindFramebuffer, glBindFramebuffer);
    BIND(enableVertexAttribArray, glEnableVertexAttribArray);
    BIND(disableVertexAttribArray, glDisableVertexAttribArray);
    BIND(drawElements, glDrawElements);
    BIND(drawArrays, glDrawArrays);
    BIND(genRenderbuffers, glGenRenderbuffers);
    BIND(deleteRenderbuffers, glDeleteRenderbuffers);
    BIND(genTextures, glGenTextures);
    BIND(deleteTextures, glDeleteTextures);
    BIND(texParameteri, glTexParameteri);
    BIND(texParameterf, glTexParameterf);
    BIND(texImage2D, glTexImage2D);
    BIND(texSubImage2D, glTexSubImage2D);
    BIND(compressedTexImage2D, glCompressedTexImage2D);
    BIND(compressedTexSubImage2D, glCompressedTexSubImage2D);
    BIND(generateMipmap, glGenerateMipmap);
    BIND(copyTexSubImage2D, glCopyTexSubImage2D);
    BIND(genFramebuffers, glGenFramebuffers);
    BIND(framebufferTexture2D, glFramebufferTexture2D);
    BIND(framebufferRenderbuffer, glFramebufferRenderbuffer);
    BIND(checkFramebufferStatus, glCheckFramebufferStatus);
    BIND(bindRenderbuffer, glBindRenderbuffer);
    BIND(renderbufferStorage, glRenderbufferStorage);
    BIND(uniform1f, glUniform1f);
    BIND(uniform1i, glUniform1i);
    BIND(uniform1iv, glUniform1iv);
    BIND(uniform2iv, glUniform2iv);
    BIND(uniform3iv, glUniform3iv);
    BIND(uniform4iv, glUniform4iv);
    BIND(uniform1fv, glUniform1fv);
    BIND(uniform2fv, glUniform2fv);
    BIND(uniform3fv, glUniform3fv);
    BIND(uniform4fv, glUniform4fv);
    BIND(uniformMatrix4fv, glUniformMatrix4fv);
    t.bufferData = [](uint32_t target, int32_t size, const void* data, uint32_t usage) { glBufferData(target, static_cast<GLsizeiptr>(size), data, usage); };
    t.bufferSubData = [](uint32_t target, int32_t offset, int32_t size, const void* data) {
        glBufferSubData(target, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
    };
}

#undef BIND

}  // namespace
#endif

GLFunctionTable& activeGLFunctionTable() {
    static GLFunctionTable table = [] {
        GLFunctionTable t;
#ifdef __ANDROID__
        bindGLES2(t);
#endif
        return t;
    }();
    return table;
}

}  // namespace glitch::video::deps
