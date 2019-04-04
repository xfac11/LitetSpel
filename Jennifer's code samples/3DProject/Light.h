#pragma once
#include "glm/glm.hpp"

class Light {
public:
	Light(glm::vec3 position, glm::vec3 color);
	~Light();

	glm::vec3 getPosition() const;
	glm::vec3 getColor() const;

	unsigned int shaderProg;

private:
	glm::vec3 pos;
	glm::vec3 color;
};

