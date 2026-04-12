		#version 430 
		
		in vec3 varyingNormal;
		in vec3 varyingLightDir;
		in vec3 varyingVertPos;
		in vec3 varyingHalfVector;
		in vec2 tc;
		out vec4 fragColor; 

		struct PositionalLight
		{
			vec4 ambient;
			vec4 diffuse;
			vec4 specular;
			vec3 position;
		};

		struct Material
		{
			vec4 ambient;
			vec4 diffuse;
			vec4 specular;
			float shininess;
		};

		uniform vec4 globalAmbient;
		uniform PositionalLight light;
		uniform Material material;
		uniform mat4 mv_matrix;
		uniform mat4 proj_matrix;
		uniform mat4 norm_matrix;

		layout(binding=0) uniform sampler2D samp;

		void main(void) 
		{ 
			//规格化光照向量、法向量、视觉向量
			vec3 L = normalize(varyingLightDir);
			vec3 N = normalize(varyingNormal);
			vec3 V = normalize(-varyingVertPos);
			vec3 H = normalize(varyingHalfVector);

			//计算光照基于N的反射向量
			vec3 R = normalize(reflect(-L, N));

			//计算光照与平面法向量的角度
			float cosTheta = dot(L,N);

			//计算法向量N与角平分线向量H之间的角度
			float cosPhi = dot(H,N);

			//计算环境分量
			vec3 ambient = (globalAmbient * material.ambient + light.ambient * material.ambient).xyz;
			vec3 diffuse = (light.diffuse * material.diffuse).xyz * max(cosTheta, 0.0);
			vec3 specular = (light.specular * material.specular).xyz * pow(max(cosPhi, 0.0), material.shininess * 3.0f);

			//fragColor = vec4(ambient + diffuse + specular, 1.0);
			vec4 texColor=texture(samp, tc);
			vec4 lightColor = vec4(ambient + diffuse + specular, 1.0);
			fragColor = texColor * (globalAmbient + light.ambient + light.diffuse * max(dot(L,N),0.0)) + lightColor * pow(max(dot(H,N),0.0),material.shininess*3.0);
		} ;