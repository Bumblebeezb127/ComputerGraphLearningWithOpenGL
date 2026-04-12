#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include"Utils.h"

using namespace std;

//宏定义
#define WINDOW_WIDTH 1980
#define WINDOW_HEIGHT 1080
#define numVAOs 1

//全局变量
Utils utils;
GLuint renderingProgram;
GLuint vao[numVAOs];
float x = 0.0f;		//水平位置
float inc = 0.01f;	//水平偏移量
float aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;




/// <summary>
/// 初始化用于渲染的 OpenGL/GLFW 状态：创建着色器程序并生成、绑定顶点数组对象（VAO）。
/// </summary>
/// <param name="window">指向 GLFW 窗口的指针；当前实现未在函数体中直接使用，但可用于基于窗口的初始化或未来扩展。</param>
void init (GLFWwindow* window) {
	renderingProgram = utils.createShaderProgram("VertexShader.glsl","FragmentShader.glsl");
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

/// <summary>
/// 使用当前渲染程序绘制一个图形以呈现帧。
/// </summary>
/// <param name="window">指向 GLFW 窗口的指针，表示渲染目标窗口。</param>
/// <param name="currentTime">当前时间（以秒为单位），可用于基于时间的渲染或动画。</param>
void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(renderingProgram);

	x += inc;
	if (x > 1.0f) {
		inc = -0.01f;
	}
	if(x < -1.0f) {
		inc = 0.01f;
	}
	utils.setFloatUniform(renderingProgram, "offset", x);			//将 x 的值传递给着色器中的 "offset" uniform 变量

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

//第二个显示函数，绘制一个点，并使其大小随时间变化
GLfloat pointSize = 1.0f;
GLfloat pointSizeInc = 0.5f;
void display2(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	pointSize += pointSizeInc;
	if(pointSize > 100.0f) {
		pointSizeInc = -0.5f;
	}
	if(pointSize < 1.0f) {
		pointSizeInc = 0.5f;
	}

	glPointSize(pointSize);
	glDrawArrays(GL_POINTS, 0, 1);
}

void display3(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	utils.setFloatUniform(renderingProgram, "aspect", aspect);	//使用 Utils 类的 setUniform 方法设置 "aspect" uniform 变量
	//GLuint aspectLoc = glGetUniformLocation(renderingProgram, "aspect");//获取着色器中 uniform 变量 "aspect" 的位置
	//glProgramUniform1f(renderingProgram, aspectLoc, aspect);
	//
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main() {
	if (!glfwInit()) {
		cerr << "Failed to initialize GLFW" << endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Window", nullptr, nullptr);
	if (!window) {
		cerr << "Failed to create GLFW window" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		exit(EXIT_FAILURE);
	}

	glfwSwapInterval(1);//开启垂直同步

	init(window);

	//主循环
	while (!glfwWindowShouldClose(window)) {



		display(window, glfwGetTime());


		glfwSwapBuffers(window);
		glfwPollEvents();


	}



	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}