# 05-Lighting - 光照模型

## 概述

光照是计算机图形学中创造真实感视觉效果的核心技术。本模块系统讲解了Phong光照模型、Blinn-Phong光照模型、Gouraud着色方法，以及光照与纹理的结合。

## 核心知识点

### 1. Phong光照模型理论基础

Phong光照模型将光照效果分解为三个分量：

```
最终颜色 = 环境光分量 + 漫反射分量 + 镜面反射分量
```

**环境光分量**：
```glsl
vec3 ambient = (globalAmbient * material.ambient + light.ambient * material.ambient).xyz;
```

**漫反射分量**：
```glsl
vec3 diffuse = (light.diffuse * material.diffuse).xyz * max(cosTheta, 0.0);
```

**镜面反射分量**：
```glsl
vec3 specular = (light.specular * material.specular).xyz * pow(max(cosPhi, 0.0), material.shininess);
```

### 2. Blinn-Phong光照模型

Blinn-Phong使用半程向量H替代反射向量R进行计算：

```glsl
vec3 H = normalize(varyingLightDir + (-varyingVertPos));
float cosPhi = dot(H, N);
```

```glsl
vec3 specular = (light.specular * material.specular).xyz * pow(max(cosPhi, 0.0), material.shininess * 3.0f);
```

### 3. Gouraud着色方法

- **Phong着色**：光照计算在片段着色器中进行
- **Gouraud着色**：光照计算在顶点着色器中进行，通过插值传递

### 4. ADS材质系统

```cpp
class ADS {
private:
    glm::vec4 ambient;    // 环境光吸收率
    glm::vec4 diffuse;   // 漫反射率
    glm::vec4 specular;  // 镜面反射率
    float shininess;     // 高光指数
};
```

预设材质类型：MT_GOLD、MT_SILVER、MT_BRONZE、MT_JADE

### 5. 光照系统的C++封装

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
```

设置光照uniform：
```cpp
void installLights(glm::mat4 vMatrix) {
    lightPosV = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0f));
    Utils::getInstance().setVec4Uniform(renderingProgram, "light.ambient", lightAmbient);
    Utils::getInstance().setVec4Uniform(renderingProgram, "light.diffuse", lightDiffuse);
    Utils::getInstance().setVec4Uniform(renderingProgram, "light.specular", lightSpecular);
    Utils::getInstance().setVec3Uniform(renderingProgram, "light.position", lightPos);
    // ...材质参数
}
```

### 6. 法线矩阵与坐标变换

```cpp
invTrMat = glm::transpose(glm::inverse(mvMat));
Utils::getInstance().setMat4Uniform(renderingProgram, "norm_matrix", invTrMat);
```

着色器中使用：
```glsl
varyingNormal = (norm_matrix * vec4(normal, 0.0)).xyz;
```

### 7. 光照与纹理的结合

```glsl
void main(void)
{
    vec3 L = normalize(varyingLightDir);
    vec3 N = normalize(varyingNormal);
    vec3 V = normalize(-varyingVertPos);
    vec3 H = normalize(varyingHalfVector);

    vec3 ambient = (globalAmbient * material.ambient + light.ambient * material.ambient).xyz;
    vec3 diffuse = (light.diffuse * material.diffuse).xyz * max(cosTheta, 0.0);
    vec3 specular = (light.specular * material.specular).xyz * pow(max(cosPhi, 0.0), material.shininess * 3.0f);

    vec4 texColor = texture(samp, tc);
    vec4 lightColor = vec4(ambient + diffuse + specular, 1.0);
    fragColor = texColor * (globalAmbient + light.ambient + light.diffuse * max(dot(L,N),0.0)) + lightColor * pow(max(dot(H,N),0.0), material.shininess*3.0);
};
```

### 8. 背面剔除

```cpp
glEnable(GL_CULL_FACE);
glFrontFace(GL_CCW);
```

## 关键点总结

- 理解Phong光照模型三个分量（环境光、漫反射、镜面反射）的数学原理
- 掌握Blinn-Phong模型使用半程向量替代反射向量的改进
- 理解Gouraud着色与Phong着色的区别
- 掌握ADS材质系统的结构和预设材质的特性
- 理解法线矩阵（逆矩阵的转置）的必要性和计算方法
- 掌握背面剔除的作用和配置方法
