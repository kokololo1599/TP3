// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class SOFTDESIGNTRAINING_API ObjectPartitionBuckets
{
private:
	static const unsigned long NUM_2D_BUCKETS = 128;
	static const unsigned long NUM_BUCKETS = NUM_2D_BUCKETS * NUM_2D_BUCKETS;
	static const unsigned long NUM_OBJECTS_PER_BUCKETS = 16;
	// partition buckets;
	AActor* m_Buckets[NUM_BUCKETS][NUM_OBJECTS_PER_BUCKETS];
	// buckets counts;
	unsigned long m_ObjectCount[NUM_BUCKETS];

	float  m_BucketSize;
public:
	ObjectPartitionBuckets() : m_BucketSize(200.f){}
	void Clear()
	{
		for (long i = 0; i < NUM_BUCKETS; i++)
		{
			m_ObjectCount[i] = 0;
		}
	}

	void Build(TArray<AActor*>& objectList);
	void Gather(AActor* ignoreActor, FVector targetPosition, float radius, TArray<AActor*>& object);

private:
	void GetUVForPosition(FVector pos, long& u, long& v);
};


class SOFTDESIGNTRAINING_API ObjectPartition
{
private:
    ObjectPartition();

    static ObjectPartition* m_Instance;

public:
	TArray<AActor*> m_ObjectList;

	ObjectPartitionBuckets m_Buckets;

public:
    static ObjectPartition* GetInstance();
	static void Destroy();

    void Tick();

	void RegisterObject(AActor* object);
	void UnregisterObject(AActor* object);

	void GatherObjectsAroundPosition(AActor* ignoreActor, FVector targetPosition, float radius, TArray<AActor*>& object);

private:
	void SlowGatherObjectsAroundPosition(AActor* ignoreActor, FVector targetPosition, float radius, TArray<AActor*>& object);
	void FastGatherObjectsAroundPosition(AActor* ignoreActor, FVector targetPosition, float radius, TArray<AActor*>& object);
};
