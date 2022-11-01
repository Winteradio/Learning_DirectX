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

bool DXM_MANAGER::Init( MODELINFO*& modelList, const int numModel, DXMPOLYGON Type )
{
	modelList = new MODELINFO[ numModel ];
	if ( !modelList )
	{
		LOG_ERROR(" Failed - Create Model List \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Model List \n ");
	}

	for ( int I = 0; I < numModel; I++ )
	{
		Create( modelList[ I ], Type );
	}

	return true;
}

void DXM_MANAGER::Release( MODELINFO*& modelList )
{
	if ( modelList )
	{
		delete[] modelList;
		modelList = nullptr;
	}
}

bool DXM_MANAGER::Frame()
{
	return true;
}

void DXM_MANAGER::Rotation( float X, float Y, float Z, MODELINFO& model )
{
	model.ROT = XMFLOAT3( X, Y, Z );
}

void DXM_MANAGER::Translation( float X, float Y, float Z, MODELINFO& model )
{
	model.POS = XMFLOAT3( X, Y, Z );
}

void DXM_MANAGER::Scale( float X, float Y, float Z ) {}

void DXM_MANAGER::InitPointer() {}

void DXM_MANAGER::Destory( const int Num ) {}

void DXM_MANAGER::Create( MODELINFO& model, DXMPOLYGON Type )
{
	model.TYPE = Type;
	model.NumVertex = model.TYPE + 1;
	model.NumIndex = model.TYPE * 3;
	model.VERTICES = new VERTEXINFO[ model.NumVertex ];
	model.INDICES = new UINT[ model.NumIndex ];

	for ( int I = 0; I < model.NumVertex; I++ )
	{
		XMFLOAT3 Position;
		XMFLOAT4 Color = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
		if ( I == 0 )
		{
			Position = XMFLOAT3( 0.0f, 0.0f, 0.0f );
		}
		else
		{
			Position = XMFLOAT3( cos( I * 2 * M_PI / model.TYPE ), sin( I * 2 * M_PI / model.TYPE ), 0.0f );
		}
		model.VERTICES[ I ].POS = Position;
		LOG_INFO(" %f %f \n ",Position.x, Position.y);
		model.VERTICES[ I ].COLOR = Color;
		model.VERTICES[ I ].NORMAL = XMFLOAT3( 0.0f, 0.0f, 1.0f );
		if ( I >= 1 )
		{
			int Temp = I + 1;
			if ( Temp > model.TYPE ) { Temp -= model.TYPE; }
			model.INDICES[ 3 * (I - 1) ] = 0;
			model.INDICES[ 3 * (I - 1) + 1 ] = I;
			model.INDICES[ 3 * (I - 1) + 2 ] = Temp;
			LOG_INFO(" %d %d \n ", I, Temp );
		}
	}
}
