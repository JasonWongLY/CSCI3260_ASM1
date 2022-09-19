/*
Type your name and student ID here
	- Name:
	- Student ID:
*/

#include "Dependencies/glew/glew.h"
#include "Dependencies/GLFW/glfw3.h"

#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>


GLint programID;
float x_delta = 0.1f;
int x_press_num = 0;
float r_delta = 0.1f;
int r_press_num = 0;

void get_OpenGL_info() {
	// OpenGL information
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID) {
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID) {
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

std::string readShaderCode(const char* fileName) {
	std::ifstream meInput(fileName);
	if (!meInput.good()) {
		std::cout << "File failed to load ... " << fileName << std::endl;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>()
	);
}

void installShaders() {
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	//adapter[0] = vertexShaderCode;
	std::string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	//adapter[0] = fragmentShaderCode;
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}

GLuint groundVaoID, groundVboID;
GLuint snorlaxVaoID, snorlaxVboID, snorlaxIndicesVboID;
GLuint snorlaxFrontVaoID, snorlaxFrontVboID, snorlaxFrontIndicesVboID;
void sendDataToOpenGL() {
	// TODO:
	// create 3D objects and/or 2D objects and/or lines (points) here and bind to VAOs & VBOs
	const GLfloat ground[] =
	{
		/*-0.5f, 0.0f, 0.0f,		+0.455f, +0.4f, +0.23f,
		-1.0f, -0.5f, 0.0f,		+0.455f, +0.4f, +0.23f,
		1.0f, -0.5f, 0.0f,		+0.455f, +0.4f, +0.23f,
								   				   
		1.0f, -0.5f, 0.0f,		+0.455f, +0.4f, +0.23f,
		0.5f, 0.0f, 0.0f,       +0.455f, +0.4f, +0.23f,
		-0.5f, 0.0f, 0.0f,		+0.455f, +0.4f, +0.23f,
								   				   
		-1.0f, -0.5f, 0.0f,		+0.455f, +0.4f, +0.23f,
		-1.0f, -1.0f, 0.0f,		+0.455f, +0.4f, +0.23f,
		1.0f, -1.0f, 0.0f,		+0.455f, +0.4f, +0.23f,
								   				   
		1.0f, -1.0f, 0.0f,		+0.455f, +0.4f, +0.23f,
		1.0f, -0.5f, 0.0f,		+0.455f, +0.4f, +0.23f,
		-1.0f, -0.5f, 0.0f,		+0.455f, +0.4f, +0.23f,*/

		-1.0f, +0.0f, -1.0f,      +0.2f, +0.2f, +0.3f,
		-1.0f, +0.0f, +1.0f,      +0.52f, +0.37f, +0.26f,
		+1.0f, +0.0f, -1.0f,      +0.2f, +0.2f, +0.3f,
		-1.0f, +0.0f, +1.0f,      +0.52f, +0.37f, +0.26f,
		+1.0f, +0.0f, +1.0f,      +0.52f, +0.37f, +0.26f,
		+1.0f, +0.0f, -1.0f,      +0.2f, +0.2f, +0.3f,
	};

	
	glGenVertexArrays(1, &groundVaoID);
	glBindVertexArray(groundVaoID);
	glGenBuffers(1, &groundVboID);
	glBindBuffer(GL_ARRAY_BUFFER, groundVboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

	
	const float snorlaxhead = 0.85f;
	const GLfloat snorlax[] =
	{
		-1.0f, +1.5f, -1.0f,  +0.0608f, +0.251f, +0.3608f,	//0
		-1.0f, -1.0f, -1.0f,  +0.1608f, +0.451f, +0.5608f,	//1
		+1.0f, +1.5f, -1.0f,  +0.1608f, +0.451f, +0.5608f,	//2
		+1.0f, -1.0f, -1.0f,  +0.1608f, +0.451f, +0.5608f,	//3
		-1.0f, -1.0f, +1.0f,  +0.1608f, +0.451f, +0.5608f,	//4
		+1.0f, -1.0f, +1.0f,  +0.0608f, +0.251f, +0.3608f,	//5
		+1.0f, +1.5f, +1.0f,  +0.1608f, +0.451f, +0.5608f,	//6
		-1.0f, +1.5f, +1.0f,  +0.1608f, +0.451f, +0.5608f,	//7

		-snorlaxhead, +snorlaxhead + 2.0f, -snorlaxhead, +0.0608f, +0.251f, +0.3608f,	//8
		-snorlaxhead, -1.0f + 2.5f, -snorlaxhead,		+0.1608f, +0.451f, +0.5608f,	//9
		+snorlaxhead, +snorlaxhead + 2.0f, -snorlaxhead, +0.1608f, +0.451f, +0.5608f,	//10
		+snorlaxhead, -1.0f + 2.5f, -snorlaxhead,		+0.1608f, +0.451f, +0.5608f,	//11
		-snorlaxhead, -1.0f + 2.5f, +snorlaxhead,		+0.1608f, +0.451f, +0.5608f,	//12
		+snorlaxhead, -1.0f + 2.5f, +snorlaxhead,		+0.0608f, +0.251f, +0.3608f,	//13
		+snorlaxhead, +snorlaxhead + 2.0f, +snorlaxhead, +0.1608f, +0.451f, +0.5608f,	//14
		-snorlaxhead, +snorlaxhead + 2.0f, +snorlaxhead, +0.1608f, +0.451f, +0.5608f,	//15

		
		-0.8f, +1.5f, +1.01f,   +0.945f, +0.898f, +0.847f,	
		+0.8f, +1.51f, +1.01f,   +0.945f, +0.898f, +0.847f,	
		-0.8f, -0.7f, +1.01f,   +0.945f, +0.898f, +0.847f,	
		+0.8f, -0.7f, +1.01f,   +0.945f, +0.898f, +0.847f,	
		-0.8f, +1.05f, +0.85f,  +0.945f, +0.898f, +0.847f,	
		+0.8f, +1.05f, +0.85f,  +0.945f, +0.898f, +0.847f,	

		-snorlaxhead+0.15f, -1.0f + 2.5f, +snorlaxhead + 0.01f,		+0.945f, +0.898f, +0.847f,	
		+snorlaxhead-0.15f, -1.0f + 2.5f, +snorlaxhead + 0.01f,		+0.945f, +0.898f, +0.847f,	
		+snorlaxhead-0.15f, +snorlaxhead + 1.9f, +snorlaxhead + 0.02f,	+0.945f, +0.898f, +0.847f,	
		-snorlaxhead+0.15f, +snorlaxhead + 1.9f, +snorlaxhead + 0.02f,	+0.945f, +0.898f, +0.847f,	
		+0.0f, +snorlaxhead + 1.65f, +snorlaxhead + 0.02f,			+0.945f, +0.898f, +0.847f,	
		+0.2f, +snorlaxhead + 1.9f, +snorlaxhead + 0.02f,				+0.945f, +0.898f, +0.847f,	
		-0.2f, +snorlaxhead + 1.9f, +snorlaxhead + 0.02f,				+0.945f, +0.898f, +0.847f,	

	};
	GLushort snorlaxIndices[] = {
		  0,2,3,
		  0,1,3,
		  0,2,6,
		  0,7,6,
		  0,7,4,
		  0,1,4,
		  5,6,2,
		  5,3,2,
		  5,3,1,
		  5,1,4,
		  5,6,7,
		  5,4,7,
		  

		8,10,11,
		8,9,11,
		8,10,14,
		8,15,14,
		8,15,12,
		8,9,12,
		13,14,10,
		13,11,10,
		13,11,9,
		13,9,12,
		13,14,15,
		13,12,15,

		18,17,16,
		17,18,19,
		20,16,17,
		20,17,21,

		22,25,28,
		22,28,26,
		22,26,23,
		23,26,27,
		23,27,24,

	};

	glGenVertexArrays(1, &snorlaxVaoID);
	glBindVertexArray(snorlaxVaoID);
	glGenBuffers(1, &snorlaxVboID);
	glBindBuffer(GL_ARRAY_BUFFER, snorlaxVboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(snorlax), snorlax, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	glGenBuffers(1, &snorlaxIndicesVboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, snorlaxIndicesVboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(snorlaxIndices) * sizeof(GLushort), snorlaxIndices, GL_STATIC_DRAW);


	
	
}

void tran(std::string x)
{
	glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
	glm::mat4 modelScalingMatrix = glm::mat4(1.0f);
	glm::mat4 modelRotationMatrix = glm::mat4(1.0f);
	if (x == "ground")
	{
		modelTransformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));;
		modelScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(4.0f, 1.0f, 4.0f));
		modelRotationMatrix = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0, 1, 0));

	}
	if (x == "snorlax")
	{
		modelTransformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 1.0f, 2.3f));
		modelScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f));
		modelRotationMatrix = glm::rotate(glm::mat4(1.0f), r_delta * r_press_num, glm::vec3(0, 1, 0));
	}

	

	GLint modelTransformMatrixUniformLocation = glGetUniformLocation(programID, "modelTransformMatrix");
	GLint modelRotateMatrixUniformLocation = glGetUniformLocation(programID, "modelRotationMatrix");
	GLint modelScalingMatrixUniformLocation = glGetUniformLocation(programID, "modelScalingMatrix");

	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glUniformMatrix4fv(modelRotateMatrixUniformLocation, 1, GL_FALSE, &modelRotationMatrix[0][0]);
	glUniformMatrix4fv(modelScalingMatrixUniformLocation, 1, GL_FALSE, &modelScalingMatrix[0][0]);

	glm::mat4 Projection = glm::perspective(30.0f, 1.0f, 2.0f, 20.0f);
	glm::mat4 Lookat = glm::lookAt(
		glm::vec3(0, 5.0, 0),
		glm::vec3(0, 0, -10),
		glm::vec3(0, -1, 0)
	);
	glm::mat4 Tmp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, -5.0f));;
	glm::mat4 ProjectionMatrix = Projection * Lookat * Tmp;
	GLuint MatrixID = glGetUniformLocation(programID, "projectionMatrix");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &ProjectionMatrix[0][0]);
}

void paintGL(void) {
	// always run
	// TODO:
	// render your objects and control the transformation here

	glClearColor(0.0f, 0.0f, 0.15f, 0.0f);  //specify the background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	tran("ground");
	glBindVertexArray(groundVaoID);
	glDrawArrays(GL_TRIANGLES, 0, 6);  //render primitives from array data

	tran("snorlax");
	glBindVertexArray(snorlaxVaoID);
	glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);

	
	
	//// with indexing (uncomment to use)
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	glFlush();

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// TODO:
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		x_press_num -= 1;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		x_press_num += 1;
	}
	if (key == GLFW_KEY_V && action == GLFW_PRESS) {
		r_press_num -= 1;
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		r_press_num += 1;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void initializedGL(void) {
	// run only once
	// TODO:
	sendDataToOpenGL();
	installShaders();
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char* argv[]) {
	GLFWwindow* window;

	/* Initialize the glfw */
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	/* glfw: configure; necessary for MAC */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* do not allow resizing */
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 600, "Assignment 1", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback); // TODO
	// TODO: mouse callback, etc.

	/* Initialize the glew */
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	get_OpenGL_info();
	initializedGL();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		paintGL();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
