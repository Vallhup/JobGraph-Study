#pragma once

#include "AnimationManager.h"

// 물리 기반 이펙트??
// ex) 칼에 베인 방향대로 이펙트 출력
//     
// 1. 칼 휘두른 방향 벡터 계산
//  - 간단하게 생각하면 (현재 프레임 위치 - 이전 프레임 위치)
//  - 이걸 위해선 이전 프레임 위치를 기억하고 있어야 할듯?
// 
// 2. 충돌 노멀
//  - 정확하게 뭘 말하는건진 모르겠는데 지금도 바로 구할 수 있다 함
// 
// 3. 베어진 방향
//  A. 검의 진행 방향
// 
//  B. 검 진행 방향을 표면에 투영
//   - 소울라이크 / 몬헌 같은 느낌 난다고 함
//   - 흠... 뭔말인지 모르겠음
// 
//  C. 충돌 노멀을 기준으로 양쪽 방향 결정
//   - 우리 알빠는 아닌듯
//   - 이거도 뭔말인지 잘 모르겠음

namespace Collision {
	bool CheckCapsuleVsCapsule(const Capsule& c1, const Capsule& c2)
	{
		XMVECTOR c1A = c1.P0();
		XMVECTOR c1B = c1.P1();
		XMVECTOR c2A = c2.P0();
		XMVECTOR c2B = c2.P1();

		XMVECTOR u = c1B - c1A;
		XMVECTOR v = c2B - c2A;
		XMVECTOR w = c1A - c2A;

		const float a = XMVectorGetX(XMVector3Dot(u, u));
		const float b = XMVectorGetX(XMVector3Dot(u, v));
		const float c = XMVectorGetX(XMVector3Dot(v, v));
		const float d = XMVectorGetX(XMVector3Dot(u, w));
		const float e = XMVectorGetX(XMVector3Dot(v, w));

		const float D = a * c - b * b;
		float s, t;

		if (D > 0.00001f)
		{
			s = (b * e - c * d) / D;
			t = (a * e - b * d) / D;

			if (s < 0.0f) {
				s = 0.0f;
				t = std::clamp<float>(e / c, 0.0f, 1.0f);
			}

			else if (s > 1.0f) {
				s = 1.0f;
				t = std::clamp<float>((e + b) / c, 0.0f, 1.0f);
			}

			if (t < 0.0f) {
				t = 0.0f;
				s = std::clamp<float>(-d / a, 0.0f, 1.0f);
			}

			else if (t > 1.0f) {
				t = 1.0f;
				s = std::clamp<float>((b - d) / a, 0.0f, 1.0f);
			}
		}
		else
		{
			auto PointToSegmentDistSq =
				[](XMVECTOR P, XMVECTOR A, XMVECTOR B) -> float
				{
					XMVECTOR AB = B - A;
					XMVECTOR AP = P - A;

					float abLenSq = XMVectorGetX(XMVector3Dot(AB, AB));
					if (abLenSq < 0.00001f)
					{
						XMVECTOR diff = P - A;
						return XMVectorGetX(XMVector3Dot(diff, diff));
					}

					float t = XMVectorGetX(XMVector3Dot(AP, AB)) / abLenSq;
					t = std::clamp<float>(t, 0.0f, 1.0f);

					XMVECTOR closest = A + AB * t;
					XMVECTOR diff = P - closest;
					return XMVectorGetX(XMVector3Dot(diff, diff));
				};

			const float d1 = PointToSegmentDistSq(c1A, c2A, c2B);
			const float d2 = PointToSegmentDistSq(c1B, c2A, c2B);
			const float d3 = PointToSegmentDistSq(c2A, c1A, c2B);
			const float d4 = PointToSegmentDistSq(c2B, c1A, c1B);

			float minDistSq = std::min({ d1, d2, d3, d4 });
			float R = c1.radius + c2.radius;

			return minDistSq <= R * R;
		}

		// TODO : 근접점(Pc, Qc)으로 충돌 지점 구하기

		XMVECTOR Pc = c1A + u * s;
		XMVECTOR Qc = c2A + v * t;

		XMVECTOR diff = Pc - Qc;
		const float distSq = XMVectorGetX(XMVector3Dot(diff, diff));

		float R = c1.radius + c2.radius;
		return distSq <= R * R;
	}

}