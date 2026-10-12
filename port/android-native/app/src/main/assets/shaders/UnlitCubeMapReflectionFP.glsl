#ifdef VERTEXCOLOR
varying lowp vec4 vColor0;
#endif

#ifdef TEXTURED
varying mediump vec2 vCoord0;
uniform lowp sampler2D Texture;
#endif

varying mediump vec3 vCoord1;

uniform lowp samplerCube EnvTexture;
uniform lowp float EnvIntensity; 

void main(void)
{
#if defined(VERTEXCOLOR) || defined(TEXTURED)
	lowp vec4 env = vec4(textureCube(EnvTexture, vCoord1).rgb, 0.0);
#    if defined(VERTEXCOLOR) && defined(TEXTURED)
	gl_FragColor = env * EnvIntensity + texture2D(Texture, vCoord0) * vColor0;
#    elif defined(VERTEXCOLOR)
	gl_FragColor = env * EnvIntensity + vColor0;
#    else //defined(TEXTURED)
	gl_FragColor = env * EnvIntensity + texture2D(Texture, vCoord0);
#    endif
#else
	gl_FragColor = vec4(textureCube(EnvTexture, vCoord1).rgb * EnvIntensity, 1.0);
#endif
}
