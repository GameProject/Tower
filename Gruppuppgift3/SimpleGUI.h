#ifndef SIMPLEGUI_H
#define SIMPLEGUI_H
#include <vector>
#include <d3dx10.h>
#include <string>
using namespace std;

class SimpleControl;
class SimpleButton;
typedef VOID ( CALLBACK*PCALLBACKGUIEVENT )(int nControlID);
//void CALLBACK OnGUIEvent(int nControlID);
struct Mouse
{
	int x;
	int y;
	bool left_click;
};

class SimpleGUI
{
public:
	SimpleGUI();
	~SimpleGUI();
	void Init(ID3D10Device* device, int screenWidth, int screenHeight);
	void SetCallback(PCALLBACKGUIEVENT callback);
	bool AddButton(int ID, LPCTSTR filename, int x, int y, int width, int height);
	void Render();
	void UpdateMouse(int x, int y, bool leftclicked);
	void DrawButton(SimpleButton* button);
	bool InsertEvent(int ID);
	SimpleButton* GetButton( int ID )
    {
        return ( SimpleButton* )GetControl( ID );
    }
	Mouse* GetMouse()
	{
		return &mouse;
	}
private:
	SimpleControl* GetControl(int ID );
	vector<SimpleControl*> controls;
	PCALLBACKGUIEVENT m_pCallbackEvent;
	ID3D10Device* m_pDevice;
	ID3DX10Sprite*	m_pSprite;
	int m_pScreenWidth;
	int m_pScreenHeight;
	Mouse mouse;
};

class SimpleControl
{
public:
	SimpleControl();
	SimpleControl(SimpleGUI* owner, int ID, int x , int y);
	~SimpleControl();
	int GetX() const
	{ 
		return x;
	}
	int GetY() const
	{ 
		return y;
	}
	int GetID() const
	{ 
		return ID;
	}
	virtual void Draw();
	virtual void Update();
protected:
	SimpleGUI* owner;
	int x;
	int y;
	int ID;
};

class SimpleButton :public SimpleControl
{
public:
	SimpleButton();
	SimpleButton(SimpleGUI* owner, int ID, ID3D10ShaderResourceView* shaderResource , int x , int y, int width, int height);
	~SimpleButton();
	ID3D10ShaderResourceView* GetResource();
	bool CheckButton(Mouse* mouse);
	void Draw();
	void Update();
	int GetWidth() const
	{ 
		return width;
	}
	int GetHeight() const
	{ 
		return height;
	}
	bool GetPressed() const
	{
		return pressed;
	}
	void SetPressed(bool pressed)
	{
		this->pressed = pressed;
	}
private:
	bool pressed;
	int width;
	int height;
	ID3D10ShaderResourceView* m_pMyShaderResource;
};

#endif