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


void DXCAMERA::Render()
{
	XMFLOAT3 Up, Pos, LookAt;
	XMVECTOR upVector, posVector, lookAtVector;
	float pitch, yaw, roll;
	XMMATRIX rotationMatrix;


	/////////////////////
	// Up Vector
	/////////////////////
	// Set Up Vector
	Up.x = 0.0f;
	Up.y = 1.0f;
	Up.z = 0.0f;


	/////////////////////
	// Position Vector
	/////////////////////
	// Set Camera Position in 3D World
	SetPosition( 0.0f, 0.0f, -5.0f );
	Pos = m_Position;


	/////////////////////
	// LookAt Vector
	/////////////////////
	// Set LookAt Vector
	LookAt.x = 0.0f;
	LookAt.y = 0.0f;
	LookAt.z = -1.0f;


	// Load Vector to XMVECTOR Structor
	upVector = XMLoadFloat3( &Up );
	posVector = XMLoadFloat3( &Pos );
	lookAtVector = XMLoadFloat3( &LookAt );


	/////////////////////
	// Rotation Matrix
	/////////////////////
	// Change Angle Rotation, Degree to Radian
	pitch = (float)( m_Rotation.x * PI / 180 );
	yaw = (float)( m_Rotation.y * PI / 180 );
	roll = (float)( m_Rotation.z * PI / 180 );

	// Make Rotation Matrix
	rotationMatrix = XMMatrixRotationRollPitchYaw( pitch, yaw, roll );

	// Rotate Vectors using rotationMatrix to Origin
	lookAtVector = XMVector3TransformCoord( lookAtVector, rotationMatrix );
	upVector = XMVector3TransformCoord( upVector, rotationMatrix );

	// Change Vector using Viewer Position
	lookAtVector = XMVectorAdd( posVector, lookAtVector );

	// Set View Matrix using Camera Vectors

	posVector = XMVectorSet( 3.0f, 3.0f, 3.0f, 0.0f );
	lookAtVector = XMVectorSet( -1.0f, -1.0f, -1.0f, 0.0f );
	upVector = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );

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