// 2022 Tygo Boons

#pragma once

#include "Core/Core.h"
#include "Entity/Components.h"
#include "Game/Gameplay/TankInput.h"

namespace Gameplay
{
	class Bullet : public Entity::ScriptComponent
	{
		IMPLEMENT_COMPONENT_IDENTIFIER(Bullet)

	public:
		virtual void Start() override;

		virtual void Update() override;

		virtual void OnCollisionEnter(RefPtr<Physics::Collision> collision) override;

	private:
		RefPtr<Entity::Transform> m_Transform;
		uint32_t m_Hits;
	};
}