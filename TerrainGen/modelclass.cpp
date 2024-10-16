////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"


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
	m_vertexCount = 9;

	// Set the number of indices in the index array.
	m_indexCount = 24;

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

	// Load the vertex array with data.
	vertices[0].position = XMFLOAT3(-1.0f, 1.0f, 0.0f);  // Top Left.
	vertices[0].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top Middle.
	vertices[1].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[2].position = XMFLOAT3(1.0f, 1.0f, 0.0f);  // Top Right.
	vertices[2].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[3].position = XMFLOAT3(-1.0f, 0.0f, 0.0f);  // Middle Left.
	vertices[3].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[4].position = XMFLOAT3(0.0f, 0.0f, 0.0f);  // Middle Center.
	vertices[4].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[5].position = XMFLOAT3(1.0f, 0.0f, 0.0f);  // Middle Right.
	vertices[5].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[6].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[6].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[7].position = XMFLOAT3(0.0f, -1.0f, 0.0f);  // Bottom Middle.
	vertices[7].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[8].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom Right.
	vertices[8].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	// Load the index array with data.

	//First Triangle
	indices[0] = 0; // TL
	indices[1] = 1; // TM
	indices[2] = 3; // ML

	//Second Triangle
	indices[3] = 4; // MC
	indices[4] = 3; // ML
	indices[5] = 1; // TM

	//Third Triangle
	indices[6] = 1; // TM
	indices[7] = 2; // TR
	indices[8] = 4; // TM

	//Fourth Triangle
	indices[9] = 5; // MR
	indices[10] = 4; // MC
	indices[11] = 2; // TR

	//Fifth Triangle
	indices[12] = 3; // MC
	indices[13] = 4; // ML
	indices[14] = 6; // TM

	//Sixth Triangle
	indices[15] = 7; // MC
	indices[16] = 6; // ML
	indices[17] = 4; // TM

	//Seven Triangle
	indices[18] = 4; // MC
	indices[19] = 5; // ML
	indices[20] = 7; // TM

	//Eighth Triagnle
	indices[21] = 8; // MC
	indices[22] = 7; // ML
	indices[23] = 5; // TM


	//Second Triangle
	//indices[3] = 0; // Bottom left
	//indices[4] = 2; // Top right
	//indices[5] = 3; // Bottom right


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