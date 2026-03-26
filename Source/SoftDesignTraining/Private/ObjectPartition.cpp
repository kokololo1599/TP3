// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPartition.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include "ReactionManager.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "AIBase.h"

ObjectPartition* ObjectPartition::m_Instance;

ObjectPartition::ObjectPartition()
{
}


ObjectPartition* ObjectPartition::GetInstance()
{
    if (!m_Instance)
    {
        m_Instance = new ObjectPartition();
    }
    return m_Instance;
}

void ObjectPartition::Destroy()
{
    delete m_Instance;
    m_Instance = nullptr;
}

void ObjectPartition::RegisterObject(AActor* object)
{
    m_ObjectList.Add(object);
}

void ObjectPartition::UnregisterObject(AActor* object)
{
    m_ObjectList.Remove(object);
}

void ObjectPartition::Tick()
{
	m_Buckets.Clear();
	m_Buckets.Build(m_ObjectList);
}

void ObjectPartition::GatherObjectsAroundPosition(AActor* ignoreActor, FVector targetPosition, float radius, TArray<AActor*>& objects)
{
	static bool fast = true;
	if (fast)
	    FastGatherObjectsAroundPosition(ignoreActor, targetPosition, radius, objects);
	else
		SlowGatherObjectsAroundPosition(ignoreActor, targetPosition, radius, objects);
}


void ObjectPartition::FastGatherObjectsAroundPosition(AActor* ignoreActor, FVector targetPosition, float radius, TArray<AActor*>& objects)
{
	m_Buckets.Gather(ignoreActor, targetPosition, radius, objects);
}

void ObjectPartition::SlowGatherObjectsAroundPosition(AActor* ignoreActor, FVector targetPosition, float radius, TArray<AActor*>& objects)
{
	float radiusSQ = radius * radius;
    for (AActor* object: m_ObjectList)
    {
		if (object && object != ignoreActor)
        {
            FVector objectPosition = object->GetActorLocation();
            FVector diffToObject = objectPosition - targetPosition;
            float distSQObject = diffToObject.SizeSquared();

            if (distSQObject < radiusSQ)
            {
                objects.Add(object);
            }
        }
    }
}

void ObjectPartitionBuckets::GetUVForPosition(FVector pos, long& u, long& v)
{
	const float positiveOffset = 1000000.f;

	// todo optimize the shit out of this
	float x = pos.X + positiveOffset;
	float y = pos.Y + positiveOffset;

	x = fmod(x / m_BucketSize, (float)NUM_2D_BUCKETS);
	y = fmod(y / m_BucketSize, (float)NUM_2D_BUCKETS);
	u = (long)x;
	v = (long)y;
}

void ObjectPartitionBuckets::Build(TArray<AActor*>& objectList)
{
	for (AActor* object : objectList)
	{
		if (object)
		{
			FVector objPos = object->GetActorLocation();
			long u, v;
			GetUVForPosition(objPos, u, v);

			long index = v * NUM_2D_BUCKETS + u;
			long objectCount = m_ObjectCount[index];
			if (objectCount < NUM_OBJECTS_PER_BUCKETS)
			{
				m_ObjectCount[index] ++;
				m_Buckets[index][objectCount] = object;
			}
		}
	}
}

void ObjectPartitionBuckets::Gather(AActor* ignoreActor, FVector targetPosition, float radius, TArray<AActor*>& objects)
{
	// Radius is in meters, so convert in pixels
	FVector Position = targetPosition;

	float squaredRadius = radius*radius;
	float width = NUM_2D_BUCKETS * m_BucketSize;
	const long scanradius = 1 + (long)(radius / m_BucketSize);

	long u, v;
	GetUVForPosition(Position, u, v);

	long minu = u - scanradius;
	long maxu = u + scanradius;
	long minv = v - scanradius;
	long maxv = v + scanradius;

	long diameter = scanradius * 2;
	if (diameter >= NUM_2D_BUCKETS)
	{
		minu = 0;
		maxu = (long)(NUM_2D_BUCKETS - 1);
	}
	if (diameter >= NUM_2D_BUCKETS)
	{
		minv = 0;
		maxv = (long)(NUM_2D_BUCKETS - 1);
	}

	for (long vv = minv; vv <= maxv; ++vv)
	{
		const long vvoffset = (vv % NUM_2D_BUCKETS) * NUM_2D_BUCKETS;

		for (long uu = minu; uu <= maxu; ++uu)
		{
			const long elemIndex = (uu % NUM_2D_BUCKETS) + vvoffset;
			const long bucketCount = m_ObjectCount[elemIndex];
			for (long cnt = 0; cnt < bucketCount; ++cnt)
			{
				AActor* object = m_Buckets[elemIndex][cnt];
				FVector oPos = object->GetActorLocation();
				float sqdist = (oPos - targetPosition).SizeSquared();
				if (sqdist <= squaredRadius && object != ignoreActor)
				{
					objects.Add(object);
				}
			}
		}
	}
}