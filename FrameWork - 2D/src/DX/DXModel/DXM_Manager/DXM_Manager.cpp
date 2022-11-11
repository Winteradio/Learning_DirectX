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

bool DXM_MANAGER::Init( TYPEINFO*& typeList, DXMPOLYGON Type )
{
	typeList = new TYPEINFO;
	if ( !typeList )
	{
		LOG_ERROR(" Failed - Create Type List \n ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Type List \n ");
	}

	typeList->TYPE = Type;
	typeList->NumModel = 0;
	typeList->Length = 1;

	typeList->MODELS = new MODELINFO[ typeList->Length ];
	if ( !typeList->MODELS )
	{
		LOG_ERROR(" Failed - Create Model List \n ");
		return false;
	}
	else
	{
		Create( typeList );
	}

	return true;
}

void DXM_MANAGER::Release( TYPEINFO*& typeList )
{
	if ( typeList )
	{
		delete[] typeList;
		typeList = nullptr;
	}
}

bool DXM_MANAGER::Frame( TYPEINFO*& typeList, bool InsertState, int mouseX, int mouseY, float Time, float prevTime )
{
	if ( InsertState && (int)(Time * 5000) != (int)(prevTime * 5000) )
	{
		Insert( typeList, mouseX, mouseY );
	}
	return true;
}

void DXM_MANAGER::Rotation( float X, float Y, float Z, MODELINFO*& model )
{
	model->ANG = XMFLOAT3( X, Y, Z );
}

void DXM_MANAGER::Translation( float X, float Y, float Z, MODELINFO*& model )
{
	model->POS = XMFLOAT3( X, Y, Z );
}

void DXM_MANAGER::Scale( float X, float Y, float Z ) {}

void DXM_MANAGER::InitPointer() {}

void DXM_MANAGER::Destory( const int Num ) {}

void DXM_MANAGER::Create( TYPEINFO*& typeList )
{
	typeList->NumVertex = typeList->TYPE + 1;
	typeList->NumIndex = typeList->TYPE * 3;
	typeList->VERTICES = new VERTEXINFO[ typeList->NumVertex ];
	typeList->INDICES = new UINT[ typeList->NumIndex ];
	float radius = 10.0f;

	for ( int I = 0; I < typeList->NumVertex; I++ )
	{
		XMFLOAT3 Position;
		XMFLOAT4 Color = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
		if ( I == 0 )
		{
			Position = XMFLOAT3( 0.0f, 0.0f, 0.0f );
		}
		else
		{
			Position = XMFLOAT3( radius * (float)cos( I * 2 * M_PI / typeList->TYPE ), radius * (float)sin( I * 2 * M_PI / typeList->TYPE ), 0.0f );
		}
		typeList->VERTICES[ I ].POS = Position;
		typeList->VERTICES[ I ].COLOR = Color;
		typeList->VERTICES[ I ].NORMAL = XMFLOAT3( 0.0f, 0.0f, 1.0f );
		if ( I >= 1 )
		{
			int Temp = I + 1;
			if ( Temp > typeList->TYPE ) { Temp -= typeList->TYPE; }
			typeList->INDICES[ 3 * (I - 1) ] = 0;
			typeList->INDICES[ 3 * (I - 1) + 1 ] = Temp;
			typeList->INDICES[ 3 * (I - 1) + 2 ] = I;
		}
	}
}

void DXM_MANAGER::Insert( TYPEINFO*& typeList, int mouseX, int mouseY )
{
	if ( typeList->NumModel + 1 > typeList->Length )
	{
		MODELINFO* Temp = new MODELINFO[ 2 * typeList->Length ];
		for ( int I = 0; I < typeList->NumModel; I++ )
		{
			Temp[ I ] = typeList->MODELS[ I ];
		}
		delete[] typeList->MODELS;
		typeList->MODELS = Temp;

		typeList->Length *= 2;
	}

	typeList->NumModel++;
	typeList->MODELS[ typeList->NumModel -1 ].POS = XMFLOAT3( (float)mouseX, (float)mouseY, 0.0f );
	typeList->MODELS[ typeList->NumModel -1 ].ACC = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	typeList->MODELS[ typeList->NumModel -1 ].VEL = XMFLOAT3( 0.0f, 0.0f, 0.0f );

	typeList->MODELS[ typeList->NumModel -1 ].ANGACC = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	typeList->MODELS[ typeList->NumModel -1 ].ANGVEL = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	typeList->MODELS[ typeList->NumModel -1 ].ANG = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	typeList->MODELS[ typeList->NumModel -1 ].MASS = 1.0f;
}