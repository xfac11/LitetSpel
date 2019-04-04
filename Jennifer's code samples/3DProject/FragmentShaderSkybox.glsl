#version 440
in vec3 texCoords;
out vec4 fragment_color;

uniform samplerCube skybox;

void main(){
	fragment_color = texture(skybox, texCoords);
}