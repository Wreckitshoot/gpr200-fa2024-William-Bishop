#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "bob/Shader.h"
#include "bob/stb_image.h"

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

//camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 60.0f;

//time
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

int main() {

	

	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}
	//Initialization goes here!
	
	Shader backgroundShader("assets/backgroundShader.vs","assets/backgroundShader.fs");

	//verticies
	
	float vertices[] = {
	//vertex coordianates: texCoords 
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	//3d cube positions
	glm::vec3 cubePositions[] = {
		glm::vec3(ew::RandomRange(-5.0f,5.0f),ew::RandomRange(-5.0f,5.0f), ew::RandomRange(-5.0f,5.0f)),
		glm::vec3(ew::RandomRange(-5.0f,5.0f),ew::RandomRange(-5.0f,5.0f), ew::RandomRange(-5.0f,5.0f)),
		glm::vec3(ew::RandomRange(-5.0f,5.0f),ew::RandomRange(-5.0f,5.0f), ew::RandomRange(-5.0f,5.0f)),
		glm::vec3(ew::RandomRange(-5.0f,5.0f),ew::RandomRange(-5.0f,5.0f), ew::RandomRange(-5.0f,5.0f)),
		glm::vec3(ew::RandomRange(-5.0f,5.0f),ew::RandomRange(-5.0f,5.0f), ew::RandomRange(-5.0f,5.0f)),
		glm::vec3(ew::RandomRange(-5.0f,5.0f),ew::RandomRange(-5.0f,5.0f), ew::RandomRange(-5.0f,5.0f)),
		glm::vec3(ew::RandomRange(-5.0f,5.0f),ew::RandomRange(-5.0f,5.0f), ew::RandomRange(-5.0f,5.0f)),
		glm::vec3(ew::RandomRange(-5.0f,5.0f),ew::RandomRange(-5.0f,5.0f), ew::RandomRange(-5.0f,5.0f)),
		glm::vec3(ew::RandomRange(-5.0f,5.0f),ew::RandomRange(-5.0f,5.0f), ew::RandomRange(-5.0f,5.0f)),
		glm::vec3(ew::RandomRange(-5.0f,5.0f),ew::RandomRange(-5.0f,5.0f), ew::RandomRange(-5.0f,5.0f)),
		glm::vec3(ew::RandomRange(-5.0f,5.0f),ew::RandomRange(-5.0f,5.0f), ew::RandomRange(-5.0f,5.0f)),
		glm::vec3(ew::RandomRange(-5.0f,5.0f),ew::RandomRange(-5.0f,5.0f), ew::RandomRange(-5.0f,5.0f)),
		glm::vec3(ew::RandomRange(-5.0f,5.0f),ew::RandomRange(-5.0f,5.0f), ew::RandomRange(-5.0f,5.0f)),
		glm::vec3(ew::RandomRange(-5.0f,5.0f),ew::RandomRange(-5.0f,5.0f), ew::RandomRange(-5.0f,5.0f)),
		glm::vec3(ew::RandomRange(-5.0f,5.0f),ew::RandomRange(-5.0f,5.0f), ew::RandomRange(-5.0f,5.0f)),
		glm::vec3(ew::RandomRange(-5.0f,5.0f),ew::RandomRange(-5.0f,5.0f), ew::RandomRange(-5.0f,5.0f)),
		glm::vec3(ew::RandomRange(-5.0f,5.0f),ew::RandomRange(-5.0f,5.0f), ew::RandomRange(-5.0f,5.0f)),
		glm::vec3(ew::RandomRange(-5.0f,5.0f),ew::RandomRange(-5.0f,5.0f), ew::RandomRange(-5.0f,5.0f)),
		glm::vec3(ew::RandomRange(-5.0f,5.0f),ew::RandomRange(-5.0f,5.0f), ew::RandomRange(-5.0f,5.0f)),
		glm::vec3(ew::RandomRange(-5.0f,5.0f),ew::RandomRange(-5.0f,5.0f), ew::RandomRange(-5.0f,5.0f)),
		
	};

	//camera position
	///Gram-Schmidt proccess
	///glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	///glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	///glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	///glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	///glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	///glm::vec3 cameraLeft = glm::normalize(glm::cross(cameraDirection,up));
	///glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	

	//Assignment 1 

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	///end of assignment 1

	
	//wall
	unsigned int texture0;
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("assets/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, texture0);



	//awesome face
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("assets/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);
	glActiveTexture(GL_TEXTURE1); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, texture1);
	

	backgroundShader.use();
	glUniform1i(glGetUniformLocation(backgroundShader.ID, "texture0"), 0);
	backgroundShader.setInt("texture1", 1);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	//enable depth
	glEnable(GL_DEPTH_TEST);

	
	//Render loop
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		
		
		processInput(window);

		//Clear framebuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		//Drawing happens here!
		
		
		
		
		//bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture1);
		//activate shader
		backgroundShader.use();

		//3d start
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 1000.0f);
		int projectionLoc = glGetUniformLocation(backgroundShader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		
		//glm::mat4 view = glm::mat4(1.0f);//fixed view shit
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));//Z+ is towards you
		// note that we're translating the scene in the reverse direction of where we want to move
		
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		int viewLoc = glGetUniformLocation(backgroundShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		
		


		//3d end



		//render containers
		glBindVertexArray(VAO);
		int modelLoc = glGetUniformLocation(backgroundShader.ID, "model");
		int scaleLoc = glGetUniformLocation(backgroundShader.ID, "scale");
		for (unsigned int i = 0; i < 20; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3f(scaleLoc, i%3 +1 ,i%3 +1,i%3 +1);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//swap graphics buffers
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}
void processInput(GLFWwindow* window) {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	std::cout << deltaTime << std::endl;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float sprint = 1;
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		sprint = 2;

	float cameraSpeed = 2.5f * deltaTime * sprint;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, glm::cross(cameraFront, cameraUp))) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, glm::cross(cameraFront, cameraUp))) * cameraSpeed;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 120.0f)
		fov = 120.0f;
}