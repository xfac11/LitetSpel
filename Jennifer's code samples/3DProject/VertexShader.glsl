#version 440
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 texture_coords;
layout(location = 2) in vec3 normals;

out vec2 aTexture;
out vec3 normalsOut;
out vec4 shadow_coord;

uniform mat4 MODEL_MAT; // PF
uniform mat4 SHADOW_MAT; // PF

void main() {
	aTexture = texture_coords; //Pass the texture info to the geometry shader
	normalsOut = normals;

	gl_Position = vec4(vertex_position, 1.0);

	shadow_coord = SHADOW_MAT * (MODEL_MAT * vec4(vertex_position, 1.0)); // PF
}