#include <iostream>
#include <thread>
#include <vector>
//#include <cstdio>
//#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>
#include "data.h"
#include "Camera.h"
#include "Shader.h"
#include "utils.h"

int main()
{
	GLFWwindow* window;
	const int WIDTH = 800, HEIGHT = 600;
	char* WINDOW_NAME = "OpenGL - basic stuff";
	initContext();
	window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_NAME, nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	initGLEW();

	// Create shader programs
	Shader sceneShader("shader/basic.vert", "shader/basic.frag");
	Shader lampShader("shader/lamp.vert", "shader/lamp.frag");

	// Create VAOs
	GLuint vaoCube;
	glGenVertexArrays(1, &vaoCube);

	// Load vertex data
	GLuint vboCube;
	glGenBuffers(1, &vboCube);

	glBindVertexArray(vaoCube);

	glBindBuffer(GL_ARRAY_BUFFER, vboCube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	GLuint vaoLight;
	glGenVertexArrays(1, &vaoLight);
	glBindVertexArray(vaoLight);
	glBindBuffer(GL_ARRAY_BUFFER, vboCube);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// Load textures
	GLuint diffuseMap = loadTextureMap("res/container2.png");
	GLuint specularMap = loadTextureMap("res/container2_specular.png");

	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		Do_Movement();
	
		// Bind our framebuffer and draw 3D scene(spinning cube)
		glEnable(GL_DEPTH_TEST);
		glUseProgram(sceneShader.Program);


		// Clear the screen to white
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLint viewPosLoc = glGetUniformLocation(sceneShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

		// Directional light
		glUniform3f(glGetUniformLocation(sceneShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(sceneShader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(sceneShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(sceneShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
		// Point light 1
		glUniform3f(glGetUniformLocation(sceneShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(sceneShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(sceneShader.Program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(sceneShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(sceneShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(sceneShader.Program, "pointLights[0].linear"), 0.09);
		glUniform1f(glGetUniformLocation(sceneShader.Program, "pointLights[0].quadratic"), 0.032);
		// Point light 2
		glUniform3f(glGetUniformLocation(sceneShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(sceneShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(sceneShader.Program, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(sceneShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(sceneShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(sceneShader.Program, "pointLights[1].linear"), 0.09);
		glUniform1f(glGetUniformLocation(sceneShader.Program, "pointLights[1].quadratic"), 0.032);
		// Point light 3
		glUniform3f(glGetUniformLocation(sceneShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(sceneShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(sceneShader.Program, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(sceneShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(sceneShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(sceneShader.Program, "pointLights[2].linear"), 0.09);
		glUniform1f(glGetUniformLocation(sceneShader.Program, "pointLights[2].quadratic"), 0.032);
		// Point light 4
		glUniform3f(glGetUniformLocation(sceneShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(sceneShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(sceneShader.Program, "pointLights[3].diffuse"), 0.8f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(sceneShader.Program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(sceneShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(sceneShader.Program, "pointLights[3].linear"), 0.09);
		glUniform1f(glGetUniformLocation(sceneShader.Program, "pointLights[3].quadratic"), 0.032);
		// SpotLight
		glUniform3f(glGetUniformLocation(sceneShader.Program, "spotLight.position"), camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform3f(glGetUniformLocation(sceneShader.Program, "spotLight.direction"), camera.Front.x, camera.Front.y, camera.Front.z);
		glUniform3f(glGetUniformLocation(sceneShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(sceneShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(sceneShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(sceneShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(sceneShader.Program, "spotLight.linear"), 0.09);
		glUniform1f(glGetUniformLocation(sceneShader.Program, "spotLight.quadratic"), 0.032);
		glUniform1f(glGetUniformLocation(sceneShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(sceneShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		GLfloat time = (GLfloat)glfwGetTime();
		GLint timer = glGetUniformLocation(sceneShader.Program, "timer");
		//glUniform1f(timer, time);
		time = 0;
		glm::mat4 viewMatrix;
		viewMatrix = camera.GetViewMatrix();
		glm::mat4 projMatrix = glm::perspective(camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

		GLint uniModel = glGetUniformLocation(sceneShader.Program, "model");
		GLint uniView = glGetUniformLocation(sceneShader.Program, "view");
		GLint uniProj = glGetUniformLocation(sceneShader.Program, "proj");

		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(projMatrix));

		// Set diffuse map
		glUniform1f(glGetUniformLocation(sceneShader.Program, "material.diffuse"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// Set specular map
		glUniform1i(glGetUniformLocation(sceneShader.Program, "material.specular"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		// Set other material properties
		glUniform1f(glGetUniformLocation(sceneShader.Program, "material.shininess"), 64.0f);

		glm::mat4 modelMatrix;
		glBindVertexArray(vaoCube);
		for (GLuint i = 0; i < 10; i++)
		{
			modelMatrix = glm::mat4();
			modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
			GLfloat angle = 20.0f * i;
			modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glUseProgram(lampShader.Program);
		uniModel = glGetUniformLocation(lampShader.Program, "model");
		uniView = glGetUniformLocation(lampShader.Program, "view");
		uniProj = glGetUniformLocation(lampShader.Program, "proj");
		// Set matrices
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(projMatrix));

		// We now draw as many light bulbs as we have point lights.
		glBindVertexArray(vaoLight);
		for (GLuint i = 0; i < 4; i++)
		{
			modelMatrix = glm::mat4();
			modelMatrix = glm::translate(modelMatrix, pointLightPositions[i]);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f)); // Make it a smaller cube
			glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}

	glDeleteTextures(1, &diffuseMap);
	glDeleteTextures(1, &specularMap);

	glDeleteBuffers(1, &vboCube);

	glDeleteVertexArrays(1, &vaoCube);

	glfwTerminate();
}