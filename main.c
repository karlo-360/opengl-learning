#include "libs/camera.h"
#include "libs/shader_s.h"
#include "libs/stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <stdbool.h>

#include "glad.c"
#include "stb_image_impl.c"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT= 600;

vec3 cameraPos = {0.0f, 0.0f,  3.0f};
vec3 cameraUp  = {0.0f, 1.0f,  0.0f};

Camera* cam;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = (float)WIDTH / 2;
float lastY = (float)HEIGHT/ 2;
float yaw;
float pitch;

int main(void) {

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Holiwi UWU", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW Window");
        glfwTerminate();
        return -1;
    }
    printf("Ventana Abierta\n");

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("Failed to initialize GLAD");
        return -1;
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);

    cam = camera_create_v(cameraPos, cameraUp, -90.0f, 0.0f);
    if (!cam) {
        fprintf(stderr, "Failed to create camera\n");
        return -1;
    }

    Shader* myShader = shader_create("shaders/vertex.vs", "shaders/fragment.fs");
    if (!myShader) {
        fprintf(stderr, "Failed to create shader\n");
        return -1;
    }
    
    Shader* lightShader = shader_create("shaders/light_vertex.vs", "shaders/light_fragment.fs");
    if (!lightShader) {
        fprintf(stderr, "Failed to create light shader\n");
        return -1;
    }
        
    float vertices[] = {
        //frente
        -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 0.0f,
         0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,

        //abajo
        -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 0.0f,
         1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 1.0f,

        //derecha
         1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
         0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,

        //izquierda
        -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
         0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    };
    
    vec3 notCubes[] = {
        { 0.0f,  0.0f,  0.0f }, 
        { 2.0f,  5.0f, -15.0f}, 
        {-1.5f, -2.2f, -2.5f },  
        {-3.8f, -2.0f, -12.3f},  
        { 2.4f, -0.4f, -3.5f },  
        {-1.7f,  3.0f, -7.5f },  
        { 1.3f, -2.0f, -2.5f },  
        { 1.5f,  2.0f, -2.5f }, 
        { 1.5f,  0.2f, -1.5f }, 
        {-1.3f,  1.0f, -1.5f }  
    };

    vec3 lightColor = {1.0f, 0.0f, 0.0f};
    vec3 lightPos   = {1.0f, 1.0f, 1.0f};

    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    while (!glfwWindowShouldClose(window)) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_use(myShader);
        shader_setVec3(myShader, "lightColor", lightColor);

        mat4 projection;
        glm_perspective(glm_rad(cam->Zoom), 800.0f/600.0f, 0.1f, 100.0f, projection);
        shader_setMat4(myShader, "projection", projection);

        mat4 view;
        get_view_matrix(cam, view);
        shader_setMat4(myShader, "view", view);

        glBindVertexArray(VAO);

        for (unsigned int i = 0; i < 10; i++){
            mat4 model;
            glm_mat4_identity(model);
            glm_translate(model, notCubes[i]);
            float angle = 20.0f * i;
            glm_rotate(model, glm_rad(angle), (vec3){1.0f, 0.3f, 0.5f});
            shader_setMat4(myShader, "model", model);

            glDrawArrays(GL_TRIANGLES, 0, 12);
        }
        
        shader_use(lightShader);
        shader_setMat4(lightShader, "projection", projection);
        shader_setMat4(lightShader, "view", view);
        mat4 model;
        glm_mat4_identity(model);
        glm_translate(model, lightPos);
        shader_setMat4(lightShader, "model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    printf("Ventana Cerrada\n");

    shader_destroy(myShader);
    shader_destroy(lightShader);
    camera_destroy(cam);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window){

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        process_keyboard(cam, FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        process_keyboard(cam, BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        process_keyboard(cam, LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        process_keyboard(cam, RIGHT, deltaTime);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
} 

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        //sign = sign * -1;
    }

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    process_mouse(cam, xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    process_scroll(cam, yoffset);
}
