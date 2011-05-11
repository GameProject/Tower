#include "wave.h"

wave::wave(int lvl, int nrOfMonsters, ID3D10Device* d3dDevice, Terrain* _land)
{
	this->land = _land;
	this->d3dDevice = d3dDevice;
	this->lvl = lvl;
	this->nrMonsters = nrOfMonsters;
	
}

Cube wave::getMonsterAt(int x)
{
	return this->pCube[x];
}

void wave::initMonsters()
{
	
	this->pCube.resize(this->nrMonsters);
	for(int i = 0; i < this->nrMonsters; ++i)
	{
		this->pCube[i].init(i,this->d3dDevice,D3DXVECTOR3(5.0f,5.0f,5.0f), D3DXVECTOR3(0.0f, -100.0f, 0.0f), (i*10.0f+10.0f));
	}
	//Funderar på att ha init delen i skriptet
	this->InitMonsterScript();
	this->CallinitHandler(this->nrMonsters,this->lvl);
	
}

void wave::render(fxUtil fx,UINT p)
{
	for(int i = 0; i < this->pCube.size();++i)
	{
		if(this->CallGetAlive(i))
		{
			fx.setMfx(GetCamera().wvp(pCube.at(i).getWorld()), pCube.at(i).getWorld(), 1, 0);
			fx.ApplyPassByIndex(p);
			this->pCube[i].Draw();
		}
	}
}

void wave::update()
{
	this->CallUpdate();
	for(int i = 0; i < this->pCube.size();++i)
	{
		if(this->CallGetAlive(i))
		{
			this->pCube[i].update(GetTime().getDeltaTime(),land);
			
		}

	}
}
int wave::InitMonsterScript()
{
	Py_Initialize();
	
	this->pModule = LoadModule("Monster");
	if (NULL == pModule)
	{ 
		if (PyErr_Occurred()) PyErr_Print();
		return 0;
	}
	return 1;
}
PyObject* wave::LoadModule(const char* module)
{
	PyObject* pName =  PyString_FromString(module);
	if (pName == NULL)
	{
		cerr << "Failed to create python string" << endl;
		return NULL;
	}
	
	PyObject* ret = PyImport_Import(pName);
	Py_DECREF(pName);

	return ret;
}

PyObject* wave::GetFunction(const char* funcname)
{
	PyObject* pFunc = PyObject_GetAttrString(this->pModule, funcname);
	if ((NULL == pFunc) || (!PyCallable_Check(pFunc)) )
	{
		if (PyErr_Occurred()) PyErr_Print();
		cerr << "Can't find " << funcname << " or its not callable" << endl;
		Py_XDECREF(pFunc); // if its not callable, release
		return NULL;
	}

	return pFunc;
}
int wave::CallUpdate()
{
	const char* func = "setUpdate";
	PyObject* pFunc = GetFunction(func);
	if (!pFunc) return 0;

	PyObject* pFuncArgs = Py_BuildValue("()"); // build a tuple of two integers


	PyObject* pRetVal = PyObject_CallObject(pFunc, pFuncArgs);
	Py_DECREF(pFuncArgs); // free the function arguments

	if (!pRetVal)
	{
		cerr << "Failed to call " << func << endl;
		PyErr_Print();
		Py_DECREF(pFunc);
		return 0;
	}

	return 1;
}

int wave::CallinitHandler(int nrOf, int lvl)
{
	const char* func = "setHandler";
	PyObject* pFunc = GetFunction(func);
	if (!pFunc) return 0;

	PyObject* pFuncArgs = Py_BuildValue("(ii)", nrOf, lvl);

	PyObject* pRetVal = PyObject_CallObject(pFunc, pFuncArgs);
	Py_DECREF(pFuncArgs); // free the function arguments

	if (!pRetVal)
	{
		cerr << "Failed to call " << func << endl;
		PyErr_Print();
		Py_DECREF(pFunc);
		return 0;
	}
	return 1;
}

int wave::CalldmgCalc(int id, int dmg, char* dmgType)
{
	const char* func = "setDmg";
	PyObject* pFunc = GetFunction(func);
	if (!pFunc) return 0;

	PyObject* pFuncArgs = Py_BuildValue("(iis)", id, dmg, dmgType);

	PyObject* pRetVal = PyObject_CallObject(pFunc, pFuncArgs);
	Py_DECREF(pFuncArgs); // free the function arguments

	if (!pRetVal)
	{
		cerr << "Failed to call " << func << endl;
		PyErr_Print();
		Py_DECREF(pFunc);
		return 0;
	}
	char* s = PyString_AsString(pRetVal);
	//cout <<"ID: " << id << " Damage Type: " << PyString_AsString(pRetVal) << "\n" << endl;
	Py_DECREF(pRetVal); // free the return value
	return 1;
}

bool wave::CallGetAlive(int pos)
{
	const char* func = "getAlive";
	PyObject* pFunc = GetFunction(func);
	if (!pFunc) return 0;

	PyObject* pFuncArgs = Py_BuildValue("(i)", pos);

	PyObject* pRetVal = PyObject_CallObject(pFunc, pFuncArgs);
	Py_DECREF(pFuncArgs); // free the function arguments

	if (!pRetVal)
	{
		cerr << "Failed to call " << func << endl;
		PyErr_Print();
		Py_DECREF(pFunc);
		return 0;
	}
	if(pRetVal == Py_True)
	{
		return true;
	}
	else if(pRetVal == Py_False)
	{
		return false;
	}
	//cout <<"ID: " << id << " Damage Type: " << PyString_AsString(pRetVal) << "\n" << endl;
	Py_DECREF(pRetVal); // free the return value
}