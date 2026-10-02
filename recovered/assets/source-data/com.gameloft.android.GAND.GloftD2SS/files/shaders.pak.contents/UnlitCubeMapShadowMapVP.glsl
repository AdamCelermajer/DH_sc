attribute highp vec4 Vertex;

#if defined(LIGHTING)
attribute highp vec3 Normal;
uniform highp mat4 WorldIT;

// Light0
uniform highp vec4 Light0Position;
uniform vec3 Light0Attenuation;
uniform vec4 Light0Ambientcolor;
uniform vec4 Light0Diffusecolor;
uniform vec4 Light0Specularcolor;
#endif

#if defined(VERTEXCOLOR)
attribute lowp vec4 Color0;
#endif

#ifdef VERTEXCOLOR
varying lowp vec4 vColor0;
#endif

#ifdef TEXTURED
attribute mediump vec2 TexCoord0;
varying mediump vec2 vCoord0;
#endif

varying mediump vec3 vVertexWorld;

uniform highp mat4 WorldViewProjectionMatrix;
uniform highp mat4 WorldMatrix;

#if defined(LIGHTING)
varying vec4 vAmbient;
varying vec4 vDiffuse;
#endif

void main(void)
{
	vVertexWorld = (WorldMatrix * Vertex).xyz;
#if defined(LIGHTING)
    vec3 transformedNormal = (WorldIT * vec4(Normal, 0.0)).xyz;
#endif

#if defined(LIGHTING)
    // Clear the light intensity accumulators
    vec3 eye      = vec3 (0.0, 0.0, 1.0);
    vec4 Ambient  = vec4 (0.0, 0.0, 0.0, 0.0);
    vec4 Diffuse  = vec4 (0.0, 0.0, 0.0, 0.0);
 
    // light 0 (point/directional light)
    vec3 Light0Vec = Light0Position.xyz - vVertexWorld * Light0Position.w;
    float d = length(Light0Vec);  // Compute distance between surface and light position
    vec3 VP = normalize(Light0Vec);// Normalize the vector from surface to light position
    float attenuation = 1.0 / (Light0Attenuation.x + Light0Attenuation.y * d + Light0Attenuation.z * d * d);
    vec3  halfVector = normalize(VP + eye);
    float nDotVP = max(0.0, dot(transformedNormal, VP));
    Ambient  += Light0Ambientcolor * attenuation;
    Diffuse  += Light0Diffusecolor * nDotVP * attenuation;

    //vec4 color = emissioncolor + (SceneAmbientLight + Ambient) * ambientcolor + Diffuse  * DiffuseColor;
    //color = clamp( color, 0.0, 1.0 );
    //vColor0 = color;
    vAmbient = Ambient;
    vDiffuse = Diffuse;
#endif

#if defined(VERTEXCOLOR)
	vColor0 = Color0;
#endif

#if defined(TEXTURED)
	vCoord0 = TexCoord0;
#endif

	gl_Position = WorldViewProjectionMatrix * Vertex;
}
