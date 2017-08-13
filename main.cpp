#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "Shader.h"

#define DISPLAY_WIDTH 800
#define DISPLAY_HEIGHT 600

// ak niektore gl funkcie davaju segfault, mozno je ta funkcia z opengl ktoru som s GLEW neskontroloval ze mam

Shader *shader;
GLuint VAO;
GLuint VBO;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void init() {
    shader = new Shader("/home/matej/CLionProjects/OpenGLtest/test_shader.glsl",
                        "/home/matej/CLionProjects/OpenGLtest/test2_shader.glsl");

    float vertices_color[] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
            0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // bottom left
    };

    // trojuholnik
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_color), vertices_color, GL_STATIC_DRAW);

    // na poziciu 0 (location = 0) davam data dlhe 3, ktore su floaty, nenormalizuj ich, jedny data maju X bajtov a zacinaj ich citat s 0 offsetom
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * 3 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void update() {

}

void render() {
    // clear
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main() {
    // glfw init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(DISPLAY_WIDTH, DISPLAY_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // callbacky
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glew init
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "Status: OpenGL 3.3 supported=" << (GLEW_VERSION_3_3 != 0) << std::endl;

    // open gl init
    // TODO: zmensit viewport, nech sa mi tam vojde aj GUI
    glViewport(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);

    init();

    // spustenie renderovania
    while (!glfwWindowShouldClose(window)) {
        // input
        process_input(window);

        // update
        update();

        // render
        render();

        // end
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}