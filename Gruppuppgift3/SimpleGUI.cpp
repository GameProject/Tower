#include "SimpleGUI.h"

SimpleGUI::SimpleGUI()
{
	m_pCallbackEvent = NULL;
	m_pDevice = NULL;
	m_pSprite = NULL;
	mouse.x = 0;
	mouse.y = 0;
	mouse.left_click = 0;
}

SimpleGUI::~SimpleGUI()
{
	for(unsigned int i = 0; i < controls.size(); i++)
		if(controls[i] != NULL)
			delete controls[i];

}

void SimpleGUI::Init(ID3D10Device* device, int screenWidth, int screenHeight)
{
	this->m_pDevice = device;
	this->m_pScreenWidth = screenWidth;
	this->m_pScreenHeight = screenHeight;
	D3DX10CreateSprite(m_pDevice, 1, &m_pSprite);
}

void SimpleGUI::SetCallback(PCALLBACKGUIEVENT callback)
{
	this->m_pCallbackEvent = callback;
}

bool SimpleGUI::AddButton(int ID, LPCTSTR filename, int x, int y, int width, int height)
{
	D3DX10_IMAGE_INFO InfoFromFile;
	D3DX10_IMAGE_LOAD_INFO LoadImageInfo;
	HRESULT hr;
	hr = D3DX10GetImageInfoFromFile(filename, NULL, &InfoFromFile,NULL);
	if(FAILED(hr))
		return false;
	LoadImageInfo.Width = InfoFromFile.Width;
	LoadImageInfo.Height = InfoFromFile.Height;
	LoadImageInfo.Depth = InfoFromFile.Depth;
	LoadImageInfo.FirstMipLevel = 1;
	LoadImageInfo.MipLevels = InfoFromFile.MipLevels;
	LoadImageInfo.Usage = D3D10_USAGE_DEFAULT;
	LoadImageInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE ;
	LoadImageInfo.CpuAccessFlags = 0;
	LoadImageInfo.MiscFlags = 0;
	LoadImageInfo.Format = InfoFromFile.Format;
	LoadImageInfo.Filter = D3DX10_FILTER_NONE;
	LoadImageInfo.MipFilter = D3DX10_FILTER_NONE;
	LoadImageInfo.pSrcInfo = &InfoFromFile;
	ID3D10ShaderResourceView* shaderResource;
	hr = D3DX10CreateShaderResourceViewFromFile(m_pDevice, filename, NULL, NULL, &shaderResource,NULL);
	if(FAILED(hr))
		return false;
	SimpleControl* tmp = new SimpleButton(this,ID,shaderResource,x,y,width,height);
	controls.push_back(tmp);
	return true;
}

bool SimpleGUI::InsertEvent(int ID)
{
	if(!m_pCallbackEvent)
		return false;
	m_pCallbackEvent(ID);
	return true;
}

void SimpleGUI::UpdateMouse(int x, int y, bool leftclicked)
{
	mouse.x = x;
	mouse.y = y;
	mouse.left_click = leftclicked;
	for(unsigned int i = 0; i < controls.size(); i++)
		controls[i]->Update();
}

void SimpleGUI::Render()
{
	for(unsigned int i = 0; i < controls.size(); i++)
		controls[i]->Draw();
}

void SimpleGUI::DrawButton(SimpleButton* button)
{
	//FLOAT OriginalBlendFactor[4];  
 //   UINT OriginalSampleMask = 0;  
 //   ID3D10BlendState* pFontBlendState10=NULL; 
 //   ID3D10BlendState* pOriginalBlendState10=NULL;
 //   // Save the current blend state

 //   m_pDevice->OMGetBlendState(&pOriginalBlendState10, OriginalBlendFactor, &OriginalSampleMask);
	D3DXMATRIX matProjection;
	D3DXMatrixOrthoOffCenterLH( &matProjection, 0.0f, (float)m_pScreenWidth, 0.0f, (float)m_pScreenHeight, 0.1f, 10);
	D3DXMATRIXA16 matScaling;
	D3DXMATRIXA16 matTranslation;
	D3DX10_SPRITE Sprite;

	D3DXMatrixScaling(&matScaling, (float)button->GetWidth(), (float)button->GetHeight(), 1.0f);
	D3DXMatrixTranslation( &matTranslation, (float)(button->GetX()+button->GetWidth()/2), (float)m_pScreenHeight-(button->GetY()+button->GetHeight()/2), 0.8f );

	m_pSprite->SetProjectionTransform( &matProjection );

	Sprite.matWorld = matScaling * matTranslation;;

	D3DX10_SPRITE SpriteToDraw;

	SpriteToDraw.matWorld = matScaling*matTranslation;

	SpriteToDraw.TexCoord.x = 0.0f;
	SpriteToDraw.TexCoord.y = 0.0f;
	SpriteToDraw.TexSize.x = 1.0f;
	SpriteToDraw.TexSize.y = 1.0f;
		
	SpriteToDraw.ColorModulate = D3DXCOLOR( 1.0f,1.0f,1.0f,1.0f );
	SpriteToDraw.pTexture = button->GetResource();
	SpriteToDraw.TextureIndex = 0;

	m_pSprite->Begin( D3DX10_SPRITE_SORT_TEXTURE );
	m_pSprite->DrawSpritesBuffered( &SpriteToDraw, 1 );
	m_pSprite->Flush();
	m_pSprite->End();

	//m_pDevice->OMSetBlendState(pOriginalBlendState10, OriginalBlendFactor, OriginalSampleMask);
}

SimpleControl* SimpleGUI::GetControl(int ID)
{
	for(int i = 0; i < controls.size(); i++)
		if(controls[i]->GetID() == ID)
			return controls[i];
}

SimpleControl::SimpleControl()
{
	ID = 0;
	x = 0;
	y = 0;
}

SimpleControl::SimpleControl(SimpleGUI* owner, int ID, int x , int y)
{
	this->owner = owner;
	this->ID = ID;
	this->x = x;
	this->y = y;
}

void SimpleControl::Update()
{
}

void SimpleControl::Draw()
{
}

SimpleControl::~SimpleControl()
{
}

SimpleButton::SimpleButton()
{
	m_pMyShaderResource = 0;
	width = 0;
	height = 0;
	pressed = false;
}

void SimpleButton::Draw()
{
	owner->DrawButton(this);
}

SimpleButton::SimpleButton(SimpleGUI* owner,int ID, ID3D10ShaderResourceView* shaderResource , int x , int y, int width, int height):SimpleControl(owner,ID,x,y)
{
	this->width = width;
	this->height = height;
	this->m_pMyShaderResource = shaderResource;
	pressed  = false;
}

SimpleButton::~SimpleButton()
{
}

void SimpleButton::Update()
{
	if(CheckButton(owner->GetMouse()))
		owner->InsertEvent(ID);
}

bool SimpleButton::CheckButton(Mouse* mouse)
{
	if(mouse->x < x)
		return false;
	if(mouse->x > x + width)
		return false;
	if(mouse->y < y)
		return false;
	if(mouse->y > y + height)
		return false;
	if(!mouse->left_click)
		return false;
	return true;
}

ID3D10ShaderResourceView* SimpleButton::GetResource()
{
	return m_pMyShaderResource;
}