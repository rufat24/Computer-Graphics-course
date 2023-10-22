#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader;

//Vertex shader
static const char* vShader = "		\n\
#version 330						\n\
									\n\
layout (location=0) in vec3 pos;    \n\
void main()							\n\
{									\n\
	gl_Position = vec4(0.5*pos.x,0.5*pos.y,pos.z, 1.0);	\n\
}";

static const char* fShader = "		\n\
#version 330						\n\
									\n\
out vec4 color;						\n\
void main()							\n\
{									\n\
	color = vec4(0.0,1.0,0.0, 1.0);	\n\
}";

void CalculateTriangle() {
	GLfloat vertices[] = {
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		0.0f, 1.0f, 1.0f
	};
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AddShader(GLuint program, const char* shaderCode, GLenum shaderType) {
	GLuint s = glCreateShader(shaderType);
	const GLchar* code[1];
	code[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(s, 1, code, codeLength);
	glCompileShader(s);

	GLint result = 0;
	GLchar elog[1024] = { 0 };


	glGetShaderiv(s, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(s, sizeof(elog), NULL, elog);
		printf("Error compiling %d shader: '%s'\n", shaderType, elog);
		return;
	}

	glAttachShader(program, s);
}

void CompileShader() {
	shader = glCreateProgram();
	if (!shader) {
		printf("Sahder was not created");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar elog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
		printf("Error linking: '%s'\n", elog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
		printf("Error validating: '%s'\n", elog);
		return;
	}
}



int main()
{
	//init GLFW
	if (!glfwInit())
	{
		printf("GLFW init failed");
		glfwTerminate();
		return 1;
	}

	//Setup GLFW window properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow* mainWind = glfwCreateWindow(WIDTH, HEIGHT, "Test", NULL, NULL);

	if (!mainWind)
	{
		printf("GLFW window init failed");
		glfwTerminate();
		return 1;
	}


	// get buffer sizek info
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWind, &bufferWidth, &bufferHeight);
	//set context to GLEW
	glfwMakeContextCurrent(mainWind);

	//allow modern extention features
	glewExperimental = GL_TRUE;




	//init glew
	if (glewInit()!=GLEW_OK)
	{
		printf("GLeW init failed");
		glfwDestroyWindow(mainWind);
		glfwTerminate();
		return 1;
	}

	//setup viewport
	glViewport(0, 0, bufferWidth, bufferHeight);


	CalculateTriangle();
	CompileShader();
	//loop untill window closed
	while (!glfwWindowShouldClose(mainWind)) {
		//Get handle useer input events
		glfwPollEvents();

		//Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES,0,3);
		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(mainWind);
	}

    return 0;
}