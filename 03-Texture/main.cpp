#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<stack>
#include<soil2/SOIL2.h>
#include"Utils.h"

using namespace std;

//宏定义
#define WINDOW_WIDTH 1980
#define WINDOW_HEIGHT 1080
#define numVAOs 1
#define numVBOs 4

//全局变量
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

//纹理对象
GLuint brickTexture;
GLuint planeTexture;


int width = WINDOW_WIDTH, height = WINDOW_HEIGHT;
float aspect;
float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;
float pyrLocX, pyrLocY, pyrLocZ;
float planeLocX, planeLocY, planeLocZ;
std::stack<glm::mat4> mvStack;
glm::mat4 pMat, vMat, mMat, mvMat;

//窗口调整回调函数，更新视口和投影矩阵
void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);//1.0472弧度=60度
}


void setupVertices() {
	GLfloat cubePositions[108] = {//36个顶点，12个三角形，每个三角形3个顶点，每个顶点3个坐标
		-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
		1.0f,-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
		1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f,1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
		-1.0f,1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
	};
	
	// 平面坐标
	GLfloat planePositions[] = {
		// 三角形 1
		-5.0f, 0.0f, -5.0f,   // 左下
		 5.0f, 0.0f, -5.0f,   // 右下
		 5.0f, 0.0f,  5.0f,   // 右上

		 // 三角形 2
		 -5.0f, 0.0f, -5.0f,   // 左下
		  5.0f, 0.0f,  5.0f,   // 右上
		 -5.0f, 0.0f,  5.0f    // 左上
	};

	GLfloat pyramidPositions[54] = {
		-1.0f,-1.0f,1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		1.0f,-1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		1.0f,-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f,-1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f,-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f
	};


	//纹理坐标，按照顶点顺序，每个顶点2个坐标
	GLfloat pyramidTexCoords[36] = {
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,    0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,    0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	//平面纹理坐标
	// 纹理坐标 (u, v)
	GLfloat planeTexCoords[] = {
		// 三角形 1
		0.0f, 0.0f,   // 左下
		1.0f, 0.0f,   // 右下
		1.0f, 1.0f,   // 右上

		// 三角形 2
		0.0f, 0.0f,   // 左下
		1.0f, 1.0f,   // 右上
		0.0f, 1.0f    // 左上
	};


	glGenVertexArrays(numVAOs, vao);//至少要一个VAO
	glBindVertexArray(vao[0]);

	glGenBuffers(numVBOs, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidTexCoords), pyramidTexCoords, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planePositions), planePositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeTexCoords), planeTexCoords, GL_STATIC_DRAW);


}


void init(GLFWwindow* window) {
	renderingProgram = Utils::getInstance().createShaderProgram("VertexShader.glsl", "FragmentShader.glsl");
	//x左右，y上下，z前后
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;
	cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f;
	pyrLocX = 1.0f; pyrLocY = -2.0f; pyrLocZ = 0.0f;
	planeLocX = 0.0f; planeLocY = 0.0f; planeLocZ = 0.0f;
	setupVertices();
	brickTexture = Utils::getInstance().loadTexture("brick1.jpg");
	planeTexture = Utils::getInstance().loadTexture("plane.jpg");
}

void mipmapTest() {
	// 设置纹理参数，启用线性过滤和自动生成mipmap(多级渐远纹理贴图)
	//只用mipmap可解决叠影问题，但会产生模糊伪影
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	//各项异性过滤测试
	//启用各向异性过滤可以在保持纹理清晰的同时解决叠影问题，但需要显卡支持
	if(glewIsSupported("GL_EXT_texture_filter_anisotropic"))
	{
		GLfloat largest = 16.0f;//设置最大的各向异性过滤级别
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);
	}
}

/// <summary>
/// 渲染四棱锥并应用纹理贴图
/// </summary>
/// <param name="window"></param>
/// <param name="currentTime"></param>
void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);


	
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(pyrLocX, pyrLocY, pyrLocZ));
	mMat = glm::scale(mMat, glm::vec3(3.0f, 3.0f, 3.0f));
	mvMat = vMat * mMat;

	Utils::getInstance().setMat4Uniform(renderingProgram, "proj_matrix", pMat);
	Utils::getInstance().setMat4Uniform(renderingProgram, "mv_matrix", mvMat);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickTexture);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 18);

}



float cameraSpeed = 2.0f; // 摄像机移动速度
double lastTime = 0.0; // 上一次更新的时间
double deltaTime = 0.0; // 时间差
/// <summary>
/// 渲染平面并应用纹理贴图,用于观察叠影现象
/// </summary>
/// <param name="window"></param>
/// <param name="currentTime"></param>
void display2(GLFWwindow* window, double currentTime) {
	if (lastTime == 0.0) {
		lastTime = currentTime;
	}
	deltaTime = currentTime - lastTime;
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glUseProgram(renderingProgram);

	cameraY = 5.0f;
	if (cameraZ < 100.0f) {
		cameraSpeed = 5.0f;
	}
	else if(cameraZ > 200.0f){
		cameraSpeed = -5.0f;
	}
	cameraZ += cameraSpeed * deltaTime;
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mMat = glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mMat = glm::translate(mMat, glm::vec3(planeLocX, planeLocY, planeLocZ));
	mMat = glm::scale(mMat, glm::vec3(300.0f, 300.0f, 300.0f));
	mvMat = vMat * mMat;

	Utils::getInstance().setMat4Uniform(renderingProgram, "proj_matrix", pMat);
	Utils::getInstance().setMat4Uniform(renderingProgram, "mv_matrix", mvMat);
	Utils::getInstance().setFloatUniform(renderingProgram, "repeatScale", 1.0f);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, planeTexture);

	//设置交替平铺模式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//mipmap和各项异性过滤测试
	mipmapTest();

	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	lastTime = currentTime;
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


	glfwSetWindowSizeCallback(window, window_reshape_callback);
	window_reshape_callback(window, WINDOW_WIDTH, WINDOW_HEIGHT);
	init(window);

	//主循环
	while (!glfwWindowShouldClose(window)) {



		//display(window, glfwGetTime());
		display2(window, glfwGetTime());


		glfwSwapBuffers(window);
		glfwPollEvents();


	}



	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}