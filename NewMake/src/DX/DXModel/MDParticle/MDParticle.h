#ifndef __MDPARTICLE_H__
#define __MDPARTICLE_H__

struct MDPARTICLE
{
	MDPARTICLE();
	MDPARTICLE( float X, float Y, float Z, float R, float G, float B )
	{
		POS.x = X;
		POS.y = Y;
		POS.z = Z;

		COLOR.x = R;
		COLOR.y = G;
		COLOR.z = B;
	};
	~MDPARTICLE();

	XMFLOAT MASS;
	XMFLOAT3 FORCE;
	XMFLOAT3 VELOCITY;
	XMFLOAT3 POS;
	XMFLOAT3 COLOR;
};

#endif