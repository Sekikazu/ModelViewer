#pragma once
#include "FSLibStructure.h"

struct DIRECTIONAL_LIGHT {
	VECTOR direction;
	COLOR color;
};

struct POINT_LIGHT {
	VECTOR pos;
	float range;
	float atten[3];
};

class LIGHT {
private:
	vector<DIRECTIONAL_LIGHT> directional;
	vector<POINT_LIGHT> point;

public:
	LIGHT(void);
	~LIGHT(void);

	/* ���s���� */
	bool CreateDirectional(DIRECTIONAL_LIGHT);
	bool CreateDirectional(VECTOR direct);
	bool GetDirectional(size_t n, DIRECTIONAL_LIGHT* dl);
	bool SetDirectional(size_t n, DIRECTIONAL_LIGHT& dl);

	/* �|�C���g���C�g */
	bool CreatePoint(POINT_LIGHT);
	bool CreatePoint(VECTOR position, float range, float atten0, float atten1, float atten2);
	bool GetPoint(size_t n, POINT_LIGHT* pl);

	/* �X�|�b�g���C�g */
};