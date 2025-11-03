#pragma once

#include <typeindex>

class System {
public:
	virtual ~System() = default;
	virtual void Update(const float dT) = 0;

	virtual std::vector<std::type_index> ReadComponents() const { return {}; }
	virtual std::vector<std::type_index> WriteComponents() const { return {}; }
};

// System 설계 원칙
//
// 1. 단일 책임 (SRP)
//  - 하나의 규칙만 처리할 것
//  - 의존 관계, 병렬화 구조를 명확히 나누기 위함
// 
// 2. 데이터 접근 기반 의존성 분리
//  - System간 의존 관계는 데이터 의존성으로 정의할 것
// 
// 3. Stateless 지향
//  - 웬만해선 상태를 가지지 말 것
//  - 예외도 있긴 함 (물리 처리 등)
// 
// 4. SoA(Structure of Arrays) 데이터 레이아웃 기반
//  - 한 System이 접근하는 데이터는 Contiguous Memory일 것
//  - Cache Hit율을 증가 -> 성능 향상을 위한 원칙
// 
// 5. 병렬화 단위는 System 내부 Batch로
// 6. IO / Network 처리 분리
//  - ECS의 System은 항상 순수 CPU 로직만 수행할 것

struct ECS;

class MovementSystem : public System {
public:
	explicit MovementSystem(ECS& ecs) : _ecs(ecs) {}
	virtual ~MovementSystem() = default;

	virtual void Update(const float) override;

	virtual std::vector<std::type_index> ReadComponents() const override;
	virtual std::vector<std::type_index> WriteComponents() const override;

private:
	ECS& _ecs;
};

class TestSystem : public System {
public: 
	virtual ~TestSystem() = default;

	virtual void Update(const float) override;
};