//
// Created by matej on 13.8.2017.
//

#include "Shader.h"
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string &vertex_path, const std::string &fragment_path) {


    shader_program = glCreateProgram();
    shaders[0] = create_shader(load_file(vertex_path), GL_VERTEX_SHADER);
    shaders[1] = create_shader(load_file(fragment_path), GL_FRAGMENT_SHADER);

    // attach
    for (int j = 0; j < SHADER_COUNT; ++j) {
        glAttachShader(shader_program, shaders[j]);
    }

    // link
    glLinkProgram(shader_program);
    check_error(shader_program, GL_LINK_STATUS, true, "Error linking shader program");

    // validate
    glValidateProgram(shader_program);
    check_error(shader_program, GL_LINK_STATUS, true, "Invalid shader program");
}

Shader::~Shader() {
    for (unsigned int shader : shaders) {
        glDetachShader(shader_program, shader);
        glDeleteShader(shader);
    }

    glDeleteProgram(shader_program);
}

void Shader::use() {
    glUseProgram(shader_program);
}

std::string Shader::load_file(const std::string &file_name) {
    std::ifstream file;
    file.open((file_name).c_str());

    std::string output;
    std::string line;

    if (file.is_open()) {
        while (file.good()) {
            getline(file, line);
            output.append(line + "\n");
        }
    } else {
        std::cerr << "Unable to load shader: " << file_name << std::endl;
    }

    return output;
}

GLuint Shader::create_shader(const std::string &text, GLenum type) {
    const GLchar *p[1];
    GLint lengths[1];

    //
    p[0] = text.c_str();
    lengths[0] = text.length();

    GLuint out = glCreateShader(type);

    if (out == 0) {
        std::cerr << "Error compiling shader type " << type << std::endl;
    }

    glShaderSource(out, 1, p, lengths);
    glCompileShader(out);

    check_error(out, GL_COMPILE_STATUS, false, "Error compiling shader");

    return out;
}

void Shader::check_error(GLuint shader, GLuint flag, bool isProgram, const std::string &errorMessage) {
    GLint success = 0;
    GLchar error[1024] = {0};


    if (isProgram) {
        glGetProgramiv(shader, flag, &success);
    } else {
        glGetShaderiv(shader, flag, &success);
    }

    if (success == GL_FALSE) {
        if (isProgram) {
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        } else {
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);
        }
        std::cerr << errorMessage << ": '" << error << "'" << std::endl;
    }
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(shader_program, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, glm::mat4 value) const {
    int loc = glGetUniformLocation(shader_program, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}