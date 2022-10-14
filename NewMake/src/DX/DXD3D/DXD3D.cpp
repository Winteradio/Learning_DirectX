#include "DXD3D.h"

DXD3D::DXD3D()
{}

DXD3D::DXD3D( const DXD3D& Other )
{
	*this = Other;
}

DXD3D::~DXD3D()
{}

bool DXD3D::Init( int Width, int Height, bool vsync, HWND hWnd, float SCREEN_DEPTH, float SCREEN_NEAR )
{
	return true;
}

void DXD3D::Done()
{}

void DXD3D::BeginScene( float Red, float Blue, float Green, float Alpha )
{}

void DXD3D::EndScene()
{}

ID3D11Device* DXD3D::GetDevice()
{
	return m_Device;
}

ID3D11DeviceContext* DXD3D::GetDeviceContext()
{
	return m_DeviceContext;
}

void DXD3D::GetProjectionMatrix( XMMATRIX& ProjectionMartix )
{
	ProjectionMartix = m_ProjectionMatrix;
}

void DXD3D::GetWorldMatrix( XMMATRIX& WorldMatrix )
{
	WorldMatrix = m_WorldMatrix;
}

void DXD3D::GetOrthoMatrix( XMMATRIX& OrthoMatrix )
{
	OrthoMatrix = m_OrthoMatrix;
}

void DXD3D::GetVideoCardInfo( char* CardName, int& Memory )
{
	strcpy_s( CardName, 128, m_VideoCardMemory );
	Memory = m_VideoCardMemory;
}