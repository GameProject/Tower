#include "TowerScript.h"

TowerScript::TowerScript()
{
	
}

void TowerScript::Init(const char* module)
{
	Py_Initialize();
	// try to execute the script.py
	PyObject* pName =  PyString_FromString(module);
	if (pName == NULL)
		cerr << "Failed to create python string" << endl;

	pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (NULL == pModule)
	{
		if (PyErr_Occurred()) 
			PyErr_Print();
	}
}

void TowerScript::EndTowerScript()
{
	// clean up the loaded module
	Py_DECREF(pModule);
	Py_Finalize();
}

TowerScript& GetTowerScript()
{
	static TowerScript towerScript;
	return towerScript;
}

PyObject* TowerScript::GetFunction(const char* funcname)
{
	PyObject* pFunc = PyObject_GetAttrString(pModule, funcname);
	if ((NULL == pFunc) || (!PyCallable_Check(pFunc)) )
	{
		if (PyErr_Occurred()) PyErr_Print();
		cerr << "Can't find " << funcname << " or its not callable" << endl;
		Py_XDECREF(pFunc); // if its not callable, release
		return NULL;
	}

	return pFunc;
}

int TowerScript::AddTower()
{
	const char* func = "AddTower";
	PyObject* pFunc = GetFunction(func);

	PyObject* pRetVal = PyObject_CallObject(pFunc, NULL);
	if (!pRetVal)
	{
		cerr << "Failed to call " << func << endl;
		PyErr_Print();
		Py_DECREF(pFunc);
	}
	return PyLong_AsLong(pRetVal);
}

void TowerScript::RemoveTower(int tID)
{
	const char* func = "RemoveTower";
	PyObject* pFunc = GetFunction(func);

	PyObject* pFuncArgs = Py_BuildValue("(i)", tID);

	PyObject* pRetVal = PyObject_CallObject(pFunc, pFuncArgs);
	if (!pRetVal)
	{
		cerr << "Failed to call " << func << endl;
		PyErr_Print();
		Py_DECREF(pFunc);
	}
}

void TowerScript::FoundEnemy(int eID, int tID)
{
	const char* func = "EnemyFound";
	PyObject* pFunc = GetFunction(func);

	PyObject* pFuncArgs = Py_BuildValue("(ii)", tID, eID);

	PyObject* pRetVal = PyObject_CallObject(pFunc, pFuncArgs);
	Py_DECREF(pFuncArgs); // free the function arguments
	if (!pRetVal)
	{
		cerr << "Failed to call " << func << endl;
		PyErr_Print();
		Py_DECREF(pFunc);
	}
}

int TowerScript::LoseEnemy(int tID)
{
	cout << "Fienden är borta!\n";
	const char* func = "EnemyLost";
	PyObject* pFunc = GetFunction(func);

	PyObject* pFuncArgs = Py_BuildValue("(i)", tID);

	PyObject* pRetVal = PyObject_CallObject(pFunc, pFuncArgs);
	if (!pRetVal)
	{
		cerr << "Failed to call " << func << endl;
		PyErr_Print();
		Py_DECREF(pFunc);
	}
	return PyLong_AsLong(pRetVal);
}

attackInfo TowerScript::AttackEnemy(int time, int tID)
{
	const char* func = "Attack";
	PyObject* pFunc = GetFunction(func);

	PyObject* pFuncArgs = Py_BuildValue("(ii)", tID, time);

	PyObject* pRetVal = PyObject_CallObject(pFunc, pFuncArgs);
	Py_DECREF(pFuncArgs); // free the function arguments

	if (!pRetVal)
	{
		cerr << "Failed to call " << func << endl;
		PyErr_Print();
		Py_DECREF(pFunc);
	}

	int n = PyObject_Length(pRetVal);
	attackInfo ai;
	for(int i=0; i<n; i++)
	{
		PyObject *item = PyTuple_GetItem(pRetVal, i);
		if (!PyString_Check(item))
		{
			if (i == 0)
				ai.eID = PyLong_AsLong(item);
			else
				ai.dmg = PyLong_AsLong(item);
		}
		else
			ai.magic = PyString_AsString(item);
	}

	Py_DECREF(pRetVal); // free the return value
	return ai;
}

void TowerScript::SetTowerMagic(string magic, int tID)
{
	const char* func = "SetTowerMagic";
	PyObject* pFunc = GetFunction(func);
	PyObject* pFuncArgs = Py_BuildValue("(is)", tID, magic.c_str());
	PyObject* pRetVal = PyObject_CallObject(pFunc, pFuncArgs);
	Py_DECREF(pFuncArgs); // free the function arguments
	if (!pRetVal)
	{
		cerr << "Failed to call " << func << endl;
		PyErr_Print();
		Py_DECREF(pFunc);
	}
}

int TowerScript::Update(int tID)
{
	const char* func = "UpdateTower";
	PyObject* pFunc = GetFunction(func);

	PyObject* pFuncArgs = Py_BuildValue("(i)", tID);

	PyObject* pRetVal = PyObject_CallObject(pFunc, pFuncArgs);
	Py_DECREF(pFuncArgs); // free the function arguments
	if (!pRetVal)
	{
		cerr << "Failed to call " << func << endl;
		PyErr_Print();
		Py_DECREF(pFunc);
	}
	return PyLong_AsLong(pRetVal);
}

int TowerScript::UpgradeTower(int tID)
{
	const char* func = "UpgradeTower";
	PyObject* pFunc = GetFunction(func);
	PyObject* pFuncArgs = Py_BuildValue("(i)", tID);

	PyObject* pRetVal = PyObject_CallObject(pFunc, pFuncArgs);
	Py_DECREF(pFuncArgs); // free the function arguments
	if (!pRetVal)
	{
		cerr << "Failed to call " << func << endl;
		PyErr_Print();
		Py_DECREF(pFunc);
	}
	return 1;
}

int TowerScript::GetRange(int tID)
{
	const char* func = "GerTowerRange";
	PyObject* pFunc = GetFunction(func);
	if (!pFunc) return -1;
	PyObject* pFuncArgs = Py_BuildValue("(i)", tID);

	PyObject* pRetVal = PyObject_CallObject(pFunc, pFuncArgs);
	Py_DECREF(pFuncArgs); // free the function arguments
	if (!pRetVal)
	{
		cerr << "Failed to call " << func << endl;
		PyErr_Print();
		Py_DECREF(pFunc);
		return -1;
	}
	return PyLong_AsLong(pRetVal);
}