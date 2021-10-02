#include "Shader.h"
#include "Renderer.h"

#include <fstream>
#include <iostream>
#include <sstream>

shader::shader(const std::string& file_path) : m_file_path(file_path), m_shader_id(0)
{
    shader_source source = parse_shader(file_path);
    m_shader_id = create_shader(source.vertex_source, source.fragment_source);
    glUseProgram(m_shader_id);
}

shader::~shader() { glDeleteProgram(m_shader_id); }

void shader::bind() const { glUseProgram(m_shader_id); }

void shader::unbind() const { glUseProgram(0); }

void shader::set_uniform4f (const std::string& name, float v1, float v2, float v3, float v4) const
{
    int location = get_uniform_location(name);
    glUniform4f(location, v1, v2, v3, v4);
}

int shader::get_uniform_location(const std::string& name) const
{
    /* if (m_uniform_location_cache.find(name) != m_uniform_location_cache.end())
        return m_uniform_location_cache[name];*/

    int location = glGetUniformLocation(m_shader_id, name.c_str());
    if (location == -1)
    {
        //std::cout << " warning: uniform " << name << " does not exist ";
        return -1;
    }
    else
    {
        //m_uniform_location_cache[name] = location;
        return location;
    } 
}

unsigned int shader::compile_shader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int lenght;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
        char* message = static_cast<char*>(alloca(sizeof(char) * lenght));
        glGetShaderInfoLog(id, lenght, &lenght, message);
        std::cout << "failed to compile shader!" << std::endl;
        std::cout << message;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int shader::create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

shader::shader_source shader::parse_shader(const std::string& filepath)
{
    enum class shader_type
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::ifstream stream(filepath);
    std::stringstream ss[2];
    shader_type type = shader_type::NONE;
    std::string line;

    while (getline(stream, line))
    {
        if (line.find("shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = shader_type::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = shader_type::FRAGMENT;
            }
        }
        else
        {
            ss[static_cast<int>(type)] << line << '\n';
        }
    }
    return {ss[0].str(), ss[1].str()};
}
