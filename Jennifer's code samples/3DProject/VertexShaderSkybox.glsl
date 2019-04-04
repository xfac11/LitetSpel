#version 440
layout(location = 0) in vec3 position;

uniform mat4 PROJ_MAT;
uniform mat4 VIEW_MAT;

out vec3 texCoords;

void main(){
	texCoords = position;
	vec4 pos = PROJ_MAT * VIEW_MAT * vec4(position, 1.0);
	gl_Position = pos.xyww; // w / w = 1.0 meaning we can ensure the skybox will always render behind other objects
}