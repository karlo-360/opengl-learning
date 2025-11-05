#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>
#include <glad/glad.h>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float SPEED       = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM        = 90.0f;

typedef struct Camera {
    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
    vec3 WorldUp;

    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

} Camera;

Camera* camera_create_v(vec3 position, vec3 up, float yaw, float pitch);
Camera* camera_create_s(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
void processKeyboard(Camera* cam, enum Camera_Movement direction, float deltaTime);
void processMouse   (Camera* cam, float xoffset, float yoffset, GLboolean constrainPitch);
void processScroll  (Camera* cam, float yoffset);
void GetViewMatrix  (Camera* cam, mat4 dest);

void updateCameraVectors(Camera* cam){
    vec3 front;

    front[0] = cos(glm_rad(cam->Yaw)) * cos(glm_rad(cam->Pitch));
    front[1] = sin(glm_rad(cam->Pitch));
    front[2] = sin(glm_rad(cam->Yaw)) * cos(glm_rad(cam->Pitch));

    glm_vec3_normalize(front);
    glm_vec3_copy(front, cam->Front);

    vec3 right;
    glm_vec3_cross(cam->Front, cam->WorldUp, right);
    glm_vec3_normalize(right);
    glm_vec3_copy(right, cam->Right);

    vec3 up;
    glm_vec3_cross(cam->Right, cam->Front, up);
    glm_vec3_normalize(up);
    glm_vec3_copy(up, cam->Up);
}

Camera* camera_create_v(vec3 position, vec3 up, float yaw, float pitch) {

    Camera* cam = (Camera*)malloc(sizeof(Camera));
    
    glm_vec3_copy(position, cam->Position);
    glm_vec3_copy(up, cam->WorldUp);
    cam->Yaw = yaw;
    cam->Pitch = pitch;

    cam->MovementSpeed = SPEED;
    cam->MouseSensitivity = SENSITIVITY;
    cam->Zoom = ZOOM;

    vec3 defaultFront = {0.0f, 0.0f, -1.0f};
    glm_vec3_copy(defaultFront, cam->Front);

    updateCameraVectors(cam);
    return cam;
}

Camera* camera_create_s(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) {

    Camera* cam = (Camera*)malloc(sizeof(Camera));

    vec3 position = {posX, posY, posZ};
    vec3 worldUp = {upX, upY, upZ};

    glm_vec3_copy(position, cam->Position);
    glm_vec3_copy(worldUp, cam->WorldUp);
    cam->Yaw = yaw;
    cam->Pitch = pitch;

    cam->MovementSpeed = SPEED;
    cam->MouseSensitivity = SENSITIVITY;
    cam->Zoom = ZOOM;

    vec3 defaultFront = {0.0f, 0.0f, -1.0f};
    glm_vec3_copy(defaultFront, cam->Front);

    updateCameraVectors(cam);

    return cam;
}

void processKeyboard(Camera* cam, enum Camera_Movement direction, float deltaTime) {

    float velocity = cam->MovementSpeed * deltaTime;

    if (direction == FORWARD)
        glm_vec3_muladds(cam->Front, velocity, cam->Position);
    if (direction == BACKWARD)
        glm_vec3_muladds(cam->Front, -velocity, cam->Position);
    if (direction == LEFT)
        glm_vec3_muladds(cam->Right, -velocity, cam->Position);
    if (direction == RIGHT)
        glm_vec3_muladds(cam->Right, velocity, cam->Position);
}

void processMouse(Camera *cam, float xoffset, float yoffset, GLboolean constrainPitch) {

    xoffset *= cam->MouseSensitivity;
    yoffset *= cam->MouseSensitivity;

    cam->Yaw   += xoffset;
    cam->Pitch += yoffset;

    if (constrainPitch) {
        if (cam->Pitch > 89.0f)
            cam->Pitch = 89.0f;

        if (cam->Pitch < -89.0f)
            cam->Pitch = -89.0f;
    }

    updateCameraVectors(cam);
}

void processScroll(Camera *cam, float yoffset) {
    cam->Zoom -= yoffset;
    if (cam->Zoom < 1.0f)
        cam->Zoom = 1.0f;
    if (cam->Zoom > 90.0f)
        cam->Zoom = 90.0f;
}

void GetViewMatrix(Camera* cam, mat4 dest){

    vec3 target;
    glm_vec3_add(cam->Position, cam->Front, target);
    glm_lookat(
        cam->Position,
        target,
        cam->Up,
        dest
    );
}

#endif
