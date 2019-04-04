#version 440
layout(location=2) in vec2 uvCoord;

layout(binding=0) uniform sampler2D colourSampler; // texture 0
layout(binding=1) uniform sampler2D depthSampler;  // texture 1

out vec4 fragment_color;

layout(location=3) uniform bool sampleDepth;

void main () {

    if (sampleDepth) {
		float depthValue = texture(depthSampler, uvCoord).r;
		fragment_color = vec4(vec3(depthValue), 1.0);
	}
	else {
		fragment_color = texture(colourSampler, uvCoord);
	}
}