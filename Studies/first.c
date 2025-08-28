#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

void changeColor(GLFWwindow *window) {

	for (float i = 0; i < 1.0f;) {
		
		glClearColor(i, i, i, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		i += 0.001f;
		
		glfwSwapBuffers(window);
		glfwPollEvents();

		sleep(0.25);
	}

}

void processInput(GLFWwindow *window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { glfwSetWindowShouldClose(window, GLFW_TRUE); }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height);
}

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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	
		/* With 'glfwGetProcAddress' we can get the OpenGL function pointer. */

		printf("Failed to initialize glad");
		return -1;
	}

	glViewport(0, 0, 640, 480); // The first two are the left lower corners. The third and fourth are width and height.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	while (!glfwWindowShouldClose(window)) {

		processInput(window);

		// glClearColor(0.2f, 0.f, 0.7f, 1.0f); // Specifying the color that we are going to use after clear.
		// glClear(GL_COLOR_BUFFER_BIT);

		changeColor(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	
	}

	glfwTerminate();
	return 0;
}
