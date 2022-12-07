// 2022 Tygo Boons

#include "mypch.h"
#include "PhysicsEnviroment.h"

namespace Physics
{
	void PhysicsEnviroment::RegisterComponent(const RefPtr<Entity::BoxCollider> collider)
	{
		// Add collider to list.
		m_ColliderComponents.push_back(collider);
	}

	void PhysicsEnviroment::RemoveComponent(const RefPtr<Entity::BoxCollider> collider)
	{
		// Loop through all colliders and find the one that has to be removed.
		for (size_t i = 0; i < m_ColliderComponents.size(); i++)
		{
			if (collider == m_ColliderComponents[i])
			{
				// Erase collider from array.
				m_ColliderComponents.erase(m_ColliderComponents.begin() + i);
			}
		}
	}

	void PhysicsEnviroment::RegisterComponent(const RefPtr<Entity::PhysicsBody> pBody)
	{
		// Add physicsbody to list.
		m_PhysicsBodyComponents.push_back(pBody);
	}

	void PhysicsEnviroment::RemoveComponent(const RefPtr<Entity::PhysicsBody> pBody)
	{
		// Loop through all physicsbodies and find the one that has to be removed.
		for (size_t i = 0; i < m_PhysicsBodyComponents.size(); i++)
		{
			if (pBody == m_PhysicsBodyComponents[i])
			{
				// Erase physicsbody from array.
				m_PhysicsBodyComponents.erase(m_PhysicsBodyComponents.begin() + i);
			}
		}
	}

	void PhysicsEnviroment::Update()
	{	
		// Loop thorugh all physics bodies.
		for (size_t i = 0; i < m_PhysicsBodyComponents.size(); i++)
		{

			// Get collider of the current pBody.
			RefPtr<Entity::BoxCollider> currentPBodyCollider = m_PhysicsBodyComponents[i]->Parent->GetComponent<Entity::BoxCollider>();

			// Continue if the pbody does not have a collider.
			if (!currentPBodyCollider)
				continue;

			if (AnyIntersections(currentPBodyCollider))
			{
				currentPBodyCollider->ResetToLastValidPosition();
			}

			currentPBodyCollider->UpdateLastValidPosition();
		}
	}

	bool PhysicsEnviroment::AnyIntersections(const RefPtr<Entity::BoxCollider>& collider)
	{
		collider->Update();

		// See if collider intersects this one.
		bool intersects = false;

		// See if there is a collision with any of the colliders.
		for (auto& col : m_ColliderComponents)
		{
			// If colliding and object is not this collider.
			if (collider->Intersects(col) && col != collider)
			{
				intersects = true;
			}
		}

		return intersects;
	}
}