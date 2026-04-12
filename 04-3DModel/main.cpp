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


int width = WINDOW_WIDTH, height = WINDOW_HEIGHT;
float aspect;
float cameraX, cameraY, cameraZ;
float SphereLocX, SphereLocY, SphereLocZ;

Sphere mySphere(128);
Torus myTorus;
ImportedModel myModel("shuttle.obj");

std::stack<glm::mat4> mvStack;
glm::mat4 pMat, vMat, mMat, mvMat;

double lastTime = 0.0; 
double deltaTime = 0.0; 

//窗口调整回调函数，更新视口和投影矩阵
void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);//1.0472弧度=60度
}


void setupVertices() {
	std::vector<int> indices = mySphere.getIndices();
	std::vector<glm::vec3> vertices = mySphere.getVertices();
	std::vector<glm::vec3> texCoords = mySphere.getTexCoords();
	std::vector<glm::vec3> normals = mySphere.getNormals();

	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	std::vector<float> normalData;


	int numIndices = mySphere.getNumIndices();
	for (int i = 0; i < numIndices; i++)
	{
		vertexData.push_back(vertices[indices[i]].x);
		vertexData.push_back(vertices[indices[i]].y);
		vertexData.push_back(vertices[indices[i]].z);

		texCoordData.push_back(texCoords[indices[i]].s);
		texCoordData.push_back(texCoords[indices[i]].t);

		normalData.push_back(normals[indices[i]].x);
		normalData.push_back(normals[indices[i]].y);
		normalData.push_back(normals[indices[i]].z);
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

void setupVertices2() {
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

void setupVertices3() {
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
}

void init(GLFWwindow* window) {
	renderingProgram = Utils::getInstance().createShaderProgram("VertexShader.glsl", "FragmentShader.glsl");

	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 2.5f;
	SphereLocX = 0.0f; SphereLocY = 0.0f; SphereLocZ = 0.0f;
	//setupVertices();
	//setupVertices2();
	setupVertices3();
	setupTextures();
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


	rotation += deltaTime * 5.0;
	if (rotation >= 360.0f)
	{
		rotation -= 360.0f;
	}
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mMat = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	mMat = glm::rotate(mMat, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::translate(mMat, glm::vec3(SphereLocX, SphereLocY, SphereLocZ));
	mMat = glm::scale(mMat, glm::vec3(1.0f, 1.0f, 1.0f));
	mvMat = vMat * mMat;

	Utils::getInstance().setMat4Uniform(renderingProgram, "proj_matrix", pMat);
	Utils::getInstance().setMat4Uniform(renderingProgram, "mv_matrix", mvMat);

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
	glBindTexture(GL_TEXTURE_2D, earthTexture);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());

	lastTime = currentTime;
}

void display2(GLFWwindow* window, double currentTime) {
	cameraY = 0.7f;
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
	mMat = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	mMat = glm::rotate(mMat, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::translate(mMat, glm::vec3(SphereLocX, SphereLocY, SphereLocZ));
	mMat = glm::scale(mMat, glm::vec3(1.0f, 1.0f, 1.0f));
	mvMat = vMat * mMat;

	Utils::getInstance().setMat4Uniform(renderingProgram, "proj_matrix", pMat);
	Utils::getInstance().setMat4Uniform(renderingProgram, "mv_matrix", mvMat);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, blocksTexture);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//glDrawArrays(GL_TRIANGLES, 0, myTorus.getNumIndices());
	glDrawElements(GL_TRIANGLES, myTorus.getNumIndices(), GL_UNSIGNED_INT, 0);

	lastTime = currentTime;
}

void display3(GLFWwindow* window, double currentTime) {
	cameraY = 0.7f;
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
	mMat = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	mMat = glm::rotate(mMat, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::translate(mMat, glm::vec3(SphereLocX, SphereLocY, SphereLocZ));
	mMat = glm::scale(mMat, glm::vec3(1.0f, 1.0f, 1.0f));
	mvMat = vMat * mMat;

	Utils::getInstance().setMat4Uniform(renderingProgram, "proj_matrix", pMat);
	Utils::getInstance().setMat4Uniform(renderingProgram, "mv_matrix", mvMat);

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

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, myTorus.getNumIndices());
	//glDrawElements(GL_TRIANGLES, myTorus.getNumIndices(), GL_UNSIGNED_INT, 0);

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



		display3(window, glfwGetTime());


		glfwSwapBuffers(window);
		glfwPollEvents();


	}



	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}