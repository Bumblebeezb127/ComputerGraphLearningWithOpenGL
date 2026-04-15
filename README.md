# OpenGL 计算机图形学学习项目

本项目是一套完整的OpenGL学习教程，从基础的图形渲染管道到高级的光照模型，逐层深入地展示了现代OpenGL开发的核心技术。每个模块都包含理论知识和实践代码，帮助学习者掌握计算机图形学的关键技术点。

---

## 项目结构

```
OpenGL/
├── 01-Pipeline/          # 渲染管道与基础绘制
├── 02-DataManagement/     # 数据管理与变换
├── 03-Texture/            # 纹理映射技术
├── 04-3DModel/           # 3D模型加载与几何体生成
├── 05-Lighting/           # 光照模型
├── 06-Shadow/             # 阴影映射
├── 07-Skybox/             # 天空盒与第一人称相机
```

---

## 模块一：01-Pipeline - 渲染管道与基础绘制

### 概述

本模块是整个学习路径的起点，专注于建立对OpenGL渲染管道的基本认知。内容涵盖了从窗口初始化、上下文创建，到顶点着色器和片段着色器的编写，再到最终的图形绘制。通过本模块，学习者将理解现代OpenGL的核心工作流程：数据如何从CPU传输到GPU，经过可编程着色器的处理，最终生成像素输出到屏幕。

### 核心知识点

#### 1. OpenGL上下文与窗口初始化

窗口创建采用GLFW库实现。GLFW是一个专门为OpenGL设计的轻量级跨平台窗口管理库，它负责处理窗口上下文、用户输入事件以及与操作系统底层的交互。

```cpp
if (!glfwInit()) {
    cerr << "Failed to initialize GLFW" << endl;
    exit(EXIT_FAILURE);
}

glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Window", nullptr, nullptr);
```

关键点在于指定OpenGL版本号（4.3）以启用现代OpenGL特性。GLFW_WINDOW_HINT用于设置窗口属性，glfwCreateWindow创建实际窗口并返回句柄供后续操作使用。创建窗口后需要将上下文设为当前活跃状态，通过glfwMakeContextCurrent实现。

GLEW（OpenGL Extension Wrangler Library）的初始化同样重要。由于现代OpenGL驱动程序支持众多扩展，GLEW负责在运行时动态加载这些扩展函数：

```cpp
glewExperimental = GL_TRUE;
if (glewInit() != GLEW_OK) {
    cerr << "Failed to initialize GLEW" << endl;
    exit(EXIT_FAILURE);
}
```

glewExperimental标记设为true是为了让GLEW使用现代OpenGL的扩展加载机制，避免初始化时出现兼容性问题。

#### 2. 着色器基础与GLSL语法

着色器是运行在GPU上的可编程程序，现代OpenGL采用GLSL（OpenGL Shading Language）编写。着色器遵循#version 430标准，意味着使用OpenGL 4.3规范定义的语法和功能。

顶点着色器负责处理输入顶点数据，每个顶点都会执行一次。核心工作是根据MVP（模型-视图-投影）矩阵计算顶点在裁剪空间中的最终位置：

```glsl
#version 430 
uniform float offset;

void main(void) 
{ 
    if(gl_VertexID == 0){
        gl_Position = vec4(0.25 + offset, -0.25+offset, 0.0, 1.0);
    }
    else if(gl_VertexID == 1){
        gl_Position = vec4(-0.25 + offset, -0.25+offset, 0.0, 1.0); 
    }
    else{
        gl_Position = vec4(0.25 + offset, 0.25+offset, 0.0, 1.0); 
    }
};
```

gl_Position是着色器的内置输出变量，表示顶点在裁剪空间中的坐标。gl_VertexID是内置输入变量，表示当前处理顶点的索引，利用它可以通过条件判断为不同顶点赋予不同位置。

片段着色器负责计算每个像素的最终颜色：

```glsl
#version 430 
out vec4 color; 
void main(void) 
{ 
    color = vec4(1.0, 0.0, 0.0, 1.0);
} ;
```

out关键字声明输出变量，vec4表示四维向量，此处输出纯红色（R=1.0, G=0.0, B=0.0, A=1.0）。

#### 3. 顶点属性与VAO/VBO机制

现代OpenGL使用VAO（顶点数组对象）和VBO（顶点缓冲区对象）管理顶点数据。VAO存储顶点属性配置，VBO存储实际顶点数据。

```cpp
#define numVAOs 1
GLuint vao[numVAOs];
glGenVertexArrays(numVAOs, vao);
glBindVertexArray(vao[0]);
```

VAO的创建首先需要调用glGenVertexArrays生成标识符，然后通过glBindVertexArray将其绑定为当前VAO。绑定后的所有顶点属性配置都会保存在这个VAO中。

VBO用于存储顶点位置、颜色、纹理坐标等属性数据：

```cpp
GLuint vbo[numVBOs];
glGenBuffers(numVBOs, vbo);
glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
```

glGenBuffers创建缓冲区对象，glBindBuffer将缓冲区绑定到对应的绑定点上。GL_ARRAY_BUFFER是最常用的绑定点，用于存储顶点属性数据。glBufferData负责将数据从CPU复制到GPU，第四个参数GL_STATIC_DRAW表示数据不会频繁修改，GPU可以将其存储在显存中以获得最佳性能。

顶点属性的配置通过glVertexAttribPointer完成：

```cpp
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
glEnableVertexAttribArray(0);
```

第一个参数指定属性位置（对应着色器中layout(location=0)声明），第二个参数指定分量数量（3表示xyz三元组），第三个参数指定数据类型，第四个参数指定是否需要归一化。最后需要启用该属性才能使其生效。

#### 4. uniform变量与CPU-GPU通信

uniform是全局变量，用于从CPU端向GPU着色器传递数据。不同于顶点属性（每个顶点数据不同），uniform对于所有顶点/片段都是相同的值。

```cpp
utils.setFloatUniform(renderingProgram, "offset", x);
```

Utils类封装了uniform的设置操作，内部实现为：

```cpp
void Utils::setFloatUniform(GLuint program, const char* name, float value) {
    GLuint loc = glGetUniformLocation(program, name);
    glUniform1f(loc, value);
}
```

glGetUniformLocation获取uniform变量在程序中的位置，然后通过glUniform1f设置单精度浮点数值。类似的还有glUniform1i（整数）、glUniform3fv（vec3向量）、glUniformMatrix4fv（mat4矩阵）等。

#### 5. 变换矩阵：平移、旋转、缩放

图形学中的变换通过矩阵实现。GLSL中手写构建变换矩阵加深理解：

**位移矩阵**（沿x轴移动tx，沿y轴移动ty，沿z轴移动tz）：

```glsl
mat4 buildTranslate(float x, float y, float z)
{
    mat4 translate = mat4(1.0, 0.0, 0.0, 0.0,
                         0.0, 1.0, 0.0, 0.0,
                         0.0, 0.0, 1.0, 0.0,
                         x, y, z, 1.0);
    return translate;
}
```

**绕X轴旋转矩阵**（angle为角度值）：

```glsl
mat4 buildRotateX(float angle)
{
    float rad = radians(angle);
    mat4 rotate = mat4(1.0, 0.0, 0.0, 0.0,
                       0.0, cos(rad), -sin(rad), 0.0,
                       0.0, sin(rad), cos(rad), 0.0,
                       0.0, 0.0, 0.0, 1.0);
    return rotate;
}
```

**绕Y轴旋转矩阵**：

```glsl
mat4 buildRotateY(float angle)
{
    float rad = radians(angle);
    mat4 rotate = mat4(cos(rad), 0.0, sin(rad), 0.0,
                       0.0, 1.0, 0.0, 0.0,
                       -sin(rad), 0.0, cos(rad), 0.0,
                       0.0, 0.0, 0.0, 1.0);
    return rotate;
}
```

**绕Z轴旋转矩阵**：

```glsl
mat4 buildRotateZ(float angle)
{
    float rad = radians(angle);
    mat4 rotate = mat4(cos(rad), -sin(rad), 0.0, 0.0,
                       sin(rad), cos(rad), 0.0, 0.0,
                       0.0, 0.0, 1.0, 0.0,
                       0.0, 0.0, 0.0, 1.0);
    return rotate;
}
```

**缩放矩阵**（Sx、Sy、Sz分别表示三个轴方向的缩放因子）：

```glsl
mat4 buildScale(float x, float y, float z)
{
    mat4 scale = mat4(x, 0.0, 0.0, 0.0,
                      0.0, y, 0.0, 0.0,
                      0.0, 0.0, z, 0.0,
                      0.0, 0.0, 0.0, 1.0);
    return scale;
}
```

#### 6. 矩阵运算与变换顺序

矩阵乘法遵循左乘原则，右边矩阵先执行变换。在MVP（模型-视图-投影）系统中，变换顺序为：先模型变换（M），再视图变换（V），最后投影变换（P）。最终顶点位置计算为：`gl_Position = P * V * M * vertex`

对于模型自身的变换组合，变换顺序与数学运算顺序相反，最后执行的变换写在最左边。示例中先旋转后平移：

```glsl
gl_Position = buildTranslate(0.5, 0.5, 0) * buildRotateZ(90) * vec4(position, 1.0);
```

这表示先绕Z轴旋转90度，再将结果平移到(0.5, 0.5, 0)位置。

#### 7. 渲染循环与双缓冲

渲染循环是图形应用程序的核心，持续执行直到用户关闭窗口：

```cpp
while (!glfwWindowShouldClose(window)) {
    display(window, glfwGetTime());
    glfwSwapBuffers(window);
    glfwPollEvents();
}
```

glfwWindowShouldClose检测窗口是否收到关闭信号。display函数执行实际的渲染工作。glfwSwapBuffers将后台缓冲区（正在渲染的）与前台缓冲区（显示的）交换，这是实现平滑动画的双缓冲机制。glfwPollEvents处理输入事件（键盘、鼠标等）。

垂直同步（Vertical Sync）通过`glfwSwapInterval(1)`启用，确保画面刷新率与显示器刷新率同步，避免撕裂现象。

#### 8. 图形绘制命令

OpenGL提供多种图元绘制命令：

```cpp
glDrawArrays(GL_TRIANGLES, 0, 3);  // 绘制三角形
glDrawArrays(GL_POINTS, 0, 1);     // 绘制点
```

第一个参数指定图元类型，可选值包括：GL_POINTS（点）、GL_LINES（线段）、GL_TRIANGLES（三角形）、GL_TRIANGLE_STRIP（三角形条带）等。第二个参数指定起始索引，第三个参数指定顶点数量。

#### 9. 着色器编译与链接

着色器程序从源码编译而来，需要经过编译和链接两个阶段：

```cpp
GLuint renderingProgram = utils.createShaderProgram("VertexShader.glsl", "FragmentShader.glsl");
```

Utils类中的实现流程为：读取着色器源码 → 创建着色器对象 → 编译源码 → 附加到程序对象 → 链接程序 → 返回程序ID。

### 关键点总结

本模块的关键点包括：理解现代OpenGL的渲染管道工作原理；掌握VAO/VBO的创建、绑定和数据传输流程；熟练编写顶点着色器和片段着色器；理解uniform变量的作用并掌握CPU-GPU数据传递方法；理解MVP矩阵的概念和变换顺序；掌握平移、旋转、缩放矩阵的数学原理；理解双缓冲机制和垂直同步的意义。

---

## 模块二：02-DataManagement - 数据管理与矩阵变换

### 概述

本模块在模块一的基础上，深入探讨数据组织方式和变换技术的实际应用。内容涵盖多物体场景构建、投影矩阵与视图矩阵的使用、矩阵栈管理多物体变换、实例化渲染大量相似物体，以及坐标系统的深入理解。通过本模块，学习者将能够构建包含多个3D物体的复杂场景，并有效地管理它们的变换。

### 核心知识点

#### 1. 投影矩阵：透视投影

透视投影模拟真实世界中远处的物体看起来更小的效果，是3D图形学的基础。

```cpp
pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
```

glm::perspective参数依次为：视野角度（1.0472弧度=60度）、宽高比、近裁剪面距离、远裁剪面距离。该函数创建一个透视投影矩阵，将视锥体内的3D坐标映射到裁剪空间。

#### 2. 视图矩阵与摄像机系统

视图矩阵模拟摄像机位置和朝向，本质上是将世界坐标系转换到摄像机视角下的坐标系：

```cpp
vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
```

摄像机放在位置(cameraX, cameraY, cameraZ)时，为了让物体看起来是相对于摄像机移动的，实际操作是将所有物体向相反方向移动。这就是为什么使用负值进行平移。

#### 3. 模型矩阵与多物体变换

模型矩阵定义物体在世界空间中的位置、朝向和大小：

```cpp
mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));
mvMat = vMat * mMat;
```

最终传入着色器的MV矩阵是视图矩阵与模型矩阵的乘积，Combined矩阵负责将顶点从模型空间转换到视图空间。

#### 4. 动态变换：旋转与振荡运动

利用时间函数创造动态效果：

```cpp
tMat = glm::translate(glm::mat4(1.0f), 
    glm::vec3(sin(0.35f * currentTime) * 2.0f, 
              cos(0.52f * currentTime) * 2.0f, 
              sin(0.7f * currentTime) * 2.0f));
rMat = glm::rotate(glm::mat4(1.0f), 1.75f * (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
```

物体位置随时间在三轴上做正弦/余弦振荡，同时绕三个轴持续旋转。currentTime由GLFW提供，表示程序启动以来的秒数。

#### 5. 矩阵栈与层级变换

矩阵栈是管理多物体层级关系的核心技术，特别适合处理父子关系的物体（如太阳系中的行星系统）：

```cpp
std::stack<glm::mat4> mvStack;

vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
mvStack.push(vMat);  // 压入视图矩阵作为公共起点

// 绘制太阳
mvStack.push(mvStack.top());
mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
// ... 绘制太阳 ...
mvStack.pop();

// 绘制地球（继承太阳的变换+自己的偏移）
mvStack.push(mvStack.top());
mvStack.top() *= glm::translate(glm::mat4(1.0f), 
    glm::vec3(sin((float)currentTime) * 4.0f, 0.0f, cos((float)currentTime) * 4.0f));
// ... 绘制地球 ...
mvStack.pop();

// 绘制月球（继承地球的变换+自己的偏移）
mvStack.push(mvStack.top());
mvStack.top() *= glm::translate(glm::mat4(1.0f), 
    glm::vec3(0.0f, sin((float)currentTime) * 2.0f, cos((float)currentTime) * 2.0f));
// ... 绘制月球 ...
mvStack.pop();
```

矩阵栈的工作原理：push操作复制栈顶矩阵并压入栈中，使其成为新的栈顶；通过修改栈顶矩阵添加新的变换；pop操作移除栈顶矩阵，恢复到之前的状态。这种方式确保子物体继承父物体的所有变换，同时能够添加自己独特的变换。

#### 6. 多VBO管理与多物体绑定

当场景中有多个不同物体时，需要多个VBO分别存储各自的顶点数据：

```cpp
#define numVBOs 4
GLuint vbo[numVBOs];

// 立方体顶点数据
glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositions), cubePositions, GL_STATIC_DRAW);

// 四棱锥顶点数据
glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);
```

绘制不同物体时，通过重新绑定对应的VBO并设置顶点属性来实现：

```cpp
// 绘制立方体
glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
glEnableVertexAttribArray(0);
glDrawArrays(GL_TRIANGLES, 0, 36);

// 绘制四棱锥
glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
glEnableVertexAttribArray(0);
glDrawArrays(GL_TRIANGLES, 0, 18);
```

#### 7. 深度测试与消隐

3D场景中物体会相互遮挡，需要深度测试来确定可见性：

```cpp
glEnable(GL_DEPTH_TEST);
glDepthFunc(GL_LEQUAL);
glClear(GL_DEPTH_BUFFER_BIT);
```

glEnable启用深度测试功能。glDepthFunc设置深度比较函数，GL_LEQUAL表示当新的深度值小于等于旧值时通过测试（近处物体遮挡远处物体）。每帧开始时需要清除深度缓冲区，否则保留上一帧的深度信息会导致错误渲染。

#### 8. 实例化渲染（Instanced Rendering）

当需要绘制大量相同几何体时（如粒子系统、森林、城市建筑），实例化渲染能大幅提升性能：

```cpp
glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 100000);
```

第三个参数是每个实例的顶点数量，第四个参数是要绘制的实例总数。相比循环调用glDrawArrays，实例化渲染只需一次绘制调用，GPU内部处理所有实例。

着色器中利用gl_InstanceID访问实例索引：

```glsl
void main(void) 
{ 
    float i = gl_InstanceID + tf;
    float a = sin(203.0 * i / 8000.0) * 403.0;
    float b = cos(301.0 * i / 4001.0) * 401.0;
    float c = sin(400.0 * i / 6003.0) * 405.0;

    mat4 localTrans = buildTranslate(a, b, c);
    mat4 localRotX = buildRotateX(i * 1000.0);
    mat4 localRotY = buildRotateY(i * 1000.0);
    mat4 localRotZ = buildRotateZ(i * 1000.0);

    mat4 newMV_matrix = v_matrix * localTrans * localRotX * localRotY * localRotZ;
    gl_Position = proj_matrix * newMV_matrix * vec4(position, 1.0);
}
```

每个实例根据其ID计算独特的位置和旋转，实现大量物体的多样化表现。

#### 9. 立方体与四棱锥的几何构建

立方体由12个三角形（6个面 × 2个三角形/面）组成，共36个顶点：

```cpp
GLfloat cubePositions[108] = {
    -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f,
    // ... 共108个浮点数，36个顶点 × 3个坐标
};
```

四棱锥（金字塔）由5个顶点构成，其中底部为正方形（2个三角形），四个侧面各1个三角形：

```cpp
GLfloat pyramidPositions[54] = {
    -1.0f,-1.0f,1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 前面
    // ... 共54个浮点数
};
```

### 关键点总结

本模块的关键点包括：理解透视投影矩阵的原理和glm::perspective的使用；掌握视图矩阵模拟摄像机的工作机制；熟练使用glm::translate和glm::rotate构建模型矩阵；理解矩阵栈的push/pop操作及其在层级变换中的应用；掌握多VBO管理不同物体的数据组织和绑定方法；理解深度测试的原理和GL_LEQUAL比较函数的作用；掌握实例化渲染glDrawArraysInstanced的使用；理解gl_InstanceID在着色器中区分不同实例的方法。

---

## 模块三：03-Texture - 纹理映射技术

### 概述

纹理映射是给3D物体表面贴图的技术，大幅提升了物体的视觉真实感。本模块涵盖纹理坐标、纹理加载与绑定、纹理参数设置（过滤方式、环绕模式）、Mipmap多级渐远纹理、各向异性过滤，以及程序化纹理坐标生成等核心内容。通过本模块，学习者将能够为物体表面应用真实图像纹理，并解决纹理映射中的常见问题。

### 核心知识点

#### 1. 纹理坐标系统

纹理坐标使用(u, v)二维坐标系，范围通常为[0, 1]：

```cpp
GLfloat pyramidTexCoords[36] = {
    0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,    // 正面三角形
    // ...
};
```

纹理坐标需要与顶点数据一一对应，按照顶点顺序排列。0.0到1.0的范围映射到纹理的左下角到右上角。

#### 2. 纹理坐标属性配置

纹理坐标作为顶点属性传入，与位置坐标类似：

```cpp
// 位置坐标
glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
glEnableVertexAttribArray(0);

// 纹理坐标
glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
glEnableVertexAttribArray(1);
```

注意纹理坐标是二维数据，因此第二个参数为2而不是3。

#### 3. 纹理对象创建与绑定

使用SOIL2库加载图像文件创建OpenGL纹理对象：

```cpp
GLuint brickTexture;
brickTexture = Utils::getInstance().loadTexture("brick1.jpg");
```

纹理对象创建流程：生成纹理ID → 绑定为当前纹理 → 配置纹理参数 → 加载图像数据到GPU。

#### 4. 纹理单元与激活

纹理单元是连接纹理对象和着色器的桥梁：

```cpp
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, brickTexture);
```

GL_TEXTURE0是第一个纹理单元，可以同时激活多个纹理单元（GL_TEXTURE0到GL_TEXTURE31）。着色器中通过uniform sampler2D类型变量和binding关键字关联到具体的纹理单元：

```glsl
layout(binding=0) uniform sampler2D samp;
```

#### 5. 片段着色器中的纹理采样

使用texture函数从纹理中获取颜色：

```glsl
#version 430 
in vec2 tc;
out vec4 color; 
layout(binding=0) uniform sampler2D samp;

void main(void) 
{ 
    color = texture(samp, tc);
} ;
```

texture的第一个参数是采样器，第二个参数是纹理坐标，返回该位置的颜色值。

#### 6. 纹理参数：过滤方式

纹理过滤决定如何根据像素密度选择纹理颜色：

```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glGenerateMipmap(GL_TEXTURE_2D);
```

GL_TEXTURE_MIN_FILTER处理纹理被缩小的情况：
- GL_NEAREST：最近邻过滤，选择最接近的纹理像素，速度快但质量差
- GL_LINEAR：双线性过滤，混合相邻四个像素，效果较好
- GL_LINEAR_MIPMAP_LINEAR：三线性过滤，在Mipmap各级之间插值，质量最佳

GL_TEXTURE_MAG_FILTER处理纹理被放大的情况，通常使用GL_NEAREST或GL_LINEAR。

#### 7. Mipmap多级渐远纹理

Mipmap是预生成的纹理金字塔，每一级是上一半的尺寸：

```
原始纹理: 256×256
Mipmap级别: 128×128, 64×64, 32×32, 16×16, 8×8, 4×4, 2×2, 1×1
```

优点：避免远处物体的闪烁（摩尔纹）和远处物体过度模糊的问题。glGenerateMipmap根据当前绑定的纹理自动生成所有级别的Mipmap。

#### 8. 各向异性过滤（Anisotropic Filtering）

各向异性过滤解决了当纹理以倾斜角度观看时出现的模糊问题：

```cpp
if(glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
    GLfloat largest;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);
}
```

各向异性过滤考虑观察角度，在倾斜方向上采集更多样本来保持纹理清晰度。EXT_texture_filter_anisotropic是常用扩展，但需要显卡支持。

#### 9. 纹理环绕模式

当纹理坐标超出[0, 1]范围时，环绕模式决定如何处理：

```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
```

- GL_REPEAT：重复纹理，坐标的整数部分被忽略
- GL_CLAMP_TO_EDGE：夹取到边缘，坐标被限制在[0,1]范围
- GL_MIRRORED_REPEAT：镜像重复，相邻块互为镜像

这对程序化生成纹理坐标特别有用。

#### 10. 程序化纹理坐标

纹理坐标可以从3D顶点位置计算，实现特殊效果：

```glsl
void main(void) 
{ 
    vec2 texCoord = worldPos.xz * repeatScale;
    color = texture(samp, texCoord);
} ;
```

worldPos是顶点在世界空间中的位置，将其xz坐标乘以缩放因子作为纹理坐标。这样可以实现纹理坐标根据物体位置连续变化的效果，避免接缝问题。

#### 11. 窗口回调与动态投影矩阵更新

当窗口大小改变时，需要更新视口和投影矩阵：

```cpp
void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight) {
    aspect = (float)newWidth / (float)newHeight;
    glViewport(0, 0, newWidth, newHeight);
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

glfwSetWindowSizeCallback(window, window_reshape_callback);
```

glViewport设置渲染区域，aspect更新为新的宽高比，pMat重新计算以保持正确的透视效果。

#### 12. 平面几何与纹理映射

平面是纹理映射的常用载体：

```cpp
GLfloat planePositions[] = {
    -5.0f, 0.0f, -5.0f,   5.0f, 0.0f, -5.0f,   5.0f, 0.0f,  5.0f,
    -5.0f, 0.0f, -5.0f,   5.0f, 0.0f,  5.0f,  -5.0f, 0.0f,  5.0f
};

GLfloat planeTexCoords[] = {
    0.0f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,
    0.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f
};
```

两个三角形组成矩形平面，每个顶点配备对应的(u, v)纹理坐标，右下角(1,0)、右上角(1,1)、左上角(0,1)。

### 关键点总结

本模块的关键点包括：理解纹理坐标(u,v)与顶点的对应关系；掌握纹理对象的创建、绑定和数据加载流程；理解纹理单元的作用和glActiveTexture的使用；掌握纹理参数设置（过滤模式和环绕模式）；理解Mipmap的工作原理及其对渲染质量的提升；理解各向异性过滤解决倾斜视角模糊的原理；掌握程序化纹理坐标的生成方法；理解窗口回调函数在动态更新投影矩阵中的应用。

---

## 模块四：04-3DModel - 3D模型加载与几何体生成

### 概述

本模块专注于复杂3D几何体的创建和外部模型的加载。内容涵盖球体和圆环体的参数化生成算法、OBJ模型格式的解析、顶点数据与索引数据的组织，以及EBO（元素缓冲区对象）的使用。通过本模块，学习者将能够生成数学定义的曲面几何体，并加载使用标准3D模型文件格式的外部模型。

### 核心知识点

#### 1. 球体的参数化生成

球体通过球面坐标公式生成顶点：

```cpp
void Sphere::init(int prec) {
    numVertices = (prec + 1) * (prec + 1);
    numIndices = 6 * prec * prec;

    for (int i = 0; i <= prec; i++) {
        for (int j = 0; j <= prec; j++) {
            int index = i * (prec + 1) + j;
            float theta = toRadians((float)i / prec * 180.0f);
            float phi = toRadians((float)j / prec * 360.0f);

            vertices[index].x = sin(theta) * cos(phi);
            vertices[index].y = cos(theta);
            vertices[index].z = sin(theta) * sin(phi);

            texCoords[index].s = (float)j / prec;
            texCoords[index].t = (float)i / prec;

            normals[index] = vertices[index];  // 球面法线等于顶点位置
        }
    }
}
```

prec参数控制生成精度（经线和纬线数量）。球面坐标公式：
- x = sin(θ) × cos(φ)
- y = cos(θ)
- z = sin(θ) × sin(φ)

其中θ是纬度角（0到180度），φ是经度角（0到360度）。纹理坐标直接使用经度比例和纬度比例，法线向量由于是单位球面因此等于顶点位置。

#### 2. 球面三角网格的索引组织

球面被分割成prec×prec个小矩形，每个矩形由两个三角形组成：

```cpp
for (int i = 0; i < prec; i++) {
    for (int j = 0; j < prec; j++) {
        int index = (i * prec + j) * 6;
        int v1 = i * (prec + 1) + j;
        int v2 = v1 + prec + 1;
        int v3 = v2 + 1;
        int v4 = v1 + 1;

        indices[index] = v1;
        indices[index + 1] = v2;
        indices[index + 2] = v3;
        indices[index + 3] = v1;
        indices[index + 4] = v3;
        indices[index + 5] = v4;
    }
}
```

v1、v2、v3、v4是矩形的四个顶点索引，按逆时针顺序组织成两个三角形（v1-v2-v3和v1-v3-v4）。

#### 3. 圆环体的参数化生成

圆环体（Torus）是一个管状环面，参数包括内半径、外半径和精度：

```cpp
Torus::Torus(float innerRadius, float outerRadius, int precIn) {
    prec = precIn;
    inner = innerRadius;
    outer = outerRadius;
    init();
}
```

生成算法分为两阶段：

**第一阶段：生成横截面圆**

```cpp
for (int i = 0; i < prec + 1; i++) {
    float amt = toRadians(i * 360.0f / prec);
    float cosV = cos(amt);
    float sinV = sin(amt);

    vertices[i] = glm::vec3(outer + inner * cosV, inner * sinV, 0.0f);
    texCoords[i] = glm::vec2(0.0f, (float)i / (float)prec);

    tTrangents[i] = glm::vec3(-inner * sinV, inner * cosV, 0.0f);
    sTrangents[i] = glm::vec3(0.0f, 0.0f, 1.0f);
    normals[i] = glm::cross(sTrangents[i], tTrangents[i]);
}
```

管壁上的点位于以(outer, 0, 0)为圆心、inner为半径的圆上。

**第二阶段：绕Y轴旋转生成完整环**

```cpp
for (int ring = 1; ring < prec + 1; ring++) {
    float amt = toRadians((float)ring * 360.0f / prec);
    glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));

    for (int vert = 0; vert < prec + 1; vert++) {
        int idx = ring * (prec + 1) + vert;

        vertices[idx] = glm::vec3(rMat * glm::vec4(vertices[vert], 1.0f));
        sTrangents[idx] = glm::vec3(rMat * glm::vec4(sTrangents[vert], 1.0f));
        tTrangents[idx] = glm::vec3(rMat * glm::vec4(tTrangents[vert], 1.0f));
        normals[idx] = glm::vec3(rMat * glm::vec4(normals[vert], 1.0f));

        texCoords[idx] = glm::vec2((float)ring / (float)prec, texCoords[vert].y);
    }
}
```

通过旋转矩阵将第一阶段生成的横截面绕Y轴复制，形成完整的环面。S切向量代表环面大圆方向的切线，T切向量代表管壁小圆方向的切线，法线通过两者的叉积得到。

#### 4. OBJ模型文件解析

OBJ是Wavefront公司定义的3D模型格式，被广泛支持。本项目实现了简化的OBJ解析器：

```cpp
void ModelImporter::parseOBJ(const char *filePath) {
    ifstream fileStream(filePath, ios::in);
    string line = "";

    while (!fileStream.eof()) {
        getline(fileStream, line);

        if (line.compare(0, 2, "v ") == 0) {  // 顶点位置
            stringstream ss(line.erase(0, 1));
            ss >> x >> y >> z;
            vertVals.push_back(x); vertVals.push_back(y); vertVals.push_back(z);
        }

        if (line.compare(0, 2, "vt") == 0) {  // 纹理坐标
            stringstream ss(line.erase(0, 2));
            ss >> x >> y;
            stVals.push_back(x); stVals.push_back(y);
        }

        if (line.compare(0, 2, "vn") == 0) {  // 法线
            stringstream ss(line.erase(0, 2));
            ss >> x >> y >> z;
            normVals.push_back(x); normVals.push_back(y); normVals.push_back(z);
        }

        if (line.compare(0, 2, "f ") == 0) {  // 面
            string oneCorner, v, t, n;
            stringstream ss(line.erase(0, 2));

            for (int i = 0; i < 3; i++) {
                getline(ss, oneCorner, ' ');
                stringstream oneCornerSS(oneCorner);
                getline(oneCornerSS, v, '/');
                getline(oneCornerSS, t, '/');
                getline(oneCornerSS, n, '/');

                int vertRef = (stoi(v) - 1) * 3;
                int tcRef = (stoi(t) - 1) * 2;
                int normRef = (stoi(n) - 1) * 3;

                triangleVerts.push_back(vertVals[vertRef]);
                triangleVerts.push_back(vertVals[vertRef + 1]);
                triangleVerts.push_back(vertVals[vertRef + 2]);

                textureCoords.push_back(stVals[tcRef]);
                textureCoords.push_back(stVals[tcRef + 1]);

                normals.push_back(normVals[normRef]);
                normals.push_back(normVals[normRef + 1]);
                normals.push_back(normVals[normRef + 2]);
            }
        }
    }
}
```

解析器逐行读取文件，识别四种关键标记：v（顶点位置）、vt（纹理坐标）、vn（顶点法线）、f（面定义）。面定义使用"顶点索引/纹理索引/法线索引"格式，通过字符串分割提取。

#### 5. 元素缓冲区对象（EBO）

EBO存储顶点索引，允许顶点数据复用：

```cpp
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
```

与GL_ARRAY_BUFFER不同，GL_ELEMENT_ARRAY_BUFFER专门存储顶点的索引值。使用EBO时，顶点属性配置不变，但绘制调用改为：

```cpp
glDrawElements(GL_TRIANGLES, myTorus.getNumIndices(), GL_UNSIGNED_INT, 0);
```

第一个参数是图元类型，第二个是索引数量，第三个是索引数据类型，第四个是EBO中的偏移量。

对于球体和OBJ模型，虽然创建了索引数据，但实际渲染时仍然使用了glDrawArrays，因为这些几何体的顶点数据已经是展开后的组织形式。

#### 6. 多VBO数据组织

复杂模型通常需要多个VBO分别存储不同类型的数据：

```cpp
glGenBuffers(numVBOs, vbo);

glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_STATIC_DRAW);

glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
glBufferData(GL_ARRAY_BUFFER, texCoordData.size() * sizeof(float), &texCoordData[0], GL_STATIC_DRAW);

glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
glBufferData(GL_ARRAY_BUFFER, normalData.size() * sizeof(float), &normalData[0], GL_STATIC_DRAW);
```

然后在渲染时按顺序绑定和配置每个属性：

```cpp
glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
glEnableVertexAttribArray(0);

glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
glEnableVertexAttribArray(1);

glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
glEnableVertexAttribArray(2);
```

#### 7. 顶点法线与光照准备

顶点法线对于正确光照计算至关重要：

```cpp
normalData.push_back(normals[indices[i]].x);
normalData.push_back(normals[indices[i]].y);
normalData.push_back(normals[indices[i]].z);
```

球体的法线等于顶点位置（单位球面）。OBJ模型从文件加载法线数据。对于程序生成的平面，需要通过叉积计算法线。

#### 8. 纹理与几何的结合渲染

将纹理应用于3D模型需要正确的顶点属性配置：

```cpp
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, earthTexture);

glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
glEnableVertexAttribArray(0);

glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
glEnableVertexAttribArray(1);

glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
glEnableVertexAttribArray(2);

glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
```

纹理坐标（属性1）必须与几何顶点（属性0）正确对应，否则纹理映射会失真。

### 关键点总结

本模块的关键点包括：理解球面坐标参数化生成的数学原理；掌握圆环体两阶段生成算法（横截面圆+旋转）；理解OBJ格式中顶点、纹理坐标、法线、面的组织方式；掌握简化的OBJ解析器实现；理解EBO（元素缓冲区）的作用和glDrawElements的使用；掌握多VBO分别存储顶点、纹理坐标、法线数据的组织方式；理解顶点法线在光照计算中的重要性；掌握纹理与几何体结合渲染的完整流程。

---

## 模块五：05-Lighting - 光照模型

### 概述

光照是计算机图形学中创造真实感视觉效果的核心技术。本模块系统讲解了Phong光照模型、Blinn-Phong光照模型、Gouraud着色方法，以及光照与纹理的结合。内容涵盖环境光、漫反射、镜面反射三个分量的数学原理，材质特性（ambient、diffuse、specular、shininess），着色器架构设计（ADS结构体），法线矩阵计算，以及多种光照实现方式的对比。通过本模块，学习者将掌握主流光照模型的实现技术，能够为3D场景添加真实的光照效果。

### 核心知识点

#### 1. Phong光照模型理论基础

Phong光照模型是图形学中最经典的光照模型，将光照效果分解为三个分量：

```
最终颜色 = 环境光分量 + 漫反射分量 + 镜面反射分量
```

**环境光分量**模拟物体被环境光照亮的整体亮度，不考虑光源方向和观察方向：

```glsl
vec3 ambient = (globalAmbient * material.ambient + light.ambient * material.ambient).xyz;
```

globalAmbient是场景全局环境光，light.ambient是光源的环境光分量，material.ambient是材质的环境光吸收率。三者相乘得到环境光贡献。

**漫反射分量**模拟粗糙表面向各个方向均匀散射光线效果，与光源入射角度相关：

```glsl
vec3 diffuse = (light.diffuse * material.diffuse).xyz * max(cosTheta, 0.0);
```

cosTheta是光线方向L与表面法线N的点积，当光线正面照射表面时（法线与光线夹角小于90度）漫反射最强；当光线与法线夹角大于90度时（即光线从背面照射），点积为负，此时漫反射为0。

**镜面反射分量**模拟光滑表面（如金属、玻璃）的镜面高光效果：

```glsl
vec3 specular = (light.specular * material.specular).xyz * pow(max(cosPhi, 0.0), material.shininess);
```

cosPhi是反射光线R与视线V的点积，shininess是高光指数。高光指数越大，高光点越集中、表面越光滑；高光指数越小，高光点越分散、表面越粗糙。

#### 2. Blinn-Phong光照模型

Blinn-Phong是Phong模型的改进版，使用半程向量H替代反射向量R进行计算：

```glsl
vec3 H = normalize(varyingLightDir + (-varyingVertPos));
float cosPhi = dot(H, N);
```

半程向量是光线方向L和视线方向V的角平分线。当视线正对表面且光线直接反射到视线时，H与法线N重合，cosPhi = 1，高光最强。

Blinn-Phong的优势在于：当视角偏离反射方向时，计算结果与Phong模型接近但计算效率更高，因为省去了反射向量的计算。此外，Blinn-Phong的高光过渡更平滑：

```glsl
vec3 specular = (light.specular * material.specular).xyz * pow(max(cosPhi, 0.0), material.shininess * 3.0f);
```

注意Blinn-Phong通常需要更高的shininess值（此处乘以3.0）来获得与Phong模型相似的高光锐度。

#### 3. Gouraud着色方法

Gouraud着色与Phong着色的根本区别在于光照计算的位置：

- **Phong着色**：光照计算在片段着色器中进行，每个像素都执行完整的光照公式
- **Gouraud着色**：光照计算在顶点着色器中进行，通过插值传递给片段着色器

```glsl
// 顶点着色器中计算颜色
void main(void) 
{ 
    vec4 P = mv_matrix * vec4(position, 1.0);
    vec3 N = normalize(vec3(norm_matrix * vec4(-normal, 1.0)));
    vec3 L = normalize(light.position - P.xyz);
    vec3 V = normalize(-P.xyz);
    vec3 R = reflect(-L, N);

    vec3 ambient = ((globalAmbient + light.ambient) * material.ambient).xyz;
    vec3 diffuse = (light.diffuse * material.diffuse).xyz * max(dot(N, L), 0.0);
    vec3 specular = (light.specular * material.specular).xyz * pow(max(dot(R, V), 0.0), material.shininess);

    varyingColor = vec4(ambient + diffuse + specular, 1.0);
    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
}
```

Gouraud着色的优点是片段着色器开销小，缺点是当高光区域跨越多个顶点时，插值可能导致高光模糊或失真。Phong着色虽然计算量大，但效果更准确，是目前主流的选择。

#### 4. ADS材质系统

ADS（Ambient-Diffuse-Specular）材质系统封装了材质的光学特性：

```cpp
class ADS {
private:
    glm::vec4 ambient;    // 环境光吸收率
    glm::vec4 diffuse;   // 漫反射率
    glm::vec4 specular;  // 镜面反射率
    float shininess;     // 高光指数
public:
    // getter和setter方法
};
```

Utils类定义了多种预设材质：

```cpp
enum MaterialType {
    MT_GOLD,
    MT_SILVER,
    MT_BRONZE,
    MT_JADE
};

void Utils::setupADS() {
    goldAds = ADS(glm::vec4(0.2473f, 0.1995f, 0.0745f, 1.0f),
                  glm::vec4(0.7516f, 0.6065f, 0.2265f, 1.0f),
                  glm::vec4(0.9283f, 0.8299f, 0.6065f, 1.0f),
                  51.2f);

    silverAds = ADS(glm::vec4(0.1923f, 0.1923f, 0.1923f, 1.0f),
                    glm::vec4(0.5075f, 0.5075f, 0.5075f, 1.0f),
                    glm::vec4(0.5083f, 0.5083f, 0.5083f, 1.0f),
                    51.2f);
    // ...
}
```

黄金材质的高光反射率很高（0.9283）但颜色偏暖（红绿分量较高），银材质的高光反射率接近1.0但颜色中性。

#### 5. 光照系统的C++封装

为了在C++和着色器之间传递光照参数，需要系统化的封装：

```cpp
struct PositionalLight {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 position;
};

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
```

C++端通过统一接口设置这些uniform变量：

```cpp
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
```

结构体uniform的使用需要使用点号访问嵌套成员，如"light.ambient"。

#### 6. 法线矩阵与坐标变换

模型变换（旋转、缩放）会改变顶点的位置，但法线方向需要相应变换才能保持与表面垂直：

```cpp
invTrMat = glm::transpose(glm::inverse(mvMat));
Utils::getInstance().setMat4Uniform(renderingProgram, "norm_matrix", invTrMat);
```

对于统一缩放（x、y、z缩放比例相同），法线可以直接使用模型视图矩阵变换。但对于非统一缩放或剪切变换，必须使用法线矩阵（逆矩阵的转置）来确保法线正确。

在着色器中使用法线矩阵：

```glsl
varyingNormal = (norm_matrix * vec4(normal, 0.0)).xyz;
```

注意normal的w分量设为0表示它是方向向量而非位置点，这样平移变换不会影响法线。

#### 7. 光照参数的可配置性

项目支持运行时切换材质类型：

```cpp
MaterialType materialType = MT_SILVER;
glm::vec4 matAmbient = Utils::getInstance().getAmbient(materialType); 
glm::vec4 matDiffuse = Utils::getInstance().getDiffuse(materialType);
glm::vec4 matSpecular = Utils::getInstance().getSpecular(materialType);
float matShininess = Utils::getInstance().getShininess(materialType);
```

只需更改materialType枚举值，即可让物体呈现不同的材质外观（金的暖色调、银的冷色调、青玉的翠绿色等）。

#### 8. 光照与纹理的结合

纹理可以与光照效果结合，创造更丰富的视觉效果：

```glsl
void main(void) 
{ 
    vec3 L = normalize(varyingLightDir);
    vec3 N = normalize(varyingNormal);
    vec3 V = normalize(-varyingVertPos);
    vec3 H = normalize(varyingHalfVector);

    float cosTheta = dot(L, N);
    float cosPhi = dot(H, N);

    vec3 ambient = (globalAmbient * material.ambient + light.ambient * material.ambient).xyz;
    vec3 diffuse = (light.diffuse * material.diffuse).xyz * max(cosTheta, 0.0);
    vec3 specular = (light.specular * material.specular).xyz * pow(max(cosPhi, 0.0), material.shininess * 3.0f);

    vec4 texColor = texture(samp, tc);
    vec4 lightColor = vec4(ambient + diffuse + specular, 1.0);
    fragColor = texColor * (globalAmbient + light.ambient + light.diffuse * max(dot(L,N),0.0)) + lightColor * pow(max(dot(H,N),0.0), material.shininess*3.0);
} ;
```

这段代码将纹理颜色与光照颜色混合：纹理颜色乘以光照的漫反射分量得到受光影响的纹理颜色，高光分量则直接叠加。效果是纹理提供表面细节，光照提供立体感和真实感。

#### 9. 背面剔除

背面剔除避免渲染看不见的面，提升性能：

```cpp
glEnable(GL_CULL_FACE);
glFrontFace(GL_CCW);
```

glEnable启用面剔除。glFrontFace指定正面顶点的缠绕顺序，GL_CCW表示逆时针缠绕的顶点构成正面（默认）。背面多边形不会被发送到片段着色器处理。

#### 10. 纹理参数与光照叠加

纹理本身也可以携带光照信息，片段着色器中可以灵活组合：

```glsl
fragColor = texColor * (globalAmbient + light.ambient + light.diffuse * max(dot(L,N),0.0)) + lightColor * pow(max(dot(H,N),0.0), material.shininess*3.0);
```

这里假设纹理已经是"基本颜色"，因此漫反射环境光分量作为系数乘以纹理颜色。高光则不受纹理颜色影响，单独计算后叠加。

#### 11. 多纹理支持

项目支持加载多个纹理对象：

```cpp
GLuint brickTexture;
GLuint planeTexture;
GLuint earthTexture;
GLuint blocksTexture;
GLuint spstobTexture;
GLuint dolphinTexture;
```

在setupTextures函数中加载：

```cpp
brickTexture = Utils::getInstance().loadTexture("brick1.jpg");
earthTexture = Utils::getInstance().loadTexture("earth.jpg");
dolphinTexture = Utils::getInstance().loadTexture("Dolphin_HighPolyUV.png");
```

渲染时通过glBindTexture绑定当前使用的纹理：

```cpp
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, dolphinTexture);
```

### 关键点总结

本模块的关键点包括：理解Phong光照模型三个分量（环境光、漫反射、镜面反射）的数学原理；掌握Blinn-Phong模型使用半程向量替代反射向量的改进；理解Gouraud着色（顶点着色器）与Phong着色（片段着色器）的区别；掌握ADS材质系统的结构和预设材质的特性；理解法线矩阵（逆矩阵的转置）的必要性和计算方法；掌握统一接口设计在C++-GPU数据传输中的应用；理解光照与纹理结合的多种方式；掌握背面剔除的作用和配置方法；理解纹理与光照如何共同作用于最终颜色计算。

---

## 模块六：06-Shadow - 阴影映射技术

### 概述

阴影是增强3D场景真实感的重要因素。本模块实现了基于深度图的阴影映射算法，这是一种实时渲染中广泛使用的阴影技术。阴影映射通过从光源视角渲染场景生成深度图，然后与正常渲染时计算每个像素相对于光源的深度进行对比，判断该像素是否处于阴影中。本模块还实现了多层采样柔和阴影算法，有效消除硬阴影边缘的锯齿感。

### 核心知识点

#### 1. 阴影映射算法原理

阴影映射是一种基于深度比较的阴影技术，基本思想是从光源位置渲染场景获取深度图，然后比较每个像素的深度值来判断是否被阴影覆盖。

**两遍渲染架构**：
- **Pass 1（深度图生成）**：从光源视角渲染场景，仅计算深度值存储到深度纹理
- **Pass 2（阴影检测）**：从相机视角渲染场景，对每个像素计算其到光源的深度，与深度图中存储的最近深度比较

#### 2. 帧缓冲区对象（FBO）与深度纹理

自定义帧缓冲区用于存储阴影深度图：

```cpp
void setupShadowBuffers(GLFWwindow* window) {
    glGenFramebuffers(1, &shadowBuffer);
    glGenTextures(1, &shadowTex);
    glBindTexture(GL_TEXTURE_2D, shadowTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
                 scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
}
```

关键参数解析：
- `GL_DEPTH_COMPONENT32`：32位深度分量格式，提供足够的深度精度
- `GL_COMPARE_REF_TO_TEXTURE`：启用深度值与纹理比较的模式，用于阴影检测
- `GL_COMPARE_FUNC`：比较函数设置为GL_LEQUAL（小于等于通过）

#### 3. 深度图渲染Pass One

第一遍渲染使用简单着色器，仅输出顶点到光源空间的变换坐标：

```glsl
#version 430
layout (location=0) in vec3 vertPos;
uniform mat4 shadowMVP;
void main(void)
{   gl_Position = shadowMVP * vec4(vertPos,1.0);
}
```

`shadowMVP`是从模型空间到光源裁剪空间的变换矩阵：

```cpp
shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
```

#### 4. 阴影偏移与阴影痤疮

由于深度图分辨率有限和浮点精度问题，直接比较深度会产生"阴影痤疮"（Shadow Acne）。解决方案是使用恒定偏移或基于斜率的偏移：

```cpp
b = glm::mat4(
    0.5f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.5f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f);
shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;
```

矩阵b将深度值从[-1,1]范围映射到[0,1]范围，便于存储到纹理中。

#### 5. 阴影检测与textureProj

第二遍着色器中进行阴影检测：

```glsl
float lookup(float x, float y)
{   float t = textureProj(shadowTex, shadow_coord + vec4(x * 0.001 * shadow_coord.w,
                                                         y * 0.001 * shadow_coord.w,
                                                         -0.01, 0.0));
    return t;
}
```

`textureProj`函数将4D坐标投影到2D纹理坐标，并与存储的深度值比较。额外添加的偏移量(x, y)用于柔和阴影采样。

#### 6. 柔和阴影与多层采样

硬阴影边缘有明显的锯齿，使用多层采样平均可以产生柔和阴影效果：

```glsl
// 64采样高分辨率柔和阴影
float width = 2.5;
float endp = width * 3.0 + width/2.0;
for (float m=-endp ; m<=endp ; m=m+width)
{   for (float n=-endp ; n<=endp ; n=n+width)
    {   shadowFactor += lookup(m,n);
    }
}
shadowFactor = shadowFactor / 64.0;
```

在阴影坐标周围2.5像素范围内进行8×8=64次采样并平均，得到柔和的阴影边界。

#### 7. 片段着色器中的阴影着色

```glsl
void main(void)
{   float shadowFactor = 0.0;
    vec3 L = normalize(vLightDir);
    vec3 N = normalize(vNormal);
    vec3 V = normalize(-vVertPos);
    vec3 H = normalize(vHalfVec);

    // 计算柔和阴影因子
    // ... 64次lookup采样循环 ...

    vec4 shadowColor = globalAmbient * material.ambient + light.ambient * material.ambient;
    vec4 lightedColor = light.diffuse * material.diffuse * max(dot(L,N),0.0)
                       + light.specular * material.specular
                       * pow(max(dot(H,N),0.0),material.shininess*3.0);

    fragColor = vec4((shadowColor.xyz + shadowFactor*(lightedColor.xyz)),1.0);
}
```

核心公式：`最终颜色 = 阴影颜色 + 阴影因子 × 受光颜色`

阴影区域（shadowFactor=0）只显示环境光，完全照亮区域（shadowFactor=1）显示完整光照效果。

#### 8. Pass管理渲染流程

```cpp
void display(GLFWwindow* window, double currentTime) {
    // 从光源视角初始化
    lightVmatrix = glm::lookAt(currentLightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    lightPmatrix = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);

    // Pass 1: 渲染到阴影FBO
    glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowTex, 0);
    glDrawBuffer(GL_NONE);
    glEnable(GL_DEPTH_TEST);
    passOne();

    // Pass 2: 渲染到屏幕
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowTex);
    glDrawBuffer(GL_FRONT);
    passTwo();
}
```

关键操作：切换帧缓冲区目标、关闭颜色绘制只保留深度、启用/禁用深度测试等。

### 关键点总结

本模块的关键点包括：理解阴影映射算法的两遍渲染架构；掌握FBO创建和深度纹理配置；理解`GL_COMPARE_REF_TO_TEXTURE`模式的作用；理解shadow痤疮产生原因和解决方案；掌握多层采样柔和阴影的原理；理解`textureProj`函数在阴影检测中的应用；掌握从光源视角构建深度图的技术；理解阴影因子如何与光照分量混合。

---

## 模块七：07-Skybox - 天空盒与第一人称相机

### 概述

本模块在阴影映射基础上添加了天空盒效果和第一人称相机控制系统。天空盒是一种模拟无限远天空和环境的技术，通过在场景周围渲染一个立方体并在其内表面贴上环境纹理实现。第一人称相机控制系统让用户可以自由探索3D场景，支持键盘移动和鼠标视角控制。此外，本模块还集成了ImGui用于运行时调试参数的图形化调整界面。

### 核心知识点

#### 1. 天空盒原理

天空盒是一个包围整个场景的立方体，立方体内表面贴有环境纹理。由于天空盒代表无限远的背景，需要特殊处理确保它始终作为背景出现：

```cpp
void passTwo(void) {
    // skybox
    isSkybox = 1.0f;
    mMat = Utils::getInstance().transform(cameraLoc, -cameraRot);
    mvMat = Utils::getInstance().transform(-cameraLoc) * mMat;

    // 禁用深度写入绘制天空盒
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glEnable(GL_DEPTH_TEST);
}
```

关键点：先绘制天空盒，然后清除深度缓冲区，再绘制其他物体，最后绘制物体时会自动覆盖天空盒。

#### 2. 天空盒着色器

天空盒使用专门的着色器路径，仅进行纹理采样不做光照计算：

```glsl
void skybox(){
    tc = texCoord;
    gl_Position = proj_matrix * mv_matrix * vec4(vertPos,1.0);
}

void main(void)
{   if(isSkybox==1.0f){
        skybox();
    } else {
        model();
    }
}
```

uniform `isSkybox`用于在运行时切换着色器行为，无需编译两个独立着色器程序。

#### 3. 立方体纹理坐标映射

立方体的6个面需要正确的纹理坐标以消除接缝：

```cpp
float cubeTextureCoord[72] = {
    // back face lower right
    1.00f, 0.6666666f, 1.00f, 0.3333333f, 0.75f, 0.3333333f,
    // ... 共6个面，每个面4个顶点，每个顶点2个纹理坐标
};
```

纹理坐标的排列确保相邻面的纹理能够正确衔接。天空盒通常使用立方体全景图或6张独立的环境图像。

#### 4. 第一人称相机数学

第一人称相机需要维护三个正交向量和位置：

```cpp
struct Camera {
    glm::vec3 position;      // 相机位置
    glm::vec3 front;         // 观察方向
    glm::vec3 up;            // 世界坐标系上向量
    glm::vec3 right;         // 右向量
    float yaw;               // 偏航角（水平旋转）
    float pitch;             // 俯仰角（垂直旋转）
    float mouseSensitivity;  // 鼠标灵敏度
    float movementSpeed;      // 移动速度
};
```

鼠标移动计算角度变化：

```cpp
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= camera.mouseSensitivity;
    yoffset *= camera.mouseSensitivity;

    camera.yaw += xoffset;
    camera.pitch += yoffset;

    if (camera.pitch > 89.0f) camera.pitch = 89.0f;
    if (camera.pitch < -89.0f) camera.pitch = -89.0f;

    camera.updateVectors();
}
```

俯仰角限制在±89度防止万向锁。

#### 5. 相机向量更新

根据偏航角和俯仰角计算观察向量：

```cpp
void Camera::updateVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    this->front = front;
    this->right = glm::normalize(glm::cross(front, worldUp));
    this->up = glm::normalize(glm::cross(right, front));
}
```

三角函数将球面坐标（yaw, pitch）转换为笛卡尔方向向量。

#### 6. 键盘移动控制

```cpp
void processKeyboard(GLFWwindow* window, float deltaTime) {
    float velocity = camera.movementSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.position += camera.front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.position -= camera.front * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.position -= camera.right * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.position += camera.right * velocity;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.position -= camera.worldUp * velocity;  // 下降
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.position += camera.worldUp * velocity;   // 上升
}
```

使用deltaTime确保移动速度与帧率无关。

#### 7. ImGui集成

ImGui提供运行时GUI控制能力：

```cpp
Utils::getInstance().ImGuiInit(window, "#version 430");

// 在渲染循环中
Utils::getInstance().ImGuiRender([]()->void {
    ImGui::Begin("Controls");
    ImGui::SliderFloat3("Light Position", &lightLoc.x, -10.0f, 10.0f);
    ImGui::SliderFloat3("Camera Position", &cameraLoc.x, -10.0f, 10.0f);
    ImGui::SliderFloat3("Camera Rotation", &cameraRot.x, -180.0f, 180.0f);
    ImGui::Text("FPS: %.0f", FPS);
    ImGui::End();
});
```

Lambda表达式封装GUI回调，SliderFloat3创建滑块控制器。

#### 8. 鼠标锁定与Tab切换

```cpp
glfwSetCursorPosCallback(window, mouse_callback);
glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        if (cursorVisible) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cursorVisible = false;
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursorVisible = true;
        }
    }
}
```

Tab键切换鼠标锁定状态，释放鼠标进行GUI操作时锁定游戏视角。

#### 9. FPS计算与显示

```cpp
#define START_DRAW currentFrame = glfwGetTime(); \
                    if (lastFrame == 0.0) lastFrame = currentFrame; \
                    deltaTime = currentFrame - lastFrame; \
                    if (FPS == 0.0) FPS = 1.0 / deltaTime; \
                    addTime += deltaTime; \
                    if (addTime >= 1.0) { FPS = 1.0 / deltaTime; addTime = 0.0; }
```

每秒更新一次FPS显示，避免数字跳动过快。deltaTime用于所有与时间相关的计算。

#### 10. 统一的transform工具函数

Utils类提供矩阵变换的统一接口：

```cpp
glm::mat4 transform(glm::vec3 translation, glm::vec3 rotation) {
    glm::mat4 mat = glm::translate(glm::mat4(1.0f), translation);
    mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    return mat;
}
```

简化了多处需要组合平移和旋转的场景代码。

### 关键点总结

本模块的关键点包括：理解天空盒作为无限远背景的实现原理；掌握立方体纹理坐标映射以消除接缝；理解第一人称相机的数学模型（偏航角、俯仰角）；掌握鼠标控制视角的算法实现；理解相机三个正交向量的计算方法；掌握键盘WASD+QE移动控制的实现；理解ImGui集成用于运行时调试；理解鼠标锁定切换和deltaTime时间无关运动；掌握多纹理单元绑定（阴影纹理+天空盒纹理）；理解分层渲染（天空盒→物体）和深度缓冲管理。

---

## 通用开发环境配置

### 依赖库

| 库名 | 版本 | 用途 |
|------|------|------|
| GLFW | 3.x | 窗口管理和上下文创建 |
| GLEW | 2.x | OpenGL扩展加载 |
| GLM | 0.9.x | 数学库（矩阵、向量） |
| SOIL2 | - | 纹理图像加载 |

### 编译环境

- Visual Studio 2022
- C++17 标准
- Windows 10/11

### 构建说明

每个子项目都是独立的Visual Studio解决方案，可以直接打开.slnx文件在VS2022中编译运行。

---

## 学习路径建议

1. **入门**：从01-Pipeline开始，理解现代OpenGL的渲染管道
2. **进阶**：02-DataManagement掌握矩阵变换和场景组织
3. **提升**：03-Texture学习纹理映射增强视觉表现
4. **扩展**：04-3DModel了解复杂几何体和模型加载
5. **深入**：05-Lighting掌握光照模型创造真实感
6. **增强**：06-Shadow学习阴影映射提升立体感
7. **综合**：07-Skybox集成天空盒、相机控制和GUI实现完整场景

建议按顺序学习每个模块，理解其中的原理后再进入下一模块。每完成一个模块，尝试修改代码参数或添加新功能以加深理解。
