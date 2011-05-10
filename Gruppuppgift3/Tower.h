#ifndef TOWER_H
#define TOWER_H

#include <d3dx10.h>
#include <vector>
#include "Vertex.h"
#include "Waypoint.h"
#include "Terrain.h"
#include "Projectile.h"
#include "TowerScript.h"

class Tower
{
private:
	int towerID;
	attackInfo ai;

	Projectile projectile;
	bool fire;
	D3DXMATRIX mWorld;
	D3DXMATRIX mRot;
	D3DXMATRIX mScale;
	D3DXVECTOR3 pos;
	ID3D10Device* d3dDevice;
	ID3D10Buffer* vertexBuffer;
	ID3D10Buffer* indexBuffer;
	DWORD nrOfVertices, nrOfFaces;
	std::vector<Vertex> v;
	Waypoint wp;
public:
	Tower();
	void release();
	void init(ID3D10Device* d3dDevice, D3DXVECTOR3 size, D3DXVECTOR3 pos);
	void rotate(float t);
	void move(D3DXVECTOR3 newPos);
	void scaleTower(float s);
	void Draw();
	void setUV(float d, bool alongX);
	D3DXMATRIX& getWorld();
	void WaypointMove(float dt, Terrain* hM, bool funnyCam);

	PyObject* LoadModule(const char* module);
	PyObject* GetFunction(PyObject* pModule, const char* funcname);
	int GetID();
	
	void SearchForEnemy(int eID, D3DXVECTOR3 ePos, float eRadius);
	void Update(float time, int enemyID);
	void Upgrade();
	void Remove();
};
#endif