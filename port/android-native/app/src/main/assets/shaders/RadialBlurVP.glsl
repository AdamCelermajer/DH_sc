attribute highp   vec4 Vertex;
attribute mediump vec2 TexCoord0;
varying	  lowp    vec2 uv;

uniform   highp   mat4 WorldViewProjectionMatrix;

void main(void)
{
	gl_Position = WorldViewProjectionMatrix * Vertex;
	uv = clamp(TexCoord0,0.0,1.0);
}
