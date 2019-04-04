#version 440
layout(location = 0) in vec3 vertex_position;

uniform mat4 SHADOW_MAT; //Projection and view
uniform mat4 MODEL_MAT;

void main() {

	gl_Position = SHADOW_MAT * MODEL_MAT * vec4(vertex_position, 1.0);

}
