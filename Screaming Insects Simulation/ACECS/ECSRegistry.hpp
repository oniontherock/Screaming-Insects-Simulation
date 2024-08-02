#ifndef __ECS_REGISTRY_H__
#define __ECS_REGISTRY_H__

#include "../Include/Common/NumberGenerator.hpp"
#include "../Include/Common/RandomPools.hpp"
#include "../Include/Simulation/TargetTypes.hpp"
#include "ECS.hpp"
#include "SFML/Graphics.hpp"
#include <functional>

namespace ECSRegistry {
	void ECSInitialize();
	void ECSTerminate();
}

// avoid having undefined constructor arguments for events or components,
// as it's more convenient to not have to define every event/component all the time.
// whenever you create a new type, ensure you register it in the implementation file of this header

namespace EntityEvents {
	struct EventScream final : public Event {
		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventScream(axisToScream, info));
		};

		EventScream() {};
		EventScream(sf::Vector2f _axisToScream, TargetTypeStepPair _info) :
			axisToScream(_axisToScream), info(_info)
		{};

		// the axis the scream was from
		sf::Vector2f axisToScream{};

		// information about the TargetType of the scream and the amount of steps the scream said
		TargetTypeStepPair info;

		void clear() final {
			axisToScream = sf::Vector2f(0, 0);
			info = TargetTypeStepPair(TargetType::TypesCount, UINT32_MAX);
		}
	};
	struct EventMove final : public Event {
		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventMove(moveX, moveY));
		};

		EventMove() {};
		EventMove(float _moveX, float _moveY) :
			moveX(_moveX),
			moveY(_moveY)
		{};

		float moveX = 0.f;
		float moveY = 0.f;

		void clear() final {
			moveX = 0.f;
			moveY = 0.f;
		}
	};
	struct EventRotate final : public Event {
		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventRotate(angle));
		};

		EventRotate() {};
		EventRotate(float _angle) :
			angle(_angle)
		{};

		float angle = 0.f;

		void clear() final {
			angle = 0.f;
		}
	};
	struct EventTargetReached final : public Event {
		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventTargetReached(type));
		};

		EventTargetReached() {};
		EventTargetReached(TargetType _type) :
			type(_type)
		{};

		TargetType type = TargetType::Food;

		void clear() final {
			type = TargetType::Food;
		}
	};
}
namespace EntityComponents {
	struct ComponentVariableRandomizer final : public Component {

		void system(Entity& entity) final;

		ComponentVariableRandomizer() {
			hasSystem = true;
		};
		ComponentVariableRandomizer(std::function<void(Entity& entity)> _randomizeFunction) :
			ComponentVariableRandomizer()
		{
			randomizeFunction = _randomizeFunction;
		};

		std::function<void(Entity& entity)> randomizeFunction;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentVariableRandomizer(randomizeFunction));
		};
	};
	struct ComponentMoveByRotation final : public Component {

		void system(Entity& entity) final;

		ComponentMoveByRotation() {
			hasSystem = true;
		};
		ComponentMoveByRotation(float _moveSpeed) :
			ComponentMoveByRotation()
		{
			moveSpeed = _moveSpeed;
		}

		float moveSpeed = 1.f;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentMoveByRotation(moveSpeed));
		};
	};
	struct ComponentBoundReflection final : public Component {

		void system(Entity& entity) final;

		ComponentBoundReflection() {
			hasSystem = true;
		};

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentBoundReflection());
		};
	};
	struct ComponentPosition final : public Component {

		void system(Entity& entity) final;

		ComponentPosition() {
			hasSystem = true;
		};
		ComponentPosition(float _x, float _y) :
			ComponentPosition()
		{
			x = _x;
			y = _y;
		};
		ComponentPosition(sf::Vector2f _position) :
			ComponentPosition(_position.x, _position.y)
		{}

		float x = 0;
		float y = 0;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentPosition(x, y));
		};
	};
	struct ComponentRotation final : public Component {

		void system(Entity& entity) final;

		ComponentRotation() {
			hasSystem = true;
		};
		ComponentRotation(float _rotation) :
			ComponentRotation()
		{
			rotation = _rotation;
		};

		float rotation = 0.f;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentRotation(rotation));
		};
	};
	struct ComponentTargetStepTracker final : public Component {

		void system(Entity&) final;

		ComponentTargetStepTracker() {
			hasSystem = true;
		};
		ComponentTargetStepTracker(TargetStepsVector _targetStepsVector) :
			ComponentTargetStepTracker()
		{
			targetStepsVector = _targetStepsVector;
		}

		TargetStepsVector targetStepsVector = TargetStepsVector(TargetType::TypesCount);

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentTargetStepTracker(targetStepsVector));
		}
	};
	// holds the entity's current target type
	struct ComponentTargetHolder final : public Component {

		ComponentTargetHolder() {
			hasSystem = false;
		};
		ComponentTargetHolder(TargetType _targetType) {
			hasSystem = false;
			targetType = _targetType;
		};

		TargetType targetType = TargetType::Food;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentTargetHolder(targetType));
		}
	};
	struct ComponentScream final : public Component {

		void system(Entity& entity) final;

		ComponentScream() {
			hasSystem = true;
		};

		// the type the entity will scream out this update,
		// this will then be set to the next type after screaming
		TargetType screamTypeCur = (RNGf::probability(0.5f) ? TargetType::Home : TargetType::Food);

		// max distance a scream can be heard at
		static constexpr float MAX_SCREAM_DIST = 16.f;

		DUPLICATE_OVERRIDE(ComponentScream)
	};
	struct ComponentHearing final : public Component {

		void system(Entity& entity) final;

		ComponentHearing() {
			hasSystem = true;
		};

		float freedom_coefficient = RNGf::getRange(0.005f, 0.05f);

		DUPLICATE_OVERRIDE(ComponentHearing)
	};
	// populates the area around an entity with the given type and radius
	struct ComponentObjectGridCellPopulator final : public Component {

		void system(Entity& entity) final;

		ComponentObjectGridCellPopulator() {
			hasSystem = true;
		};
		ComponentObjectGridCellPopulator(float _popRadius, TargetType _popType) :
			ComponentObjectGridCellPopulator()
		{
			popRadius = _popRadius;
			popType = _popType;
		}

		// radius to around the entity to populate
		float popRadius = 0;
		// type to populate the cells with
		TargetType popType = TargetType::Home;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentObjectGridCellPopulator(popRadius, popType));
		};
	};
	// depopulates the area around an entity with the given radius
	struct ComponentObjectGridCellDepopulator final : public Component {

		void system(Entity& entity) final;

		ComponentObjectGridCellDepopulator() {
			hasSystem = true;
		};
		ComponentObjectGridCellDepopulator(float _popRadius) :
			ComponentObjectGridCellDepopulator()
		{
			popRadius = _popRadius;
		}

		// radius to around the entity to populate
		float popRadius = 0;
		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentObjectGridCellDepopulator(popRadius));
		};
	};
	// checks if the entity is on a cell containing a target, and if so, sends an EventTargetReached
	struct ComponentTargetCollisionChecker final : public Component {

		void system(Entity& entity) final;

		ComponentTargetCollisionChecker() {
			hasSystem = true;
		};

		DUPLICATE_OVERRIDE(ComponentTargetCollisionChecker)
	};
	struct ComponentStepsResetOnTargetReached final : public Component {

		void system(Entity& entity) final;

		ComponentStepsResetOnTargetReached() {
			hasSystem = true;
		};

		DUPLICATE_OVERRIDE(ComponentStepsResetOnTargetReached)
	};
	struct ComponentChangeTargetOnTargetReached final : public Component {

		void system(Entity& entity) final;

		ComponentChangeTargetOnTargetReached() {
			hasSystem = true;
		};

		DUPLICATE_OVERRIDE(ComponentChangeTargetOnTargetReached)
	};
	struct ComponentRotationRandomMovement final : public Component {

		void system(Entity& entity) final;

		ComponentRotationRandomMovement() {
			hasSystem = true;
		};

		uint16_t anglePoolCurInd = RNGu16::getRange(0, ANGLE_POOL_SIZE);

		DUPLICATE_OVERRIDE(ComponentRotationRandomMovement)
	};
}

#endif