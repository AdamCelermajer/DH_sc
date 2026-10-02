#ifdef GL_FRAGMENT_PRECISION_HIGH
	// define maxp based on user preference
	#ifdef GLITCH_USE_HIGHP
		#define MAXP highp
	#else
		#define MAXP mediump
	#endif
#else
	// force maxp as medium
	#define MAXP mediump

	// don't use bias, even if the user specify it, unless he/she explicitly force it
	#ifdef GLITCH_FORCE_USE_BIAS
		#ifndef GLITCH_USE_BIAS
			#define GLITCH_USE_BIAS
		#endif
	#else
		#ifdef GLITCH_USE_BIAS
			#undef GLITCH_USE_BIAS
		#endif
	#endif
#endif

#ifdef GLITCH_USE_BIAS
	#define TEXTURE2D(sampler, coord, bias) texture2D(sampler, coord, bias)
#else
	#define TEXTURE2D(sampler, coord, bias) texture2D(sampler, coord)
#endif

#ifdef TEXTURED
uniform lowp sampler2D Sampler0;
varying MAXP vec2 vTexCoord0;
uniform mediump float sampler0_bias;
#endif
#ifdef MULTITEXTURED
uniform lowp sampler2D Sampler1;
varying MAXP vec2 vTexCoord1;
uniform lowp float envmapIntensity;
uniform mediump float sampler1_bias;
#endif //MULTITEXTURED
#ifdef LIGHTMAP
varying MAXP vec2 vTexCoord2;
uniform lowp sampler2D Sampler2;
uniform mediump float sampler2_bias;
#endif

#ifdef FOG
uniform lowp vec4 fogcolor;
varying lowp float fogFactor;
#endif //FOG

varying lowp vec4 vColor0;
uniform lowp float AlphaRef;

void main()
{
	lowp vec4 color = vColor0;
#ifdef TEXTURED
	color *= TEXTURE2D(Sampler0, vTexCoord0, sampler0_bias);
#endif
#ifdef LIGHTMAP
	color *= TEXTURE2D(Sampler2, vTexCoord2, sampler2_bias);
#endif

#ifdef ALPHATEST
	if (color.a <= AlphaRef)
	{
		discard;
	}
#endif

#ifdef MULTITEXTURED
	color.rgb += TEXTURE2D(Sampler1, vTexCoord1, sampler1_bias).rgb * envmapIntensity;
#endif //MULTITEXTURED	

#ifdef FOG
    color.xyz = mix(color.xyz, fogcolor.xyz, fogFactor);
#ifdef ADDITIVEBLEND
    color.xyz *= (1.0 - fogFactor);
#endif	
#endif //FOG
	gl_FragColor = color;
}
