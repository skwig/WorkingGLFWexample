//
// Created by matej on 13.8.2017.
//

#ifndef PROJECT_SHADER_H
#define PROJECT_SHADER_H


#include <GL/glew.h>
#include <iostream>

class Shader {
public:
    Shader(const std::string &vertex_path, const std::string &fragment_path);

    virtual ~Shader();

    void use();

    void setInt(const std::string &name, int value) const;

protected:

private:
    static const int SHADER_COUNT = 2;
    GLuint shader_program;
    GLuint shaders[SHADER_COUNT];

    std::string load_file(const std::string &file_name);

    GLuint create_shader(const std::string &text, GLenum type);

    void check_error(GLuint shader, GLuint flag, bool isProgram, const std::string &errorMessage);
};


#endif //PROJECT_SHADER_H
