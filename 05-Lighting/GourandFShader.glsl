		#version 430 
		in vec4 varyingColor;
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
			//color = texture(samp, tc);
			//color = vec4(tc, 0.0, 1.0);
			fragColor = varyingColor;
		} ;