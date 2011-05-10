#include "Waypoint.h"

Waypoint::Waypoint()
{
	this->current = 0;
	this->next = current+1;
	this->t = 0.0f;
	this->twisted = false;
	wP.push_back(D3DXVECTOR3(200.0f,0.0f,200.0f));
	wP.push_back(D3DXVECTOR3(50.0f,0.0f,200.0f));
	wP.push_back(D3DXVECTOR3(100.0f,0.0f,100.0f));
	wP.push_back(D3DXVECTOR3(0.0f,0.0f,0.0f));
	wP.push_back(D3DXVECTOR3(-100.0f,0.0f,-100.0f));
	wP.push_back(D3DXVECTOR3(-100.0f,0.0f,-200.0f));
	wP.push_back(D3DXVECTOR3(-50.0f,0.0f,-200.0f));
	wP.push_back(D3DXVECTOR3(-200.0f,0.0f,-200.0f));
	
	
}
void Waypoint::Addpoint(D3DXVECTOR3 v)
{
	wP.push_back(v);
}
void Waypoint::Removepoint(D3DXVECTOR3 v)
{
	for(int i = 0; i < wP.size(); i++)
	{
		if(wP[i] = v)
		{
			wP.erase(wP.begin()+i);
		}
	}
}
void Waypoint::SaveToFile(string s)
{
	ofstream wPstream;
	wPstream.open(s + ".txt");
	for(int i = 0; i<wP.size();i++)
	{
		string temp = "wp " + Convert(wP[i].x) + " " + Convert(wP[i].y) + " " + Convert(wP[i].z) + "\n";
		wPstream << temp;
	}
	wP.erase(wP.begin(),wP.end());
}
void Waypoint::LoadFromFile(string s)
{
	D3DXVECTOR3 pn;
	ifstream fin;
	char input, input2;
	fin.open(s + ".txt");
	fin.get(input);
	while(input == 'w')
	{
		fin.get(input);
		if(input == 'p')
		{
			fin >> pn.x >> pn.y >> pn.z;
			wP.push_back(pn);
		}
		while(input != '\n')
		{
			fin.get(input);
		}
		fin.get(input);
	}
	fin.close();
	this->startPos = wP[0];
	this->endPos = wP[wP.size()-1];

}
D3DXVECTOR3 Waypoint::getMove(float dt, float speed)
{
	
	if(t < 1.0f)
	{
		if(current !=  wP.size()-1)
		{
			if(current == 0)
			{
				D3DXVECTOR3 tempV;
				D3DXVec3CatmullRom(&tempV, &wP[current],&wP[current],&wP[next],&wP[next+1],t);
				oldPos = tempV;
				t += speed*dt;
				return tempV;
			}
			else if(current < wP.size()-2)
			{
				D3DXVECTOR3 tempV;
				D3DXVec3CatmullRom(&tempV, &wP[current-1],&wP[current],&wP[next],&wP[next+1],t);
				oldPos = tempV;
				t += speed*dt;
				return tempV;
			}
			else if(current >= wP.size()-2)
			{
				D3DXVECTOR3 tempV;
				D3DXVec3CatmullRom(&tempV, &wP[current-1],&wP[current],&wP[next],&wP[next],t);
				oldPos = tempV;
				t += speed*dt;
				return tempV;
			}
		}
		else
		{
			twist();
			current = 0;
			next = current+1;
			return oldPos;
		}
	}
	else
	{
		current++;
		next++;
		t = 0.0f;
	}
	return oldPos;
}
std::string Waypoint::Convert(float number)
{
	std::ostringstream buff;
	buff<<number;
	return buff.str();
}
vector<D3DXVECTOR3> Waypoint::getwP()
{
	return this->wP;
}
void Waypoint::twist()
{
	int start = 0;
	int end = wP.size()-1;
	D3DXVECTOR3 temp;
	while(start <= end)
	{
		temp = wP[start];
		wP[start] = wP[end];
		wP[end] = temp;
		start++;
		end--;
	}
	current = 0;
	next = current+1;
}
int Waypoint::getCurrent()
{
	return this->current;
}
bool Waypoint::getTwisted()
{
	return this->twisted;
}