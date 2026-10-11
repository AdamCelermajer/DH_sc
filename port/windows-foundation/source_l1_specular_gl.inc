// B114: narrowly scoped original L1 SP varying/blue-sampler transport. Using
// fixed primary color would clamp the authored attenuation BEFORE sampling.
struct SourceSpecularGl {
    using CreateShader=GLuint(APIENTRY*)(GLenum);
    using ShaderSource=void(APIENTRY*)(GLuint,GLsizei,const char* const*,const GLint*);
    using Shader=void(APIENTRY*)(GLuint);
    using ShaderIv=void(APIENTRY*)(GLuint,GLenum,GLint*);
    using CreateProgram=GLuint(APIENTRY*)();
    using AttachShader=void(APIENTRY*)(GLuint,GLuint);
    using UniformLocation=GLint(APIENTRY*)(GLuint,const char*);
    using Uniform1i=void(APIENTRY*)(GLint,GLint);
    using Uniform1f=void(APIENTRY*)(GLint,GLfloat);
    using Uniform3fv=void(APIENTRY*)(GLint,GLsizei,const GLfloat*);
    CreateShader createShader{};ShaderSource shaderSource{};Shader compileShader{},deleteShader{},linkProgram{},useProgram{},deleteProgram{};
    ShaderIv shaderIv{},programIv{};CreateProgram createProgram{};AttachShader attachShader{};
    UniformLocation uniformLocation{};Uniform1i uniform1i{};Uniform1f uniform1f{};Uniform3fv uniform3fv{};
    void* (*loader)(const char*){};
    bool ready()const{return createShader&&shaderSource&&compileShader&&deleteShader&&linkProgram&&useProgram&&deleteProgram&&shaderIv&&programIv&&createProgram&&attachShader&&uniformLocation&&uniform1i&&uniform1f&&uniform3fv;}
};
SourceSpecularGl& sourceSpecularGl(void* (*loader)(const char*)) {
    static SourceSpecularGl api;
    if(loader&&api.loader!=loader){api={};api.loader=loader;
#define DH_SPEC_GL(member,type,name) api.member=reinterpret_cast<SourceSpecularGl::type>(loader(name))
        DH_SPEC_GL(createShader,CreateShader,"glCreateShader");DH_SPEC_GL(shaderSource,ShaderSource,"glShaderSource");
        DH_SPEC_GL(compileShader,Shader,"glCompileShader");DH_SPEC_GL(deleteShader,Shader,"glDeleteShader");
        DH_SPEC_GL(linkProgram,Shader,"glLinkProgram");DH_SPEC_GL(useProgram,Shader,"glUseProgram");DH_SPEC_GL(deleteProgram,Shader,"glDeleteProgram");
        DH_SPEC_GL(shaderIv,ShaderIv,"glGetShaderiv");DH_SPEC_GL(programIv,ShaderIv,"glGetProgramiv");
        DH_SPEC_GL(createProgram,CreateProgram,"glCreateProgram");DH_SPEC_GL(attachShader,AttachShader,"glAttachShader");
        DH_SPEC_GL(uniformLocation,UniformLocation,"glGetUniformLocation");DH_SPEC_GL(uniform1i,Uniform1i,"glUniform1i");
        DH_SPEC_GL(uniform1f,Uniform1f,"glUniform1f");DH_SPEC_GL(uniform3fv,Uniform3fv,"glUniform3fv");
#undef DH_SPEC_GL
    }return api;
}
GLuint createSourceSpecularProgram(SourceSpecularGl& api){
    if(!api.ready())throw std::runtime_error("Original L1 Sp rendering requires OpenGL 2 shader transport");
    // View rotation is orthonormal, so these eye-space dot products are the
    // original world-space equation. The inverse-transpose normal stays raw.
    const char* vs=R"GLSL(#version 120
uniform vec3 sourceSpecularColor;
uniform float sourceGlossiness;
varying vec3 sourceSpecular;
varying vec2 sourceUv;
void main(){
    gl_Position=ftransform();
    vec3 p=(gl_ModelViewMatrix*gl_Vertex).xyz;
    vec3 n=gl_NormalMatrix*gl_Normal;
    vec3 lightVector=gl_LightSource[1].position.xyz-p;
    float d=length(lightVector);
    float attenuation=1.0/(gl_LightSource[1].constantAttenuation+gl_LightSource[1].linearAttenuation*d+gl_LightSource[1].quadraticAttenuation*d*d);
    vec3 l=normalize(lightVector);
    vec3 h=normalize(l+normalize(-p));
    float factor=dot(n,l)>0.0?attenuation*pow(max(0.0,dot(n,h)),sourceGlossiness):0.0;
    sourceSpecular=sourceSpecularColor*factor;
    sourceUv=(gl_TextureMatrix[0]*gl_MultiTexCoord0).xy;
})GLSL";
    const char* fs="#version 120\nuniform sampler2D specularSampler;varying vec3 sourceSpecular;varying vec2 sourceUv;void main(){gl_FragColor=clamp(vec4(texture2D(specularSampler,sourceUv).b*sourceSpecular,0.0),0.0,1.0);}";
    GLuint shaders[2]{},program=0;bool okay=true;
    for(unsigned i=0;i<2;++i){shaders[i]=api.createShader(i?0x8B30:0x8B31);const char* code=i?fs:vs;api.shaderSource(shaders[i],1,&code,nullptr);api.compileShader(shaders[i]);GLint status=0;api.shaderIv(shaders[i],0x8B81,&status);okay=okay&&status;}
    if(okay){program=api.createProgram();for(auto shader:shaders)api.attachShader(program,shader);api.linkProgram(program);GLint status=0;api.programIv(program,0x8B82,&status);okay=status;}
    for(auto shader:shaders)if(shader)api.deleteShader(shader);
    if(!okay){if(program)api.deleteProgram(program);throw std::runtime_error("Original L1 Sp shader compilation/link failed");}
    return program;
}
