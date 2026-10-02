//#define AL //Alpha
//#define AT //Alpha Test
//#define LM //Light Map
//#define SP //Specular Map
//#define FG //Fog

#define LIGHTING
#define VC	//Vertex Color

#ifdef AL_LM
	#define AL
	#define LM
#endif

#ifdef AL_SP
	#define AL
	#define SP
#endif

#ifdef AL_FG
	#define AL
	#define FG
#endif

#ifdef AL_SP_LM
	#define AL
	#define SP
	#define LM
#endif

#ifdef AL_SP_FG
	#define AL
	#define SP
	#define FG
#endif

#ifdef AL_LM_FG
	#define AL
	#define LM
	#define FG
#endif

#ifdef AL_SP_LM_FG
	#define AL
	#define SP
	#define LM
	#define FG
#endif

#ifdef AT_LM
	#define AT
	#define LM
#endif

#ifdef AT_SP
	#define AT
	#define SP
#endif

#ifdef AT_FG
	#define AT
	#define FG
#endif

#ifdef AT_SP_LM
	#define AT
	#define SP
	#define LM
#endif

#ifdef AT_SP_FG
	#define AT
	#define SP
	#define FG
#endif

#ifdef AT_LM_FG
	#define AT
	#define LM
	#define FG
#endif

#ifdef AT_SP_LM_FG
	#define AT
	#define SP
	#define LM
	#define FG
#endif

#ifdef SP_LM
	#define SP
	#define LM
#endif

#ifdef LM_FG
	#define LM
	#define FG
#endif

#ifdef SP_LM_FG
	#define SP
	#define LM
	#define FG
#endif

#ifdef SP_FG
	#define SP
	#define FG
#endif

#if defined AL || defined AT
	#define ALPHA_MAP
#endif

varying mediump vec4 vTexCoord0;

#if defined VC || defined LIGHTING
	varying lowp vec4 vColor;
#endif

#ifdef SP
	//varying mediump vec2 vAttNdotHV;
	varying mediump float vAttNdotHV;
#endif

#ifdef FG
	varying lowp float vFogFactor;
#endif

uniform lowp sampler2D DiffuseSampler;

#ifdef ALPHA_MAP
	uniform lowp sampler2D AlphaSampler;
#endif

#ifdef AL
	//uniform lowp float Object_Alpha;
#endif

#ifdef SP
	uniform lowp sampler2D SpecularSampler;
	uniform lowp vec4 Light0Specularcolor;
	//uniform lowp float Spec_Saturation;
	//uniform lowp float Spec_Level;
	//uniform mediump float Glossiness;
#endif

#ifdef LM
	uniform lowp sampler2D LightMapSampler;
#endif

#ifdef FG
	uniform lowp vec4 fogcolor;   
#endif

 // main procedure, the original name was f
void main()
{
	lowp vec4 Diffuse = texture2D(DiffuseSampler, vTexCoord0.xy);

#ifdef ALPHA_MAP
	Diffuse.w = texture2D(AlphaSampler, vTexCoord0.xy).z;
#endif	
	
#ifdef AT
	if (Diffuse.w < 0.8) discard;	
#endif

#if defined VC || defined LIGHTING
	lowp vec4 color = vColor;
	color *= Diffuse;
#else
	lowp vec4 color = Diffuse;
#endif

#ifdef LM
	color *= texture2D(LightMapSampler, vTexCoord0.zw);
#endif	

#ifdef SP
	lowp float Specular = texture2D(SpecularSampler, vTexCoord0.xy).z;
	//lowp float OutGrey = dot(color.xyz, vec3(0.3, 0.59, 0.11));
	//lowp vec3 Blend = mix(vec3(OutGrey, OutGrey, OutGrey), color.xyz, Spec_Saturation);
	//lowp vec3 specularColor = Specular * Specular * Spec_Level * Blend * Light0Specularcolor.xyz;
	lowp vec3 specularColor = Specular * Light0Specularcolor.xyz;
	
	//mediump float pf = vAttNdotHV.x * pow(vAttNdotHV.y, max(Specular * Glossiness, 0.0001));
	//mediump float pf = vAttNdotHV.x * pow(vAttNdotHV.y, Specular);
	//specularColor *= pf;
	
	specularColor *= vAttNdotHV;
	color += vec4(specularColor, 0.0);
#endif

#ifdef FG
	color.xyz = mix(color.xyz, fogcolor.xyz, vFogFactor);
	#ifdef ADDITIVEBLEND
		color.xyz *= fogFactor;
	#endif
#endif

#ifdef AL
	//color.w *= Object_Alpha;
#endif
	gl_FragColor = clamp(color, 0.0, 1.0);
	//gl_FragColor = color;
    return;
} // main end
