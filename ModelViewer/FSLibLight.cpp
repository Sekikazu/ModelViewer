#include "FSLibLight.h"

LIGHT::LIGHT(void) {
	directional.clear();
	directional.shrink_to_fit();

	DIRECTIONAL_LIGHT dl;
	dl.direction = VNorm(1.f, -1.f, 1.f);
	dl.color = GetColor(1.f, 1.f, 1.f, 1.f);
	directional.push_back(dl);

	point.clear();
	point.shrink_to_fit();
}

LIGHT::~LIGHT(void) {
	directional.clear();
	directional.shrink_to_fit();

	point.clear();
	point.shrink_to_fit();
}

/* 平行光源 */
bool LIGHT::CreateDirectional(DIRECTIONAL_LIGHT dl) {
	directional.push_back(dl);

	return true;
}

bool LIGHT::CreateDirectional(VECTOR direct) {
	DIRECTIONAL_LIGHT dl;
	dl.direction = direct;

	directional.push_back(dl);

	return true;
}

bool LIGHT::GetDirectional(size_t n, DIRECTIONAL_LIGHT* dl) {
	if (n >= directional.size())return false;

	*dl = directional[n];
	return true;
}

bool LIGHT::SetDirectional(size_t n, DIRECTIONAL_LIGHT& dl) {
	if (n >= directional.size())return false;

	directional[n] = dl;
	return true;
}

/* ポイントライト */
bool LIGHT::CreatePoint(POINT_LIGHT pl) {
	point.push_back(pl);
	return true;
}

bool LIGHT::CreatePoint(VECTOR position, float range, float atten0, float atten1, float atten2) {
	POINT_LIGHT pl;
	pl.pos = position;
	pl.range = range;
	pl.atten[0] = atten0;
	pl.atten[1] = atten1;
	pl.atten[2] = atten2;
	point.push_back(pl);
	return true;
}

bool LIGHT::GetPoint(size_t n, POINT_LIGHT* pl) {
	if (n >= point.size())return false;

	*pl = point[n];
	return true;
}