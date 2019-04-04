#version 440
layout(location = 0) in vec4 pos;

uniform mat4 VIEW_MAT;
uniform mat4 MODEL_MAT;

out vec3 color;

void main(){
	if(pos.y < -1.0){
		color = vec3(1.0, 0.0, 1.0);
	}
	else{
		color = vec3(0.0, 0.0, 1.0);
	}
//	color = vec3(pos.y, pos.x, pos.z);
	gl_Position = (VIEW_MAT * MODEL_MAT) * vec4(pos.xyz, 1.0);
}