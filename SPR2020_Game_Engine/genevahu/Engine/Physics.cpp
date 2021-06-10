#include "Physics.h"
#include "stdafx.h"
#include "CollisionComponent.h"

Physics::Physics()
{
}

Physics::~Physics()
{
}

bool Physics::Intersect(const AABB& a, const AABB& b, AABB* pOverlap)
{
	/* AABB intersection detection -- if ObjOne's maximum value is 
        still less than even ObjTwo's minimum value, then they are 
        destined to not touch.

        Depending on initial intersection results, function will then
        compare the passed-in overlap results with a calculated overlap
        result, which should match each other. The result is returned 
        true for a match, false for a mismatch.
    */

    // Check for intersection.
    if ((a.m_MaximumCorner.x < b.m_MinimumCorner.x) ||
    (a.m_MaximumCorner.y < b.m_MinimumCorner.y) ||
    (a.m_MaximumCorner.z < b.m_MinimumCorner.z) ||
    (b.m_MaximumCorner.x < a.m_MinimumCorner.x) ||
    (b.m_MaximumCorner.y < a.m_MinimumCorner.y) ||
    (b.m_MaximumCorner.z < a.m_MinimumCorner.z)) {

        // No intersection detected.
        if (pOverlap != nullptr) {
            // Set overlap values.
            pOverlap->m_MinimumCorner.x = 1.0f;
            pOverlap->m_MinimumCorner.y = 1.0f;
            pOverlap->m_MinimumCorner.z = 1.0f;
            pOverlap->m_MaximumCorner.x = 0.0f;
            pOverlap->m_MaximumCorner.y = 0.0f;
            pOverlap->m_MaximumCorner.z = 0.0f;
        }

        return false;
    }

    // Intersection found.
    if (pOverlap != nullptr) {
        // Set overlap values.
        pOverlap->m_MinimumCorner.x = max(a.m_MinimumCorner.x, b.m_MinimumCorner.x);
        pOverlap->m_MaximumCorner.x = min(a.m_MaximumCorner.x, b.m_MaximumCorner.x);
        pOverlap->m_MinimumCorner.y = max(a.m_MinimumCorner.y, b.m_MinimumCorner.y);
        pOverlap->m_MaximumCorner.y = min(a.m_MaximumCorner.y, b.m_MaximumCorner.y);
        pOverlap->m_MinimumCorner.z = max(a.m_MinimumCorner.z, b.m_MinimumCorner.z);
        pOverlap->m_MaximumCorner.z = min(a.m_MaximumCorner.z, b.m_MaximumCorner.z);
    }

    return true;         
}

bool Physics::Intersect(const LineSegment& segment, const AABB& box, Vector3* pHitPoint)
{
    float tmin = 0;
    float tmax = FLT_MAX;
    Vector3 direction = segment.m_EndingPoint - segment.m_StartingPoint;
    Vector3 q;
    float d = 0.0f;
    float p = 0.0f;
    float a_min = 0.0f;
    float a_max = 0.0f;

    for (int i = 0; i < 3; i++) {
        if (i == 0) {
            d = direction.x;
            p = segment.m_StartingPoint.x;
            a_min = box.m_MinimumCorner.x;
            a_max = box.m_MaximumCorner.x;
        }
        else if (i == 1) {
            d = direction.y;
            p = segment.m_StartingPoint.y;
            a_min = box.m_MinimumCorner.y;
            a_max = box.m_MaximumCorner.y;
        }
        else {
            d = direction.z;
            p = segment.m_StartingPoint.z;
            a_min = box.m_MinimumCorner.z;
            a_max = box.m_MaximumCorner.z;
        }
        
        if (abs(d) < FLT_EPSILON) {
            // Ray is parallel to the slab. No hit if origin is not within the slab.
            if (p < a_min || p > a_max) {
                return false;
            }
        }
        else {
            float ood = 1.0f / d;
            float t1 = (a_min - p) * ood;
            float t2 = (a_max - p) * ood;

            // Make t1 be intersection w/ near plane, t2 w/ far plane.
            if (t1 > t2) {
                // Swap values.
                float temp = t1;
                t1 = t2;
                t2 = temp;
            }

            // Compute the intersection of slab intersection intervals.
            tmin = max(tmin, t1);
            tmax = min(tmax, t2);
                
            // Exit with no collision as soon as slab intersection becomes empty
            if (tmin > tmax) {
                return false;
            }
        }
    }

    // Ray intersects all 3 slabs. Check with segment.
    q = segment.m_StartingPoint + (direction * tmin);
    float segMinX, segMaxX, segMinY, segMaxY, segMinZ, segMaxZ;
    segMinX = min(segment.m_StartingPoint.x, segment.m_EndingPoint.x);
    segMaxX = max(segment.m_StartingPoint.x, segment.m_EndingPoint.x);
    segMinY = min(segment.m_StartingPoint.y, segment.m_EndingPoint.y);
    segMaxY = max(segment.m_StartingPoint.y, segment.m_EndingPoint.y);
    segMinZ = min(segment.m_StartingPoint.z, segment.m_EndingPoint.z);
    segMaxZ = max(segment.m_StartingPoint.z, segment.m_EndingPoint.z);

    if (q.x < segMinX || q.x > segMaxX || q.y < segMinY || q.y > segMaxY || q.z < segMinZ || q.z > segMaxZ) {
        return false;
    }
    
    // Return intersection / check with unit test.
    if(pHitPoint != nullptr) {
        pHitPoint->x = q.x;
        pHitPoint->y = q.y;
        pHitPoint->z = q.z;
    }

    return true;
}

bool Physics::UnitTest()
{
    // Return value for the entire unit test ---------------------------------------
    bool isOK = true;

    // AABB vs AABB overlap test ---------------------------------------------------
    struct TestAABB
    {
        AABB a;
        AABB b;
        AABB overlap;
    };
    TestAABB testAABB[] =
    {
        {
        AABB(Vector3(0.0f, 0.0f, 0.0f), Vector3(100.0f, 100.0f, 100.0f)),
        AABB(Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f)),
        AABB(Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f))
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        AABB(Vector3(-110.0f, -10.0f, -10.0f), Vector3(-90.0f, 10.0f, 10.0f)),
        AABB(Vector3(-100.0f, -10.0f, -10.0f), Vector3(-90.0f, 10.0f, 10.0f))
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        AABB(Vector3(90.0f, -10.0f, -10.0f), Vector3(110.0f, 10.0f, 10.0f)),
        AABB(Vector3(90.0f, -10.0f, -10.0f), Vector3(100.0f, 10.0f, 10.0f))
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        AABB(Vector3(-10.0f, -110.0f, -10.0f), Vector3(10.0f, -90.0f, 10.0f)),
        AABB(Vector3(-10.0f, -100.0f, -10.0f), Vector3(10.0f, -90.0f, 10.0f))
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        AABB(Vector3(-10.0f, 90.0f, -10.0f), Vector3(10.0f, 110.0f, 10.0f)),
        AABB(Vector3(-10.0f, 90.0f, -10.0f), Vector3(10.0f, 100.0f, 10.0f))
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        AABB(Vector3(-10.0f, -10.0f, -110.0f), Vector3(10.0f, 10.0f, -90.0f)),
        AABB(Vector3(-10.0f, -10.0f, -100.0f), Vector3(10.0f, 10.0f, -90.0f))
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        AABB(Vector3(-10.0f, -10.0f, 90.0f), Vector3(10.0f, 10.0f, 110.0f)),
        AABB(Vector3(-10.0f, -10.0f, 90.0f), Vector3(10.0f, 10.0f, 100.0f))
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        AABB(Vector3(-120.0f, -10.0f, -10.0f), Vector3(-110.0f, 10.0f, 10.0f)),
        AABB(Vector3::One, Vector3::Zero)
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        AABB(Vector3(110.0f, -10.0f, -10.0f), Vector3(120.0f, 10.0f, 10.0f)),
        AABB(Vector3::One, Vector3::Zero)
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        AABB(Vector3(-10.0f, -120.0f, -10.0f), Vector3(10.0f, -110.0f, 10.0f)),
        AABB(Vector3::One, Vector3::Zero)
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        AABB(Vector3(-10.0f, 110.0f, -10.0f), Vector3(10.0f, 120.0f, 10.0f)),
        AABB(Vector3::One, Vector3::Zero)
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        AABB(Vector3(-10.0f, -10.0f, -120.0f), Vector3(10.0f, -10.0f, -110.0f)),
        AABB(Vector3::One, Vector3::Zero)
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        AABB(Vector3(-10.0f, -10.0f, 110.0f), Vector3(10.0f, 10.0f, 120.0f)),
        AABB(Vector3::One, Vector3::Zero)
        },
    };

    AABB dummy = AABB(Vector3::Zero, Vector3::Zero);
    for (int i = 0; i < 13; i++) {
        isOK &= Intersect(testAABB[i].a, testAABB[i].b, &dummy);
        isOK &= (dummy.m_MinimumCorner.x == testAABB[i].overlap.m_MinimumCorner.x) &&
            (dummy.m_MinimumCorner.y == testAABB[i].overlap.m_MinimumCorner.y) &&
            (dummy.m_MinimumCorner.z == testAABB[i].overlap.m_MinimumCorner.z) &&
            (dummy.m_MaximumCorner.x == testAABB[i].overlap.m_MaximumCorner.x) &&
            (dummy.m_MaximumCorner.y == testAABB[i].overlap.m_MaximumCorner.y) &&
            (dummy.m_MaximumCorner.z == testAABB[i].overlap.m_MaximumCorner.z);
    }

    // Segment overlap test ---------------------------------------------------------
    struct TestSegment
    {
        AABB box;
        LineSegment segment;
        bool hit;
        Vector3 point;
    };
    TestSegment testSegment[] =
    {
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        LineSegment(Vector3(-110.0f, 0.0f, 0.0f), Vector3(-90.0f, 0.0f, 0.0f)),
        true, Vector3(-100.0f, 0.0f, 0.0f)
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        LineSegment(Vector3(0.0f, -110.0f, 0.0f), Vector3(0.0f, -90.0f, 0.0f)),
        true, Vector3(0.0f, -100.0f, 0.0f)
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        LineSegment(Vector3(0.0f, 0.0f, -110.0f), Vector3(0.0f, 0.0f, -90.0f)),
        true, Vector3(0.0f, 0.0f, -100.0f)
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        LineSegment(Vector3(110.0f, 0.0f, 0.0f), Vector3(90.0f, 0.0f, 0.0f)),
        true, Vector3(100.0f, 0.0f, 0.0f)
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        LineSegment(Vector3(0.0f, 110.0f, 0.0f), Vector3(0.0f, 90.0f, 0.0f)),
        true, Vector3(0.0f, 100.0f, 0.0f)
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        LineSegment(Vector3(0.0f, 0.0f, 110.0f), Vector3(0.0f, 0.0f, 90.0f)),
        true, Vector3(0.0f, 0.0f, 100.0f)
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        LineSegment(Vector3(-120.0f, 0.0f, 0.0f), Vector3(-110.0f, 0.0f, 0.0f)),
        false, Vector3::Zero
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        LineSegment(Vector3(0.0f, -120.0f, 0.0f), Vector3(0.0f, -110.0f, 0.0f)),
        false, Vector3::Zero
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        LineSegment(Vector3(0.0f, 0.0f, -120.0f), Vector3(0.0f, 0.0f, -110.0f)),
        false, Vector3::Zero
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        LineSegment(Vector3(120.0f, 0.0f, 0.0f), Vector3(110.0f, 0.0f, 0.0f)),
        false, Vector3::Zero
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        LineSegment(Vector3(0.0f, 120.0f, 0.0f), Vector3(0.0f, 110.0f, 0.0f)),
        false, Vector3::Zero
        },
        {
        AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
        LineSegment(Vector3(0.0f, 0.0f, 120.0f), Vector3(0.0f, 0.0f, 110.0f)),
        false, Vector3::Zero
        },
    };

    for (int i = 0; i < 12; i++) {
        isOK &= testSegment[i].hit == Intersect(testSegment[i].segment, testSegment[i].box, &(testSegment[i].point));
    }

    // --------------------------------------------------------------------------------
    return isOK;
}

void Physics::AddObj(CollisionComponent* pObj)
{
    m_CollComponents.push_back(pObj);
}

void Physics::RemoveObj(CollisionComponent* pObj)
{
    std::vector<CollisionComponent*>::iterator it = std::find(m_CollComponents.begin(), m_CollComponents.end(), pObj);
    if (it != m_CollComponents.end()) {
        m_CollComponents.erase(it);
    }
}

bool Physics::RayCast(const LineSegment& segment, Vector3* pHitPoint)
{
    float dist = FLT_MAX;
    Vector3 closestPoint = Vector3::Zero;

    // Search all AABB's for collision.
    for (int i = 0; i < m_CollComponents.size(); i++) {
        CollisionComponent* coll = m_CollComponents[i];
        Physics::AABB aabb = coll->GetAABB();
        if (Intersect(segment, aabb, pHitPoint)) {
            Vector3 d = segment.m_StartingPoint - *pHitPoint;
            if (d.LengthSq() < dist) {
                dist = d.LengthSq();
                closestPoint = *pHitPoint;
            }
        }
    }

    // Set the values to the point closest to the beginning of the segment.
    if (dist != FLT_MAX) {
        pHitPoint->x = closestPoint.x;
        pHitPoint->y = closestPoint.y;
        pHitPoint->z = closestPoint.z;
        return true;
    }

    return false;
}

Physics::AABB::AABB()
{
}

Physics::AABB::AABB(Vector3 min, Vector3 max)
{
    m_MinimumCorner = min;
    m_MaximumCorner = max;
}

Physics::AABB::~AABB()
{
}

Physics::LineSegment::LineSegment(Vector3 start, Vector3 end)
{
    m_StartingPoint = start;
    m_EndingPoint = end;
}

Physics::LineSegment::~LineSegment()
{
}
