// 2022 Tygo Boons

// Intersection normal calculation Inspired from:
// https://gamedev.stackexchange.com/questions/48816/how-to-calculate-collision-normal-between-two-axisalignedboxs 
// Old method did not work properly.

#pragma once

#include "glm/glm.hpp"
#include "Core/Core.h"

// Forward declaration for the GameObject class to prevent linking errors.
namespace Entity
{
	class GameObject;
}

namespace Physics
{

	/*
				 -----------	|	
		       /|          /|	|	      y+	
		      / |         / |	|		  | z-
		     /  |        /  |	|     	  |/ 
		     ---A-------B   |	|  x- ----o---- x+ 
		    |  /        |  /	|	     /|
		    | /         | /		|	    z+| 
		    |/          |/		|	      y-
		     -----------		o--------------------
	  
			  AABB Collision box point reference &
			  Transformation axis reference.
			  
			  Points[0] is A.
			  Points[1] is B.
	*/

	struct Collision
	{
		glm::vec3 Normal = { 0, 0, 0 };
		Entity::GameObject* ThisObject = nullptr;
		Entity::GameObject* HitObject = nullptr;
	};

	class AABB
	{
	public:
		AABB();

		// Set size of bounding box.
		void SetSize(const glm::vec3& size);

		// Set position of bounding box.
		void SetPosition(const glm::vec3& position);

		// Check if point is inside of AABB.
		bool IsPointInsideBounds(const glm::vec3& point) const;

		// Check if AABB is inside of AABB.
		RefPtr<Collision> Intersects(const AABB& aabb);

		// Returns if the AABB is inverted.
		bool IsInverted() const;

		// Recalculate bounding box points.
		void RecalculatePoints();

		// Get the center of this AABB.
		glm::vec3 Center() const { return m_Points[0] + m_Points[1] - m_Points[0]; }

		// Gets the points of the aabb
		std::array<glm::vec3, 2> GetPoints() const { return m_Points; }
	public:
		// Overload getter operator to get AABB points.
		const glm::vec3& operator [](int i) const
		{
			ASSERT((i < 2), "Only 2 points on Bounding box")
			return m_Points[i];
		}

	private:
		std::array<glm::vec3, 2> m_Points;
		glm::vec3 m_Position, m_Size;
	};
}
