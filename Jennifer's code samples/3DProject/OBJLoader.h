#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "RawModel.h"

class OBJLoader {
public:
	OBJLoader();
	~OBJLoader();

	bool loadOBJ(RawModel *model);
	bool loadMTL(RawModel *model);

private:
};

