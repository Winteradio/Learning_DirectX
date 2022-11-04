#include "DXCamera.h"


DXCAMERA::DXCAMERA()
{
	m_Position = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	m_Rotation = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	m_DirFocus = XMFLOAT3( 0.0f, 0.0f, 1.0f );
	m_DirUp = XMFLOAT3( 0.0f, 1.0f, 0.0f );
}


DXCAMERA::DXCAMERA( const DXCAMERA* Other )
{
	*this = *Other;
}


DXCAMERA::~DXCAMERA(){}

bool DXCAMERA::Frame( bool LeftButton, int screenWidth, int screenHeight, int mouseX, int mouseY, int prevMouseX, int prevMouseY, int WheelDir )
{
	float MoveX = LeftButton * (float)( - mouseX + prevMouseX ) / ( screenWidth * 30 );
	float MoveY = LeftButton * (float)( - mouseY + prevMouseY ) / ( screenHeight * 30 );
	float MoveZ = (float)WheelDir / 1000;

	m_Position.x += MoveX;
	m_Position.y += MoveY;
	m_Position.z += MoveZ;

	m_DirFocus.x += MoveX;
	m_DirFocus.y += MoveY;
	return true;
}

void DXCAMERA::Render()
{
	XMVECTOR upVector, posVector, lookAtVector;
	float pitch, yaw, roll;
	XMMATRIX rotationMatrix;

	posVector = XMLoadFloat3( &m_Position );
	lookAtVector = XMLoadFloat3( &m_DirFocus );
	upVector = XMLoadFloat3( &m_DirUp );

	/////////////////////
	// Rotation Matrix
	/////////////////////
	// Change Angle Rotation, Degree to Radian
	pitch = (float)( m_Rotation.x * M_PI / 180 );
	yaw = (float)( m_Rotation.y * M_PI / 180 );
	roll = (float)( m_Rotation.z * M_PI / 180 );

	// Make Rotation Matrix
	rotationMatrix = XMMatrixRotationRollPitchYaw( pitch, yaw, roll );

	// Rotate Vectors using rotationMatrix to Origin
	lookAtVector = XMVector3TransformCoord( lookAtVector, rotationMatrix );
	upVector = XMVector3TransformCoord( upVector, rotationMatrix );

	// Change Vector using Viewer Position
	lookAtVector = XMVectorAdd( posVector, lookAtVector );

	// Set View Matrix using Camera Vectors
	m_ViewMatrix = XMMatrixLookAtLH( posVector, lookAtVector, upVector );
}


void DXCAMERA::SetPosition( float POS_X, float POS_Y, float POS_Z )
{
	m_Position.x = POS_X;
	m_Position.y = POS_Y;
	m_Position.z = POS_Z;
}


void DXCAMERA::SetRotation( float ANG_P, float ANG_Y, float ANG_R )
{
	m_Rotation.x = ANG_P;
	m_Rotation.y = ANG_Y;
	m_Rotation.z = ANG_R;
}

void DXCAMERA::SetLookAt( float POS_X, float POS_Y, float POS_Z )
{
	m_DirFocus.x = POS_X;
	m_DirFocus.y = POS_Y;
	m_DirFocus.z = POS_Z;
}

void DXCAMERA::SetLookUp( float POS_X, float POS_Y, float POS_Z )
{
	m_DirUp.x = POS_X;
	m_DirUp.y = POS_X;
	m_DirUp.z = POS_Z;
}

void DXCAMERA::GetViewMatrix( XMMATRIX& ViewMatrix ) { ViewMatrix = m_ViewMatrix; }

XMFLOAT3 DXCAMERA::GetPosition() { return m_Position; }

XMFLOAT3 DXCAMERA::GetRotation() { return m_Rotation; }

XMFLOAT3 DXCAMERA::GetLookAt() { return m_DirFocus; }
XMFLOAT3 DXCAMERA::GetLookUp() { return m_DirUp; }