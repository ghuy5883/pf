#pragma once
#include "engineMath.h"

class CollisionComponent;
class Physics
{
	public:
		// Constructor/destructor
		Physics();
		~Physics();

		// Internal classes
		class AABB {
			public:
				AABB();
				AABB(Vector3 min, Vector3 max);
				~AABB();
				Vector3 m_MinimumCorner;
				Vector3 m_MaximumCorner;
		};

		class LineSegment {
			public:
				LineSegment(Vector3 start, Vector3 end);
				~LineSegment();
				Vector3 m_StartingPoint;
				Vector3 m_EndingPoint;
		};

		// Functions
		static bool Intersect(const AABB& a, const AABB& b,
			AABB* pOverlap = nullptr);
		static bool Intersect(const LineSegment& segment, const AABB& box,
			Vector3* pHitPoint = nullptr);
		static bool UnitTest();
		void AddObj(CollisionComponent* pObj);
		void RemoveObj(CollisionComponent* pObj);
		bool RayCast(const LineSegment& segment, Vector3*
			pHitPoint = nullptr);

	private:
		std::vector<CollisionComponent*> m_CollComponents;
};

