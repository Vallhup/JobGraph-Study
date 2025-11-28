#pragma once

#include <DirectXMath.h>
#include "Component.h"

using namespace DirectX;

namespace TransformHelper
{
	inline XMMATRIX ToMatrix(const Transform& t)
	{
		XMVECTOR S = XMLoadFloat3(&t.scale);
		XMVECTOR R = XMLoadFloat4(&t.rotation);
		XMVECTOR P = XMLoadFloat3(&t.position);
		return XMMatrixAffineTransformation(S, XMVectorZero(), R, P);
	}

	inline XMVECTOR Forward(const Transform& t)
	{
		return XMVector3Normalize(
			XMVector3TransformNormal(
				XMVectorSet(0, 0, -1, 0),
				ToMatrix(t)
			)
		);
	}
}