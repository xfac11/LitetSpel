#version 440
layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec2 vertex_uv;

layout(location = 2) out vec2 uvCoord;

layout(location = 5) uniform mat4 transform;

void main() {
	uvCoord = vertex_uv;
	gl_Position = vec4(vertex_position, 0.0f, 1.0f) * transform;
}
