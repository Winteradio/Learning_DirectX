#ifndef __DXM_MANAGER_H__
#define __DXM_MANAGER_H__

#include "DXM_Common.h"
#include "DXM_Physics.h"

class DXM_MANAGER
{
	public :

		DXM_MANAGER();
		DXM_MANAGER( const DXM_MANAGER* );
		~DXM_MANAGER();

	public :

		bool Init( TYPEINFO*&, DXMPOLYGON );
		void Release( TYPEINFO*& );
		bool Frame( TYPEINFO*&, bool, int, int, float, float );

	private :

		void Rotation( float, float, float, MODELINFO*& );
		void Translation( float, float, float, MODELINFO*& );
		void Scale( float, float ,float );

		void InitPointer();

		void Destory( const int );
		void Create( TYPEINFO*& );
		void Insert( TYPEINFO*&, int, int );

		int m_InsertSpeed;
};

#endif