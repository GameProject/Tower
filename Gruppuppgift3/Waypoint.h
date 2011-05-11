#ifndef WAYPOINT_H
#define WAYPOINT_H
#include <vector>
#include <string>
#include <d3dx10.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <math.h>
using namespace std;

class Waypoint
{
private:
	vector<D3DXVECTOR3> wP;
	float t;
	int current;
	int next;
	D3DXVECTOR3 oldPos;
	bool twisted;
	D3DXVECTOR3 startPos;
	D3DXVECTOR3 endPos;
public:
	Waypoint();
	void Addpoint(D3DXVECTOR3 v);
	void Removepoint(D3DXVECTOR3 v);
	void SaveToFile(string s);
	void LoadFromFile(string s);
	D3DXVECTOR3 getMove(float dt, float speed);
	std::string Convert(float number);
	vector<D3DXVECTOR3> getwP();
	void twist();
	int getCurrent();
	bool getTwisted();
	void reset();
	D3DXVECTOR3 getEndPos();
};
#endif