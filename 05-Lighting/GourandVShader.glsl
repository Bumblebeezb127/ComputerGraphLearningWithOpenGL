		#version 430 

		layout(location = 0) in vec3 position;
		layout(location = 1) in vec2 texCoord;
		layout(location = 2) in vec3 normal;
		layout(binding=0) uniform sampler2D samp;

		//out vec2 tc;
		out vec4 varyingColor;

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

		void main(void) 
		{ 
			vec4 color;
			vec4 P=mv_matrix * vec4(position, 1.0);
			vec3 N=normalize(vec3(norm_matrix * vec4(-normal, 1.0)));
			vec3 L=normalize(light.position - P.xyz);

			vec3 V=normalize(-P.xyz);

			vec3 R=reflect(-L, N);

			vec3 ambient=((globalAmbient + light.ambient) * material.ambient).xyz;
			vec3 diffuse=(light.diffuse * material.diffuse).xyz * max(dot(N, L), 0.0);
			vec3 specular=(light.specular * material.specular).xyz * pow(max(dot(R, V), 0.0), material.shininess);

			varyingColor = vec4(ambient + diffuse + specular, 1.0);

			gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);

		}


		