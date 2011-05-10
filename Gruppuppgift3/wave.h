#ifndef WAVE_H
#define WAVE_H
#include "Camera.h"
#include <stdio.h>
#include <math.h>
#include <d3dx10.h>
#include <vector>
#include <string>
#include "Terrain.h"
#include "Cube.h"//ska bytas ut mot monster
#include "Time.h"
#include "fxUtil.h"
#if defined(_DEBUG)
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif
class wave
{
private:
	ID3D10Device* d3dDevice;
	PyObject* pModule;
	Terrain* land;
	vector<Cube> pCube;//ska bytas ut mot monster
	int nrMonsters;
	int lvl;

public:
	wave(int lvl, int nrOfMonsters, ID3D10Device* d3dDevice, Terrain* _land);
	Cube getMonsterAt(int x);//ska bytas ut mot monster
	void initMonsters();
	void update();
	void render(fxUtil fx, UINT p);
	//SCRIPT
	int InitMonsterScript();
	PyObject* LoadModule(const char* module);
	PyObject* GetFunction( const char* funcname);
	int CalldmgCalc(int id, int dmg, char* dmgType);
	int CallinitHandler(int nrOf, int lvl);
	int CallUpdate();
	bool CallGetAlive(int pos);
};
#endif