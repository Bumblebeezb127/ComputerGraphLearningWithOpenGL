		#version 430 
		in vec2 tc;
		out vec4 color; 
		in vec3 worldPos;
		uniform float repeatScale; 
		uniform mat4 mv_matrix;
		uniform mat4 proj_matrix;

		layout(binding=0) uniform sampler2D samp;

		void main(void) 
		{ 
			vec2 texCoord = worldPos.xz * repeatScale;
			color = texture(samp, texCoord);
			//color = texture(samp, tc);
		} ;