#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "Shader.h"
#include "Camera.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#define DISPLAY_WIDTH 800
#define DISPLAY_HEIGHT 600

#define MOUSE_SENSITIVITY 0.02f

// ak niektore gl funkcie davaju segfault, mozno je ta funkcia z opengl ktoru som s GLEW neskontroloval ze mam

Camera *camera;
Shader *shader;

GLuint VAO;
GLuint VBO;
GLuint EBO;
GLuint texture_0;
GLuint texture_1;
//glm::mat4 transform;
glm::mat4 projection;
glm::mat4 model;
glm::mat4 view;


float lastTime;

float lastX = DISPLAY_WIDTH / 2;
float lastY = DISPLAY_HEIGHT / 2;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

bool firstMouse = true;

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera->processLook(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera->processZoom(yoffset);
}

void process_input(GLFWwindow *window, float delta) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->processMovement(Camera_Movement::FORWARD, delta);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->processMovement(Camera_Movement::BACKWARD, delta);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->processMovement(Camera_Movement::LEFT, delta);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->processMovement(Camera_Movement::RIGHT, delta);
    }
}

void init() {
    lastTime = 0;

    shader = new Shader("/home/matej/CLionProjects/OpenGLtest/vertex_shader.glsl",
                        "/home/matej/CLionProjects/OpenGLtest/fragment_shader.glsl");

    camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

//    float vertices[] = {
//            // positions          // colors           // texture coords
//            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
//            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom right
//            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // bottom left
//            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f    // top left
//    };

    float vertices[] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    // trojuholnik
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // na poziciu 0 (location = 0) davam data dlhe 3, ktore su floaty, nenormalizuj ich, jedny data maju X bajtov a zacinaj ich citat s 0 offsetom
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
//    glEnableVertexAttribArray(2);

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    // textura
    int texture_width, texture_height, texture_channel_count;
    unsigned char *texture_data = stbi_load("/home/matej/CLionProjects/OpenGLtest/container.jpg",
                                            &texture_width, &texture_height, &texture_channel_count, 0);

    glGenTextures(1, &texture_0);
    glBindTexture(GL_TEXTURE_2D, texture_0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (texture_data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     texture_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(texture_data);

    //
//    stbi_set_flip_vertically_on_load(true);
    texture_data = stbi_load("/home/matej/CLionProjects/OpenGLtest/awesomeface.png",
                             &texture_width, &texture_height, &texture_channel_count, 0);

    glGenTextures(1, &texture_1);
    glBindTexture(GL_TEXTURE_2D, texture_1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (texture_data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     texture_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(texture_data);


    // matrix
//    transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
//    transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));

    // coordinate system
    projection = glm::perspective(glm::radians(45.0f), (float) DISPLAY_WIDTH / (float) DISPLAY_HEIGHT, 0.1f, 100.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
                       glm::vec3(0.0f, 0.0f, 0.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f));
    // camera
//    cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//    cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
//    cameraDirection = glm::normalize(cameraPos - cameraTarget);
//    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
//    cameraRight = glm::normalize(glm::cross(up, cameraDirection));
//    cameraUp = glm::cross(cameraDirection, cameraRight);
//    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//
//    direction = glm::vec3(0.0f, 0.0f, 0.0f);
}

void update(float delta) {
//    direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
//    direction.y = sin(glm::radians(pitch));
//    direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
}

void render() {
    // clear
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();
    shader->setInt("texture_0", 0);
    shader->setInt("texture_1", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_1);

//    unsigned int transform_location = glGetUniformLocation(shader->get_shader_program(), "transform");
//    glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(transform));

//    model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    view = camera->getViewMatrix();
    shader->setMat4("view", view);

    projection = glm::perspective(glm::radians(camera->getZoom()), (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 100.0f);
    shader->setMat4("projection", projection);

    shader->setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);

    // callbacky
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

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
    glEnable(GL_DEPTH_TEST);

    init();

    // spustenie renderovania
    float delta;

    while (!glfwWindowShouldClose(window)) {
        delta = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        // input
        process_input(window, delta);

        // update
        update(delta);

        // render
        render();

        // end
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}