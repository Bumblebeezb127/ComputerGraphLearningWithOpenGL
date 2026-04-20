# 02-DataManagement - 数据管理与矩阵变换

## 概述

本模块深入探讨数据组织方式和变换技术的实际应用。内容涵盖多物体场景构建、投影矩阵与视图矩阵的使用、矩阵栈管理多物体变换、实例化渲染大量相似物体，以及坐标系统的深入理解。

## 核心知识点

### 1. 投影矩阵：透视投影

```cpp
pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
```

### 2. 视图矩阵与摄像机系统

```cpp
vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
```

### 3. 模型矩阵与多物体变换

```cpp
mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));
mvMat = vMat * mMat;
```

### 4. 动态变换：旋转与振荡运动

```cpp
tMat = glm::translate(glm::mat4(1.0f),
    glm::vec3(sin(0.35f * currentTime) * 2.0f,
              cos(0.52f * currentTime) * 2.0f,
              sin(0.7f * currentTime) * 2.0f));
rMat = glm::rotate(glm::mat4(1.0f), 1.75f * (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
```

### 5. 矩阵栈与层级变换

矩阵栈是管理多物体层级关系的核心技术：

```cpp
std::stack<glm::mat4> mvStack;

vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
mvStack.push(vMat);

// 绘制太阳
mvStack.push(mvStack.top());
mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
// ... 绘制太阳 ...
mvStack.pop();

// 绘制地球
mvStack.push(mvStack.top());
mvStack.top() *= glm::translate(glm::mat4(1.0f),
    glm::vec3(sin((float)currentTime) * 4.0f, 0.0f, cos((float)currentTime) * 4.0f));
// ... 绘制地球 ...
mvStack.pop();
```

### 6. 多VBO管理与多物体绑定

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

### 7. 深度测试与消隐

```cpp
glEnable(GL_DEPTH_TEST);
glDepthFunc(GL_LEQUAL);
glClear(GL_DEPTH_BUFFER_BIT);
```

### 8. 实例化渲染（Instanced Rendering）

当需要绘制大量相同几何体时，实例化渲染能大幅提升性能：

```cpp
glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 100000);
```

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

## 关键点总结

- 理解透视投影矩阵的原理和glm::perspective的使用
- 掌握视图矩阵模拟摄像机的工作机制
- 熟练使用矩阵栈的push/pop操作进行层级变换
- 掌握多VBO管理不同物体的数据组织和绑定方法
- 理解深度测试的原理和GL_LEQUAL比较函数的作用
- 掌握实例化渲染glDrawArraysInstanced的使用
