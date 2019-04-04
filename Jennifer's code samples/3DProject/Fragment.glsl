#version 440

// these values are interpolated at the rasteriser
in vec2 texUVs;
in vec3 finalNormals;

// this is the final pixel colour
out vec4 fragment_color;

// this is a uniform value, the very same value for ALL pixel shader executions
uniform sampler2D textureSampler;
uniform mat4 MODEL_MAT;
uniform vec3 light_positions[];
uniform vec3 light_colors[];

uniform vec3 ambient_val;	//Ambient color
uniform vec3 diffuse_val;	//Diffuse color (Not used because of texture)
uniform vec3 specular_val;	//Specular color

in vec4 fragPos;
in vec3 pointToCamera;

in vec4 final_shadow_coord;
uniform sampler2D shadowMap;

float shadowCalc(vec4 shadow_coord, vec3 normal, vec3 light_pos){
	vec3 proj_coord = shadow_coord.xyz/shadow_coord.w;
	proj_coord = proj_coord * 0.5 + 0.5;
	float closetsDepth = texture(shadowMap, proj_coord.xy).r;
	float currentDepth = proj_coord.z;
	vec3 lightDir = normalize(light_pos - fragPos.xyz);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, proj_coord.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

	if(proj_coord.z > 1.0)
		shadow = 0.0;

	return shadow;
}

vec4 calcDiffuse(vec3 light_pos, vec3 light_color, vec3 normal){
	//Diffuse shading
	vec3 pointToLight = normalize(light_pos - fragPos.xyz);
	float diffuseFactor = dot(pointToLight, normal) / (length(pointToLight) * length(normal));
	diffuseFactor = clamp(diffuseFactor, 0, 1); //Make sure the diffuse factor isn't negative or above 1
	vec3 diffuse = diffuseFactor * light_color;
	vec4 final = vec4(diffuse, 1.0 );
	return final;
}

vec4 calcSpecular(vec3 light_pos, vec3 normal, float shadow){
	//Specularity
	vec3 unitPointToCam = normalize(pointToCamera);
	vec3 lightDir = -normalize(light_pos - fragPos.xyz);
	vec3 reflectedLightDir = reflect(lightDir, normal);
	float specFactor = dot(reflectedLightDir, unitPointToCam);
	specFactor = clamp(specFactor, 0, 1); //Make sure the specular factor isn't negative or above 1
	float shineDamper = 32f;
	float reflectivity;
	if (shadow == 1.0)
		reflectivity = 0.0f;
	else 
		reflectivity = 0.8f;
	float dampedSpec = pow(specFactor, shineDamper);
	vec3 specular = dampedSpec * reflectivity * specular_val;
	vec4 final = vec4(specular, 1.0f);
	return final;
}

vec4 calcAmbient(vec3 light_color){
	vec3 amb = ambient_val * light_color;
	return vec4(amb, 1.0);
}

void main () {
	vec4 texSample = texture(textureSampler, vec2(texUVs.s, 1 - texUVs.t)); //Texture

	vec3 norm = normalize(mat3(MODEL_MAT) * finalNormals); //Make sure the vectors are normalized in world space

	vec4 result = vec4(0.0f);

	float shadow = shadowCalc(final_shadow_coord,norm, light_positions[0]);
	result += (calcAmbient(light_colors[0]) + (1.0 - shadow ) * calcDiffuse(light_positions[0], light_colors[0], norm)) * (texSample + calcSpecular(light_positions[0], norm, shadow));
//	result += (calcAmbient(light_colors[1]) + (1.0 - shadowCalc(final_shadow_coord)) * calcDiffuse(light_positions[1], light_colors[1], norm)) * texSample + calcSpecular(light_positions[1], light_colors[1], norm);

	fragment_color = result;
}