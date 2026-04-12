		#version 430 
		uniform float offset;
		//确保图形比例
		uniform float aspect;
		
		//绘制点
		void point(){
			gl_Position = vec4(0.0, 0.0, 0.0, 1.0); 
		}

		//绘制三角形
		void triangle()
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
		}
		
		//绘制等边三角形
				void equalTriangle()
		{
			if(gl_VertexID == 0){
				gl_Position = vec4(0.25, 0, 0.0, 1.0);
			}
			else if(gl_VertexID == 1){
				gl_Position = vec4(0, 0.577, 0.0, 1.0); 
			}
			else{
				gl_Position = vec4(-0.25 , 0, 0.0, 1.0); 
			}
		}

		//构建位移矩阵
		mat4 buildTranslate(float x, float y, float z)
		{
			// mat4 translate = mat4(1.0);
			// translate[3][0] = x;
			// translate[3][1] = y;
			// translate[3][2] = z;
			// return translate;

			mat4 translate=mat4(1.0, 0.0, 0.0, 0.0,
								0.0, 1.0, 0.0, 0.0,
								0.0, 0.0, 1.0, 0.0,
								x, y, z, 1.0);
			return translate;
		}


		//构建绕X轴旋转矩阵
		mat4 buildRotateX(float angle)
		{
			// mat4 rotate = mat4(1.0);
			// float c = cos(angle);
			// float s = sin(angle);
			// rotate[1][1] = c;
			// rotate[1][2] = -s;
			// rotate[2][1] = s;
			// rotate[2][2] = c;
			// return rotate;
			float rad = radians(angle);
			mat4 rotate=mat4(1.0, 0.0, 0.0, 0.0,
							 0.0, cos(rad), -sin(rad), 0.0,
							 0.0, sin(rad), cos(rad), 0.0,
							 0.0, 0.0, 0.0, 1.0);
			return rotate;
		}

		//构建绕Y轴旋转矩阵
		mat4 buildRotateY(float angle)
		{
			// mat4 rotate = mat4(1.0);
			// float c = cos(angle);
			// float s = sin(angle);
			// rotate[0][0] = c;
			// rotate[0][2] = s;
			// rotate[2][0] = -s;
			// rotate[2][2] = c;
			// return rotate;
			float rad = radians(angle);
			mat4 rotate=mat4(cos(rad), 0.0, sin(rad), 0.0,
							 0.0, 1.0, 0.0, 0.0,
							 -sin(rad), 0.0, cos(rad), 0.0,
							 0.0, 0.0, 0.0, 1.0);
			return rotate;
		}

		//构建绕Z轴旋转矩阵
		mat4 buildRotateZ(float angle)
		{
			// mat4 rotate = mat4(1.0);
			// float c = cos(angle);
			// float s = sin(angle);
			// rotate[0][0] = c;
			// rotate[0][1] = -s;
			// rotate[1][0] = s;
			// rotate[1][1] = c;
			// return rotate;
			float rad = radians(angle);
			mat4 rotate=mat4(cos(rad), -sin(rad), 0.0, 0.0,
							 sin(rad), cos(rad), 0.0, 0.0,
							 0.0, 0.0, 1.0, 0.0,
							 0.0, 0.0, 0.0, 1.0);
			return rotate;
		}

		//构建缩放矩阵
		mat4 buildScale(float x, float y, float z)
		{
			// mat4 scale = mat4(1.0);
			// scale[0][0] = x;
			// scale[1][1] = y;
			// scale[2][2] = z;
			// return scale;

			mat4 scale=mat4(x, 0.0, 0.0, 0.0,
							0.0, y, 0.0, 0.0,
							0.0, 0.0, z, 0.0,
							0.0, 0.0, 0.0, 1.0);
			return scale;
		}

		//先平移，后旋转
		void TriangleTranslate1()
		{
			if(gl_VertexID == 0){
				//MVP遵循左乘原则，且右边的优先级高
				gl_Position = buildRotateZ(90)*buildTranslate(0.5,0.5,0)*vec4(0.25, 0, 0.0, 1.0);
			}
			else if(gl_VertexID == 1){
				gl_Position = buildRotateZ(90)*buildTranslate(0.5,0.5,0)*vec4(0, 0.577, 0.0, 1.0); 
			}
			else{
				gl_Position = buildRotateZ(90)*buildTranslate(0.5,0.5,0)*vec4(-0.25 , 0, 0.0, 1.0); 
			}
		}
		//先旋转，后平移
		void TriangleTranslate2()
		{
			if(gl_VertexID == 0){
				//MVP遵循左乘原则，且右边的优先级高,以下三种转换均属于Model范畴
				gl_Position = buildScale(1.0, aspect, 1.0)*buildTranslate(0.5,0.5,0)*buildRotateZ(90)*vec4(0.25, 0, 0.0, 1.0);
			}
			else if(gl_VertexID == 1){
				gl_Position = buildScale(1.0, aspect, 1.0)*buildTranslate(0.5,0.5,0)*buildRotateZ(90)*vec4(0, 0.433, 0.0, 1.0); 
			}
			else{
				gl_Position = buildScale(1.0, aspect, 1.0)*buildTranslate(0.5,0.5,0)*buildRotateZ(90)*vec4(-0.25 , 0, 0.0, 1.0); 
			}
		}

		void main(void) 
		{ 
			//TriangleTranslate2();
			triangle();
		};
