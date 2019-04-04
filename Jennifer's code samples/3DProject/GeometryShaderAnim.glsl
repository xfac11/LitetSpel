#version 440
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 PROJ_MAT;
uniform mat4 VIEW_MAT;
uniform mat4 MODEL_MAT;

in vec2 aTexture[];
in vec3 normalsOut[];
in vec4 fragPosOut[];
out vec2 texUVs;
out vec4 fragPos;
out vec3 finalNormals;
out vec3 pointToCamera;

out float diffValue;
out float specValue;

void main(){
	//First determine the normal of the face by calculating the cross product of two vectors created by 3 triangle vertices
	//This gives us a vector that is perpendicular to the two vectors
	vec3 n = cross(gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz, gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz); //Order determines if the resulting vector will be positive or negative.
	vec3 faceNormal = mat3(VIEW_MAT * MODEL_MAT) * n; //Make sure the normal is in view space

//	Display triangle if it's facing the camera
	float angle = dot(faceNormal, vec3((VIEW_MAT * MODEL_MAT) * gl_in[0].gl_Position)); //Calculate the dot product between the face normal and camera view (point is in view space)
	if(angle <= 0.0f){
		for(int i = 0; i < gl_in.length(); i++){
			texUVs = aTexture[i];
			finalNormals = mat3(MODEL_MAT) * normalsOut[i];
			fragPos = fragPosOut[i]; //Vertex position in model-space
			gl_Position = (PROJ_MAT * VIEW_MAT * MODEL_MAT) * gl_in[i].gl_Position;
			pointToCamera = (inverse(VIEW_MAT) * vec4(0.0f, 0.0f, 0.0f, 1.0f)).xyz - fragPos.xyz; //Vector that goes from point to camera
			EmitVertex();
		}
		EndPrimitive();
	}
}