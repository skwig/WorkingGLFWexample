//
// Created by matej on 14.8.2017.
//

#ifndef PROJECT_CAMERA_H
#define PROJECT_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVTY = 0.02f;
const float ZOOM = 45.0f;

const float PITCH_MAX = 90.0f;
const float PITCH_MIN = -90.0f;

const float ZOOM_MAX = 45.0f;
const float ZOOM_MIN = 1.0f;

class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                   float yaw = YAW, float pitch = PITCH);

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    glm::mat4 getViewMatrix();

    void processMovement(Camera_Movement direction, float delta);

    void processLook(float xoffset, float yoffset, bool constrainPitch = true);

    void processZoom(float yoffset);

    const glm::vec3 &getPosition() const;

    const glm::vec3 &getFront() const;

    const glm::vec3 &getUp() const;

    const glm::vec3 &getRight() const;

    const glm::vec3 &getWorldUp() const;

    float getYaw() const;

    float getPitch() const;

    float getZoom() const;

    float getSensitivity() const;

    float getMovementSpeed() const;

protected:
private:
    glm::vec3 position_;

    // helper vektory na pohyb
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;

    glm::vec3 worldUp_;

    float yaw_;
    float pitch_;

    float zoom_;
    float sensitivity_;
    float movementSpeed_;

    void updateCameraVectors();
};


#endif //PROJECT_CAMERA_H
