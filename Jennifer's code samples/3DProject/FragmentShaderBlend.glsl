#version 440
// these values are interpolated at the rasteriser
in vec2 texUVs;
in vec3 finalNormals;

// this is the final pixel colour
out vec4 fragment_color;

uniform sampler2D textureSampler;
uniform sampler2D rTexSampler; //Red channel
uniform sampler2D gTexSampler; //Green channel
uniform sampler2D bTexSampler; //Blue channel
uniform sampler2D blendmapSampler;
uniform sampler2D shadowMap;

uniform mat4 MODEL_MAT;
uniform vec3 light_positions[];
uniform vec3 light_colors[];

uniform vec3 ambient_val;	//Ambient color
uniform vec3 diffuse_val;	//Diffuse color (Not used because of texture)
uniform vec3 specular_val;	//Specular color

in vec4 fragPos;
in vec3 pointToCamera;

in vec4 final_shadow_coord;

float shadowCalc(vec4 shadow_coord, vec3 normal, vec3 light_pos){
	
	vec3 proj_coord = shadow_coord.xyz/shadow_coord.w;
	proj_coord = proj_coord * 0.5 + 0.5;
	float closetsDepth = texture(shadowMap, proj_coord.xy).r;
	float currentDepth = proj_coord.z;
	//float bias = 0.005;
	//float shadow = currentDepth - bias > closetsDepth ? 1.0 : 0.0;
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
//	vec3 am = ambient_val * light_color; // PF: the ambient 
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
	vec4 blendmapSample = texture(blendmapSampler, vec2(texUVs.s, 1 - texUVs.t)); //Blendmap texture
	float bgTexValue = 1 - (blendmapSample.r + blendmapSample.g + blendmapSample.b); //How much of the texture is black

	vec4 texSample  = texture(textureSampler, vec2(texUVs.s, 1 - texUVs.t)) * bgTexValue;	 //Default texture is applied to black parts of the blendmap
	vec4 rTexSample = texture(rTexSampler, vec2(texUVs.s, 1 - texUVs.t)) * blendmapSample.r; //This texture is applied to red parts of the blendmap
	vec4 gTexSample = texture(gTexSampler, vec2(texUVs.s, 1 - texUVs.t)) * blendmapSample.g; //This texture is applied to green parts of the blendmap
	vec4 bTexSample = texture(bTexSampler, vec2(texUVs.s, 1 - texUVs.t)) * blendmapSample.b; //This texture is applied to blue parts of the blendmap

	vec4 finalTex = texSample + rTexSample + gTexSample + bTexSample;

	vec3 norm = normalize(mat3(MODEL_MAT) * finalNormals); //Make sure the vectors are normalized in world space

	vec4 result = vec4(0.0f);
//	result += calcDiffuse(light_positions[0], light_colors[0], norm) * finalTex + calcSpecular(light_positions[0], light_colors[0], norm);
//	result += calcDiffuse(light_positions[1], light_colors[1], norm) * finalTex + calcSpecular(light_positions[1], light_colors[1], norm);

	float shadow = shadowCalc(final_shadow_coord, norm, light_positions[0]);
	result += (calcAmbient(light_colors[0]) + (1.0 - shadow) * calcDiffuse(light_positions[0], light_colors[0], norm)) * (finalTex + calcSpecular(light_positions[0], norm, shadow));

	fragment_color = result;
}