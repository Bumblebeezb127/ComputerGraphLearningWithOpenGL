# OpenGL 计算机图形学学习项目

本项目是一套完整的OpenGL学习教程，从基础的图形渲染管道到高级的光照模型，逐层深入地展示了现代OpenGL开发的核心技术。

---

## 项目结构

```
OpenGL/
├── doc/                   # 各模块详细文档
│   ├── 01-Pipeline.md
│   ├── 02-DataManagement.md
│   ├── 03-Texture.md
│   ├── 04-3DModel.md
│   ├── 05-Lighting.md
│   ├── 06-Shadow.md
│   └── 07-Skybox.md
├── 01-Pipeline/          # 渲染管道与基础绘制
├── 02-DataManagement/    # 数据管理与变换
├── 03-Texture/           # 纹理映射技术
├── 04-3DModel/           # 3D模型加载与几何体生成
├── 05-Lighting/          # 光照模型
├── 06-Shadow/           # 阴影映射
└── 07-Skybox/           # 天空盒与第一人称相机
```

---

## 模块索引

| 模块 | 名称 | 核心内容 |
|------|------|----------|
| [01-Pipeline](doc/01-Pipeline.md) | 渲染管道与基础绘制 | GLFW/GLEW初始化、VAO/VBO、着色器、GLSL、变换矩阵、渲染循环 |
| [02-DataManagement](doc/02-DataManagement.md) | 数据管理与变换 | 投影矩阵、视图矩阵、矩阵栈、实例化渲染、深度测试 |
| [03-Texture](doc/03-Texture.md) | 纹理映射技术 | 纹理坐标、Mipmap、各向异性过滤、纹理单元、环绕模式 |
| [04-3DModel](doc/04-3DModel.md) | 3D模型加载与几何体生成 | 球体/圆环体参数化生成、OBJ解析、EBO |
| [05-Lighting](doc/05-Lighting.md) | 光照模型 | Phong/Blinn-Phong/Gouraud模型、ADS材质系统、法线矩阵 |
| [06-Shadow](doc/06-Shadow.md) | 阴影映射技术 | FBO深度纹理、阴影痤疮、柔和阴影(64层采样) |
| [07-Skybox](doc/07-Skybox.md) | 天空盒与第一人称相机 | 天空盒、偏航角/俯仰角相机、ImGui集成 |

---

## 依赖环境

- GLFW 3.x
- GLEW 2.x
- GLM 0.9.x
- SOIL2
- Visual Studio 2022 / C++17

---

## 学习路径

1. **01-Pipeline** - 理解现代OpenGL渲染管道
2. **02-DataManagement** - 掌握矩阵变换和场景组织
3. **03-Texture** - 学习纹理映射增强视觉表现
4. **04-3DModel** - 了解复杂几何体和模型加载
5. **05-Lighting** - 掌握光照模型创造真实感
6. **06-Shadow** - 学习阴影映射提升立体感
7. **07-Skybox** - 集成天空盒、相机控制和GUI实现完整场景
