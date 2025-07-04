#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{

private:
    void check_compile_errors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
public:
    unsigned int id;

    Shader() {}

    Shader &use() 
    {
        glUseProgram(id);
        return *this;
    }

    void set_bool(const std::string& name, bool value, bool use_shader = false) const
    {
        if (use_shader) {
            glUseProgram(id);
        }

        glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
    }

    void set_int(const std::string& name, int value, bool use_shader = false) const
    {
        if (use_shader) {
            glUseProgram(id);
        }
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }

    void set_float(const std::string& name, float value, bool use_shader = false) const
    {
        if (use_shader) {
            glUseProgram(id);
        }
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }

    void set_vec2(const std::string& name, const glm::vec2& value, bool use_shader = false) const
    {
        if (use_shader) {
            glUseProgram(id);
        }
        glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void set_vec2(const std::string& name, float x, float y, bool use_shader = false) const
    {
        if (use_shader) {
            glUseProgram(id);
        }
        glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
    }

    void set_vec3(const std::string& name, const glm::vec3& value, bool use_shader = false) const
    {
        if (use_shader) {
            glUseProgram(id);
        }
        glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void set_vec3(const std::string& name, float x, float y, float z, bool use_shader = false) const
    {
        if (use_shader) {
            glUseProgram(id);
        }
        glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
    }

    void set_vec4(const std::string& name, const glm::vec4& value, bool use_shader = false) const
    {
        if (use_shader) {
            glUseProgram(id);
        }
        glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void set_vec4(const std::string& name, float x, float y, float z, float w, bool use_shader = false) const
    {
        if (use_shader) {
            glUseProgram(id);
        }
        glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
    }

    void set_mat2(const std::string& name, const glm::mat2& mat, bool use_shader = false) const
    {
        if (use_shader) {
            glUseProgram(id);
        }
        glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void set_mat3(const std::string& name, const glm::mat3& mat, bool use_shader = false) const
    {
        if (use_shader) {
            glUseProgram(id);
        }
        glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void set_mat4(const std::string& name, const glm::mat4& mat, bool use_shader = false) const
    {
        if (use_shader) {
            glUseProgram(id);
        }
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void compile(const char* vsource, const char* fsource, const char* gsource) {
        unsigned int vertex, fragment, geometry;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vsource, NULL);
        glCompileShader(vertex);
        check_compile_errors(vertex, "VERTEX");

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fsource, NULL);
        glCompileShader(fragment);
        check_compile_errors(fragment, "FRAGMENT");

        if (gsource != nullptr) {
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gsource, NULL);
            glCompileShader(geometry);
            check_compile_errors(geometry, "GEOMETRY");
        }

        id = glCreateProgram();

        glAttachShader(id, vertex);
        glAttachShader(id, fragment);

        if (gsource != nullptr) {
            glAttachShader(id, geometry);
        }

        glLinkProgram(id);

		check_compile_errors(id, "PROGRAM");

        //연결 후 제거
        glDeleteShader(vertex);
        glDeleteShader(fragment);

        if (gsource != nullptr) {
            glDeleteShader(geometry);
        }
    }
};
#endif