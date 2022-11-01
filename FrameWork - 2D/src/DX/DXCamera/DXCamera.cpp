#include "DXCamera.h"


DXCAMERA::DXCAMERA()
{
	m_Position = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	m_Rotation = XMFLOAT3( 0.0f, 0.0f, 0.0f );
}


DXCAMERA::DXCAMERA( const DXCAMERA* Other )
{
	*this = *Other;
}


DXCAMERA::~DXCAMERA(){}

bool DXCAMERA::Frame( int mouseX, int mouseY )
{
	return true;
}

void DXCAMERA::Render()
{
	XMFLOAT3 Up, Pos, LookAt;
	XMVECTOR upVector, posVector, lookAtVector;
	float pitch, yaw, roll;
	XMMATRIX rotationMatrix;

	// Set Up Vector
	Up.x = 0.0f;
	Up.y = 1.0f;
	Up.z = 0.0f;

	upVector = XMLoadFloat3( &Up );


	// Set Camera Position in 3D World
	Pos = m_Position;

	posVector = XMLoadFloat3( &m_Position );


	// Set LookAt Vector
	LookAt.x = 0.0f;
	LookAt.y = 0.0f;
	LookAt.z = 1.0f;

	lookAtVector = XMLoadFloat3( &LookAt );

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

void DXCAMERA::GetViewMatrix( XMMATRIX& ViewMatrix ) { ViewMatrix = m_ViewMatrix; }

XMFLOAT3 DXCAMERA::GetPosition() { return m_Position; }

XMFLOAT3 DXCAMERA::GetRotation() { return m_Rotation; }