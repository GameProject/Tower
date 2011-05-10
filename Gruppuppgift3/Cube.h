#ifndef CUBE_H
#define CUBE_H

#include "Vertex.h"
#include "Camera.h"
#include <stdio.h>
#include <math.h>
#include <d3dx10.h>
#include <vector>
#include "Terrain.h"
#include "Waypoint.h"
#include <time.h>
#include "Time.h"

class Cube
{
private:
	D3DXMATRIX mWorld;
	D3DXMATRIX mRot;
	D3DXMATRIX mScale;
	D3DXVECTOR3 pos;
	ID3D10Device* d3dDevice;
	ID3D10Buffer* vertexBuffer;
	ID3D10Buffer* indexBuffer;
	DWORD nrOfVertices, nrOfFaces;
	std::vector<Vertex> v;

	int cId;
	float sTimer;
	float sStart;
	D3DXVECTOR3 offsetPos;
	Waypoint wp;
	float speed;
	bool isAlive;
	int health;
	float FireRes;
	float IceRes;
	float EarthRes;
public:
	Cube();
	void release();
	void init(int id, ID3D10Device* d3dDevice, D3DXVECTOR3 size, D3DXVECTOR3 pos, float sTimer);
	void initStats(int hp, float FireRes, float EarthRes, float IceRes);
	void update(float dt, Terrain* hM);
	void rotate(float t);
	void move(D3DXVECTOR3 newPos);
	void scaleCube(float s);
	void Draw();
	void setUV(float d, bool alongX);
	D3DXMATRIX& getWorld();

	void getT(float dt);
	void WaypointMove(float dt, Terrain* hM);
	float getTimeToStart();
	Waypoint getWp();
	bool getIsAlive();
	void setIsAlive(bool x);
};
#endif