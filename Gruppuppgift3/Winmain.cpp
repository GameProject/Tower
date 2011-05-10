//********************************************//
//	Made by Oskar Karlsson 880401-4614		  //
//____________________________________________//
//											  //
//	Move around the world: A, W, S, D		  //
//	Look around the world: Left mouse-button  //
//	Zoom: Right mouse-button				  //
//	Crouch: Ctrl							  //
//	Jump: Space								  //
//											  //
//	Toggle cube rotation: F					  //
//	Edit cube size: B*1, N*2, M*3			  //
//	Edit cube Texture-pos: L				  //
//											  //
//	Switch between light sources: 1, 2, 3	  //
//	Edit light attributes: +(x,y,z) T, Y, U	  //
//						   -(x,y,z) G, H, J	  //
//											  //
//	Toggle QuadTree grid: R					  //
//											  //
//	Create more cubes:						  //
//	 start: O								  //
//		Edit pos: P							  //
//		Toggle cube rotation: F				  //
//		Edit size: B*1, N*2, M*3			  //
//		Edit cube Texture Y pos: K			  //
//		Edit cube Texture X pos: L			  //
//	 End: O									  //
//											  //
//********************************************//

#include "Billboard.h"
#include "Effects.h"
#include "Cube.h"
#include "Pyramid.h"
#include "Cylinder.h"
#include "fxUtil.h"
#include "Time.h"
#include "Camera.h"
#include "PSystem.h"
#include "Sky.h"
#include "Obj.h"
#include "QuadTree.h"
#include "Terrain.h"
#include "wave.h"
#include "Tower.h"
#include "SimpleGUI.h"
#include "TowerScript.h"

// The main window handle; this is used to identify a created window.
HWND g_Hwnd = 0; 

// Wraps the code necessary to initialize a Windows
// application.  Function returns true if initialization
// was successful, otherwise it returns false.
bool InitWindowsApp(HINSTANCE instanceHandle, int show);

// Wraps the message loop code.
int Run();

// The window procedure handles events our window receives.
LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); 
void Update(float deltaTime);
void Render();

//____________________________
unsigned int SCREEN_WIDTH = 1280;
unsigned int SCREEN_HEIGHT = 800;

//value for button
const int PLACE_TOWER = 1;
//Gui
SimpleGUI gui;
//GUI callback
void CALLBACK 
	OnGUIEvent(int nControlID);


//Heightmap mTerrain;
Pyramid mPyramid;
Cylinder mCylinder;
Light mLight;
fxUtil fxU;
PSystem fire;
PSystem rain;
Sky sky;
std::vector<Cube> mCubes;
std::vector<Tower> mTowers;
std::vector<Obj> mBTH;
Billboard mTrees;
QuadTree qtc;
Terrain land;

float yPosOffset;
wave* pWave;

float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};

POINT mOldMousePos;

float pi = 3.1415926535897932384626433832795f;

std::vector<int> nr;
std::vector<int> nrOfTowers;
int currentCube = 0;
int currentTower = 0;

bool funRot = false;
bool jumping = false;
bool newCube = false;
bool grid = false;

HINSTANCE mhAppInst;
ID3D10Device* md3dDevice = 0;
IDXGISwapChain* mSwapChain = 0;
ID3D10Texture2D* mDepthStencil = 0;
ID3D10RenderTargetView* mRenderTargetView = 0;
ID3D10DepthStencilView* mDepthStencilView = 0;
D3DXCOLOR mClearColor(0.2f, 0.2f, 0.2f, 1.0f);

D3D10_VIEWPORT vp;
//____________________________

// Windows equivalant to main()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	/*
	Some dual core systems have a problem where the different CPUs return different
	QueryPerformanceCounter values. So when this thread is schedule on the other CPU
	in a later frame, we could even get a negative frameTime. To solve this we force
	the main thread to always run on CPU 0.
	*/
	SetThreadAffinityMask(GetCurrentThread(), 1);

	if (!InitWindowsApp(hInstance, showCmd))
		return 0;

	return Run();
}

bool InitWindowsApp(HINSTANCE instanceHandle, int show)
{
	//ShowCursor(0);

	// The first task to creating a window is to describe some of its 
	// characteristics by filling out a WNDCLASS structure.
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = instanceHandle;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_CROSS);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = L"BasicWndClass";

	// Next, we register this WNDCLASS instance with Windows so that we can 
	// create a window based on it.
	if(!RegisterClass(&wc)) 
	{
		MessageBox(0, L"RegisterClass FAILED", 0, 0);
		return false;
	}

	// With our WNDCLASS instance registered, we can create a window with the
	// CreateWindow function.  This function returns a handle to the window it
	// creates (an HWND).  If the creation failed, the handle will have the value
	// of zero.  A window handle is a way to refer to the window, which is internally
	// managed by Windows.  Many of the Win32 API functions that operate on windows
	// require an HWND so that they know what window to act on.

	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = SCREEN_WIDTH;
	rc.bottom = SCREEN_HEIGHT;
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	g_Hwnd = CreateWindow(
		L"BasicWndClass",	 // Registered WNDCLASS instance to use.
		L"Gruppuppgift",	// window title
		WS_OVERLAPPEDWINDOW, // style flags
		CW_USEDEFAULT,		 // x-coordinate
		CW_USEDEFAULT,       // y-coordinate
		rc.right - rc.left,        // width
		rc.bottom - rc.top,       // height
		0,               // parent window
		0,               // menu handle
		instanceHandle,      // app instance
		0);              // extra creation parameters

	if(g_Hwnd == 0)
	{
		MessageBox(0, L"CreateWindow FAILED", 0, 0);
		return false;
	}

	// Even though we just created a window, it is not initially shown.  
	// Therefore, the final step is to show and update the window we just
	// created, which can be done with the following two function calls.
	// Observe that we pass the handle to the window we want to show and
	// update so that these functions know which window to show and update.

	ShowWindow(g_Hwnd, show);
	UpdateWindow(g_Hwnd);

	//________________________________

	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width  = SCREEN_WIDTH;
	sd.BufferDesc.Height = SCREEN_HEIGHT;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// No multisampling.
	sd.SampleDesc.Count   = 1;
	sd.SampleDesc.Quality = 0;

	sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount  = 1;
	sd.OutputWindow = g_Hwnd;
	sd.Windowed     = true;
	sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags        = 0;

	D3D10CreateDeviceAndSwapChain(
		0,                 //default adapter
		D3D10_DRIVER_TYPE_HARDWARE,
		0,                 // no software device
		0, 
		D3D10_SDK_VERSION,
		&sd,
		&mSwapChain,
		&md3dDevice);

	ID3D10Texture2D* backBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&backBuffer);
	md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);
	backBuffer->Release();
	backBuffer = 0;

	
	// Create depth stencil texture
	D3D10_TEXTURE2D_DESC descDepth;
	ZeroMemory( &descDepth, sizeof(descDepth) );
	descDepth.Width = SCREEN_WIDTH;
	descDepth.Height = SCREEN_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D10_USAGE_DEFAULT;
	descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	md3dDevice->CreateTexture2D( &descDepth, NULL, &mDepthStencil );

	// Create the depth stencil view
	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof(descDSV) );
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	// Bind the render target view and depth/stencil view to the pipeline.
	md3dDevice->CreateDepthStencilView( mDepthStencil, &descDSV, &mDepthStencilView );
	md3dDevice->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView );

	// Set the viewport transform.
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width    = SCREEN_WIDTH;
	vp.Height   = SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	md3dDevice->RSSetViewports(1, &vp);
	//________________________________

	Effects::InitAll(md3dDevice);
	fxU.init(md3dDevice, Effects::MeshFX);

	Terrain::InitInfo tii;
	tii.CellSpacing = 1.0f;
	tii.HeightmapFilename = L"flat513.raw";
	tii.HeightOffset = -30.0f;
	tii.HeightScale = 0.2f;
	tii.NumCols = 513;
	tii.NumRows = 513;
	land.init(md3dDevice, tii);
	//mTerrain.init(md3dDevice, (std::string)"Textures/Terrain/HeightMap.raw", 0.35f, -50.0f, 0.1f, 512, 512);

	//Cube c;
	//c.init(md3dDevice, D3DXVECTOR3(2.0f, 2.0f, 2.0f), D3DXVECTOR3(-16.0f, land.getHeight(-16.0f, -16.0f)+1.0f, -16.0f));
	//mCubes.push_back(c);
	//nr.push_back(0);

	nrOfTowers.push_back(0);
	
	mCubes.resize(10);
	for(int i = 0; i < mCubes.size();i++)
	{
		mCubes[i].init(i, md3dDevice, D3DXVECTOR3(5.0f,5.0f,5.0f), D3DXVECTOR3(0.0f, land.getHeight(0.0f,0.0f), 0.0f), (i*0.50f+0.50f));
		nr.push_back(0);
	}
	pWave = new wave(1, 20, md3dDevice, &land);
	pWave->initMonsters();
	
	mPyramid.init(md3dDevice, D3DXVECTOR3(2.0f, 2.0f, 2.0f), D3DXVECTOR3(-16.0f, land.getHeight(-16.0f, -16.0f)+5.0f, -16.0f));
	mCylinder.init(md3dDevice, 1.0f, D3DXVECTOR3(-2.0f, land.getHeight(-2.0f, -8.0f)+1.0f, -8.0f));

	//GetCamera().setPosY(land.getHeight(GetCamera().getPos().x, GetCamera().getPos().z));
	GetCamera().setLens(0.30f*pi, (float)SCREEN_WIDTH/SCREEN_HEIGHT, 1.0f, 1000.0f);

	fire.init(md3dDevice, Effects::FireFX, L"Textures/Particle/flare0.dds", 500);
	fire.setEmitPos(D3DXVECTOR3(-2.0f, land.getHeight(-5.0f, 2.0f)+2.5f, -8.0f));

	rain.init(md3dDevice, Effects::RainFX, L"Textures/Particle/raindrop.gif", 1000);

	sky.init(md3dDevice, L"Textures/Terrain/grassenvmap1024.dds", 5000.0f);
	
	//Trees
	D3DXVECTOR3 treeCenters[6];

	float x = -20.0f;
	float z = 20.0f;
	treeCenters[0] = D3DXVECTOR3(x,land.getHeight(x,z) + 10.0f,z);

	x = -23.0f;
	z = 16.0f;
	treeCenters[1] = D3DXVECTOR3(x,land.getHeight(x,z) + 10.0f,z);

	x = -3.0f;
	z = 18.0f;
	treeCenters[2] = D3DXVECTOR3(x,land.getHeight(x,z) + 10.0f,z);

	x = 22.0f;
	z = 13.0f;
	treeCenters[3] = D3DXVECTOR3(x,land.getHeight(x,z) + 10.0f,z);

	x = 17.0f;
	z = -23.0f;
	treeCenters[4] = D3DXVECTOR3(x,land.getHeight(x,z) + 10.0f,z);

	x = 22.0f;
	z = -20.0f;
	treeCenters[5] = D3DXVECTOR3(x,land.getHeight(x,z) + 10.0f,z);
	mTrees.init(md3dDevice, treeCenters, 6, L"Textures/Wood/tree3.dds");

	//QuadTree
	qtc.init(md3dDevice, &land, GetCamera().proj(), GetCamera().view());

	//init GUI
	gui.Init(md3dDevice,SCREEN_WIDTH,SCREEN_HEIGHT);
	//set gui callback
	gui.SetCallback(OnGUIEvent);
	//adding a button
	gui.AddButton(PLACE_TOWER,L"test.png",10,10,100,100);

	GetTowerScript().Init("tower");

	return true;
}

void Update(float deltaTime)
{
	GetCamera().rebuildView();

	if (jumping)
		jumping = GetCamera().jump(GetTime().getGameTime());

	//cam.setPosY(mTerrain.getPosY(cam.getPos().x, cam.getPos().z));
	GetCamera().keyMsg(deltaTime);

	//Light rotation
	mLight.rot(GetTime().getGameTime(), deltaTime, GetCamera().getLook(), GetCamera().getPos());

	//Light attributes
	mLight.keyMsg(deltaTime);

	//Cube rotation
 	if (funRot)
	{
		if (newCube)
			mTowers.back().rotate(GetTime().getGameTime());
		else
			mTowers.front().rotate(GetTime().getGameTime());
	}

	if (GetAsyncKeyState('B') & 0x8000) 
	{
		if (newCube)
			mTowers.back().scaleTower(1.0f);
		else
			mTowers.front().scaleTower(1.0f);
	}
	if (GetAsyncKeyState('N') & 0x8000) 
	{
		if (newCube)
			mTowers.back().scaleTower(2.0f);
		else
			mTowers.front().scaleTower(2.0f);
	}
	if (GetAsyncKeyState('M') & 0x8000) 
	{
		if (newCube)
			mTowers.back().scaleTower(3.0f);
		else
			mTowers.front().scaleTower(3.0f);
	}

	if (GetAsyncKeyState('P') & 0x8000 && newCube)
		mTowers.back().move(GetCamera().getPos());

	if (GetAsyncKeyState('L') & 0x8000)
	{
		if (newCube)
			mTowers.back().setUV(deltaTime*0.1f, true);
		else
			mTowers.front().setUV(deltaTime*0.1f, true);
	}
	if (GetAsyncKeyState('K') & 0x8000)
	{
		if (newCube)
			mTowers.back().setUV(deltaTime*0.1f, false);
		else
			mTowers.front().setUV(deltaTime*0.1f, false);
	}

	fxU.setSpecial(GetCamera().getPos(), mLight, mLight.lightType);

	fire.setEyePos(GetCamera().getPos());
	fire.update(deltaTime, GetTime().getGameTime());

	rain.setEyePos(GetCamera().getPos());
	rain.setEmitPos(D3DXVECTOR3(GetCamera().getPos().x, GetCamera().getPos().y+50, GetCamera().getPos().z));
	rain.update(deltaTime, GetTime().getGameTime());

	for(int i = 0; i < mTowers.size(); i++)
		mTowers.at(i).Update(GetTime().getGameTime(), 3);

	//mCubes.at(0).WaypointMove(deltaTime, &land);

	for(int i = 0; i < mCubes.size(); i++)
		mCubes[i].update(deltaTime, &land);
	pWave->update();
}

void Render()
{
	md3dDevice->ClearRenderTargetView(mRenderTargetView, mClearColor);
	md3dDevice->ClearDepthStencilView(mDepthStencilView, D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0f, 0);
	md3dDevice->OMSetDepthStencilState(0, 0);

	md3dDevice->OMSetBlendState(0, blendFactor, 0xffffffff);

	//draw Trees
	mTrees.draw(GetCamera().getPos(), GetCamera().view()*GetCamera().proj());

	D3D10_TECHNIQUE_DESC techDesc;
	fxU.setRenderUtil(techDesc);

	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		//draw Cube
		pWave->render(fxU,p);

		//draw Cube "Tower"
		for (UINT i=0; i < mTowers.size(); ++i)
		{
			fxU.setMfx(GetCamera().wvp(mTowers.at(i).getWorld()), mTowers.at(i).getWorld(), 1, nrOfTowers[i]);
			fxU.ApplyPassByIndex(p);
			mTowers.at(i).Draw();
		}

		//draw Pyramid
		fxU.setMfx(GetCamera().wvp(mPyramid.getWorld()), mPyramid.getWorld(), 2, 3);
		fxU.ApplyPassByIndex(p);
		mPyramid.Draw();

		//draw Cylinder
		fxU.setMfx(GetCamera().wvp(mCylinder.getWorld()), mCylinder.getWorld(), 3, 6);
		fxU.ApplyPassByIndex(p);
		mCylinder.Draw();

		//draw Terrain
		//fxU.setMfx(GetCamera().wvp(mTerrain.getWorld()), mTerrain.getWorld(), 0, 9);
		//fxU.ApplyPassByIndex(p);
		//mTerrain.Draw();

		fxU.setMfx(GetCamera().wvp(land.getWorld()), land.getWorld(), 0, 9);
		fxU.ApplyPassByIndex(p);
		land.draw();
	}

	//draw gui
	gui.Render();

	//draw grid
	qtc.draw(GetCamera().view(), GetCamera().proj(), grid);

	//draw Sky
	sky.draw(GetCamera().view(), GetCamera().proj(), GetCamera().getPos(), mLight.lightType);

	//draw Fire
	fire.draw(GetCamera().view(), GetCamera().proj());
	
	md3dDevice->OMSetBlendState(0, blendFactor, 0xffffffff);
	
	//draw Rain
	rain.draw(GetCamera().view(), GetCamera().proj());
	
	mSwapChain->Present(0, 0);
}

int Run()
{
	MSG msg = {0};

	while(msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		// Otherwise, do animation/game stuff.
		else
		{
			Update(GetTime().getDeltaTime());
			Render();
		}
	}
	if( md3dDevice )
		md3dDevice->ClearState();

	if (!mCubes.empty())
		for (UINT i=0; i<mCubes.size(); ++i)
			mCubes.at(i).release();
	if (!mTowers.empty())
		for (UINT i=0; i<mTowers.size(); ++i)
			mTowers.at(i).release();
	GetTowerScript().EndTowerScript();
	if (!mBTH.empty())
		for (UINT i=0; i<mBTH.size(); ++i)
			mBTH.at(i).Destroy();
	Effects::DestroyAll();

	return (int)msg.wParam;
}

void CALLBACK
	OnGUIEvent(int ID)
{
	switch(ID)
	{
	case PLACE_TOWER:
		gui.GetButton(PLACE_TOWER)->SetPressed(true);
		return;
	}

}

LRESULT CALLBACK
WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	POINT mousePos;
	int dx = 0;
	int dy = 0;

	switch( msg )
	{
	case WM_LBUTTONDOWN:
		if( wParam & MK_LBUTTON )
		{
			SetCapture(hwnd);

			mOldMousePos.x = ( short )LOWORD(lParam);
			mOldMousePos.y = ( short )HIWORD(lParam);
			gui.UpdateMouse(mOldMousePos.x,mOldMousePos.y,true);
		}
		return 0;

	case WM_LBUTTONUP:
		ReleaseCapture();
		return 0;

	case WM_RBUTTONDOWN:
		if( wParam & MK_RBUTTON )
		{
			SetCapture(hwnd);

			mOldMousePos.x = ( short )LOWORD(lParam);
			mOldMousePos.y = ( short )HIWORD(lParam);
			POINT d;
			d.x = ( short )LOWORD(lParam);
			d.y = ( short )HIWORD(lParam);
			//qtc.TEST(vp, d, qtc.getBLS(), 5, md3dDevice);
			if(gui.GetButton(PLACE_TOWER)->GetPressed())
			{
				D3DXVECTOR3 res = qtc.PlaceATower(vp, d);
				if (res != D3DXVECTOR3(-99,-99,-99))
				{
					mTowers.push_back(Tower());
					mTowers.back().init(md3dDevice, D3DXVECTOR3(7.5f, 7.5f, 7.5f), res);
					newCube = true;
					nrOfTowers.resize(mTowers.size());
					currentTower = (int)nrOfTowers.size()-1;
					gui.GetButton(PLACE_TOWER)->SetPressed(false);
				}
			}
			//cam.setLens(0.30f*(gameTime.getDeltaTime()*20), (float)SCREEN_WIDTH/SCREEN_HEIGHT, 1.0f, 1000.0f);
		}
		return 0;

	case WM_RBUTTONUP:
		GetCamera().setLens(0.30f*pi, (float)SCREEN_WIDTH/SCREEN_HEIGHT, 1.0f, 1000.0f);
		ReleaseCapture();
		return 0;

	case WM_MOUSEMOVE:
		if( wParam & MK_LBUTTON || wParam & MK_RBUTTON )
		{
			mousePos.x = ( short )LOWORD(lParam);
			mousePos.y = ( short )HIWORD(lParam);

			dx = mousePos.x - mOldMousePos.x;
			dy = mousePos.y - mOldMousePos.y;

			GetCamera().rotateY( dx * 0.0087266f );
			GetCamera().pitch( dy * 0.0087266f );

			mOldMousePos = mousePos;
		}
		return 0;

	case WM_KEYDOWN:
		if(wParam == VK_CONTROL) GetCamera().crouch();
		if(wParam == VK_SPACE)	
		{
			if (!jumping)
				GetCamera().setJump(GetTime().getGameTime());
			jumping = true;
		}
		if (GetAsyncKeyState('F') & 0x8000)
		{
			if (funRot)
				funRot = false;
			else
				funRot = true;
		}
		if (GetAsyncKeyState('R') & 0x8000)
		{
			if (grid)
				grid = false;
			else
				grid = true;
		}
		if (GetAsyncKeyState('O') & 0x8000)
		{
			if(!newCube)
			{
				Tower c;
				mTowers.push_back(c);
				mTowers.back().init(md3dDevice, D3DXVECTOR3(1.0f, 1.0f, 1.0f), GetCamera().getPos());
				newCube = true;
				nrOfTowers.resize(mTowers.size());
				currentTower = (int)nrOfTowers.size()-1;
			}
			else
				newCube = false;
		}
		if (GetAsyncKeyState('Z') & 0x8000) 
		{
			if(newCube)
				nrOfTowers[currentTower] = 0;
			else
				nrOfTowers[0] = 0;
		}
		if (GetAsyncKeyState('X') & 0x8000)
		{
			if(newCube)
				nrOfTowers[currentTower] = 3;
			else
				nrOfTowers[0] = 3;
		}
		if (GetAsyncKeyState('C') & 0x8000)
		{
			if(newCube)
				nrOfTowers[currentTower] = 6;
			else
				nrOfTowers[0] = 6;
		}
		if (GetAsyncKeyState('Q') & 0x8000)
		{
			pWave->CalldmgCalc(1,10000,"Fire");
			pWave->CalldmgCalc(0,10000,"Fire");
		}
		return 0;

		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		DestroyWindow(g_Hwnd);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}