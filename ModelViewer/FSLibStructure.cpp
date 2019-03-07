#include "FSLibStructure.h"

// 3���_��UV�l����w����W�ł�U���iTangent�j�y��V���iBinormal�j���Z�o
//
// p0, p1, p2    : ���[�J����Ԃł̒��_���W�i�|���S���`�揇�ɂ��邱�Ɓj
// uv0, uv1, uv2 : �e���_��UV���W
// outTangent    : U���iTangent�j�o��
// outBinormal   : V���iBinormal�j�o��

void CalcTangentAndBinormal(
	VECTOR* p0, VECTOR2D* uv0,
	VECTOR* p1, VECTOR2D* uv1,
	VECTOR* p2, VECTOR2D* uv2,
	VECTOR* outTangent, VECTOR* outBinormal) {
	// 5������3�������_��
	VECTOR CP0[3] = {
		VGet(p0->x, uv0->x, uv0->y),
		VGet(p0->y, uv0->x, uv0->y),
		VGet(p0->z, uv0->x, uv0->y),
	};
	VECTOR CP1[3] = {
		VGet(p1->x, uv1->x, uv1->y),
		VGet(p1->y, uv1->x, uv1->y),
		VGet(p1->z, uv1->x, uv1->y),
	};
	VECTOR CP2[3] = {
		VGet(p2->x, uv2->x, uv2->y),
		VGet(p2->y, uv2->x, uv2->y),
		VGet(p2->z, uv2->x, uv2->y),
	};

	// ���ʃp�����[�^����UV�����W�Z�o
	float U[3], V[3];
	for (int i = 0; i < 3; ++i) {
		VECTOR V1 = CP1[i] - CP0[i];
		VECTOR V2 = CP2[i] - CP1[i];
		VECTOR ABC = VCross(V1, V2);
		//D3DXVec3Cross(&ABC, &V1, &V2);

		if (ABC.x == 0.0f) {
			// �|���S����UV��̃|���S�����k��
			_ASSERT(0);
			memset(outTangent, 0, sizeof(VECTOR));
			memset(outBinormal, 0, sizeof(VECTOR));
			return;
		}
		U[i] = -ABC.y / ABC.x;
		V[i] = -ABC.z / ABC.x;
	}

	memcpy(outTangent, U, sizeof(float) * 3);
	memcpy(outBinormal, V, sizeof(float) * 3);

	// ���K��
	*outTangent = VNorm(*outTangent);

	*outBinormal = VNorm(*outBinormal);
	//*outBinormal = *outBinormal * -1.f;
}