#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 color) {
	this->pos = position;
	this->color = color;
}

Light::~Light() {

}

glm::vec3 Light::getPosition() const {
	return this->pos;
}

glm::vec3 Light::getColor() const {
	return this->color;
}
