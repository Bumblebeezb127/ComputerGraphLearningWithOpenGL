#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<stack>
#include<soil2/SOIL2.h>
#include"Utils.h"
#include"Sphere.h"
#include"Torus.h"
#include"ImportedModel.h"

using namespace std;

//宏定义
#define WINDOW_WIDTH 1980
#define WINDOW_HEIGHT 1080
#define numVAOs 1
#define numVBOs 4
#define VERTEX_SHADER "BlinnPhongVShader.glsl"
#define FRAGMENT_SHADER "BlinnPhongFShader.glsl"

//全局变量
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

//纹理对象
GLuint brickTexture;
GLuint planeTexture;
GLuint earthTexture;
GLuint blocksTexture;
GLuint spstobTexture;
GLuint dolphinTexture;

//窗口尺寸和宽高比
int width = WINDOW_WIDTH, height = WINDOW_HEIGHT;
float aspect;

//摄像机位置、物体位置
float cameraX, cameraY, cameraZ;
float modelLocX, modelLocY, modelLocZ;

//光照参数
glm::vec3 currentLightPos, lightPosV, lightPos;
//初始化光照信息
glm::vec3 initLightPos(5.0f, 2.0f, 2.0f);
//白光特性
glm::vec4 globalAmbient(0.7f, 0.7f, 0.7f, 1.0f);
glm::vec4 lightAmbient(0.0f, 0.0f, 0.0f, 1.0f);
glm::vec4 lightDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
glm::vec4 lightSpecular(1.0f, 1.0f, 1.0f, 1.0f);
//黄金材质特性
MaterialType materialType = MT_SILVER;
glm::vec4 matAmbient = Utils::getInstance().getAmbient(materialType); 
glm::vec4 matDiffuse = Utils::getInstance().getDiffuse(materialType);
glm::vec4 matSpecular = Utils::getInstance().getSpecular(materialType);
float matShininess = Utils::getInstance().getShininess(materialType);


//创建几何对象
Sphere mySphere(128);
Torus myTorus(0.2f,0.5f,128);
ImportedModel myModel("shuttle.obj");

std::stack<glm::mat4> mvStack;
glm::mat4 pMat, vMat, mMat, mvMat, invTrMat;

//时间变量
double lastTime = 0.0; 
double deltaTime = 0.0; 


//窗口调整回调函数，更新视口和投影矩阵
void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);//1.0472弧度=60度
}

void setupVertices() {
	std::vector<int> indices = myTorus.getIndices();
	std::vector<glm::vec3> vertices = myTorus.getVertices();
	std::vector<glm::vec2> texCoords = myTorus.getTexCoords();
	std::vector<glm::vec3> normals = myTorus.getNormals();
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	std::vector<float> normalData;


	//int numIndices = myTorus.getNumIndices();
	int numVertices = myTorus.getNumVertices();
	for (int i = 0; i < numVertices; i++)
	{
		vertexData.push_back(vertices[i].x);
		vertexData.push_back(vertices[i].y);
		vertexData.push_back(vertices[i].z);

		texCoordData.push_back(texCoords[i].s);
		texCoordData.push_back(texCoords[i].t);

		normalData.push_back(normals[i].x);
		normalData.push_back(normals[i].y);
		normalData.push_back(normals[i].z);
	}
	glGenVertexArrays(numVAOs, vao);//至少要一个VAO
	glBindVertexArray(vao[0]);

	glGenBuffers(numVBOs, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, texCoordData.size() * sizeof(float), &texCoordData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, normalData.size() * sizeof(float), &normalData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

}

void setupVertices2() {
	std::vector<glm::vec3> vertices = myModel.getVertices();
	std::vector<glm::vec2> texCoords = myModel.getTextureCoords();
	std::vector<glm::vec3> normals = myModel.getNormals();
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	std::vector<float> normalData;

	int numVertices = myModel.getNumVertices();
	for (int i = 0; i < numVertices; i++)
	{
		vertexData.push_back(vertices[i].x);
		vertexData.push_back(vertices[i].y);
		vertexData.push_back(vertices[i].z);

		texCoordData.push_back(texCoords[i].s);
		texCoordData.push_back(texCoords[i].t);

		normalData.push_back(normals[i].x);
		normalData.push_back(normals[i].y);
		normalData.push_back(normals[i].z);
	}
	glGenVertexArrays(numVAOs, vao);//至少要一个VAO
	glBindVertexArray(vao[0]);

	glGenBuffers(numVBOs, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, texCoordData.size() * sizeof(float), &texCoordData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, normalData.size() * sizeof(float), &normalData[0], GL_STATIC_DRAW);


}

void setupTextures() {
	brickTexture = Utils::getInstance().loadTexture("brick1.jpg");
	planeTexture = Utils::getInstance().loadTexture("plane.jpg");
	earthTexture = Utils::getInstance().loadTexture("earth.jpg");
	blocksTexture = Utils::getInstance().loadTexture("blocks.jpg");
	spstobTexture = Utils::getInstance().loadTexture("spstob_1.jpg");
	dolphinTexture = Utils::getInstance().loadTexture("Dolphin_HighPolyUV.png");
}

void init(GLFWwindow* window) {
	renderingProgram = Utils::getInstance().createShaderProgram(VERTEX_SHADER, FRAGMENT_SHADER);

	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 2.0f;
	modelLocX = 0.0f; modelLocY = 0.0f; modelLocZ = 0.0f;
	//setupVertices();
	setupVertices2();
	setupTextures();
}

void installLights(glm::mat4 vMatrix) {
	lightPosV = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0f));
	lightPos = glm::vec3(lightPosV);
	Utils::getInstance().setVec4Uniform(renderingProgram, "globalAmbient", globalAmbient);
	Utils::getInstance().setVec4Uniform(renderingProgram, "light.ambient", lightAmbient);
	Utils::getInstance().setVec4Uniform(renderingProgram, "light.diffuse", lightDiffuse);
	Utils::getInstance().setVec4Uniform(renderingProgram, "light.specular", lightSpecular);
	Utils::getInstance().setVec3Uniform(renderingProgram, "light.position", lightPos);
	Utils::getInstance().setVec4Uniform(renderingProgram, "material.ambient", matAmbient);
	Utils::getInstance().setVec4Uniform(renderingProgram, "material.diffuse", matDiffuse);
	Utils::getInstance().setVec4Uniform(renderingProgram, "material.specular", matSpecular);
	Utils::getInstance().setFloatUniform(renderingProgram, "material.shininess", matShininess);
}

float rotation = 0.0;
void display(GLFWwindow* window, double currentTime) {
	if (lastTime == 0.0)
	{
		lastTime = currentTime;
	}
	deltaTime = currentTime - lastTime;

	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);


	rotation += deltaTime * 15.0;
	if (rotation >= 360.0f)
	{
		rotation -= 360.0f;
	}
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

	//更新光源位置
	currentLightPos = glm::vec3(initLightPos);
	installLights(vMat);

	mMat = glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mMat = glm::rotate(mMat, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	mMat = glm::translate(mMat, glm::vec3(modelLocX, modelLocY, modelLocZ));
	mMat = glm::scale(mMat, glm::vec3(1.0f, 1.0f, 1.0f));
	mvMat = vMat * mMat;

	invTrMat = glm::transpose(glm::inverse(mvMat));

	Utils::getInstance().setMat4Uniform(renderingProgram, "proj_matrix", pMat);
	Utils::getInstance().setMat4Uniform(renderingProgram, "mv_matrix", mvMat);
	Utils::getInstance().setMat4Uniform(renderingProgram, "norm_matrix", invTrMat);


	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);



	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, blocksTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
	glDrawElements(GL_TRIANGLES, myTorus.getNumIndices(), GL_UNSIGNED_INT, 0);

	lastTime = currentTime;
}

//结合光照与纹理
void display2(GLFWwindow* window, double currentTime) {
	if (lastTime == 0.0)
	{
		lastTime = currentTime;
	}
	deltaTime = currentTime - lastTime;

	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);


	rotation += deltaTime * 15.0;
	if (rotation >= 360.0f)
	{
		rotation -= 360.0f;
	}
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

	//更新光源位置
	currentLightPos = glm::vec3(initLightPos);
	installLights(vMat);

	mMat = glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mMat = glm::rotate(mMat, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::translate(mMat, glm::vec3(modelLocX, modelLocY, modelLocZ));
	mMat = glm::scale(mMat, glm::vec3(1.0f, 1.0f, 1.0f));
	mvMat = vMat * mMat;

	invTrMat = glm::transpose(glm::inverse(mvMat));

	Utils::getInstance().setMat4Uniform(renderingProgram, "proj_matrix", pMat);
	Utils::getInstance().setMat4Uniform(renderingProgram, "mv_matrix", mvMat);
	Utils::getInstance().setMat4Uniform(renderingProgram, "norm_matrix", invTrMat);


	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);



	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, spstobTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, myModel.getNumVertices());

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



		display2(window, glfwGetTime());


		glfwSwapBuffers(window);
		glfwPollEvents();


	}



	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}