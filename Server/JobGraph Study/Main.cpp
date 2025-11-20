#include "Framework.h"

#include <DirectXMath.h>
#include <iostream>
#include <fstream>

#include "json.hpp"

using json = nlohmann::json;
using namespace DirectX;

struct CapsuleCollider {
	int bone{ 0 };
	XMFLOAT3 p0;
	XMFLOAT3 p1;
	float radius{ 0.0f };
};

struct PrebakedAnimation {
	float fps;
	int numFrames;
	std::vector<std::vector<CapsuleCollider>> frames;
};

PrebakedAnimation LoadPrebakedAnimation(std::string_view path)
{
	PrebakedAnimation anim;

	std::ifstream ifs(path.data());
	if (not ifs.is_open())
		throw std::runtime_error("파일을 열 수 없습니다: " + std::string(path));

	json j;
	ifs >> j;

	anim.fps = j["fps"].get<float>();
	anim.numFrames = j["numFrames"].get<int>();

	const auto& jFrames = j["frames"];
	anim.frames.resize(anim.numFrames);

	for (int frame = 0; frame < anim.numFrames; ++frame)
	{
		const auto& jFrame = jFrames[frame];
		anim.frames[frame].reserve(jFrame.size());

		for (const auto& item : jFrame)
		{
			CapsuleCollider capsule;
			capsule.bone = item["bone"].get<int>();

			auto p0 = item["p0"];
			capsule.p0 = XMFLOAT3(p0[0].get<float>(), p0[1].get<float>(), p0[2].get<float>());

			auto p1 = item["p1"];
			capsule.p1 = XMFLOAT3(p1[0].get<float>(), p1[1].get<float>(), p1[2].get<float>());

			capsule.radius = item["radius"].get<float>();

			anim.frames[frame].push_back(capsule);
		}
	}

	return anim;
}

int main() 
{
	//Framework::Get().Start();

	try
	{
		PrebakedAnimation anim =
			LoadPrebakedAnimation("C:/Users/Hadenpel/Knight_Walk_COLLIDER_PREBAKED.json");

		// 예시 출력
		for (int i = 0; i < 3; ++i)
		{
			const auto& col = anim.frames[0][i];
			std::cout << "Bone " << col.bone
				<< "  p0=(" << col.p0.x << "," << col.p0.y << "," << col.p0.z << ")"
				<< "  p1=(" << col.p1.x << "," << col.p1.y << "," << col.p1.z << ")"
				<< "  r=" << col.radius
				<< "\n";
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "오류: " << e.what() << "\n";
	}
}