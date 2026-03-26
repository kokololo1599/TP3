// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrolPathManager.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include <time.h>

PatrolPathManager* PatrolPathManager::m_Instance;

PatrolPathManager* PatrolPathManager::GetInstance()
{
	if (!m_Instance)
	{
        m_Instance = new PatrolPathManager();
	}

	return m_Instance;
}

PatrolPathManager::PatrolPathManager()
{
    srand(time(NULL));
}

PatrolPathManager::~PatrolPathManager()
{
}

void PatrolPathManager::RegisterPath(UPatrolPathComponent* patrolPath)
{
	m_PathList.Add(patrolPath);
}

void PatrolPathManager::UnregisterPath(UPatrolPathComponent* patrolPath)
{
	m_PathList.Remove(patrolPath);
}

UPatrolPathComponent* PatrolPathManager::GetRandomPath()
{
	if (m_PathList.Num() > 0)
	{
		return m_PathList[rand() % m_PathList.Num()];
		
	}
	return nullptr;
}

