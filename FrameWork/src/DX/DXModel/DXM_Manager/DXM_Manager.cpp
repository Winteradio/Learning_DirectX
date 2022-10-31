#include "DXM_Manager.h"

DXM_MANAGER::DXM_MANAGER()
{
	InitPointer();
}

DXM_MANAGER::DXM_MANAGER( const DXM_MANAGER* Other )
{
	*this = *Other;
}

DXM_MANAGER::~DXM_MANAGER() {}

bool DXM_MANAGER::Init( const VertexType* vertices, const UINT* indices, const int VertexCount, const int numModel )
{

	for ( int I = 0; I < numModel; I++ )
	{
		Create( vertices, indices, VertexCount );
	}
	//system("pause");
	return true;
}

void DXM_MANAGER::Release()
{

}

bool DXM_MANAGER::Frame()
{
	return true;
}

void DXM_MANAGER::Rotation( float X, float Y, float Z ) {}

void DXM_MANAGER::Translation( float X, float Y, float Z ) {}

void DXM_MANAGER::Scale( float X, float Y, float Z ) {}

void DXM_MANAGER::InitPointer() {}

void DXM_MANAGER::Destory( const int Num ) {}

void DXM_MANAGER::Create( const VertexType* vertices, const UINT* indices, const int VertexCount )
{
	for ( int I = 0; I < VertexCount; I++ )
	{
		LOG_INFO(" NUM : %d , POS : %f %f %f \n ", I, vertices[ I ].POS.x, vertices[ I ].POS.y, vertices[ I ].POS.z );
		LOG_INFO(" NUM : %d , NOR : %f %f %f \n ", I, vertices[ I ].NORMAL.x, vertices[ I ].NORMAL.y, vertices[ I ].NORMAL.z );
	}

	PlaneType* Info = new PlaneType[ 2 ];
	for ( int I = 0; I < VertexCount/3; I++ )
	{
		Info->POINTS = new XMFLOAT3[ 3 ];
		for ( int J = 0; J < 3; J++ )
		{
			Info->POINTS[ J ] = vertices[ indices[ 3 * I + J ] ].POS;
		}
		Info->NORMAL = vertices[ indices[ 3 * I ] ].NORMAL;
	}

	delete[] Info;
	Info = nullptr;
}
