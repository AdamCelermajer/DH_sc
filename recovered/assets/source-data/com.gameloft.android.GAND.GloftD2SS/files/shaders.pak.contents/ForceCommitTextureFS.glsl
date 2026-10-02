#define TEXTURE_2D 0
#define TEXTURE_3D 1
#define TEXTURE_CUBE_MAP 2
//#define TEXTURE_RECT 3

#ifndef TEXTURE_TYPE
#    define TEXTURE_TYPE TEXTURE_2D
#endif

#if GL_ES
#    if TEXTURE_TYPE == TEXTURE_3D && !defined(GL_OES_texture_3D)
#        undef TEXTURE_TYPE
#        define TEXTURE_TYPE TEXTURE_2D
// #    elif TEXTURE_TYPE == TEXTURE_RECT
// #        undef TEXTURE_TYPE
// #        define TEXTURE_TYPE TEXTURE_2D
#    endif
#endif

#if TEXTURE_TYPE == TEXTURE_3D
#    if GL_ES
#        extension GL_OES_texture_3D : enable
#    endif
uniform lowp    sampler3D texture;
#elif TEXTURE_TYPE == TEXTURE_CUBE_MAP
uniform lowp    samplerCube texture;
// #elif TEXTURE_TYPE == TEXTURE_RECT
// #    if GL_ES
// #        error texture rectangle not supported with GLES2
// #    else
// #        extension GL_ARB_texture_rectangle : enable
// #    endif
// uniform lowp    sampler2DRect texture;
#else
uniform lowp    sampler2D texture;
#endif

#if TEXTURE_TYPE == TEXTURE_3D || TEXTURE_TYPE == TEXTURE_CUBE_MAP
varying mediump vec3 vCoord0;
#else
varying mediump vec2 vCoord0;
#endif

void main()
{
#if TEXTURE_TYPE == TEXTURE_3D
    gl_FragColor = texture3D(texture, vCoord0);
#elif TEXTURE_TYPE == TEXTURE_CUBE_MAP
    gl_FragColor = textureCube(texture, vCoord0);
// #elif TEXTURE_TYPE == TEXTURE_RECT
    // gl_FragColor = texture2DRect(texture, vCoord0);
#else
    gl_FragColor = texture2D(texture, vCoord0);
#endif
}
