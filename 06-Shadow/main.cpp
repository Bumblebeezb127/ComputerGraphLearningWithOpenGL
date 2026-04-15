#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<stack>
#include<soil2/SOIL2.h>
#include<Imgui/imgui.h>
#include"Utils.h"
#include"Sphere.h"
#include"Torus.h"
#include"ImportedModel.h"

using namespace std;

//宏定义
#define WINDOW_WIDTH 1980
#define WINDOW_HEIGHT 1080
#define numVAOs 1
#define numVBOs 5
#define VERTEX_SHADER "BlinnPhongVShader.glsl"
#define FRAGMENT_SHADER "BlinnPhongFShader.glsl"
#define START_DRAW	if (lastTime == 0.0)lastTime = currentTime; \
								deltaTime = currentTime - lastTime;
#define END_DRAW lastTime = currentTime;

//全局变量
GLuint renderingProgram1;
GLuint renderingProgram2;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

//窗口尺寸和宽高比
int width = WINDOW_WIDTH, height = WINDOW_HEIGHT;
float aspect;

//摄像机位置、物体位置和光源位置
glm::vec3 torusLoc(1.6f, 0.0f, -0.3f);
glm::vec3 pyrLoc(-1.0f, 0.1f, 0.3f);
glm::vec3 cameraLoc(0.0f, 0.2f, 6.0f);
glm::vec3 lightLoc(-3.8f, 2.2f, 1.1f);

//光照参数
glm::vec3 currentLightPos, lightPosV, lightPos;
//初始化光照信息
glm::vec3 initLightPos(5.0f, 2.0f, 2.0f);
//白光特性
glm::vec4 globalAmbient(0.7f, 0.7f, 0.7f, 1.0f);
glm::vec4 lightAmbient(0.0f, 0.0f, 0.0f, 1.0f);
glm::vec4 lightDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
glm::vec4 lightSpecular(1.0f, 1.0f, 1.0f, 1.0f);

//黄金材质
glm::vec4 goldMatAmb = Utils::getInstance().getAmbient(MT_GOLD); 
glm::vec4 goldMatDif = Utils::getInstance().getDiffuse(MT_GOLD);
glm::vec4 goldMatSpec = Utils::getInstance().getSpecular(MT_GOLD);
float goldMatShi = Utils::getInstance().getShininess(MT_GOLD);
//青铜材质
glm::vec4 bronzeMatAmb = Utils::getInstance().getAmbient(MT_BRONZE);
glm::vec4 bronzeMatDif = Utils::getInstance().getDiffuse(MT_BRONZE);
glm::vec4 bronzeMatSpec = Utils::getInstance().getSpecular(MT_BRONZE);
float bronzeMatShi = Utils::getInstance().getShininess(MT_BRONZE);
//当前材质
glm::vec4 currMatAmb, currMatDif, currMatSpec;
float currMatShi;
//创建几何对象
Torus myTorus(0.2f,0.5f,128);
ImportedModel pyramid("pyr.obj");
int numPyramidVertices, numTorusVertices, numTorusIndices;

std::stack<glm::mat4> mvStack;
glm::mat4 pMat, vMat, mMat, mvMat, invTrMat;

//时间变量
double lastTime = 0.0; 
double deltaTime = 0.0; 

//阴影相关
int scSizeX, scSizeY;
GLuint shadowTex, shadowBuffer;
glm::mat4 lightVmatrix;
glm::mat4 lightPmatrix;
glm::mat4 shadowMVP1;
glm::mat4 shadowMVP2;
glm::mat4 b;

//窗口调整回调函数，更新视口和投影矩阵
void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);//1.0472弧度=60度
}

void setupVertices() {
	// pyramid definition

	numPyramidVertices = pyramid.getNumVertices();
	std::vector<glm::vec3> vert = pyramid.getVertices();
	std::vector<glm::vec3> norm = pyramid.getNormals();

	std::vector<float> pyramidPvalues;
	std::vector<float> pyramidNvalues;

	for (int i = 0; i < numPyramidVertices; i++) {
		pyramidPvalues.push_back((vert[i]).x);
		pyramidPvalues.push_back((vert[i]).y);
		pyramidPvalues.push_back((vert[i]).z);
		pyramidNvalues.push_back((norm[i]).x);
		pyramidNvalues.push_back((norm[i]).y);
		pyramidNvalues.push_back((norm[i]).z);
	}

	// torus definition

	numTorusVertices = myTorus.getNumVertices();
	numTorusIndices = myTorus.getNumIndices();
	std::vector<int> ind = myTorus.getIndices();
	vert = myTorus.getVertices();
	norm = myTorus.getNormals();

	std::vector<float> torusPvalues;
	std::vector<float> torusNvalues;

	for (int i = 0; i < numTorusVertices; i++) {
		torusPvalues.push_back(vert[i].x);
		torusPvalues.push_back(vert[i].y);
		torusPvalues.push_back(vert[i].z);
		torusNvalues.push_back(norm[i].x);
		torusNvalues.push_back(norm[i].y);
		torusNvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, torusPvalues.size() * 4, &torusPvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, pyramidPvalues.size() * 4, &pyramidPvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, torusNvalues.size() * 4, &torusNvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, pyramidNvalues.size() * 4, &pyramidNvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);
}


void setupTextures() {
	Utils::getInstance().addTextureID("blocksTexture", "blocks.jpg");
}

void setupShadowBuffers(GLFWwindow* window) {
	glfwGetFramebufferSize(window, &width, &height);
	scSizeX = width;
	scSizeY = height;

	glGenFramebuffers(1, &shadowBuffer);

	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void init(GLFWwindow* window) {
	renderingProgram1 = Utils::getInstance().createShaderProgram("vert1Shader.glsl","frag1Shader.glsl");
	renderingProgram2 = Utils::getInstance().createShaderProgram("vert2Shader.glsl", "frag2Shader.glsl");

	setupVertices();
	setupTextures();
	setupShadowBuffers(window);

	b = glm::mat4(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);
}

void installLights(GLuint renderingProgram,glm::mat4 vMatrix) {
	lightPosV = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0f));
	lightPos = glm::vec3(lightPosV);
	//使用中间变量存储防止直接传递全局变量时可能出现的类型转换问题
	glm::vec4 matAmb = currMatAmb;
	glm::vec4 matDif = currMatDif;
	glm::vec4 matSpec = currMatSpec;
	float matShi = currMatShi;
	Utils::getInstance().setVec4Uniform(renderingProgram, "globalAmbient", globalAmbient);
	Utils::getInstance().setVec4Uniform(renderingProgram, "light.ambient", lightAmbient);
	Utils::getInstance().setVec4Uniform(renderingProgram, "light.diffuse", lightDiffuse);
	Utils::getInstance().setVec4Uniform(renderingProgram, "light.specular", lightSpecular);
	Utils::getInstance().setVec3Uniform(renderingProgram, "light.position", lightPos);
	Utils::getInstance().setVec4Uniform(renderingProgram, "material.ambient", matAmb);
	Utils::getInstance().setVec4Uniform(renderingProgram, "material.diffuse", matDif);
	Utils::getInstance().setVec4Uniform(renderingProgram, "material.specular", matSpec);
	Utils::getInstance().setFloatUniform(renderingProgram, "material.shininess", matShi);
}

void passOne() {
	glUseProgram(renderingProgram1);

	// draw the torus

	mMat = glm::translate(glm::mat4(1.0f), torusLoc);
	mMat = glm::rotate(mMat, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
	Utils::getInstance().setMat4Uniform(renderingProgram1, "shadowMVP", shadowMVP1);


	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[4]);
	glDrawElements(GL_TRIANGLES, numTorusIndices, GL_UNSIGNED_INT, 0);

	// draw the pyramid

	mMat = glm::translate(glm::mat4(1.0f), pyrLoc);
	mMat = glm::rotate(mMat, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mMat = glm::rotate(mMat, glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
	Utils::getInstance().setMat4Uniform(renderingProgram1, "shadowMVP", shadowMVP1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, numPyramidVertices);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void passTwo(void) {
	glUseProgram(renderingProgram2);

	// draw the torus

	currMatAmb = bronzeMatAmb; 
	currMatDif = bronzeMatDif;
	currMatSpec = bronzeMatSpec;
	currMatShi = bronzeMatShi;

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraLoc.x, -cameraLoc.y, -cameraLoc.z));

	mMat = glm::translate(glm::mat4(1.0f), torusLoc);
	mMat = glm::rotate(mMat, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	currentLightPos = glm::vec3(lightLoc);
	installLights(renderingProgram2, vMat);

	mvMat = vMat * mMat;
	invTrMat = glm::transpose(glm::inverse(mvMat));
	shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;

	Utils::getInstance().setMat4Uniform(renderingProgram2, "mv_matrix", mvMat);
	Utils::getInstance().setMat4Uniform(renderingProgram2, "proj_matrix", pMat);
	Utils::getInstance().setMat4Uniform(renderingProgram2, "norm_matrix", invTrMat);
	Utils::getInstance().setMat4Uniform(renderingProgram2, "shadowMVP", shadowMVP2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[4]);
	glDrawElements(GL_TRIANGLES, numTorusIndices, GL_UNSIGNED_INT, 0);

	// draw the pyramid

	currMatAmb = goldMatAmb;
	currMatDif = goldMatDif;
	currMatSpec = goldMatSpec;
	currMatShi = goldMatShi;

	mMat = glm::translate(glm::mat4(1.0f), pyrLoc);
	mMat = glm::rotate(mMat, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mMat = glm::rotate(mMat, glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	currentLightPos = glm::vec3(lightLoc);
	installLights(renderingProgram2, vMat);

	mvMat = vMat * mMat;
	invTrMat = glm::transpose(glm::inverse(mvMat));
	shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;

	Utils::getInstance().setMat4Uniform(renderingProgram2, "mv_matrix", mvMat);
	Utils::getInstance().setMat4Uniform(renderingProgram2, "proj_matrix", pMat);
	Utils::getInstance().setMat4Uniform(renderingProgram2, "norm_matrix", invTrMat);
	Utils::getInstance().setMat4Uniform(renderingProgram2, "shadowMVP", shadowMVP2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, numPyramidVertices);
}


float rotation = 0.0;
void display(GLFWwindow* window, double currentTime) {
	START_DRAW
	std::cout << 1.0 / deltaTime << " FPS" << std::endl;
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	//从光源视角初始化
	lightVmatrix = glm::lookAt(currentLightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	lightPmatrix = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);

	//使用自定义帧缓冲区，将阴影纹理附着到其上
	glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowTex, 0);

	//关闭绘制颜色缓冲区，启用深度测试
	glDrawBuffer(GL_NONE);
	glEnable(GL_DEPTH_TEST);

	passOne();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glDrawBuffer(GL_FRONT);

	passTwo();

	END_DRAW
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



		display(window, glfwGetTime());


		glfwSwapBuffers(window);
		glfwPollEvents();


	}



	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}