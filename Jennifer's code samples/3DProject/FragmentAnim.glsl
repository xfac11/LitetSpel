#version 440

// these values are interpolated at the rasteriser
in vec2 texUVs;
in vec3 finalNormals;

// this is the final pixel colour
out vec4 fragment_color;

// this is a uniform value, the very same value for ALL pixel shader executions
uniform sampler2D textureSampler;
uniform vec3 light_positions[];
uniform vec3 light_colors[];

uniform vec3 ambient_val;	//Ambient color
uniform vec3 diffuse_val;	//Diffuse color (Not used because of texture)
uniform vec3 specular_val;	//Specular color

in vec4 fragPos;
in vec3 pointToCamera;

vec4 calcDiffuse(vec3 light_pos, vec3 light_color, vec3 normal){
	vec3 am = ambient_val * light_color; // PF: the ambient 
	//Diffuse shading
	vec3 pointToLight = normalize(light_pos - fragPos.xyz);
	float diffuseFactor = dot(pointToLight, normal) / (length(pointToLight) * length(normal));
	diffuseFactor = clamp(diffuseFactor, 0, 1); //Make sure the diffuse factor isn't negative or above 1
	vec3 diffuse = diffuseFactor * light_color;
	vec4 final = vec4(am + diffuse, 1.0 );
	return final;
}

vec4 calcSpecular(vec3 light_pos, vec3 light_color, vec3 normal){
	//Specularity
	vec3 unitPointToCam = normalize(pointToCamera);
	vec3 lightDir = -normalize(light_pos - fragPos.xyz);
	vec3 reflectedLightDir = reflect(lightDir, normal);
	float specFactor = dot(reflectedLightDir, unitPointToCam);
	specFactor = clamp(specFactor, 0, 1); //Make sure the specular factor isn't negative or above 1
	float shineDamper = 32f;
	float reflectivity = 0.8f;
	float dampedSpec = pow(specFactor, shineDamper);
	vec3 specular = dampedSpec * reflectivity * specular_val * light_color;
	vec4 final = vec4(specular, 1.0f);
	return final;
}

void main () {
	vec4 texSample = texture(textureSampler, vec2(texUVs.s, 1 - texUVs.t)); //Texture

	vec3 norm = normalize(finalNormals); //Make sure the vectors are normalized in world space

	vec4 result = vec4(0.0f); //This value should be 0.0f but is right now used to verify that this shader is being used
	result += calcDiffuse(light_positions[0], light_colors[0], norm) * texSample + calcSpecular(light_positions[0], light_colors[0], norm);
//	result += calcDiffuse(light_positions[1], light_colors[1], norm) * texSample + calcSpecular(light_positions[1], light_colors[1], norm);

	fragment_color = result;
}