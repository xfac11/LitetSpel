#version 440
layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 color;

uniform mat4 VIEW_MAT;
uniform mat4 MODEL_MAT;

out vec4 colorOut;

void main(){
	vec4 color2 = vec4(1.0, 1.0, 1.0, 1.0);

	if(pos.y < -1.0){
		color2.xyz = vec3(1.0, 0.0, 1.0);
	}
	else{
		color2.xyz = vec3(0.0, 0.0, 1.0);
	}

	colorOut = color * color2;

	gl_Position = (VIEW_MAT * MODEL_MAT) * vec4(pos.xyz, 1.0);
}