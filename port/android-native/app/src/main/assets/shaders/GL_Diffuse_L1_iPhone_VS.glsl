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


attribute highp vec4 Vertex;
attribute highp vec3 Normal;					// Lighting
attribute mediump vec2 texcoord0;

#ifdef VC
	attribute lowp vec4 Color;					// Vertex Color
#endif

#ifdef LM
	attribute mediump vec2 texcoord1;
#endif

#ifdef ENABLE_TANGENT_SPACE
	attribute mediump vec4 tangent;
	attribute mediump vec4 binormal;
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

#ifdef SP
	uniform mediump float Glossiness;
#endif

#ifdef LIGHTING
	// Light0
	uniform highp vec4 Light0Position;
	uniform vec3 Light0Attenuation;
	uniform vec4 Light0Ambientcolor;
	uniform vec4 Light0Diffusecolor;
	//uniform vec3 Ambient_Color;
#endif

#ifdef FG
	uniform mediump vec2 fogstartend;
#	ifdef CUSTOM_FOG
		uniform mediump vec3 fogDirectionMask;
		uniform mediump vec3 fogCameraMask;
		uniform mediump vec3 fogCameraOffset;
#	endif
	uniform highp mat4 matWorldViewT;
#endif

uniform highp mat4 matWorldViewProjection;
uniform highp mat4 matWorld;					// Lighting
uniform highp mat4 matWorldIT;


uniform highp mat4 matWorldI;
uniform highp mat4 matViewI;

uniform highp mat4 matViewIT;

// create the light vector
#ifdef ENABLE_TANGENT_SPACE
	vec3 lightVec_func(vec3 worldSpacePos, vec4 lightVector, mat3 objTangentXf)
	{
		vec3 _lightVec = (vec4((lightVector.xyz - worldSpacePos), 1.0) * matWorld).xyz;
		vec3 lightVec = objTangentXf * _lightVec;
		return lightVec;
	}
#else
	vec3 lightVec_func(vec3 lightPosition, vec3 P) 
	{ 
		return lightPosition - P; 
	}
#endif

float attenuation_func(vec3 lightAttenuation, vec3 lightVec)
{
	float d = length(lightVec);
	float att = 1.0 / (lightAttenuation.x + (lightAttenuation.y/1000.0) * d + (lightAttenuation.z/1000000.0) * d * d);
	return att;
}

// main procedure, the original name was v
void main()
{	
// Vertex Position output
	gl_Position = matWorldViewProjection * Vertex;

    highp vec3 transformedNormal = (matWorldIT * vec4(Normal, 0.0)).xyz;
	highp vec3 worldSpacePos = (matWorld * Vertex).xyz;
    
#ifdef ENABLE_TANGENT_SPACE  
    highp mat3 objTangentXf;
 	objTangentXf[0] =  tangent.xyz;
	objTangentXf[1] = -binormal.xyz;
	objTangentXf[2] =  Normal.xyz;
#else
	//varNormal = Normal.xyz;
#endif
   	
#ifdef LIGHTING
#ifdef SP
#	ifdef ENABLE_TANGENT_SPACE
		highp vec4 eyePos = vec4(matViewIT[0].w, matViewIT[1].w, matViewIT[2].w, matViewIT[3].w);
		highp vec4 osIPos = worldI * eyePos;
		highp vec3 osIVec = osIPos.xyz - Vertex.xyz;
		highp vec3 eye = objTangentXf * osIVec;
#	else
		highp vec3 eyePos = vec3(matViewIT[0].w, matViewIT[1].w, matViewIT[2].w);
		highp vec3 eye = normalize(eyePos.xyz - worldSpacePos);
#	endif
#endif
	//vec3 eye      = vec3 (0.0, 0.0, 1.0);
	
    // Clear the light intensity accumulators
    //vec4 Ambient  = vec4 (0.0, 0.0, 0.0, 0.0);
    //vec4 Diffuse  = vec4 (0.0, 0.0, 0.0, 0.0);
    //vec4 Specular = vec4 (0.0, 0.0, 0.0, 0.0);
 
    // light 0 (point/directional light)
#ifdef ENABLE_TANGENT_SPACE
	highp vec3 Light0Vec = lightVec_func(worldSpacePos, Light0Position.xyz, objTangentXf); 
#else
	highp vec3 Light0Vec = Light0Position.xyz - worldSpacePos * Light0Position.w; //lightVec_func(light0Pos, worldSpacePos);
#endif    
    // Light Attenuation
    mediump float d = length(Light0Vec);  	// Compute distance between surface and light position
    mediump float attenuation = 1.0 / (Light0Attenuation.x + (Light0Attenuation.y) * d + (Light0Attenuation.z) * d * d);
    
    highp vec3 VP = normalize(Light0Vec);	// Normalize the vector from surface to light position
    mediump float nDotVP = max(0.0, dot(transformedNormal, VP));
    
#ifdef SP
	//vAttNdotHV.x = attenuation;
    highp vec3  halfVector = normalize(VP + eye);
    vAttNdotHV = nDotVP > 0.0 ? attenuation * pow(max(0.0, dot(transformedNormal, halfVector)), Glossiness) : 0.0;
    
    //float pf = ( nDotVP > 0.0 ? pow(nDotHV, max(shininess, 0.0001)) : 0.0 );
    //Specular += Light0Specularcolor * pf * attenuation;
#endif
    
    vec4 Ambient  = Light0Ambientcolor;// * vec4(Ambient_Color, 1.0); // * attenuation;
    vec4 Diffuse  = Light0Diffusecolor * nDotVP * attenuation;

	// Specular lighting will be calculated in the fragment
    vec4 color =  vec4(Ambient.xyz + Diffuse.xyz, 1.0);
    //color = clamp( color, 0.0, 1.0 );
#ifdef VC
    vColor = color * vec4(Color.xyz, 1.0);
#else
	vColor = color
#endif

#else //end LIGHTING    
#	ifdef VC
		vColor = vec4(Color.xyz, 1.0);
#	endif
#endif

// UV coordinates output	
	vTexCoord0.xy = texcoord0.xy;	// Diffuse/Specular
#ifdef LM
	vTexCoord0.zw = texcoord1.xy;	// LightMap
#endif

// Fog factor output	
#ifdef FG
    //vec3 fogVector = Vertex.xyz;
	#ifdef CUSTOM_FOG
		vec3 fogCameraPosition = eyePos.xyz;
		vec3 fogReferencePoint = (fogCameraPosition * fogCameraMask) + fogCameraOffset;
		fogVector = (worldSpacePos.xyz - fogReferencePoint) * fogDirectionMask;
	#endif
	float fogDistance = dot(matWorldViewT[2], Vertex);//length(fogVector);
	vFogFactor = clamp((fogDistance - fogstartend[0]) / (fogstartend[1] - fogstartend[0]), 0.0, 1.0);
#endif
    return;
} // main end
