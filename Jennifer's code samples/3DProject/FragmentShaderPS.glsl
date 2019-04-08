#version 440

uniform sampler2D particleTex;

in vec2 texCoords;
in vec4 colorFinal;
out vec4 finalColor;

void main(){
	finalColor = texture(particleTex, texCoords) * colorFinal;
}