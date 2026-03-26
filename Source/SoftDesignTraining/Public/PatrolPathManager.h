// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UPatrolPathComponent;

/**
 * 
 */
class SOFTDESIGNTRAINING_API PatrolPathManager
{
private:
	static PatrolPathManager* m_Instance;

	TArray<UPatrolPathComponent*> m_PathList;

public:
	static PatrolPathManager* GetInstance();
	static void Destroy();

	void RegisterPath(UPatrolPathComponent* patrolPath);
	void UnregisterPath(UPatrolPathComponent* patrolPath);

	UPatrolPathComponent* GetRandomPath();

	PatrolPathManager();
	~PatrolPathManager();
};
