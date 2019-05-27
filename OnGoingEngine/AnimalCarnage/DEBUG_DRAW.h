#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H

#include "SimpleShaderClass.h"
#include "Primitives.h"

class DEBUG_DRAW
{
private:
	SimpleShader* shader;
	std::vector<Primitives*> shapes;

public:
	DEBUG_DRAW();
	~DEBUG_DRAW();
	
	bool DebugDraw = true;
	bool addPrimitives(Primitives* shape);
	void Draw(XMMATRIX view, XMMATRIX proj);
	bool DeletePrimitiv(Primitives* shape);
};

#endif //DEBUG_DRAW_H
