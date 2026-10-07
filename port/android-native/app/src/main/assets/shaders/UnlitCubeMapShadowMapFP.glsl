
#if defined(VERTEXCOLOR)
varying lowp    vec4        vColor0;
#endif

#if defined(TEXTURED)
varying mediump vec2        vCoord0;
uniform lowp    sampler2D   Texture;
#endif

#if defined(LIGHTING)
// MaterialColor
uniform lowp vec4 DiffuseColor;
uniform lowp vec4 emissioncolor;
uniform lowp vec4 ambientcolor;
uniform lowp vec4 SceneAmbientLight;

varying lowp vec4 vAmbient;
varying lowp vec4 vDiffuse;
#endif

varying mediump vec3        vVertexWorld;

uniform mediump vec3        LightPosition0;
uniform mediump vec2        ShadowNearFar0;
uniform lowp    samplerCube ShadowTexture0;
uniform lowp    float       ShadowOpacity0; 

void main(void)
{
    // Do these calc in mediump as the values can be outside lowp's [-2,2]
    mediump vec3 vertexDistanceVec =  vVertexWorld - LightPosition0;
    mediump vec3  vCoord1 = normalize(vertexDistanceVec);
    
    // take only the longest component as the distance the the light's projection plan
    mediump float vertexDistance =  max(max(abs(vertexDistanceVec.x), abs(vertexDistanceVec.y)), abs(vertexDistanceVec.z)); 
    mediump float lightDepth = textureCube(ShadowTexture0, vCoord1).r;
    
    // Project the distance in the light's depth projection
    // new.z = old.z * far / ( far - near ) -  old.w * far * near / (far - near)
    // new.w = old.z
    // dist = new.z / new.w
    mediump float transformedVertexDistance = (vertexDistance * ShadowNearFar0.y / (ShadowNearFar0.y-ShadowNearFar0.x) - (ShadowNearFar0.x*ShadowNearFar0.y/(ShadowNearFar0.y-ShadowNearFar0.x))) / vertexDistance;
    
    // Final color calc will be in lowp
    lowp float percentShadowed = 0.0;
    // compare the distance to the light with the light's "distance map"
    if( vertexDistance < ShadowNearFar0.y && transformedVertexDistance > lightDepth) 
    {
        percentShadowed = ShadowOpacity0;
    }
   
    lowp vec4 color = vec4(1.0);
#if defined(LIGHTING)
    lowp vec4 Diffuse = vec4(vDiffuse.rgb * (1.0 - percentShadowed), vDiffuse.a);
#if defined(TEXTURED)
    Diffuse *= texture2D(Texture, vCoord0);
#endif
    color = emissioncolor + (SceneAmbientLight + vAmbient) * ambientcolor + Diffuse * DiffuseColor;    
    gl_FragColor = clamp( color, 0.0, 1.0 );
#else
#if defined(VERTEXCOLOR) || defined(TEXTURED)
#    if defined(VERTEXCOLOR) && defined(TEXTURED)
    color *= texture2D(Texture, vCoord0) * vColor0;
#    elif defined(VERTEXCOLOR)
    color *= vColor0;
#    else //defined(TEXTURED)
    color *= texture2D(Texture, vCoord0);
#    endif
    gl_FragColor = vec4(color.rgb * (1.0 - percentShadowed), color.a);
#else
    gl_FragColor = vec4((1.0 - percentShadowed), 1.0);
#endif
#endif
}
