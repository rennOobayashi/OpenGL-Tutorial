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
public:
    unsigned int id;

    Shader(const char* vertex_path, const char* fragment_path) {
        init(vertex_path, fragment_path);
    }

    void init(const char* vertex_path, const char* fragment_path) {
        const char* v_shader_code;
        const char* f_shader_code;
        std::string vertex_code;
        std::string fragment_code;
        std::ifstream v_shader_file;
        std::ifstream f_shader_file;
        unsigned int vertex, fragment;
        int success;
        char info_log[512];

        //예외 받게 하기
        v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            std::stringstream v_shader_stream, f_shader_stream;

            //파일 열기
            v_shader_file.open(vertex_path);
            f_shader_file.open(fragment_path);

            //stream에 파일 버퍼 내용 읽기
            v_shader_stream << v_shader_file.rdbuf();
            f_shader_stream << f_shader_file.rdbuf();

            //파일 핸들러 닫기
            v_shader_file.close();
            f_shader_file.close();

            //stream을 string으로 변환
            vertex_code = v_shader_stream.str();
            fragment_code = f_shader_stream.str();
        }
        catch (std::ifstream::failure) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
        }

        v_shader_code = vertex_code.c_str();
        f_shader_code = fragment_code.c_str();

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &v_shader_code, NULL);

        glCompileShader(vertex);

        glGetProgramiv(vertex, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(vertex, 512, NULL, info_log);
            std::cout << "ERROR::VERTEX::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
        }

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &f_shader_code, NULL);
        glCompileShader(fragment);

        glGetProgramiv(fragment, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(fragment, 512, NULL, info_log);
            std::cout << "ERROR::FRAGMENT::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
        }

        id = glCreateProgram();

        glAttachShader(id, vertex);
        glAttachShader(id, fragment);
        glLinkProgram(id);

        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(id, 512, NULL, info_log);
            std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << info_log << std::endl;
        }

        //연결 후 제거
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use() const
    {
        glUseProgram(id);
    }

    void set_bool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
    }

    void set_int(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }

    void set_float(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }

    void set_vec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void set_vec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
    }

    void set_vec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void set_vec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
    }

    void set_vec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void set_vec4(const std::string& name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
    }

    void set_mat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void set_mat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void set_mat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    void check_compileErrors(GLuint shader, std::string type)
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
};
#endif