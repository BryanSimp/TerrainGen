////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"

ApplicationClass::ApplicationClass()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_ColorShader = 0;
}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd, InputClass* input)
{
	bool result;

	m_Input = input;


	// Create and initialize the Direct3D object.
	m_Direct3D = new D3DClass;

	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 1.0f, -4.0f);

	// Create and initialize the model object.
	m_Model = new ModelClass;

	result = m_Model->Initialize(m_Direct3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the color shader object.
	m_ColorShader = new ColorShaderClass;

	result = m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void ApplicationClass::Shutdown()
{
	// Release the color shader object.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	return;
}

// Add this new method to handle camera movement
void ApplicationClass::HandleCameraMovement()
{
	float moveSpeed = 0.5f;  // Speed of camera movement
	float rotateSpeed = 1.0f;

	XMFLOAT3 cameraPos = m_Camera->GetPosition();
	XMFLOAT3 cameraRot = m_Camera->GetRotation();

	// Calculate forward and right vectors based on the camera's yaw (rotation around the Y-axis)
	float yaw = XMConvertToRadians(cameraRot.y);

	XMFLOAT3 forward(-sinf(yaw), 0.0f, -cosf(yaw));
	XMFLOAT3 right(cosf(yaw), 0.0f, -sinf(yaw));

	// Forward/Backward movement
	if (m_Input->IsKeyDown('W'))  // Move forward
	{
		cameraPos.x -= forward.x * moveSpeed;
		cameraPos.z -= forward.z * moveSpeed;
	}
	if (m_Input->IsKeyDown('S'))  // Move backward
	{
		cameraPos.x += forward.x * moveSpeed;
		cameraPos.z += forward.z * moveSpeed;
	}

	// Left/Right movement
	if (m_Input->IsKeyDown('A'))  // Move left
	{
		cameraPos.x -= right.x * moveSpeed;
		cameraPos.z -= right.z * moveSpeed;
	}
	if (m_Input->IsKeyDown('D'))  // Move right
	{
		cameraPos.x += right.x * moveSpeed;
		cameraPos.z += right.z * moveSpeed;
	}

	// Up/Down movement
	if (m_Input->IsKeyDown(VK_UP))  // Move Up
	{
		cameraPos.y += moveSpeed/2;
	}
	if (m_Input->IsKeyDown(VK_DOWN))  // Move Down
	{
		cameraPos.y -= moveSpeed/2;
	}

	// Rotation (yaw)
	if (m_Input->IsKeyDown(VK_RIGHT))
	{
		cameraRot.y += rotateSpeed;
	}
	if (m_Input->IsKeyDown(VK_LEFT))
	{
		cameraRot.y -= rotateSpeed;
	}

	// Update camera position and rotation
	m_Camera->SetPosition(cameraPos.x, cameraPos.y, cameraPos.z);
	m_Camera->SetRotation(cameraRot.x, cameraRot.y, cameraRot.z);
}


bool ApplicationClass::Frame()
{
	bool result;

	HandleCameraMovement(); // Handle camera movement
	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}


bool ApplicationClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// Render the model using the color shader.
	result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}