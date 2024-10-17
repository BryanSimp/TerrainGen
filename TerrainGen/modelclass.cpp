////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"
#include "perlinclass.h"


ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device)
{
	bool result;


	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}


void ModelClass::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}


bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the number of vertices in the vertex array.
	m_vertexCount = (RESOLUTION + 1) * (RESOLUTION + 1);

	// Set the number of indices in the index array.
	m_indexCount = RESOLUTION * RESOLUTION * 6;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Terrain size parameters (e.g., how large the grid should be in world units)
	float terrainWidth = 1000.0f;  // Width of the terrain
	float terrainHeight = 1000.0f; // Depth of the terrain

	// Calculate the step sizes in world units based on the resolution
	float stepX = terrainWidth / RESOLUTION;
	float stepZ = terrainHeight / RESOLUTION;

	// Calculate half-sizes for centering
	float halfWidth = terrainWidth / 2.0f;
	float halfHeight = terrainHeight / 2.0f;

	PerlinClass perlinNoise;
	float heightScale = 15.0f;

	// Initialize min and max height
	float minHeight = FLT_MAX;
	float maxHeight = -FLT_MAX;

	float x, z;
	int index = 0;

	// Create the vertices and find min/max heights
	for (int row = 0; row <= RESOLUTION; row++) {
		z = halfHeight - row * stepZ;
		for (int col = 0; col <= RESOLUTION; col++) {
			x = -halfWidth + col * stepX;
			float noiseScale = 0.05f;
			float y = perlinNoise.perlin(col * noiseScale, row * noiseScale) * heightScale;

			// Update min and max heights
			if (y < minHeight) minHeight = y;
			if (y > maxHeight) maxHeight = y;

			vertices[index].position = XMFLOAT3(x, y, z);

			// Color calculation based on normalized height
			float normalizedHeight = (y - minHeight) / (maxHeight - minHeight);
			vertices[index].color = XMFLOAT4(normalizedHeight * 0.8f + 0.2f, normalizedHeight * 0.8f + 0.2f, normalizedHeight * 0.8f + 0.2f, 1.0f); // Dark color at low height

			index++;
		}
	}

	// Load the indices based on the resolution (as before)
	index = 0;
	for (int col = 0; col < RESOLUTION; col++) {
		for (int row = 0; row < RESOLUTION; row++) {
			// Get the top-left vertex of the current quad
			int topLeft = (row * (RESOLUTION + 1)) + col;
			int topRight = topLeft + 1;
			int bottomLeft = topLeft + (RESOLUTION + 1);
			int bottomRight = bottomLeft + 1;

			// First triangle
			indices[index++] = topLeft;
			indices[index++] = topRight;
			indices[index++] = bottomLeft;

			// Second triangle
			indices[index++] = bottomRight;
			indices[index++] = bottomLeft;
			indices[index++] = topRight;
		}
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}