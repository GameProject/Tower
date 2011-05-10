#ifndef BOXNODE_H
#define BOXNODE_H

#include <d3dx10.h>
#include <vector>

struct BoxNode
{
	D3DXVECTOR3 minB;
	D3DXVECTOR3 maxB;
	std::vector<int> id;
	std::vector<BoxNode> ChildNode;
};

#endif