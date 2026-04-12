		#version 430 


		//MVP矩阵值
		uniform mat4 mv_matrix;
		uniform mat4 proj_matrix;
		uniform mat4 v_matrix;
		uniform float tf;

		//接收来自VBO的值
		layout(location = 0) in vec3 position;

		out vec4 varyingColor;

		mat4 buildTranslate(float x, float y, float z);
		mat4 buildRotateX(float rad);
		mat4 buildRotateY(float rad);
		mat4 buildRotateZ(float rad);
		mat4 buildScale(float x, float y, float z);


		mat4 instancedRender()
		{
			//实例化渲染的代码
			//可以使用gl_InstanceID来获取当前实例的ID，并根据ID进行不同的变换或颜色计算
			float i=gl_InstanceID+tf;
			float a=sin(203.0*i/8000.0)*403.0;
			float b=cos(301.0*i/4001.0)*401.0;
			float c=sin(400.0*i/6003.0)*405.0;

			mat4 localRotX=buildRotateX(i*1000.0);
			mat4 localRotY=buildRotateY(i*1000.0);
			mat4 localRotZ=buildRotateZ(i*1000.0);
			mat4 localTrans=buildTranslate(a, b, c);

			mat4 newMV_matrix=v_matrix*localTrans*localRotX*localRotY*localRotZ;
			return newMV_matrix;
		}

		void main(void) 
		{ 
			mat4 mv_matrix=instancedRender();

			gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
			varyingColor = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
		}


				//构建位移矩阵
		mat4 buildTranslate(float x, float y, float z)
		{
			mat4 translate=mat4(1.0, 0.0, 0.0, 0.0,
								0.0, 1.0, 0.0, 0.0,
								0.0, 0.0, 1.0, 0.0,
								x, y, z, 1.0);
			return translate;
		}


		//构建绕X轴旋转矩阵
		mat4 buildRotateX(float rad)
		{
			//float rad = radians(angle);
			mat4 rotate=mat4(1.0, 0.0, 0.0, 0.0,
							 0.0, cos(rad), -sin(rad), 0.0,
							 0.0, sin(rad), cos(rad), 0.0,
							 0.0, 0.0, 0.0, 1.0);
			return rotate;
		}

		//构建绕Y轴旋转矩阵
		mat4 buildRotateY(float rad)
		{
			//float rad = radians(angle);
			mat4 rotate=mat4(cos(rad), 0.0, sin(rad), 0.0,
							 0.0, 1.0, 0.0, 0.0,
							 -sin(rad), 0.0, cos(rad), 0.0,
							 0.0, 0.0, 0.0, 1.0);
			return rotate;
		}

		//构建绕Z轴旋转矩阵
		mat4 buildRotateZ(float rad)
		{
			//float rad = radians(angle);
			mat4 rotate=mat4(cos(rad), -sin(rad), 0.0, 0.0,
							 sin(rad), cos(rad), 0.0, 0.0,
							 0.0, 0.0, 1.0, 0.0,
							 0.0, 0.0, 0.0, 1.0);
			return rotate;
		}

		//构建缩放矩阵
		mat4 buildScale(float x, float y, float z)
		{
			mat4 scale=mat4(x, 0.0, 0.0, 0.0,
							0.0, y, 0.0, 0.0,
							0.0, 0.0, z, 0.0,
							0.0, 0.0, 0.0, 1.0);
			return scale;
		}