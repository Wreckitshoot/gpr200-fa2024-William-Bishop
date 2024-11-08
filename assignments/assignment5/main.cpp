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
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void ImGuiFunction();

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
glm::vec3 front;

//time
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//lighting
float ambience = 0.5f;
float specularStrength = 0.5f;
float diffuseStrength = 0.3f;
float shininess = 32.0f;
glm::vec3 lightPos(1.0f, 1.0f, 1.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

//debug
bool spinD = true;
float rotationD = 50.0f;


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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}
	//imgui Initialization!
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();



	//Initialization goes here!
	



	Shader backgroundShader("assets/backgroundShader.vs","assets/backgroundShader.fs");
	Shader lightingShader("assets/lightShader.vs","assets/lightShader.fs");
	Shader lampShader("assets/lampShader.vs","assets/lampShader.fs");

	//verticies
	
	float vertices[] = {
	//vertex coordianates: texCoords : normals 
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f
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

	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	

	//vertex coord
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//tex coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//aNormals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//vertex coord
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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
		


		 


		glm::mat4 projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 1000.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		
		//bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture1);
		
		//activate shader
		backgroundShader.use();


		//3d start
		
		int projectionLoc = glGetUniformLocation(backgroundShader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		
		//glm::mat4 view = glm::mat4(1.0f);//fixed view shit
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));//Z+ is towards you
		// note that we're translating the scene in the reverse direction of where we want to move
		
		
		int viewLoc = glGetUniformLocation(backgroundShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		
		


		//3d end


		//render containers
		glBindVertexArray(cubeVAO);
		//container lighting
		int fragAmbience = glGetUniformLocation(backgroundShader.ID, "ambience");
		glUniform3f(fragAmbience, ambience, ambience, ambience);
		int lightPosLoc = glGetUniformLocation(backgroundShader.ID, "lightPos");
		glUniform3f(lightPosLoc, lightPos.x,lightPos.y,lightPos.z);
		int lightColLoc = glGetUniformLocation(backgroundShader.ID, "lightColor");
		glUniform3f(lightColLoc, lightColor.x, lightColor.y, lightColor.z);
		int viewPosLoc = glGetUniformLocation(backgroundShader.ID, "viewPos");
		glUniform3f(viewPosLoc, cameraPos.x,cameraPos.y,cameraPos.z);
		int specularStrengthLoc = glGetUniformLocation(backgroundShader.ID, "specularStrength");
		glUniform1f(specularStrengthLoc, specularStrength);
		int diffuseStrengthLoc = glGetUniformLocation(backgroundShader.ID, "diffuseStrength");
		glUniform1f(diffuseStrengthLoc, diffuseStrength);
		int shininessLoc = glGetUniformLocation(backgroundShader.ID, "shininess");
		glUniform1f(shininessLoc, shininess);


		//position
		int modelLoc = glGetUniformLocation(backgroundShader.ID, "model");
		int scaleLoc = glGetUniformLocation(backgroundShader.ID, "scale");
		for (unsigned int i = 0; i < 20; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			if (spinD) {
				rotationD = (float)glfwGetTime();
			}
			model = glm::rotate(model, rotationD * glm::radians(50.0f), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3f(scaleLoc, i%3 +1 ,i%3 +1,i%3 +1);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		// be sure to activate shader when setting uniforms/drawing objects
		

		//light cube


		lampShader.use();
		lightColLoc = glGetUniformLocation(lampShader.ID, "lightColor");
		glUniform3f(lightColLoc, lightColor.x, lightColor.y, lightColor.z);

		projectionLoc = glGetUniformLocation(lampShader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		viewLoc = glGetUniformLocation(lampShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(9.2f));
		modelLoc = glGetUniformLocation(lampShader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);



		ImGuiFunction();

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

	
	
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		ambience += deltaTime;
		if (ambience > 1.0) {
			ambience = 1.0;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		ambience -= deltaTime;
		if (ambience < 0.01) {
			ambience = 0.01;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		spinD = true;
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		spinD = false;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		lightPos = cameraPos;
	}
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

	
	
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstMouse = true;
	}
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
void ImGuiFunction() {
	//ImGUI
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	//Window Text
	ImGui::Begin("Settings");
	ImGui::Text("Stop/Start Spin N and M");
	ImGui::Text("L: Move Light to Camera");
	ImGui::DragFloat3("Light Position", &lightPos.x, 0.1f);
	ImGui::ColorEdit3("Light Color", &lightColor.r);
	ImGui::SliderFloat("Ambience", &ambience, 0.0f, 1.0f);
	ImGui::SliderFloat("Diffuse Strength", &diffuseStrength, 0.0f, 1.0f);
	ImGui::SliderFloat("Specular Strength", &specularStrength, 0.0f, 1.0f);
	ImGui::SliderFloat("Shininess", &shininess, 2.0f, 1024.0f);

	ImGui::End();

	//Ambience


	//Render the Window
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}