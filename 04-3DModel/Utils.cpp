#include "Utils.h"

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
