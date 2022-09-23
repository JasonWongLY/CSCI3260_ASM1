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
#include <time.h>

clock_t now_time;
int start_time = 0;
clock_t second_time;
int diff_time = 0;

float angle = 0.0f;
GLint programID;
float x_delta = 0.2f;
int x_press_num = 0;
float z_delta = 0.2f;
int z_press_num = 0;
float r_delta = 0.1f;
int r_press_num = 0;
float scale_delta = 0.05f;
int scale_press_num = 0;
bool sleep = false;
int pre_x, pre_z;
bool poo = false;

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
GLuint snorlaxEyeVaoID, snorlaxEyeVboID, snorlaxEyeIndicesVboID;
GLuint sleepingZVaoID, sleepingZVboID, sleepingZIndicesVboID;
GLuint pokeballVaoID, pokeballVboID, pokeballIndicesVboID;
GLuint bedVaoID, bedVboID, bedIndicesVboID;
GLuint toiletVaoID, toiletVboID, toiletIndicesVboID;

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

		-1.0f, +0.0f, -1.0f,      0.3373f, 0.4902f, 0.2745f,
		-1.0f, +0.0f, +1.0f,      0.3373f, 0.4902f, 0.2745f,
		+1.0f, +0.0f, -1.0f,      0.3373f, 0.4902f, 0.2745f,
		-1.0f, +0.0f, +1.0f,      0.3373f, 0.4902f, 0.2745f,
		+1.0f, +0.0f, +1.0f,      0.3373f, 0.4902f, 0.2745f,
		+1.0f, +0.0f, -1.0f,      0.3373f, 0.4902f, 0.2745f,
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
		-1.0f, +1.5f, -1.0f+0.2f,    +0.0608f, +0.251f, +0.3608f,	//0
		-1.0f, -1.0f, -1.0f + 0.2f,  +0.1608f, +0.451f, +0.5608f,	//1
		+1.0f, +1.5f, -1.0f + 0.2f,  +0.1608f, +0.451f, +0.5608f,	//2
		+1.0f, -1.0f, -1.0f + 0.2f,  +0.1608f, +0.451f, +0.5608f,	//3
		-1.0f, -1.0f, +1.0f - 0.2f,  +0.1608f, +0.451f, +0.5608f,	//4
		+1.0f, -1.0f, +1.0f - 0.2f,  +0.0608f, +0.251f, +0.3608f,	//5
		+1.0f, +1.5f, +1.0f - 0.2f,  +0.1608f, +0.451f, +0.5608f,	//6
		-1.0f, +1.5f, +1.0f - 0.2f,  +0.1608f, +0.451f, +0.5608f,	//7

		-snorlaxhead, +snorlaxhead + 2.0f, -snorlaxhead + 0.2f,     +0.0608f, +0.251f, +0.3608f,	//8
		-snorlaxhead, -1.0f + 2.5f, -snorlaxhead + 0.2f,		    +0.1608f, +0.451f, +0.5608f,	//9
		+snorlaxhead, +snorlaxhead + 2.0f, -snorlaxhead + 0.2f,     +0.1608f, +0.451f, +0.5608f,	//10
		+snorlaxhead, -1.0f + 2.5f, -snorlaxhead + 0.2f,		    +0.1608f, +0.451f, +0.5608f,	//11
		-snorlaxhead, -1.0f + 2.5f, +snorlaxhead - 0.2f,		    +0.1608f, +0.451f, +0.5608f,	//12
		+snorlaxhead, -1.0f + 2.5f, +snorlaxhead - 0.2f,		    +0.0608f, +0.251f, +0.3608f,	//13
		+snorlaxhead, +snorlaxhead + 2.0f, +snorlaxhead - 0.2f,     +0.1608f, +0.451f, +0.5608f,	//14
		-snorlaxhead, +snorlaxhead + 2.0f, +snorlaxhead - 0.2f,     +0.1608f, +0.451f, +0.5608f,	//15

		
		-0.8f, +1.5f, +1.01f - 0.2f,   +0.945f, +0.898f, +0.847f,	//16
		+0.8f, +1.51f, +1.01f - 0.2f,   +0.945f, +0.898f, +0.847f,	//17
		-0.8f, -0.7f, +1.01f - 0.2f,   +0.945f, +0.898f, +0.847f,	//18
		+0.8f, -0.7f, +1.01f - 0.2f,   +0.945f, +0.898f, +0.847f,	//19
		-0.8f, +1.05f, +0.85f - 0.2f,  +0.945f, +0.898f, +0.847f,	//20
		+0.8f, +1.05f, +0.85f - 0.2f,  +0.945f, +0.898f, +0.847f,	//21

		-snorlaxhead+0.15f, -1.0f + 2.5f, +snorlaxhead + 0.01f - 0.2f,		+0.945f, +0.898f, +0.847f,		//22
		+snorlaxhead-0.15f, -1.0f + 2.5f, +snorlaxhead + 0.01f - 0.2f,		+0.945f, +0.898f, +0.847f,		//23
		+snorlaxhead-0.15f, +snorlaxhead + 1.9f, +snorlaxhead + 0.02f - 0.2f,	+0.945f, +0.898f, +0.847f,	//24
		-snorlaxhead+0.15f, +snorlaxhead + 1.9f, +snorlaxhead + 0.02f - 0.2f,	+0.945f, +0.898f, +0.847f,	//25
		+0.0f, +snorlaxhead + 1.65f, +snorlaxhead + 0.02f - 0.2f,			+0.945f, +0.898f, +0.847f,		//26
		+0.2f, +snorlaxhead + 1.9f, +snorlaxhead + 0.02f - 0.2f,				+0.945f, +0.898f, +0.847f,	//27
		-0.2f, +snorlaxhead + 1.9f, +snorlaxhead + 0.02f - 0.2f,				+0.945f, +0.898f, +0.847f,	//28

		+1.0f, +1.4f, -0.25f,	+0.0608f, +0.251f, +0.3608f,	//29
		+1.0f, +0.4f, -0.25f,	+0.1608f, +0.451f, +0.5608f,	//30
		+1.6f, +1.0f, -0.25f,	+0.1608f, +0.451f, +0.5608f,	//31
		+1.6f, +0.0f, -0.25f,	+0.1608f, +0.451f, +0.5608f,	//32
		+1.0f, +0.4f, +0.25f,	+0.1608f, +0.451f, +0.5608f,	//33
		+1.6f, +0.0f, +0.25f,	+0.0608f, +0.251f, +0.3608f,	//34
		+1.6f, +1.0f, +0.25f,	+0.1608f, +0.451f, +0.5608f,	//35
		+1.0f, +1.4f, +0.25f,	+0.1608f, +0.451f, +0.5608f,	//36

		-1.0f, +1.4f, -0.25f,	+0.0608f, +0.251f, +0.3608f,	//37
		-1.0f, +0.4f, -0.25f,	+0.1608f, +0.451f, +0.5608f,	//38
		-1.6f, +1.0f, -0.25f,	+0.1608f, +0.451f, +0.5608f,	//39
		-1.6f, +0.0f, -0.25f,	+0.1608f, +0.451f, +0.5608f,	//40
		-1.0f, +0.4f, +0.25f,	+0.1608f, +0.451f, +0.5608f,	//41
		-1.6f, +0.0f, +0.25f,	+0.0608f, +0.251f, +0.3608f,	//42
		-1.6f, +1.0f, +0.25f,	+0.1608f, +0.451f, +0.5608f,	//43
		-1.0f, +1.4f, +0.25f,	+0.1608f, +0.451f, +0.5608f,	//44

		//left leg
		0.55f, +0.0f, 0.8f,		+0.745f, +0.698f, +0.647f,		//45
		0.55f, -1.0f, 0.8f,		+0.945f, +0.898f, +0.847f,		//46
		1.05f, +0.0f, 0.8f,		+0.945f, +0.898f, +0.847f,		//47
		1.05f, -1.0f, 0.8f,		+0.945f, +0.898f, +0.847f,		//48
		0.55f, -1.0f, 1.1f,		+0.945f, +0.898f, +0.847f,		//49
		1.05f, -1.0f, 1.1f,		+0.745f, +0.698f, +0.647f,		//50
		1.05f, +0.0f, 1.1f,		+0.945f, +0.898f, +0.847f,		//51
		0.55f, +0.0f ,1.1f,		+0.945f, +0.898f, +0.847f,		//52

		0.65f, -0.91f, 1.11f,		+0.498f, +0.435f, +0.384f,		//53
		0.95f, -0.9f, 1.11f,		+0.498f, +0.435f, +0.384f,		//54
		0.95f, -0.31f, 1.11f,		+0.498f, +0.435f, +0.384f,		//55
		0.65f, -0.31f ,1.11f,		+0.498f, +0.435f, +0.384f,		//56

		//right leg
		-0.55f, +0.0f, 0.8f,		+0.745f, +0.698f, +0.647f,		//57
		-0.55f, -1.0f, 0.8f,		+0.945f, +0.898f, +0.847f,		//58
		-1.05f, +0.0f, 0.8f,		+0.945f, +0.898f, +0.847f,		//59
		-1.05f, -1.0f, 0.8f,		+0.945f, +0.898f, +0.847f,		//60
		-0.55f, -1.0f, 1.1f,		+0.945f, +0.898f, +0.847f,		//61
		-1.05f, -1.0f, 1.1f,		+0.745f, +0.698f, +0.647f,		//62
		-1.05f, +0.0f, 1.1f,		+0.945f, +0.898f, +0.847f,		//63
		-0.55f, +0.0f ,1.1f,		+0.945f, +0.898f, +0.847f,		//64
		
		-0.65f, -0.91f, 1.11f,		+0.498f, +0.435f, +0.384f,		//65
		-0.95f, -0.9f, 1.11f,		+0.498f, +0.435f, +0.384f,		//66
		-0.95f, -0.31f, 1.11f,		+0.498f, +0.435f, +0.384f,		//67
		-0.65f, -0.31f ,1.11f,		+0.498f, +0.435f, +0.384f,		//68

		//left ear
		+snorlaxhead - 0.5f, 3.35f, -0.2f,		+0.0608f, +0.251f, +0.3608f,		//69
		+snorlaxhead - 0.5f, 2.85f, -0.2f,		+0.1608f, +0.451f, +0.5608f,		//70
		+snorlaxhead - 0.2f, 3.35f, -0.2f,		+0.1608f, +0.451f, +0.5608f,		//71
		+snorlaxhead - 0.2f, 2.85f, -0.2f,		+0.1608f, +0.451f, +0.5608f,		//72
		+snorlaxhead - 0.5f, 2.85f, 0.2f,		+0.1608f, +0.451f, +0.5608f,		//73
		+snorlaxhead - 0.2f, 2.85f, 0.2f,		+0.0608f, +0.251f, +0.3608f,		//74
		+snorlaxhead - 0.2f, 3.35f, 0.2f,		+0.1608f, +0.451f, +0.5608f,		//75
		+snorlaxhead - 0.5f, 3.35f, 0.2f,		+0.1608f, +0.451f, +0.5608f,		//76

		//right ear
		-snorlaxhead + 0.5f, 3.35f, -0.2f,		+0.0608f, +0.251f, +0.3608f,		//77
		-snorlaxhead + 0.5f, 2.85f, -0.2f,		+0.1608f, +0.451f, +0.5608f,		//78
		-snorlaxhead + 0.2f, 3.35f, -0.2f,		+0.1608f, +0.451f, +0.5608f,		//79
		-snorlaxhead + 0.2f, 2.85f, -0.2f,		+0.1608f, +0.451f, +0.5608f,		//80
		-snorlaxhead + 0.5f, 2.85f, 0.2f,		+0.1608f, +0.451f, +0.5608f,		//81
		-snorlaxhead + 0.2f, 2.85f, 0.2f,		+0.0608f, +0.251f, +0.3608f,		//82
		-snorlaxhead + 0.2f, 3.35f, 0.2f,		+0.1608f, +0.451f, +0.5608f,		//83
		-snorlaxhead + 0.5f, 3.35f, 0.2f,		+0.1608f, +0.451f, +0.5608f,		//84
	};
	GLushort snorlaxIndices[] = {
		// body
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
		  
		//head
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

		//body
		18,17,16,
		17,18,19,
		20,16,17,
		20,17,21,
		
		//head
		22,25,28,
		22,28,26,
		22,26,23,
		23,26,27,
		23,27,24,

		//left hand
		29,31,32,
		29,32,30,
		29,31,35,
		29,36,35,
		29,36,33,
		29,33,30,
		34,35,31,
		34,32,31,
		34,30,32,
		34,33,30,
		34,35,36,
		34,33,36,
		//right hand
		29+8,31+8,32+8,
		29+8,30+8,32+8,
		29+8,31+8,35+8,
		29+8,36+8,35+8,
		29+8,36+8,33+8,
		29+8,30+8,33+8,
		34+8,35+8,31+8,
		34+8,32+8,31+8,
		34+8,32+8,30+8,
		34+8,30+8,33+8,
		34+8,35+8,36+8,
		34+8,33+8,36+8,

		//left leg
		0+45,2+45,3+45,
		0+45,1+45,3+45,
		0+45,2+45,6+45,
		0+45,7+45,6+45,
		0+45,7+45,4+45,
		0+45,1+45,4+45,
		5+45,6+45,2+45,
		5+45,3+45,2+45,
		5+45,3+45,1+45,
		5+45,1+45,4+45,
		5+45,6+45,7+45,
		5+45,4+45,7+45,
		53,54,55,
		53,55,56,

		0+57,2+57,3+57,
		0+57,1+57,3+57,
		0+57,2+57,6+57,
		0+57,7+57,6+57,
		0+57,7+57,4+57,
		0+57,1+57,4+57,
		5+57,6+57,2+57,
		5+57,3+57,2+57,
		5+57,3+57,1+57,
		5+57,1+57,4+57,
		5+57,6+57,7+57,
		5+57,4+57,7+57,
		65,66,67,
		65,67,68,

		0+69,2+69,3+69,
		0+69,1+69,3+69,
		0+69,2+69,6+69,
		0+69,7+69,6+69,
		0+69,7+69,4+69,
		0+69,1+69,4+69,
		5+69,6+69,2+69,
		5+69,3+69,2+69,
		5+69,3+69,1+69,
		5+69,1+69,4+69,
		5+69,6+69,7+69,
		5+69,4+69,7+69,

		0+77, 2+77, 3+77,
		0+77, 1+77, 3+77,
		0+77, 2+77, 6+77,
		0+77, 7+77, 6+77,
		0+77, 7+77, 4+77,
		0+77, 1+77, 4+77,
		5+77, 6+77, 2+77,
		5+77, 3+77, 2+77,
		5+77, 3+77, 1+77,
		5+77, 1+77, 4+77,
		5+77, 6+77, 7+77,
		5+77, 4+77, 7+77,
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


	const GLfloat snorlaxEye[] =
	{
		-snorlaxhead + 0.5f, +1.9f, +snorlaxhead + 0.02f - 0.2f,	+0.0f, +0.0f, +0.0f,
		-snorlaxhead + 0.5f, +1.8f, +snorlaxhead + 0.02f - 0.2f,	+0.0f, +0.0f, +0.0f,
		+snorlaxhead - 0.5f, +1.9f, +snorlaxhead + 0.02f - 0.2f,	+0.0f, +0.0f, +0.0f,
		+snorlaxhead - 0.5f, +1.8f, +snorlaxhead + 0.02f - 0.2f,	+0.0f, +0.0f, +0.0f,

		-snorlaxhead + 0.3f, +snorlaxhead + 1.5f, +snorlaxhead + 0.02f - 0.2f,	+0.0f, +0.0f, +0.0f,
		-snorlaxhead + 0.3f, +snorlaxhead + 1.4f, +snorlaxhead + 0.02f - 0.2f,	+0.0f, +0.0f, +0.0f,
		-0.2f, +snorlaxhead + 1.5f, +snorlaxhead + 0.02f - 0.2f,	+0.0f, +0.0f, +0.0f,
		-0.2f, +snorlaxhead + 1.4f, +snorlaxhead + 0.02f - 0.2f,	+0.0f, +0.0f, +0.0f,

		+0.2f, +snorlaxhead + 1.5f, +snorlaxhead + 0.02f - 0.2f,	+0.0f, +0.0f, +0.0f,
		+0.2f, +snorlaxhead + 1.4f, +snorlaxhead + 0.02f - 0.2f,	+0.0f, +0.0f, +0.0f,
		+snorlaxhead - 0.3f, +snorlaxhead + 1.5f, +snorlaxhead + 0.02f - 0.2f,	+0.0f, +0.0f, +0.0f,
		+snorlaxhead - 0.3f, +snorlaxhead + 1.4f, +snorlaxhead + 0.02f - 0.2f,	+0.0f, +0.0f, +0.0f,
	};
	GLushort snorlaxEyeIndices[] =
	{
		0,1,2,
		1,2,3,

		4,5,6,
		5,6,7,

		8,9,10,
		9,10,11,
	};
	glGenVertexArrays(1, &snorlaxEyeVaoID);
	glBindVertexArray(snorlaxEyeVaoID);
	glGenBuffers(1, &snorlaxEyeVboID);
	glBindBuffer(GL_ARRAY_BUFFER, snorlaxEyeVboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(snorlaxEye), snorlaxEye, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	glGenBuffers(1, &snorlaxEyeIndicesVboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, snorlaxEyeIndicesVboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(snorlaxEyeIndices) * sizeof(GLushort), snorlaxEyeIndices, GL_STATIC_DRAW);

	
	const GLfloat sleepingZ[] =
	{
		-1.0f, +1.0f, -1.0f,			1.0f, 1.0f, 1.0f,
		-1.0f, +0.5f, -1.0f,			1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,			1.0f, 1.0f, 1.0f,
		+1.0f, +1.0f, -1.0f,			1.0f, 1.0f, 1.0f,
		+1.0f, +0.5f, -1.0f,			1.0f, 1.0f, 1.0f,
		+1.0f, -0.5f, -1.0f,			1.0f, 1.0f, 1.0f,
		+1.0f, -1.0f, -1.0f,			1.0f, 1.0f, 1.0f,
	};

	GLushort sleepingZIndices[] =
	{
		0,1,3,
		3,4,2,
		2,5,6,
	};
	
	glGenVertexArrays(1, &sleepingZVaoID);
	glBindVertexArray(sleepingZVaoID);
	glGenBuffers(1, &sleepingZVboID);
	glBindBuffer(GL_ARRAY_BUFFER, sleepingZVboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sleepingZ), sleepingZ, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	glGenBuffers(1, &sleepingZIndicesVboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sleepingZIndicesVboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sleepingZIndices) * sizeof(GLushort), sleepingZIndices, GL_STATIC_DRAW);


	const GLfloat pokeball[] =
	{
		-1.0f, +1.0f, -1.0f,		0.741f, 0.0f, 0.0f,
		-1.0f, +0.001f, -1.0f,		0.941f, 0.0f, 0.0f,
		+1.0f, +1.0f, -1.0f,		0.941f, 0.0f, 0.0f,
		+1.0f, +0.001f, -1.0f,		0.941f, 0.0f, 0.0f,
		-1.0f, +0.001f, +1.0f,		0.941f, 0.0f, 0.0f,
		+1.0f, +0.001f, +1.0f,		0.741f, 0.0f, 0.0f,
		+1.0f, +1.0f, +1.0f,		0.941f, 0.0f, 0.0f,
		-1.0f, +1.0f, +1.0f,		0.941f, 0.0f, 0.0f,


		-1.0f, -0.001f, -1.0f,		0.741f, 0.741f, 0.741f,
		-1.0f, -1.0f,   -1.0f,		0.941f, 0.941f, 0.941f,
		+1.0f, -0.001f, -1.0f,		0.941f, 0.941f, 0.941f,
		+1.0f, -1.0f,   -1.0f,		0.941f, 0.941f, 0.941f,
		-1.0f, -1.0f,   +1.0f,		0.941f, 0.941f, 0.941f,
		+1.0f, -1.0f,   +1.0f,		0.741f, 0.741f, 0.741f,
		+1.0f, -0.001f, +1.0f,		0.941f, 0.941f, 0.941f,
		-1.0f, -0.001f, +1.0f,		0.941f, 0.941f, 0.941f,

		-1.001f, +0.101f, -1.001f,		0.1333f, 0.1333f, 0.1411f,
		-1.001f, -0.101f, -1.001f,		0.1333f, 0.1333f, 0.1411f,
		+1.001f, +0.101f, -1.001f,		0.1333f, 0.1333f, 0.1411f,
		+1.001f, -0.101f, -1.001f,		0.1333f, 0.1333f, 0.1411f,
		-1.001f, -0.101f, +1.001f,		0.1333f, 0.1333f, 0.1411f,
		+1.001f, -0.101f, +1.001f,		0.1333f, 0.1333f, 0.1411f,
		+1.001f, +0.101f, +1.001f,		0.1333f, 0.1333f, 0.1411f,
		-1.001f, +0.101f, +1.001f,		0.1333f, 0.1333f, 0.1411f,

		-0.402f, +0.402f, +1.002f,		0.1333f, 0.1333f, 0.1411f,
		+0.402f, +0.402f, +1.002f,		0.1333f, 0.1333f, 0.1411f,
		-0.402f, -0.402f, +1.002f,		0.1333f, 0.1333f, 0.1411f,
		+0.402f, -0.402f, +1.002f,		0.1333f, 0.1333f, 0.1411f,

		-0.203f, +0.203f, +1.003f,		0.941f, 0.941f, 0.941f,
		+0.203f, +0.203f, +1.003f,		0.941f, 0.941f, 0.941f,
		-0.203f, -0.203f, +1.003f,		0.941f, 0.941f, 0.941f,
		+0.203f, -0.203f, +1.003f,		0.941f, 0.941f, 0.941f,
	};

	GLushort pokeballIndices[] =
	{
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

		0+8,2+8,3+8,
		0+8,1+8,3+8,
		0+8,2+8,6+8,
		0+8,7+8,6+8,
		0+8,7+8,4+8,
		0+8,1+8,4+8,
		5+8,6+8,2+8,
		5+8,3+8,2+8,
		5+8,3+8,1+8,
		5+8,1+8,4+8,
		5+8,6+8,7+8,
		5+8,4+8,7+8,

		0+16,2+16,3+16,
		0+16,1+16,3+16,
		0+16,2+16,6+16,
		0+16,7+16,6+16,
		0+16,7+16,4+16,
		0+16,1+16,4+16,
		5+16,6+16,2+16,
		5+16,3+16,2+16,
		5+16,3+16,1+16,
		5+16,1+16,4+16,
		5+16,6+16,7+16,
		5+16,4+16,7+16,

		24,25,26,
		25,26,27,
		28,29,30,
		29,30,31,
	};
	glGenVertexArrays(1, &pokeballVaoID);
	glBindVertexArray(pokeballVaoID);
	glGenBuffers(1, &pokeballVboID);
	glBindBuffer(GL_ARRAY_BUFFER, pokeballVboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pokeball), pokeball, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	glGenBuffers(1, &pokeballIndicesVboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pokeballIndicesVboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pokeballIndices) * sizeof(GLushort), pokeballIndices, GL_STATIC_DRAW);


	const GLfloat bed[] =
	{
		-1.5f, 0.55f, -1.2f,		0.741f, 0.741f, 0.741f,
		-1.5f, 0.0f, -1.2f,			0.941f, 0.941f, 0.941f,
		-0.5f, 0.55f, -1.2f,		0.941f, 0.941f, 0.941f,
		-0.5f, 0.0f, -1.2f,			0.941f, 0.941f, 0.941f,
		-1.5f, 0.0f, 1.5f,			0.941f, 0.941f, 0.941f,
		-0.5f, 0.0f, 1.5f,			0.741f, 0.741f, 0.741f,
		-0.5f, 0.55f, 1.5f,			0.941f, 0.941f, 0.941f,
		-1.5f, 0.55f, 1.5f,			0.941f, 0.941f, 0.941f,

		-0.4999f, 0.55f, -1.2f,		0.741f, 0.0f, 0.0f,
		-0.4999f, 0.0f, -1.2f,		0.941f, 0.0f, 0.0f,
		2.5f, 0.55f, -1.2f,			0.941f, 0.0f, 0.0f,
		2.5f, 0.0f, -1.2f,			0.941f, 0.0f, 0.0f,
		-0.4999f, 0.0f, 1.5f,		0.941f, 0.0f, 0.0f,
		2.5f, 0.0f, 1.5f,			0.741f, 0.0f, 0.0f,
		2.5f, 0.55f, 1.5f,			0.941f, 0.0f, 0.0f,
		-0.4999f, 0.55f, 1.5f,		0.941f, 0.0f, 0.0f,
	};

	GLushort bedIndices[] =
	{
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

		0 + 8,2 + 8,3 + 8,
		0 + 8,1 + 8,3 + 8,
		0 + 8,2 + 8,6 + 8,
		0 + 8,7 + 8,6 + 8,
		0 + 8,7 + 8,4 + 8,
		0 + 8,1 + 8,4 + 8,
		5 + 8,6 + 8,2 + 8,
		5 + 8,3 + 8,2 + 8,
		5 + 8,3 + 8,1 + 8,
		5 + 8,1 + 8,4 + 8,
		5 + 8,6 + 8,7 + 8,
		5 + 8,4 + 8,7 + 8,
	};
	glGenVertexArrays(1, &bedVaoID);
	glBindVertexArray(bedVaoID);
	glGenBuffers(1, &bedVboID);
	glBindBuffer(GL_ARRAY_BUFFER, bedVboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bed), bed, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	glGenBuffers(1, &bedIndicesVboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bedIndicesVboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bedIndices) * sizeof(GLushort), bedIndices, GL_STATIC_DRAW);

	const GLfloat toilet[] =
	{
		-1.0f, +0.25f, -1.0f,		0.741f, 0.741f, 0.741f,
		-1.0f, -1.0f, -1.0f,	    0.941f, 0.941f, 0.941f,
		+1.0f, +0.25f, -1.0f,		0.941f, 0.941f, 0.941f,
		+1.0f, -1.0f, -1.0f,		0.741f, 0.741f, 0.741f,
		-1.0f, -1.0f, +1.0f,		0.741f, 0.741f, 0.741f,
		+1.0f, -1.0f, +1.0f,		0.941f, 0.941f, 0.941f,
		+1.0f, +0.25f, +1.0f,		0.741f, 0.741f, 0.741f,
		-1.0f, +0.25f, +1.0f,		0.941f, 0.941f, 0.941f,

		/*-0.8f, +0.25f, -0.8f,	0.741f, 0.741f, 0.741f,
		-0.8f, -1.0f, -0.8f,	0.941f, 0.941f, 0.941f,
		+0.8f, +0.25f, -0.8f,	0.941f, 0.941f, 0.941f,
		+0.8f, -1.0f, -0.8f,	0.741f, 0.741f, 0.741f,
		-0.8f, -1.0f, +0.8f,	0.741f, 0.741f, 0.741f,
		+0.8f, -1.0f, +0.8f,	0.941f, 0.941f, 0.941f,
		+0.8f, +0.25f, +0.8f,	0.741f, 0.741f, 0.741f,
		-0.8f, +0.25f, +0.8f,	0.941f, 0.941f, 0.941f,*/

		-2.0f, +2.0f, -1.0f,    0.741f, 0.741f, 0.741f,
		-2.0f, -1.0f, -1.0f,    0.941f, 0.941f, 0.941f,
		-1.0f, +2.0f, -1.0f,    0.941f, 0.941f, 0.941f,
		-1.0f, -1.0f, -1.0f,    0.941f, 0.941f, 0.941f,
		-2.0f, -1.0f, +1.0f,    0.941f, 0.941f, 0.941f,
		-1.0f, -1.0f, +1.0f,    0.741f, 0.741f, 0.741f,
		-1.0f, +2.0f, +1.0f,    0.941f, 0.941f, 0.941f,
		-2.0f, +2.0f, +1.0f,    0.941f, 0.941f, 0.941f,

		-0.8f, +0.2501f, -0.8f,	0.653f, 0.653f, 0.653f,
		+0.8f, +0.2501f, -0.8f,	0.653f, 0.653f, 0.653f,
		+0.8f, +0.2501f, +0.8f,	0.653f, 0.653f, 0.653f,
		-0.8f, +0.2501f, +0.8f,	0.653f, 0.653f, 0.653f,
	};

	GLushort toiletIndices[] =
	{
		/*0,2,3,
		0,1,3,
		0,7,4,
		0,1,4,
		5,6,2,
		5,3,2,
		5,6,7,
		5,4,7,

		0,8,7,
		8,7,15,
		0,8,2,
		8,2,10,
		10,14,2,
		14,2,6,
		7,6,15,
		6,15,14,*/

		//15,8,12,
		//8,12,9,
		//9,8,11,
		//8,11,10,
		//15,14,12,
		//12,14,13,
		//13,14,11,
		//14,11,10,

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

		0+8,2+8,3+8,
		0+8,1+8,3+8,
		0+8,2+8,6+8,
		0+8,7+8,6+8,
		0+8,7+8,4+8,
		0+8,1+8,4+8,
		5+8,6+8,2+8,
		5+8,3+8,2+8,
		5+8,3+8,1+8,
		5+8,1+8,4+8,
		5+8,6+8,7+8,
		5+8,4+8,7+8,

		16,17,18,
		16,18,19,
	};

	glGenVertexArrays(1, &toiletVaoID);
	glBindVertexArray(toiletVaoID);
	glGenBuffers(1, &toiletVboID);
	glBindBuffer(GL_ARRAY_BUFFER, toiletVboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(toilet), toilet, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	glGenBuffers(1, &toiletIndicesVboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, toiletIndicesVboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(toiletIndices) * sizeof(GLushort), toiletIndices, GL_STATIC_DRAW);
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
	if ((x == "snorlax") || (x=="snorlaxEye"))
	{
		modelTransformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x_delta * x_press_num + 1.5f, 1.0f, z_delta * z_press_num + 2.3f));
		modelScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f + scale_delta * scale_press_num, 0.3f + scale_delta * scale_press_num, 0.3f + scale_delta * scale_press_num));
		modelRotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
	}
	if ((x == "snorlaxSleep") || (x == "snorlaxSleepEye"))
	{
		modelTransformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.5f, 1.5f, -0.6f));
		modelScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f + scale_delta * scale_press_num, 0.3f + scale_delta * scale_press_num, 0.3f + scale_delta * scale_press_num));
		modelRotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));
	}
	if ((x == "snorlaxPoo") || (x == "snorlaxEyePoo"))
	{
		modelTransformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, 1.3f, -1.3f));
		modelScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f+scale_delta*scale_press_num, 0.3f+ scale_delta * scale_press_num, 0.3f + scale_delta * scale_press_num));
		modelRotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0, 1, 0));
		if (diff_time >= 2000)
		{
			now_time = clock();
			diff_time = 0;
			start_time = 0;
			poo = false;
			scale_press_num -= 1;
		}
		else
		{
			diff_time = (clock() - now_time);
		}
	}
	if (x == "sleepingZ")
	{
		modelTransformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 2.0f, 0.4f));
		modelScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.0001f*diff_time+0.3f, 0.0001*diff_time+0.3f, 3.0f));
		modelRotationMatrix = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0, 1, 0));
		if (diff_time >= 2000)
		{
			now_time = clock();
			diff_time = 0;
			start_time = 0;
		}
		else
		{
			diff_time = (clock() - now_time);
		}
	}
	if (x == "pokeball")
	{
		modelTransformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.1f, 1.0f, 0.5f));
		modelScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
		modelRotationMatrix = glm::rotate(glm::mat4(1.0f),  r_delta * r_press_num, glm::vec3(0, 1, 0));
	}

	if (x == "bed")
	{
		modelTransformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.5f, 1.0f, -1.0f));
		modelScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f , 0.5f , 0.5f ));
		modelRotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f) , glm::vec3(0, 1, 0));
		
	}

	if (x == "toilet")
	{
		modelTransformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, 1.0f, -1.5f));
		modelScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
		modelRotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0, 1, 0));
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

	if ((sleep == false) && (poo==false)) {
		tran("snorlax");
		glBindVertexArray(snorlaxVaoID);
		glDrawElements(GL_TRIANGLES, 120 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);

		tran("snorlaxEye");
		glBindVertexArray(snorlaxEyeVaoID);
		glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
	}
	else if (sleep==true)
	{
		tran("snorlaxSleep");
		glBindVertexArray(snorlaxVaoID);
		glDrawElements(GL_TRIANGLES, 120 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);

		tran("snorlaxSleepEye");
		glBindVertexArray(snorlaxEyeVaoID);
		glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);

		tran("sleepingZ");
		glBindVertexArray(sleepingZVaoID);
		glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
	}
	else if (poo == true)
	{
		tran("snorlaxPoo");
		glBindVertexArray(snorlaxVaoID);
		glDrawElements(GL_TRIANGLES, 120 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);

		tran("snorlaxEyePoo");
		glBindVertexArray(snorlaxEyeVaoID);
		glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
	}

	tran("pokeball");
	glBindVertexArray(pokeballVaoID);
	glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);

	tran("bed");
	glBindVertexArray(bedVaoID);
	glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);

	tran("toilet");
	glBindVertexArray(toiletVaoID);
	glDrawElements(GL_TRIANGLES, 80 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
	//// with indexing (uncomment to use)
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	if ((x_press_num <= 5) &&(x_press_num>=0) && (z_press_num >= -18) &&(z_press_num<=-11)) {

		if (sleep == false)
		{
			now_time = clock();
		}

		sleep = true;

	}
	else sleep = false;
	
	if ((x_press_num < -2)&&(x_press_num>-12))
	{
		if ((z_press_num < -4) && (z_press_num > -14))
		{
			x_press_num = pre_x;
			z_press_num = pre_z;
		}
	}

	if ((z_press_num < -11) && (z_press_num > -18))
	{
		if ((x_press_num < 5) && (x_press_num > 0))
		{
			x_press_num = pre_x;
			z_press_num = pre_z;
		}
	}

	if ((x_press_num <= -15) && (x_press_num >= -20) && (z_press_num <= -13) && (z_press_num >= -18))
	{
		if (poo == false)
		{
			now_time = clock();
		}
		poo = true;
	}
	else poo = false;

	/*else if ((z_press_num < -4) && (z_press_num) > -14)
	{
		if (x_press_num < -2)	x_press_num = -2;
		if (x_press_num < -13)	x_press_num = -13;
	}*/
	if (x_press_num > 5) x_press_num = 5;
	if (x_press_num < -20) x_press_num = -20;
	if (z_press_num > 3) z_press_num = 3;
	if (z_press_num < -18) z_press_num = -18;
	if (scale_press_num > 4) scale_press_num = 4;
	if (scale_press_num < -2) scale_press_num = -2;
	pre_x = x_press_num;
	pre_z = z_press_num;
	glFlush();

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// TODO:
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		x_press_num -= 1;
		angle = -90.0f;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		x_press_num += 1;
		angle = 90.0f;
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		z_press_num -= 1;
		angle = 180.0f;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		z_press_num += 1;
		angle = 0.0f;
	}
	if (key == GLFW_KEY_V && action == GLFW_PRESS) {
		r_press_num -= 1;
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		r_press_num += 1;
	}
	if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		scale_press_num -= 1;
	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		scale_press_num += 1;
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
