#include "Framework.h"

int main() 
{
	Framework::Get().Start();

	//try
	//{
	//	PrebakedAnimation anim =
	//		LoadPrebakedAnimation("C:/Users/Hadenpel/Knight_Walk_COLLIDER_PREBAKED.json");

	//	Transform tA;
	//	Transform tB;

	//	tA.position = XMFLOAT3(0, 0, 0);
	//	tB.position = XMFLOAT3(0, 0, 6.f);

	//	const float moveStep = 3;

	//	for (int frame = 0; frame < anim.numFrames; ++frame)
	//	{
	//		tA.position.z += moveStep;
	//		tB.position.z -= moveStep;

	//		XMMATRIX worldA = TransformHelper::ToMatrix(tA);
	//		XMMATRIX worldB = TransformHelper::ToMatrix(tB);

	//		for (size_t i = 0; i < anim.frames[frame].size(); ++i)
	//		{
	//			Capsule A = anim.frames[frame][i];
	//			Capsule B = anim.frames[frame][i];

	//			// transform.scale에서 이미 0.01 적용됨 → 절대 추가 곱 금지
	//			XMVECTOR p0A = XMVector3Transform(XMLoadFloat3(&A.p0), worldA);
	//			XMVECTOR p1A = XMVector3Transform(XMLoadFloat3(&A.p1), worldA);

	//			XMVECTOR p0B = XMVector3Transform(XMLoadFloat3(&B.p0), worldB);
	//			XMVECTOR p1B = XMVector3Transform(XMLoadFloat3(&B.p1), worldB);

	//			XMStoreFloat3(&A.p0, p0A);
	//			XMStoreFloat3(&A.p1, p1A);

	//			XMStoreFloat3(&B.p0, p0B);
	//			XMStoreFloat3(&B.p1, p1B);

	//			// radius도 단 한 번만 cm→m 변환
	//			A.radius *= 0.01;
	//			B.radius *= 0.01;

	//			if (CheckCapsuleVsCapsule(A, B))
	//				printf("Collision! Frame[%d] A(bone %zu) <-> B(bone %zu)\n",
	//					frame, i, i);
	//		}
	//	}
	//}
	//catch (const std::exception& e)
	//{
	//	std::cerr << "오류: " << e.what() << "\n";
	//}
}