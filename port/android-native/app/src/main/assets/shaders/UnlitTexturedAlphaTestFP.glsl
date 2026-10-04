uniform lowp sampler2D texture;
uniform lowp float alpharef;

varying mediump vec2 vCoord0;
varying	lowp vec4 vColor0;

void main()
{
	lowp vec4 color = texture2D(texture, vCoord0) * vColor0;
	if (color.a <= alpharef)
	{
		discard;
	}
    gl_FragColor = color;
}
