#include "Shader.h"

enum class
ShaderType
{
    NONE = -1, VERTEX = 0, FRAGMENT = 1
};

Shader::Shader(const std::string& filepath):m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}
Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

unsigned int
Shader::CompileShader(unsigned int type, const std::string& source) 
{
   unsigned int id  = glCreateShader(type);
   const char* src = source.c_str();
   glShaderSource(id, 1, &src, nullptr);
   glCompileShader(id);
    
   int result;
   glGetShaderiv(id, GL_COMPILE_STATUS, &result);
   // Error handling
   if (result == GL_FALSE) {
       int length;
       glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
       char* message = (char*)alloca(length* sizeof(char));
       glGetShaderInfoLog(id, length, &length, message);
       std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex": "fragment") << std::endl;

       std::cout << message << std::endl;
       glDeleteShader(id); // our compilation did not work , we should delete the shader
       return 0;
   }
   return id;
}

ShaderProgramSource
Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream,line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if(line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

unsigned int 
Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Shader::Bind()
{
    GLCall(glUseProgram(m_RendererID));

}
void Shader::Unbind()
{
    GLCall(glUseProgram(0));
}

void
Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

unsigned int
Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];
    GLCall(unsigned int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning uniform" << name << " does not exist!" << std::endl;
    m_UniformLocationCache[name] = location;
    return location;
}
