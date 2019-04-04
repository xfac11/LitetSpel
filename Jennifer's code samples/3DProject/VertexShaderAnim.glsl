#version 440
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 texture_coords;
layout(location = 2) in vec3 normals;

//Vertex joint data
layout(location = 3) in ivec4 joint_indices;
layout(location = 4) in vec4 weights;

const int MAX_JOINTS = 50;
uniform mat4 jointTransforms[MAX_JOINTS];

out vec2 aTexture;
out vec3 normalsOut;
out vec4 fragPosOut;
out vec4 justPos;

uniform mat4 MODEL_MAT;

void main() {
	aTexture = texture_coords; //Pass the texture info to the geometry shader

	mat4 jointTransform  = jointTransforms[joint_indices[0]] * weights[0];
	jointTransform		+= jointTransforms[joint_indices[1]] * weights[1];
	jointTransform		+= jointTransforms[joint_indices[2]] * weights[2];
	jointTransform		+= jointTransforms[joint_indices[3]] * weights[3];

	vec4 transformedPos = jointTransform * vec4(vertex_position, 1.0);

	normalsOut = mat3(jointTransform) * normals;
	fragPosOut = MODEL_MAT * transformedPos; //Vertex position in model-space
	gl_Position = transformedPos;
}