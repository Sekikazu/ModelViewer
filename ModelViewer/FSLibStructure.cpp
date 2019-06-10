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
		VGet((*p0)[0], (*uv0)[0], (*uv0)[1]),
		VGet((*p0)[1], (*uv0)[0], (*uv0)[1]),
		VGet((*p0)[2], (*uv0)[0], (*uv0)[1]),
	};
	VECTOR CP1[3] = {
		VGet((*p1)[0], (*uv1)[0], (*uv1)[1]),
		VGet((*p1)[1], (*uv1)[0], (*uv1)[1]),
		VGet((*p1)[2], (*uv1)[0], (*uv1)[1]),
	};
	VECTOR CP2[3] = {
		VGet((*p2)[0], (*uv2)[0], (*uv2)[1]),
		VGet((*p2)[1], (*uv2)[0], (*uv2)[1]),
		VGet((*p2)[2], (*uv2)[0], (*uv2)[1]),
	};

	// ���ʃp�����[�^����UV�����W�Z�o
	float U[3], V[3];
	for (int i = 0; i < 3; ++i) {
		VECTOR V1 = CP1[i] - CP0[i];
		VECTOR V2 = CP2[i] - CP1[i];
		VECTOR ABC = VCross(V1, V2);
		//D3DXVec3Cross(&ABC, &V1, &V2);

		if (ABC[0] == 0.0f) {
			// �|���S����UV��̃|���S�����k��
			_ASSERT(0);
			memset(outTangent, 0, sizeof(VECTOR));
			memset(outBinormal, 0, sizeof(VECTOR));
			return;
		}
		U[i] = -ABC[1] / ABC[0];
		V[i] = -ABC[2] / ABC[0];
	}

	memcpy(outTangent, U, sizeof(float) * 3);
	memcpy(outBinormal, V, sizeof(float) * 3);

	// ���K��
	*outTangent = VNorm(*outTangent);

	*outBinormal = VNorm(*outBinormal);
	//*outBinormal = *outBinormal * -1.f;
}