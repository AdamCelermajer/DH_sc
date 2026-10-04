attribute highp vec4 Vertex;

#ifdef VERTEXCOLOR
attribute lowp vec4 Color0;
varying lowp vec4 vColor0;
#endif

#ifdef TEXTURED
attribute mediump vec2 TexCoord0;
varying mediump vec2 vCoord0;
#endif

//varying mediump vec3 vCoord1;
varying mediump vec3 vVertexWorld;

uniform highp mat4 WorldViewProjectionMatrix;
uniform highp mat4 WorldMatrix;
uniform highp vec3 Light0Position;

void main(void)
{
#ifdef VERTEXCOLOR
	vColor0 = Color0;
#endif
#ifdef TEXTURED
	vCoord0 = TexCoord0;
#endif

	vVertexWorld = (WorldMatrix * Vertex).xyz;
	gl_Position = WorldViewProjectionMatrix * Vertex;
}
