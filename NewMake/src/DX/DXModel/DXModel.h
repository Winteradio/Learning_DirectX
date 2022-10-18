#ifndef __DXMODEL_H__
#define __DXMODEL_H__

#include "DXCommon.h"

#define COORDINATE_1 100
#define COORDINATE_2 200
#define COORDINATE_3 300

class DXMODEL
{
	public :

		DXMODEL();
		DXMODEL( const DXMODEL* );
		~DXMODEL();

	public :

		bool Init();
		void Release();

	private :
};

#endif