#include "Utils.h"
//static std::unordered_map<const char*, GLuint> textureIDs;
Utils::Utils()
{
	setupADS();
}

GLuint Utils::createShaderProgram(const char* vp, const char* fp)
{
	std::string vshaderSource = readShaderSource(vp).c_str();
	std::string fshaderSource = readShaderSource(fp).c_str();
	const char* vshaderSourceSrc = vshaderSource.c_str();
	const char* fshaderSourceSrc = fshaderSource.c_str();
	//编译顶点着色器
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vshaderSourceSrc, nullptr);
	glCompileShader(vShader);

	//编译片段着色器
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fshaderSourceSrc, nullptr);
	glCompileShader(fShader);

	//链接着色器程序
	GLuint vfProgram = glCreateProgram();
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);
	glLinkProgram(vfProgram);


	return vfProgram;
}

GLuint Utils::createShaderProgram(const char* vp, const char* gp, const char* fp)
{
	std::string vshaderSource = readShaderSource(vp).c_str();
	std::string fshaderSource = readShaderSource(fp).c_str();
	std::string gshaderSource = readShaderSource(gp).c_str();
	const char* vshaderSourceSrc = vshaderSource.c_str();
	const char* fshaderSourceSrc = fshaderSource.c_str();
	const char* gshaderSourceSrc = gshaderSource.c_str();
	//编译顶点着色器
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vshaderSourceSrc, nullptr);
	glCompileShader(vShader);

	//编译片段着色器
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fshaderSourceSrc, nullptr);
	glCompileShader(fShader);

	//编译几何着色器
	GLuint gShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(gShader, 1, &gshaderSourceSrc, nullptr);
	glCompileShader(gShader);

	//链接着色器程序
	GLuint vfgProgram = glCreateProgram();
	glAttachShader(vfgProgram, vShader);
	glAttachShader(vfgProgram, fShader);
	glAttachShader(vfgProgram, gShader);
	glLinkProgram(vfgProgram);
	return vfgProgram;
}

GLuint Utils::createShaderProgram(const char* vp, const char* tCS, const char* tES, const char* fp)
{
	return GLuint();
}

GLuint Utils::createShaderProgram(const char* vp, const char* tCS, const char* tES, const char* gp, const char* fp)
{
	return GLuint();
}


std::string Utils::readShaderSource(const char* filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);
	if (!fileStream.is_open()) {
		std::cerr << "Failed to read " << filePath << "! File doesn't exist." << std::endl;
		return "";
	}
	std::string line = "";
	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

GLuint Utils::loadTexture(const char* filePath)
{
	GLuint textureID;
	//使用 SOIL2 库加载纹理图像
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	textureID = SOIL_load_OGL_texture(
		filePath,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	if (textureID == 0) {
		std::cerr << "Failed to load texture: " << filePath << std::endl;
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}
	return textureID;
}

/// <summary>
/// 在指定的着色器程序中设置一个浮点型 uniform 变量。
/// </summary>
/// <param name="program">目标着色器程序的 GLuint 标识符（程序句柄）。</param>
/// <param name="name">要设置的 uniform 变量名（以 C 字符串表示）。</param>
/// <param name="value">要写入的浮点值。</param>
void Utils::setFloatUniform(GLuint program, const char* name, float value)
{
	GLuint pointer = glGetUniformLocation(program, name);
	glProgramUniform1f(program, pointer, value);
}

void Utils::setIntUniform(GLuint program, const char* name, int value)
{
	GLuint pointer = glGetUniformLocation(program, name);
	glProgramUniform1i(program, pointer, value);
}

void Utils::setVec3Uniform(GLuint program, const char* name, glm::vec3 value)
{
	GLuint pointer = glGetUniformLocation(program, name);
	glProgramUniform3fv(program, pointer, 1, glm::value_ptr(value));
}

void Utils::setVec4Uniform(GLuint program, const char* name, glm::vec4 value)
{
	GLuint pointer = glGetUniformLocation(program, name);
	//glProgramUniform4f(program, pointer, value.x, value.y, value.z, value.w);//使用 glProgramUniform4f 可直接设置 vec4 uniform 变量的值，无需指针
	glProgramUniform4fv(program, pointer, 1, glm::value_ptr(value));//使用 glProgramUniform4fv 可设置 vec4 uniform 变量的值，传递一个包含 vec4 数据的数组指针
}

void Utils::setMat4Uniform(GLuint program, const char* name, glm::mat4 value)
{
	GLuint pointer = glGetUniformLocation(program, name);
	glProgramUniformMatrix4fv(program, pointer, 1, GL_FALSE, glm::value_ptr(value));
}
//-----------------------------------------Texture相关---------------------------------------
glm::vec4& Utils::getAmbient(MaterialType type)
{
	switch (type) {
	case MT_GOLD:
		return goldAds.getAmbient();
	case MT_SILVER:
		return silverAds.getAmbient();
	case MT_BRONZE:
		return bronzeAds.getAmbient();
	case MT_JADE:
		return jadeAds.getAmbient();
	default:
		return goldAds.getAmbient();
	}
}

glm::vec4& Utils::getDiffuse(MaterialType type)
{
    switch (type) {
	case MT_GOLD:
		return goldAds.getDiffuse();
	case MT_SILVER:
		return silverAds.getDiffuse();
	case MT_BRONZE:
		return bronzeAds.getDiffuse();
	case MT_JADE:
		return jadeAds.getDiffuse();
	default:
		return goldAds.getDiffuse();
	}
}

glm::vec4& Utils::getSpecular(MaterialType type)
{
    switch (type) {
	case MT_GOLD:
		return goldAds.getSpecular();
	case MT_SILVER:
		return silverAds.getSpecular();
	case MT_BRONZE:
		return bronzeAds.getSpecular();
	case MT_JADE:
		return jadeAds.getSpecular();
	default:
		return goldAds.getSpecular();
	}
}

float& Utils::getShininess(MaterialType type)
{
    switch (type) {
	case MT_GOLD:
		return goldAds.getShininess();
	case MT_SILVER:
		return silverAds.getShininess();
	case MT_BRONZE:
		return bronzeAds.getShininess();
	case MT_JADE:
		return jadeAds.getShininess();
	default:
		return goldAds.getShininess();
	}
}

void Utils::addTextureID(const char* name, const char* path)
{
	textureIDs[name] = loadTexture(path);
}

GLuint Utils::getTextureID(const char* name)
{
	if (textureIDs.find(name) != textureIDs.end()) {
		return textureIDs[name];
	}
	else {
		std::cerr << "Texture ID not found for name: " << name << std::endl;
		return 0; 
	}
}

void Utils::setupADS() {
	goldAds = ADS(
		glm::vec4(0.24725f, 0.1995f, 0.0745f, 1.0f), 
		glm::vec4(0.75164f, 0.60648f, 0.22648f, 1.0f), 
		glm::vec4(0.628281f, 0.555802f, 0.366065f, 1.0f), 
		51.2f
	);
	silverAds = ADS(
		glm::vec4(0.19225f, 0.19225f, 0.19225f, 1.0f),
		glm::vec4(0.50754f, 0.50754f, 0.50754f, 1.0f),
		glm::vec4(0.508273f, 0.508273f, 0.508273f, 1.0f),
		51.2f
	);
	bronzeAds = ADS(
		glm::vec4(0.2125f, 0.1275f, 0.054f, 1.0f),
		glm::vec4(0.714f, 0.4284f, 0.18144f, 1.0f),
		glm::vec4(0.393548f, 0.271906f, 0.166721f, 1.0f),
		25.6f
	);
	jadeAds = ADS(
		glm::vec4(0.135f, 0.2225f, 0.1575f, 1.0f),
		glm::vec4(0.54f, 0.89f, 0.63f, 1.0f),
		glm::vec4(0.316228f, 0.316228f, 0.316228f, 1.0f),
		12.8f
	);
}

//-----------------------------------------Imgui相关---------------------------------------
bool Utils::ImGuiInit(GLFWwindow* window, const char* glsl_version) {
	if (isImGuiInit) {
		std::cerr << "[ImGui] Already initialized.\n";
		return false;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // 可选
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; imguiconfigflags_ // 如需停靠功能

	ImGui::StyleColorsDark();

	if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
		std::cerr << "[ImGui] GLFW backend init failed.\n";
		return false;
	}
	if (!ImGui_ImplOpenGL3_Init(glsl_version)) {
		std::cerr << "[ImGui] OpenGL3 backend init failed.\n";
		return false;
	}

	isImGuiInit = true;
	return true;
}

void Utils::ImGuiRender(void (*uiCallback)()) {
	if (!isImGuiInit) return;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (uiCallback) {
		uiCallback();
	}
	else {
		// 默认示例窗口：可以使用 ImGui::ShowDemoWindow() 或自定义一个简单的窗口
		ImGui::ShowDemoWindow();  // 或者自定义
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Utils::ImGuiShutdown() {
	if (!isImGuiInit) return;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	isImGuiInit = false;
}
//-----------------------------------------Transform---------------------------------------
glm::mat4 Utils::transform(glm::vec3 translate)
{
	return glm::translate(glm::mat4(1.0f), translate);
}

glm::mat4 Utils::transform(glm::vec3 translate, glm::vec3 rotation)
{
	glm::mat4 mat = glm::translate(glm::mat4(1.0f), translate);
	mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	return mat;
}

glm::mat4 Utils::transform(glm::vec3 translate, glm::vec3 rotation, glm::vec3 scale)
{
	glm::mat4 mat = glm::translate(glm::mat4(1.0f), translate);
	mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	mat = glm::scale(mat, scale);
	return mat;
}

glm::mat4 Utils::transform(glm::mat4& mat, glm::vec3 translate)
{
	return glm::mat4();
}

glm::mat4 Utils::transform(glm::mat4& mat, glm::vec3 translate, glm::vec3 rotation)
{
	return glm::mat4();
}

glm::mat4 Utils::transform(glm::mat4& mat, glm::vec3 translate, glm::vec3 rotation, glm::vec3 scale)
{
	return glm::mat4();
}


