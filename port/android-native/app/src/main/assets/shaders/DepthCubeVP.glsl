attribute highp   vec4 Vertex;

uniform highp mat4 WorldViewProjectionMatrix;
uniform highp vec3 Light0Position;

void main()
{
    gl_Position = WorldViewProjectionMatrix * Vertex;
}
