#ifdef GL_FRAGMENT_PRECISION_HIGH
	// define maxp based on user preference
	#ifdef GLITCH_USE_HIGHP
		#define MAXP highp
	#else
		#define MAXP mediump
	#endif
#else
	// highp not supported, force maxp as medium
	#define MAXP mediump
#endif

attribute highp vec4 Position;

attribute lowp vec4 Color0;

#ifdef TEXTURED
attribute mediump vec2 TexCoord0;
varying MAXP vec2 vTexCoord0;
uniform mediump mat4 TextureMatrix0;
#endif
#ifdef MULTITEXTURED
varying MAXP vec2 vTexCoord1;
uniform highp mat4 matWorldViewIT;
#endif //MULTITEXTURED
#ifdef LIGHTMAP
varying MAXP vec2 vTexCoord2;
uniform mediump mat4 TextureMatrix2;
#    ifdef TEXTURED
#    define LightMapCoord TexCoord1
attribute mediump vec2 TexCoord1;
#    else
#    define LightMapCoord TexCoord0
attribute mediump vec2 TexCoord0;
#    endif
#endif

uniform highp mat4 WorldViewProjectionMatrix;

uniform mediump vec4 DiffuseColor;

#ifdef FOG
uniform highp mat4 WorldViewT;
varying lowp float fogFactor;
uniform mediump vec2 fogstartend;
#endif //FOG

#if defined(GLITCH_OPENGLES_2) || defined(GLITCH_OPENGLES_1_1)
#if defined (TEXTURESKINNED)
#undef TEXTURESKINNED
#endif
#endif

#if defined(SKINNED)
uniform mediump mat4 BoneMatrices[48];
uniform lowp vec4 WeightMask;
attribute mediump vec4 SkinWeights;
attribute mediump vec4 SkinIndices;

#elif defined(QUATSKINNED)
uniform mediump vec4 BoneQuat0[96];
uniform mediump vec4 BoneQuat1[96];
uniform lowp vec4 WeightMask;
attribute mediump vec4 SkinWeights;
attribute mediump vec4 SkinIndices;

#elif defined(TEXTURESKINNED)
uniform lowp sampler2D BoneTexture;
uniform mediump float BoneTextureParams;
uniform lowp vec4 WeightMask;
attribute mediump vec4 SkinWeights;
attribute mediump vec4 SkinIndices;
#endif //SKINNED

#if defined LIGHTING || defined MULTITEXTURED
attribute highp vec3 Normal;
uniform highp mat4 matWorld;
#endif

#if defined LIGHTING || defined MULTITEXTURED
//uniform highp mat4 matmodelview;
uniform highp mat4 matWorldIT;
#endif

#ifdef LIGHTING
// Light0
uniform highp vec4 Light0Position;
uniform vec3 Light0Attenuation;
uniform vec4 Light0Ambientcolor;
uniform vec4 Light0Diffusecolor;
uniform vec4 Light0Specularcolor;

// MaterialColor
uniform vec4 emissioncolor;
uniform vec4 ambientcolor;
uniform vec4 specularcolor;
uniform float shininess;
uniform vec4 SceneAmbientLight;
#endif //LIGHTING

varying lowp vec4 vColor0;

vec3 dqTransform(mediump vec4 dq0, mediump vec4 dq1, highp vec3 v)
{
    highp vec3 netPosition = v + 2.0 * cross(cross(v, dq0.xyz) + dq0.w * v, dq0.xyz);
    netPosition += 2.0 * (dq0.w * dq1.xyz - dq1.w * dq0.xyz + cross(dq0.xyz, dq1.xyz));
    return netPosition;
}

void main(void)
{
#if defined SKINNED
    vec4 weights = SkinWeights * WeightMask;
    vec3 netPosition = vec3(0.0, 0.0, 0.0);
#   if defined LIGHTING || defined MULTITEXTURED
    vec3 netNormal = vec3(0.0, 0.0, 0.0);
#   endif
    for (int i = 0; i < 4; i++)
    {
        int index = int(SkinIndices[i]);
        netPosition += weights[i] * (BoneMatrices[index] * vec4(Position.xyz, 1)).xyz;
#   if defined LIGHTING || defined MULTITEXTURED
        netNormal += weights[i] * (BoneMatrices[index] * vec4(Normal.xyz, 1)).xyz;
#   endif
    }
    gl_Position = WorldViewProjectionMatrix * vec4(netPosition,1);
#   if defined LIGHTING || defined MULTITEXTURED
    vec3 transformedNormal = (matWorldIT * vec4(netNormal, 0.0)).xyz;
    vec3 worldPosition = (matWorld * vec4(netPosition,1)).xyz;
#   endif
#   ifdef FOG
    float depth = dot(WorldViewT[2], vec4(netPosition,1));
#   endif

#elif defined QUATSKINNED

    vec4 weights = SkinWeights * WeightMask;
    vec3 netPosition = vec3(0.0, 0.0, 0.0);
#   if defined LIGHTING || defined MULTITEXTURED
    vec3 netNormal = vec3(0.0, 0.0, 0.0);
#   endif

    for(int i = 0; i < 4; ++i)
    {
        int index = int(SkinIndices[i]);
        netPosition += dqTransform(BoneQuat0[index], BoneQuat1[index], Position.xyz) * weights[i];
#   if defined LIGHTING || defined MULTITEXTURED
        netNormal += dqTransform(BoneQuat0[index], BoneQuat1[index], Normal.xyz) * weights[i];
#   endif
    }

    gl_Position = WorldViewProjectionMatrix * vec4(netPosition,1);
#   if defined LIGHTING || defined MULTITEXTURED
    vec3 transformedNormal = (matWorldIT * vec4(netNormal, 0.0)).xyz;
    vec3 worldPosition = (matWorld * vec4(netPosition,1)).xyz;
#   endif
#   ifdef FOG
    float depth = dot(WorldViewT[2], vec4(netPosition,1));
#   endif

#elif defined TEXTURESKINNED

    vec4 uv_base = vec4(BoneTextureParams * 0.5, BoneTextureParams * 1.5, BoneTextureParams * 2.5, BoneTextureParams * 4.);
    vec4 weights = SkinWeights * WeightMask;
    vec3 netPosition = vec3(0.0, 0.0, 0.0);
#   if defined LIGHTING || defined MULTITEXTURED
    vec3 netNormal = vec3(0.0, 0.0, 0.0);
#   endif

    for (int i = 0; i < 4; i++)
    {
        highp mat4 BoneMatrix;
        highp vec4 weightedIndices = SkinIndices[i] * uv_base.wwww;
        highp vec3 index = weightedIndices.xxx + uv_base.xyz;

        BoneMatrix[0] = texture2D(BoneTexture, index.xx);
        BoneMatrix[1] = texture2D(BoneTexture, index.yy);
        BoneMatrix[2] = texture2D(BoneTexture, index.zz);
        BoneMatrix[3] = vec4(0, 0, 0, 1);

        netPosition += weights[i] * (BoneMatrix * Position).xyz;
#   if defined LIGHTING || defined MULTITEXTURED
        netNormal += weights[i] * (BoneMatrix * vec4(Normal, 0.0)).xyz;
#   endif
    }

    gl_Position = WorldViewProjectionMatrix * vec4(netPosition,1);
#   if defined LIGHTING || defined MULTITEXTURED
    vec3 transformedNormal = (matWorldIT * vec4(netNormal, 0.0)).xyz;
    vec3 worldPosition = (matWorld * vec4(netPosition,1)).xyz;
#   endif
#   ifdef FOG
    float depth = dot(WorldViewT[2], vec4(netPosition,1));
#   endif

#else //No skinning
    gl_Position = WorldViewProjectionMatrix * Position;
#   if defined LIGHTING || defined MULTITEXTURED
    vec3 transformedNormal = (matWorldIT * vec4(Normal, 0.0)).xyz;
    vec4 worldPosition = matWorld * Position;
#   endif
#   ifdef FOG
    float depth = dot(WorldViewT[2], Position);
#   endif
#endif //SKINNED

    vColor0 = Color0 * DiffuseColor;
 
#ifdef TEXTURED
    vTexCoord0 = (TextureMatrix0 * vec4(TexCoord0, 1, 0)).xy;
#endif
#ifdef MULTITEXTURED
    vec2 n = (matWorldViewIT * vec4(Normal, 1)).xy;
    vTexCoord1.x = n.x * 0.5 + 0.5;
    vTexCoord1.y = 0.5 - n.y * 0.5;
#endif //MULTITEXTURED
#ifdef LIGHTMAP
    vTexCoord2 = (TextureMatrix2 * vec4(LightMapCoord, 1, 0)).xy;
#endif
 
#ifdef FOG
    fogFactor = clamp((depth - fogstartend[0])/(fogstartend[1] - fogstartend[0]), 0.0, 1.0);
#endif //FOG

#ifdef LIGHTING
    // Clear the light intensity accumulators
    vec3 eye      = vec3 (0.0, 0.0, 1.0);
    vec4 Ambient  = vec4 (0.0, 0.0, 0.0, 0.0);
    vec4 Diffuse  = vec4 (0.0, 0.0, 0.0, 0.0);
    vec4 Specular = vec4 (0.0, 0.0, 0.0, 0.0);
 
    // light 0 (point/directional light)
    vec3 Light0Vec = Light0Position.xyz - worldPosition.xyz * Light0Position.w;
    float d = length(Light0Vec);  // Compute distance between surface and light position
    vec3 VP = normalize(Light0Vec);// Normalize the vector from surface to light position
    float attenuation = 1.0 / (Light0Attenuation.x + Light0Attenuation.y * d + Light0Attenuation.z * d * d);
    vec3  halfVector = normalize(VP + eye);
    float nDotVP = max(0.0, dot(transformedNormal, VP));
    float nDotHV = max(0.0, dot(transformedNormal, halfVector));
    float pf = ( nDotVP > 0.0 ? pow(nDotHV, max(shininess, 0.0001)) : 0.0 );
    Specular += Light0Specularcolor * pf * attenuation;
    Ambient  += Light0Ambientcolor * attenuation;
    Diffuse  += Light0Diffusecolor * nDotVP * attenuation;

    vec4 color = emissioncolor + (SceneAmbientLight + Ambient) * ambientcolor + Diffuse  * DiffuseColor + Specular * specularcolor;
    color = clamp( color, 0.0, 1.0 );
    vColor0 = color;
#endif //LIGHTING
}
