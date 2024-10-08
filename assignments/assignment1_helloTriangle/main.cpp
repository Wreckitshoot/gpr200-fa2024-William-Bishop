#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "bob/Shader.h"

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

int main() {

	

	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Assignment 2", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}
	//Initialization goes here!
	
	Shader theShader("assets/shader.vs","assets/shader.fs");

	//vertex shader
	
	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);


	

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//Drawing happens here!
		theShader.use();

		float timeValue = 2*glfwGetTime();
		float colorValue = abs((sin(timeValue) / 2.0f) + 0.5f);
		float positionValue = (sin(timeValue) / 2.0f);
		float positionValue2 = (cos(timeValue)/ 2.0f);
		int vertexColorLocation = glGetUniformLocation(theShader.ID, "colorEffect");
		int vertexPositionLocation = glGetUniformLocation(theShader.ID, "positionEffect");
		glUniform3f(vertexColorLocation, colorValue, colorValue, colorValue);
		glUniform3f(vertexPositionLocation, 0, positionValue/3, positionValue);
		

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		float vertices[] = {
			// positions         // colors
			 0.5f, -0.5f + (positionValue / 3), 0.0f,  0.0f, 1.0f, 0.0f,   // bottom right
			-0.5f, -0.5f + (positionValue2 / 3), 0.0f,  1.0f, 0.0f, 0.0f,   // bottom left
			 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
		};
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}
