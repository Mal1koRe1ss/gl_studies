#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include "shader_handler.h"

void processInput(GLFWwindow *window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { glfwSetWindowShouldClose(window, GLFW_TRUE); }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height);
}

float vertices[] = {
	0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom right
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom left
	0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f // Top
};

unsigned int VBO; // Vertex buffer objects.
unsigned int VAO; // Vertex array object.
unsigned int shaderProgram;

int main(void) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "learnopengl.com", NULL, NULL);

	if (window == NULL) {
		printf("Failed to create GLFW window.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	
		/* With 'glfwGetProcAddress' we can get the OpenGL function pointer. */

		printf("Failed to initialize glad");
		return -1;
	}

	Shader* myShader = shader_create("./vertex.vs", "./fragment.fs");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {

		processInput(window);

		glClearColor(0.07f, 0.07f, 0.07f, 1.0f); // Specifying the color that we are going to use after clear.
		glClear(GL_COLOR_BUFFER_BIT);

		shader_use(myShader);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	shader_destroy(myShader);
	glfwTerminate();
	return 0;
}
