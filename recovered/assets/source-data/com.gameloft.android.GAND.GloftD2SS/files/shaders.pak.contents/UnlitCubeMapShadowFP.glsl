#ifdef VERTEXCOLOR
varying lowp vec4 vColor0;
#endif

#ifdef TEXTURED
varying mediump vec2 vCoord0;
uniform lowp sampler2D Texture;
#endif

uniform lowp samplerCube ShadowTexture0;
uniform lowp float ShadowOpacity0; 

varying mediump vec3 vVertexWorld;
uniform mediump vec3 Light0Position;

void main(void)
{
	mediump vec3 vCoord1 = normalize(vVertexWorld - Light0Position);
	
	lowp float percentShadowed = (1.0 - textureCube(ShadowTexture0, vCoord1).r) * ShadowOpacity0;
	lowp vec4 color = vec4(1.0);
#if defined(VERTEXCOLOR) || defined(TEXTURED)
#    if defined(VERTEXCOLOR) && defined(TEXTURED)
	color = texture2D(Texture, vCoord0) * vColor0;
#    elif defined(VERTEXCOLOR)
	color = vColor0;
#    else //defined(TEXTURED)
	color = texture2D(Texture, vCoord0);
#    endif
#endif
	gl_FragColor = vec4(color.rgb * (1.0 - percentShadowed), color.a);
}
