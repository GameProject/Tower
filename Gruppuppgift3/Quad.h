#ifndef QUAD_H
#define QUAD_H

#include <d3dx10.h>

class Quad
{
private:
	ID3D10Device* d3dDevice;
	ID3D10Buffer* vertexBuffer;
	ID3D10Buffer* indexBuffer;
public:
	Quad();
	~Quad();
	void init(ID3D10Device* d3dDevice, float size);
	void Draw();
};
#endif