#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>
#include <cglm/cglm.h>
#include <glad/glad.h>
#include <string.h>

typedef struct Shader{
    unsigned int ID;
    
} Shader;

Shader* shader_create(const char*, const char*);

void shader_use    (Shader* shader);
void shader_destroy(Shader* shader);

void shader_setBool (Shader* shader, const char* name, bool  value);
void shader_setInt  (Shader* shader, const char* name, int   value);
void shader_setFloat(Shader* shader, const char* name, float value);
void shader_setMat4 (Shader* shader, const char* name, mat4  value);

void shader_destroy(Shader* shader) {
    if (shader) {
        glDeleteProgram(shader->ID);
        free(shader);
    }
}

void shader_use(Shader* shader) {
    glUseProgram(shader->ID);
}

void shader_setBool(Shader* shader, const char* name, bool value) {
    glUniform1i(glGetUniformLocation(shader->ID, name), value);
}

void shader_setInt(Shader* shader, const char* name, int value) {
    glUniform1i(glGetUniformLocation(shader->ID, name), value);
}

void shader_setFloat(Shader* shader, const char* name, float value) {
    glUniform1f(glGetUniformLocation(shader->ID, name), value);
}

void shader_setMat4(Shader* shader, const char* name, mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, name), 1, GL_FALSE, (float*)value);
}


static char* read_file(const char* filePath) {
    FILE* file = fopen(filePath, "rb");
    if (!file) {
        fprintf(stderr, "ERROR:SHADER:FILE_NOT_FOUND: %s\n", filePath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(length + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    fclose(file);

    return buffer;
}

void shader_checkCompileErrors(unsigned int shader, const char *type) {
    int success;
    char infoLog[1024];
    
    if (strcmp(type, "PROGRAM") != 0) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            fprintf(stderr, "ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n -- --------------------------------------------------- -- \n", type, infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            fprintf(stderr, "ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n -- --------------------------------------------------- -- \n", type, infoLog);
        }
    }
}

Shader* shader_create(const char* vertexPath, const char* fragmentPath) {
    
    char* vertexCode = read_file(vertexPath);
    char* fragmentCode = read_file(fragmentPath);
    
    if (!vertexCode || !fragmentCode) {
        free(vertexCode);
        free(fragmentCode);
        return NULL;
    }
    
    unsigned int vertex, fragment;
    
    // Vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (const char* const*)&vertexCode, NULL);
    glCompileShader(vertex);
    shader_checkCompileErrors(vertex, "VERTEX");
    
    // Fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const char* const*)&fragmentCode, NULL);
    glCompileShader(fragment);
    shader_checkCompileErrors(fragment, "FRAGMENT");
    
    free(vertexCode);
    free(fragmentCode);
    
    // Crear programa shader
    Shader* shader = (Shader*)malloc(sizeof(Shader));
    if (!shader) {
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return NULL;
    }
    
    shader->ID = glCreateProgram();
    glAttachShader(shader->ID, vertex);
    glAttachShader(shader->ID, fragment);
    glLinkProgram(shader->ID);
    shader_checkCompileErrors(shader->ID, "PROGRAM");
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    return shader;
}
#endif // !SHADER_H
