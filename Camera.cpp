//
// Created by matej on 14.8.2017.
//

#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up,
               float yaw, float pitch)
        : front_(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed_(SPEED), sensitivity_(SENSITIVTY), zoom_(ZOOM) {
    position_ = position;
    worldUp_ = up;
    yaw_ = yaw;
    pitch_ = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
        : front_(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed_(SPEED), sensitivity_(SENSITIVTY), zoom_(ZOOM) {
    position_ = glm::vec3(posX, posY, posZ);
    worldUp_ = glm::vec3(upX, upY, upZ);
    yaw_ = yaw;
    pitch_ = pitch;
    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 tempFront;
    tempFront.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    tempFront.y = sin(glm::radians(pitch_));
    tempFront.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(tempFront);
    right_ = glm::normalize(glm::cross(front_, worldUp_));
    up_ = glm::normalize(glm::cross(right_, front_));
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position_, position_ + front_, up_);
}

void Camera::processMovement(Camera_Movement direction, float delta) {
    float velocity = delta * movementSpeed_;

    switch (direction) {
        case FORWARD:
            position_ += front_ * velocity;
            break;
        case BACKWARD:
            position_ -= front_ * velocity;
            break;
        case LEFT:
            position_ -= right_ * velocity;
            break;
        case RIGHT:
            position_ += right_ * velocity;
            break;
    }
}

void Camera::processLook(float xoffset, float yoffset, bool constrainPitch) {
    // update
    yaw_ += xoffset * sensitivity_;
    pitch_ += yoffset * sensitivity_;

    // constrain
    yaw_ = glm::mod(yaw_, 360.0f);

    if (constrainPitch) {
        if (pitch_ > PITCH_MAX) {
            pitch_ = PITCH_MAX;
        } else if (pitch_ < PITCH_MIN) {
            pitch_ = PITCH_MIN;
        }
    }

    updateCameraVectors();
}

void Camera::processZoom(float offset) {
    zoom_ -= offset;
    if (zoom_ > ZOOM_MAX) {
        zoom_ = ZOOM_MAX;
    } else if (zoom_ < ZOOM_MIN) {
        zoom_ = ZOOM_MIN;
    }
}

const glm::vec3 &Camera::getPosition() const {
    return position_;
}

const glm::vec3 &Camera::getFront() const {
    return front_;
}

const glm::vec3 &Camera::getUp() const {
    return up_;
}

const glm::vec3 &Camera::getRight() const {
    return right_;
}

const glm::vec3 &Camera::getWorldUp() const {
    return worldUp_;
}

float Camera::getYaw() const {
    return yaw_;
}

float Camera::getPitch() const {
    return pitch_;
}

float Camera::getZoom() const {
    return zoom_;
}

float Camera::getSensitivity() const {
    return sensitivity_;
}

float Camera::getMovementSpeed() const {
    return movementSpeed_;
}
