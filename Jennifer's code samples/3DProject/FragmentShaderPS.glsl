#version 440

uniform sampler2D particleTex;

in vec2 texCoords;
in vec3 colorFinal;
out vec4 finalColor;

void main(){
	finalColor = texture(particleTex, texCoords) * vec4(colorFinal, 1.0);
}