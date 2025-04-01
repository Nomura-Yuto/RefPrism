//===========================================================================
// vector.cpp				����Vector�N���X
// ����ҁF�쑺 �I�l
//===========================================================================
#include "vector.h"


Vector2 Vector2::Reflect(const Vector2& normal) const
{
	Vector2 incidentVec = Normalize();
	Vector2 normalVec = normal.Normalize();

	float dot = incidentVec.Dot(normalVec);
	if (dot > 0.0f) normalVec = -normalVec;

	Vector2 refVec = incidentVec - 2.0f * incidentVec.Dot(normalVec) * normalVec;

	return refVec.Normalize();
}

Vector2 Vector2::Refract(const Vector2& normal) const
{
	Vector2 incidentVec = Normalize();
	Vector2 normalVec = normal.Normalize();
	float eta = 1.0f / 1.5f;

	float dot = incidentVec.Dot(normalVec);
	if (dot > 0.0f) normalVec = -normalVec;

	float cosThetaI = -incidentVec.Dot(normalVec);

	float sinThetaT2 = eta * eta * (1.0f - cosThetaI * cosThetaI);

	if (sinThetaT2 > 1.0f) return Reflect(normal);

	float cosThetaT = sqrtf(1.0f - sinThetaT2);

	Vector2 refVec = eta * incidentVec + (eta * cosThetaI - cosThetaT) * normalVec;

	return refVec.Normalize();
}

Vector3 Vector3::Reflect(const Vector3& normal) const
{
	Vector3 incidentVec = Normalize();
	Vector3 normalVec = normal.Normalize();

	// ���˂Ɩ@���x�N�g���̓��όv�Z
	float dot = incidentVec.Dot(normalVec);
	// �@������ɓ����������悤�ɕ␳
	if (dot > 0.0f) normalVec = -normalVec;

	// ���˃x�N�g���̌v�Z
	Vector3 refVec = incidentVec - 2.0f * incidentVec.Dot(normalVec) * normalVec;

	return refVec.Normalize();
}

Vector3 Vector3::Refract(const Vector3& normal) const
{
	Vector3 incidentVec = Normalize();
	Vector3 normalVec = normal.Normalize();
	float eta = 1.0f / 1.5f;	// ���ܗ��̔�

	// ���˂Ɩ@���x�N�g���̓��όv�Z
	float dot = incidentVec.Dot(normalVec);
	// �@������ɓ����������悤�ɕ␳
	if (dot > 0.0f) normalVec = -normalVec;

	// ���ˊp�̃R�T�C�����v�Z
	float cosThetaI = -incidentVec.Dot(normalVec);

	// ���܊p�̃T�C�����v�Z(�X�l���̖@��)
	float sinThetaT2 = eta * eta * (1.0f - cosThetaI * cosThetaI);

	// �S���˂̔���
	if (sinThetaT2 > 1.0f) return Reflect(normal);

	// ���܊p�̃R�T�C�����v�Z
	float cosThetaT = sqrtf(1.0f - sinThetaT2);

	// ���܃x�N�g���̌v�Z
	Vector3 refVec = eta * incidentVec + (eta * cosThetaI - cosThetaT) * normalVec;

	return refVec.Normalize();
}

void StoreVector3(Vector3* pStrLoc, XMVECTOR& v)
{
	assert(pStrLoc);

	pStrLoc->x = v.m128_f32[0];
	pStrLoc->y = v.m128_f32[1];
	pStrLoc->z = v.m128_f32[2];
}

XMVECTOR LoadVector3(Vector3* pSource)
{
	assert(pSource);

	__m128 xy = _mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(pSource)));
	__m128 z = _mm_load_ss(&pSource->z);
	return _mm_movelh_ps(xy, z);
}
