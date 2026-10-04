attribute highp vec4 Vertex;
attribute highp vec3 Normal;

#ifdef VERTEXCOLOR
attribute lowp vec4 Color0;
varying lowp vec4 vColor0;
#endif

#ifdef TEXTURED
attribute mediump vec2 TexCoord0;
varying mediump vec2 vCoord0;
#endif

varying mediump vec3 vCoord1;

uniform highp mat4 WorldViewProjectionMatrix;
uniform highp mat4 WorldMatrix;
uniform highp vec3 EyePosition;

void main(void)
{
#ifdef VERTEXCOLOR
	vColor0 = Color0;
#endif
#ifdef TEXTURED
	vCoord0 = TexCoord0;
#endif

	highp vec3 eye2Vertex = normalize((WorldMatrix * Vertex).xyz - EyePosition);
	highp vec3 normal = (WorldMatrix * vec4(Normal, 0)).xyz;
	vCoord1 = reflect(eye2Vertex, normal);

	gl_Position = WorldViewProjectionMatrix * Vertex;
}
