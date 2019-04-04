#version 440
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec3 color[];

out vec2 texCoords;
out vec3 colorFinal;

uniform mat4 PROJ_MAT;

const float size = 0.2;
const vec2 offsets[4] = { vec2(0.0, 0.0), vec2(1.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0) };

void main(){
	for(int i = 0; i < 4; i++){
		colorFinal = color[0];
		texCoords = offsets[i];
		vec4 pos = gl_in[0].gl_Position;
		pos.xy += size * (offsets[i] - vec2(size));
		gl_Position = PROJ_MAT * pos;
		EmitVertex();
	}
	EndPrimitive();
}