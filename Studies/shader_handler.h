#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned int ID;
} Shader;

Shader* shader_create(const char* vertexPath, const char* fragmentPath);
void shader_destroy(Shader* shader);
void shader_use(Shader* shader);
void shader_set_bool(Shader* shader, const char* name, int value);
void shader_set_int(Shader* shader, const char* name, int value);
void shader_set_float(Shader* shader, const char* name, float value);

static char* read_file(const char* filePath);
static void check_compile_errors(unsigned int shader, const char* type);

Shader* shader_create(const char* vertexPath, const char* fragmentPath) {
    Shader* shader = (Shader*)malloc(sizeof(Shader));
    if (!shader) {
        printf("ERROR::SHADER::MEMORY_ALLOCATION_FAILED\n");
        return NULL;
    }

    char* vertexCode = read_file(vertexPath);
    char* fragmentCode = read_file(fragmentPath);
    
    if (!vertexCode || !fragmentCode) {
        printf("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n");
        printf("Vertex file: %s %s\n", vertexPath, vertexCode ? "OK" : "MISSING");
        printf("Fragment file: %s %s\n", fragmentPath, fragmentCode ? "OK" : "MISSING");
        free(vertexCode);
        free(fragmentCode);
        free(shader);
        return NULL;
    }

    unsigned int vertex, fragment;
    int success;
    
    // Vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (const char**)&vertexCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        check_compile_errors(vertex, "VERTEX");
        glDeleteShader(vertex);
        free(vertexCode);
        free(fragmentCode);
        free(shader);
        return NULL;
    }

    // Fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const char**)&fragmentCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        check_compile_errors(fragment, "FRAGMENT");
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        free(vertexCode);
        free(fragmentCode);
        free(shader);
        return NULL;
    }

    // Shader program
    shader->ID = glCreateProgram();
    glAttachShader(shader->ID, vertex);
    glAttachShader(shader->ID, fragment);
    glLinkProgram(shader->ID);
    glGetProgramiv(shader->ID, GL_LINK_STATUS, &success);
    if (!success) {
        check_compile_errors(shader->ID, "PROGRAM");
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(shader->ID);
        free(vertexCode);
        free(fragmentCode);
        free(shader);
        return NULL;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    free(vertexCode);
    free(fragmentCode);
    
    return shader;
}

void shader_destroy(Shader* shader) {
    if (shader) {
        glDeleteProgram(shader->ID);
        free(shader);
    }
}

void shader_use(Shader* shader) {
    if (shader) glUseProgram(shader->ID);
}

void shader_set_bool(Shader* shader, const char* name, int value) {
    if (shader) glUniform1i(glGetUniformLocation(shader->ID, name), value);
}

void shader_set_int(Shader* shader, const char* name, int value) {
    if (shader) glUniform1i(glGetUniformLocation(shader->ID, name), value);
}

void shader_set_float(Shader* shader, const char* name, float value) {
    if (shader) glUniform1f(glGetUniformLocation(shader->ID, name), value);
}

static char* read_file(const char* filePath) {
    FILE* file = fopen(filePath, "rb");  // Binary mode for consistent reading
    if (!file) {
        printf("ERROR::SHADER::FILE_NOT_FOUND: %s\n", filePath);
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    if (fileSize <= 0) {
        printf("ERROR::SHADER::INVALID_FILE_SIZE: %ld\n", fileSize);
        fclose(file);
        return NULL;
    }
    fseek(file, 0, SEEK_SET);
    
    char* content = (char*)malloc(fileSize + 1);
    if (!content) {
        printf("ERROR::SHADER::MEMORY_ALLOCATION_FAILED\n");
        fclose(file);
        return NULL;
    }
    
    size_t bytesRead = fread(content, 1, fileSize, file);
    if (bytesRead != (size_t)fileSize) {
        printf("ERROR::SHADER::READ_INCOMPLETE: %zu/%ld\n", bytesRead, fileSize);
        free(content);
        fclose(file);
        return NULL;
    }
    content[bytesRead] = '\0';
    
    fclose(file);
    return content;
}

static void check_compile_errors(unsigned int shader, const char* type) {
    int success;
    char infoLog[1024];
    
    if (strcmp(type, "PROGRAM") != 0) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n", type, infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            printf("ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n", type, infoLog);
        }
    }
}

#endif
