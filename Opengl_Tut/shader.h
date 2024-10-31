#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	unsigned int ID;

	Shader(const GLchar* vertex_path, const GLchar* fragment_path) {
		std::string vertex_code;
		std::string fragment_code;
		std::ifstream vshader_file;
		std::ifstream fshader_file;
		unsigned int vertex, fragment;
		int success;
		char info_log[512];

		//ifstream 객체들이 예외를 던질 수 있도록 함
		vshader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fshader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			vshader_file.open(vertex_path);
			fshader_file.open(fragment_path);
			std::stringstream vshader_stream, fshader_stream;

			//stream 파일의 버퍼 내용 읽기
			vshader_stream << vshader_file.rdbuf();
			fshader_stream << fshader_file.rdbuf();

			//파일 핸들러 닫기
			vshader_file.close();
			fshader_file.close();

			//stream을 string으로 변환
			vertex_code = vshader_stream.str();
			fragment_code = fshader_stream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
		}
		const char* vshader_code = vertex_code.c_str();
		const char* fshader_code = fragment_code.c_str();

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vshader_code, NULL);
		glCompileShader(vertex);

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(vertex, 512, NULL, info_log);
			std::cout << "ERROR::VERTEX::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
		}
		
		fragment = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(fragment, 1, &vshader_code, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(fragment, 512, NULL, info_log);
			std::cout << "ERROR::FRAGMENT::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
		}

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);

		glGetShaderiv(ID, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, info_log);
			std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << info_log << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}

	void use() {
		glUseProgram(ID);
	}
	void set_bool(const std::string &name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void set_int(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void set_float(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);

	}
};

#endif