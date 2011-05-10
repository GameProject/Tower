#ifndef TOWERSCRIPT_H
#define TOWERSCRIPT_H

#if defined(_DEBUG)
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif

#include <iostream>
#include "ScriptUtil.h"
using namespace std;

class TowerScript
{
private:
	PyObject* pModule;
	attackInfo aI;
	PyObject* GetFunction(const char* funcname);

public:
	TowerScript();
	void Init(const char* module);
	void EndTowerScript();
	int AddTower();
	void RemoveTower(int tID);
	void FoundEnemy(int eID, int tID);
	int LoseEnemy(int tID);
	attackInfo AttackEnemy(int time, int tID);
	void SetTowerMagic(string magic, int tID);
	int Update(int tID);
	int UpgradeTower(int tID);
	int GetRange(int tID);
};
TowerScript& GetTowerScript();
#endif