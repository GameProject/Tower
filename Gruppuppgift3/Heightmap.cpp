#include "Heightmap.h"
#include <fstream>

Heightmap::Heightmap()
{
	d3dDevice = 0; 
	vertexBuffer = 0;
	indexBuffer = 0;

	mHeightScale = 0.0f;
	mHeightOffset = 0.0f;
}

Heightmap::~Heightmap()
{
	vertexBuffer->Release();
	indexBuffer->Release();
	vertexBuffer = 0;
	indexBuffer  = 0;
}

void Heightmap::init(ID3D10Device* device, std::string& filename, 
					 float HeightScale, float HeightOffset, 
					 float size, int NumRows, int NumCols)
{
	d3dDevice		= device;

	D3DXMatrixIdentity(&mWorld);

	mHeightScale	= HeightScale;
	mHeightOffset	= HeightOffset;
	mSize			= size;

	mNumRows		= NumRows;
	mNumCols		= NumCols;

	mNumVertices	= mNumRows*mNumCols;
	mNumFaces		= (mNumRows-1)*(mNumCols-1)*2;

	//A height for each vertex
	std::vector<unsigned char> vertexHeights(mNumVertices);

	//Open file of reading
	std::ifstream fin;
	fin.open(filename.c_str(), std::ios_base::binary);

	if (fin)
	{
		//Read all the RAW bytes in once
		fin.read((char*)&vertexHeights[0], (std::streamsize)vertexHeights.size());
		fin.close();
	}

	//Copy array data into a float table (matrix) format
	//Scale and offset the heights
	mHeightmap.resize(mNumVertices, 0);
	for(UINT i = 0; i < (mNumVertices); ++i)
	{
		mHeightmap[i] = (float)vertexHeights[i] * mHeightScale + mHeightOffset;
	}
	//Filter the table to smooth it out
	boxFilter();
	buildVB();
}

void Heightmap::boxFilter()
{
	std::vector<float> FilteredHeightMap(mHeightmap.size());

	for (UINT i=0; i < mNumRows; ++i)
		for (UINT j=0; j < mNumCols; ++j)
			FilteredHeightMap[i*mNumCols+j] = getAverage(i, j);
	
	mHeightmap = FilteredHeightMap;
}

float Heightmap::getAverage(UINT i, UINT j)
{
	float average = 0.0f;
	float sample = 0.0f;

	for (UINT m = i-1; m <= i+1; ++m)
	{
		for (UINT n = j-1; n <= j+1; ++n)
		{
			if (inBoundsOfHeightMap(m, n))
			{
				average += mHeightmap[m*mNumCols+n];
				sample  += 1.0f;
			}
		}
	}
	return average/sample;
}

bool Heightmap::inBoundsOfHeightMap(UINT i, UINT j)
{
	// True if ij are valid indices; false otherwise.
	return i >= 0 && i < mNumRows && j >= 0 && j < mNumCols;
}

void Heightmap::buildVB()
{
	//Write vertices
	vpnt.resize(mNumVertices);

	float du = 1.0f / (mNumCols-1);
	float dv = 1.0f / (mNumRows-1);

	float halfWidth = (mNumCols-1)*mSize*0.5f;
	float halfDepth = (mNumRows-1)*mSize*0.5f;

	for(UINT i = 0; i < mNumRows; ++i)
	{
		float z = -halfDepth + i*mSize;
		for(UINT j = 0; j < mNumCols; ++j)
		{
			float x = -halfWidth + j*mSize;

			float y = mHeightmap[i*mNumCols+j]*mSize;
			vpnt[i*mNumCols+j].pos    = D3DXVECTOR3(x, y, z);
			vpnt[i*mNumCols+j].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			vpnt[i*mNumCols+j].texC.x = j*du;
			vpnt[i*mNumCols+j].texC.y = i*dv;
		}
	}

	D3D10_BUFFER_DESC vbd;
	vbd.Usage = D3D10_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(TerrainVertex) * mNumVertices;
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vpnt[0];
	d3dDevice->CreateBuffer(&vbd, &vinitData, &vertexBuffer);
	buildIB();
}

void Heightmap::buildIB()
{
	std::vector<int> indices(mNumFaces*3); // 3 indices per face

	// Iterate over each quad and compute indices.
	int k = 0;
	for(UINT i = 0; i < mNumRows-1; ++i)
	{
		for(UINT j = 0; j < mNumCols-1; ++j)
		{
			indices[k+1] = i*mNumCols+j;
			indices[k]   = i*mNumCols+j+1;
			indices[k+2] = (i+1)*mNumCols+j;

			indices[k+4] = (i+1)*mNumCols+j;
			indices[k+3] = i*mNumCols+j+1;
			indices[k+5] = (i+1)*mNumCols+j+1;

			k += 6; // next quad
		}
	}

	D3D10_BUFFER_DESC ibd;
	ibd.Usage = D3D10_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
	ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	d3dDevice->CreateBuffer(&ibd, &iinitData, &indexBuffer);
}

float Heightmap::getPosY(float x, float z)
{
	UINT i = 0;
	if (x > vpnt[(mNumRows/2)*mNumCols].pos.x)
		if(z > vpnt[(mNumCols/2)*mNumCols].pos.z)
			i = mNumRows/2;
	for(i; i < mNumRows; ++i)
	{
		for(UINT j = 0; j < mNumCols; ++j)
		{
			if (vpnt[i*mNumCols+j].pos.x >= x-1.0f && vpnt[i*mNumCols+j].pos.x <= x+1.0f && 
				vpnt[i*mNumCols+j].pos.z >= z-1.0f && vpnt[i*mNumCols+j].pos.z <= z+1.0f)
			{
				lastY = vpnt[i*mNumCols+j].pos.y;
				return lastY;
			}
		}
	}
	return lastY;
}

float Heightmap::getHeight(float x, float z)
{
	// Transform from terrain local space to "cell" space.
	float c = (x + 0.5f*((mNumRows-1)*mSize)) /  mSize;
	float d = (z - 0.5f*((mNumCols-1)*mSize)) / -mSize;

	// Get the row and column we are in.
	int row = (int)floorf(d);
	int col = (int)floorf(c);

	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = mHeightmap[row*mNumCols + col];
	float B = mHeightmap[row*mNumCols + col + 1];
	float C = mHeightmap[(row+1)*mNumCols + col];
	float D = mHeightmap[(row+1)*mNumCols + col + 1];

	// Where we are relative to the cell.
	float s = c - (float)col;
	float t = d - (float)row;

	// If upper triangle ABC.
	if( s + t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		return A + s*uy + t*vy;
	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		return D + (1.0f-s)*uy + (1.0f-t)*vy;
	}
}

void Heightmap::Draw()
{
	UINT stride = sizeof(TerrainVertex);
	UINT offset = 0;
	d3dDevice->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	d3dDevice->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	d3dDevice->DrawIndexed(mNumFaces*3, 0, 0);
}

D3DXMATRIX& Heightmap::getWorld()
{
	return mWorld;
}

int Heightmap::GetVertexCount()
{
	return (int)vpnt.size();
}

std::vector<TerrainVertex> Heightmap::CopyVertexArray()
{
	return vpnt;
}

void Heightmap::DoMinMax()
{
	float minX = NULL;
	float maxX = NULL;
	float minY = NULL;
	float maxY = NULL;
	float minZ = NULL;
	float maxZ = NULL;
	for (int i=0; i<(int)vpnt.size(); ++i)
	{
		if (minX == NULL)
			minX = vpnt.at(i).pos.x;
		else if (minX > vpnt.at(i).pos.x)
			minX = vpnt.at(i).pos.x;

		if (maxX == NULL)
			maxX = vpnt.at(i).pos.x;
		else if (maxX < vpnt.at(i).pos.x)
			maxX = vpnt.at(i).pos.x;

		if (minY == NULL)
			minY = vpnt.at(i).pos.y;
		else if (minY > vpnt.at(i).pos.y)
			minY = vpnt.at(i).pos.y;

		if (maxY == NULL)
			maxY = vpnt.at(i).pos.y;
		else if (maxY < vpnt.at(i).pos.y)
			maxY = vpnt.at(i).pos.y;

		if (minZ == NULL)
			minZ = vpnt.at(i).pos.z;
		else if (minZ > vpnt.at(i).pos.z)
			minZ = vpnt.at(i).pos.z;

		if (maxZ == NULL)
			maxZ = vpnt.at(i).pos.z;
		else if (maxZ < vpnt.at(i).pos.z)
			maxZ = vpnt.at(i).pos.z;
	}
	minPos = D3DXVECTOR3(minX, minY, minZ);
	maxPos = D3DXVECTOR3(maxX, maxY, maxZ);
}

D3DXVECTOR3 Heightmap::getMin()
{
	return minPos;
}

D3DXVECTOR3 Heightmap::getMax()
{
	return maxPos;
}