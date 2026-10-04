uniform lowp sampler2D texture;

varying lowp vec2 uv;

// some const, tweak for best look
//const mediump float sampleDist = 1.0;
const lowp float sampleStrength = 2.0;

void main()
{
    //lowp float samples[10] = { -0.08, -0.05, -0.03, -0.02, -0.01, 0.01, 0.02, 0.03, 0.05, 0.08};
	
    //=float[10](-0.08, -0.05, -0.03, -0.02, -0.01, 0.01, 0.02, 0.03, 0.05, 0.08);

    // 0.5,0.5 is the center of the screen
    // so substracting uv from it will result in
    // a vector pointing to the middle of the screen
    lowp vec2 dir = 0.5 - uv; 
 
    // calculate the distance to the center of the screen
    lowp float dist = length(uv);
    
    // normalize the direction (reuse the distance)
    dir = dir/dist; 
 
    // this is the original colour of this fragment
    // using only this would result in a nonblurred version
    const lowp float scale = 1.0/11.0;
    lowp vec4 color = texture2D(texture,uv)*scale; 
 
    lowp vec4 sum = color;
 
    // take 10 additional blur samples in the direction towards
    // the center of the screen
    sum += texture2D(texture, uv + dir * -0.08 )*scale;//* sampleDist);
    sum += texture2D(texture, uv + dir * -0.05 )*scale;//* sampleDist);
    sum += texture2D(texture, uv + dir * -0.03 )*scale;//* sampleDist);
    sum += texture2D(texture, uv + dir * -0.02 )*scale;//* sampleDist);
    sum += texture2D(texture, uv + dir * -0.01 )*scale;//* sampleDist);
    sum += texture2D(texture, uv + dir *  0.01 )*scale;//* sampleDist);
    sum += texture2D(texture, uv + dir *  0.02 )*scale;//* sampleDist);
    sum += texture2D(texture, uv + dir *  0.03 )*scale;//* sampleDist);
    sum += texture2D(texture, uv + dir *  0.05 )*scale;//* sampleDist);
    sum += texture2D(texture, uv + dir *  0.08 )*scale;//* sampleDist);
    //for (int i = 0; i < 10; i++)
    //{
    //    sum += texture2D(texture, uv + dir * samples[i] * sampleDist);
    //}
 
    // we have taken eleven samples
	//lowp vec4 result = sum / 11.0;
 
    // weighten the blur effect with the distance to the
    // center of the screen ( further out is blurred more)
    lowp float t = clamp(dist * sampleStrength,0.0,1.0);
 
    //Blend the original color with the averaged pixels
    //gl_FragColor = mix( color, sum, t );
    gl_FragColor = color + t * (sum -color);//(1.0-t)*color + t * sum;
}
