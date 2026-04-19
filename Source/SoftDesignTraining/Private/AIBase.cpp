// Fill out your copyright notice in the Description page of Project Settings.

#include "AIBase.h"
#include "SoftDesignTraining/SoftDesignTraining.h"
#include "DrawDebugHelpers.h"
#include "ReactionManager.h"
#include "PatrolPathManager.h"
#include "PatrolPathComponent.h"
#include "ObjectPartition.h"
#include "SoftDesignTraining/SDTUtils.h"
#include "AI/SoftDesignAIController.h"
#include "AI/AiAgentGroupManager.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
AAIBase::AAIBase()
:m_IsBlind(false)
,m_currentBrainLogic(EAIBrainMode::IfElse)
,m_CurrentWayPoint(0)
,m_PatrolPath(nullptr)
,m_IsPlayerDetected(false)
,m_IsScared(false)
,m_ScaredTimeLeft(0.f)
,m_isInvestigatingExplosion(false)
,m_isRetreating(false)
,m_isInvestigatingLKP(false)
{
     // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    m_bombDropPos = FVector::ZeroVector;
    m_lastExplosionPos = FVector::ZeroVector;
    m_sightedBombPos = FVector::ZeroVector;
    m_retreatDestination = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void AAIBase::BeginPlay()
{
    Super::BeginPlay();

    ReactionManager* reactionManager = ReactionManager::GetInstance();
    if (reactionManager)
    {
        reactionManager->RegisterNPC(this);
    }

    PatrolPathManager* ppm = PatrolPathManager::GetInstance();
    if (ppm)
    {
        m_PatrolPath = ppm->GetRandomPath();
        m_CurrentWayPoint = rand() % m_PatrolPath->GetNWayPoints();
        m_PathDirection = 1;
    }

    GetCharacterMovement()->MaxWalkSpeed = (float)(rand() % 200 + 100);

    FVector actorLocation = GetActorLocation();
    m_NextDestination = actorLocation;
    m_CurrentDestination = actorLocation;


    if (ASoftDesignAIController* aiController = Cast<ASoftDesignAIController>(GetController()))
    {
        if (m_currentBrainLogic == EAIBrainMode::BehaviorTree)
        {
            aiController->StartBehaviorTree(this);
        }
        else
        {
            aiController->StopBehaviorTree(this);
        }
    }
}


void AAIBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    AiAgentGroupManager::GetInstance()->LeavePursuitGroup(this);

    ReactionManager* reactionManager = ReactionManager::GetInstance();
    if (reactionManager)
    {
        reactionManager->UnregisterNPC(this);
    }

    m_PatrolPath = nullptr;
}

// Called every frame
void AAIBase::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
   
    UpdateCurrentLogic(DeltaTime);    
}

void AAIBase::UpdateCurrentLogic(float deltaTime)
{
    FColor color = FColor::Black;

    switch (m_currentBrainLogic)
    {
    case EAIBrainMode::IfElse:
        UpdateIfElseLogic(deltaTime);
    	break;
    case EAIBrainMode::BehaviorTree:
        UpdateBTLogic(deltaTime);
        color = FColor::Cyan;
        break;
    case EAIBrainMode::FSM:
        UpdateFSMLogic(deltaTime);
        color = FColor::Red;
        break;
    }
    DrawDebugCircle(GetWorld(), GetActorLocation(), 100.f, 360, color);
}

void AAIBase::UpdateIfElseLogic(float DeltaTime)
{
    //Current elaspsed Gametime
    float currentElapsedTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());

    //Try player detection
    DetectPlayer();
    //Try bomb detection
    DetectBomb();

    AActor* targetPlayer = NULL;
    if (ReactionManager* reactionManager = ReactionManager::GetInstance())
    {
        targetPlayer = reactionManager->GetPlayer();
    }

    FVector actorLocation = GetActorLocation();

    //Action fait chaque update selon l'�tat courant de l'agent
    if (m_IsBlind)
    {
        if (m_BlindTimeLeft > 0)
        {
            m_BlindTimeLeft -= DeltaTime;
            SetNewMoveDestination(actorLocation);
            return;
        }
        else
        {
            m_BlindTimeLeft = 0.0f;
            m_IsBlind = false;

            GoToNextPatrol();
        }
    }
    else if (m_IsScared)
    {
        if (m_ScaredTimeLeft > 0)
        {
            if (m_scaredDestination == FVector::ZeroVector)
            {
                //Acquire Scared Destination
                ProcessScaredDestination(m_scaredDestination, m_ScaredTimeLeft);
            }

            m_ScaredTimeLeft -= DeltaTime;

            //Move To scared destination
            SetNewMoveDestination(m_scaredDestination);
            return;
        }
        else
        {
            m_ScaredTimeLeft = 0.0f;
            m_IsScared = false;

            GoToNextPatrol();
        }
    }
    else if (m_isRetreating)
    {
        //DrawDebugSphere(GetWorld(), m_retreatDestination, 30.0f, 32, FColor::Green);
        //DrawDebugSphere(GetWorld(), actorLocation + FVector(0.f, 0.f, 100.f), 15.0f, 32, FColor::Green);

    }
    else if (m_isInvestigatingExplosion)
    {
        //DrawDebugSphere(GetWorld(), m_lastExplosionPos, 30.0f, 32, FColor::Blue);
        //DrawDebugSphere(GetWorld(), actorLocation + FVector(0.f, 0.f, 100.f), 15.0f, 32, FColor::Blue);

        //Has the agent reached the investigation point or saw the a target in the process       
    }
    else if (m_isInvestigatingLKP)
    {
        TargetLKPInfo::ELKPState currentInvestigatedLKPState = TargetLKPInfo::ELKPState::LKPState_Invalid;
        
        FVector lkpPos = FVector::ZeroVector;

        if (lkpPos != FVector::ZeroVector)
        {
            SetNewMoveDestination(lkpPos);
        }

        //DrawDebugSphere(GetWorld(), lkpPos, 30.0f, 32, FColor::Purple);
        //DrawDebugSphere(GetWorld(), actorLocation + FVector(0.f,0.f,100.f), 15.0f, 32, FColor::Purple);


        //Has the agent reached the investigation point or saw the a target in the process or ...
        if ((actorLocation - lkpPos).Size2D() < 50.f)
        {
           
        }
        //else if ....
        
    }
    else if (m_IsPlayerDetected)
    {
        if (targetPlayer)
        {
            FVector npcPosition = GetActorLocation();
            FVector targetPosition = targetPlayer->GetActorLocation();
            FVector distToTarget = npcPosition - targetPosition;
            if (distToTarget.Size() < 250.0f)
            {
                SetNewMoveDestination(actorLocation);
            }
            else
            {
                SetNewMoveDestination(targetPosition);
            }
            DrawDebugSphere(GetWorld(), actorLocation + FVector(0.f, 0.f, 100.f), 15.0f, 32, FColor::Black);

            //Update LKP            

            m_CurrentDestination = FVector::ZeroVector;
            return;
        }
    }
    else if (m_CurrentDestination == FVector::ZeroVector)
    {
        //Player was lost can the agent investigate
        bool canAgentInvestigate = false;
        if (canAgentInvestigate)
        {
            //Set Lkp and investigation
        }
        else 
        {
            bool targetFoundByGroup = false;
            //Check if group knows where player might be  [Exer on LKP sharing week 11]
            if (targetFoundByGroup)
            {
               
            }
            else
            {
                // setup patrolling
                m_CurrentDestination = m_NextDestination;
                SetNewMoveDestination(m_NextDestination);
            }
        }

    }

    // check if we are close enough to our next destination
    if ((actorLocation - m_NextDestination).Size2D() < 50.f)
    {
        GoToNextPatrol();
    }

    ProcessAvoidance();
}

void AAIBase::UpdateBTLogic(float DeltaTime)
{
    AiAgentGroupManager::GetInstance()->DrawDebugForAgent(this);

    if (m_IsBlind)
    {
        if (m_BlindTimeLeft > 0)
        {
            m_BlindTimeLeft -= DeltaTime;
        }
        else
        {
            m_BlindTimeLeft = 0.0f;
            m_IsBlind = false;
        }
    }
    else if (m_IsScared)
    {
        if (m_ScaredTimeLeft > 0)
        {
            m_ScaredTimeLeft -= DeltaTime;
        }
        else
        {
            m_ScaredTimeLeft = 0.0f;
            m_IsScared = false;
        }
    }
}

void AAIBase::UpdateFSMLogic(float deltaTime)
{
}

// Called to bind functionality to input
void AAIBase::SetupPlayerInputComponent(class UInputComponent* inputComponent)
{
    Super::SetupPlayerInputComponent(inputComponent);
}

void AAIBase::SetNewMoveDestination(const FVector DestLocation)
{
    UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), DestLocation);
}

void AAIBase::ProcessReaction(ReactionEvent* reactionEvent)
{
    if (!reactionEvent)
        return;

    FVector npcPosition = GetActorLocation();
    FVector npcHead = npcPosition + FVector::UpVector * 200.0f;
    UWorld* npcWorld = GetWorld();

    //Blind
    if (reactionEvent->m_ReactionLOS == ReactionLOS_Visual && reactionEvent->m_ReactionType == ReactionType_Blind && !m_IsScared)
    {
        m_IsBlind = true;
        m_BlindTimeLeft = 3.0f;

        if (m_currentBrainLogic == EAIBrainMode::IfElse)
        {
                DrawDebugSphere(npcWorld, npcHead, 30.0f,
                    32,
                    FColor::Red,
                    false,
                    3.0f);
        }
    }
    //Bomb
    else if (reactionEvent->m_ReactionType == ReactionType_Explosion)
    {
      //sound or visual??
    }
    //Force Flee
    else if (reactionEvent->m_ReactionLOS == ReactionLOS_Sound && reactionEvent->m_ReactionType == ReactionType_ForceFlee)
    {
        if (!m_IsScared)
        {
            m_IsScared = true;
            m_scaredDestination = FVector::ZeroVector;
            m_ScaredTimeLeft = 3.0f;
            if (m_currentBrainLogic == EAIBrainMode::IfElse)
            {
                DrawDebugSphere(npcWorld, npcHead, 30.0f, 32, FColor::Orange, false, 3.0f);
            }
        }
    }
	//Need to persist the bomb position sinced it's not an entity
	else if (reactionEvent->m_ReactionType == ReactionType_PlacedBomb)
	{
		m_bombDropPos = reactionEvent->m_TargetPosition;
	}
}

void AAIBase::ProcessAvoidance()
{
    FVector npcPosition = GetActorLocation();
    FVector npcHead = npcPosition + FVector::UpVector * 200.0f;
    UWorld* npcWorld = GetWorld();

    ObjectPartition* op = ObjectPartition::GetInstance();
    if (op)
    {
        TArray<AActor*> actors;
        op->GatherObjectsAroundPosition(this, npcPosition, 250.f, actors);
    }
}

void AAIBase::ProcessNextPatrolDestination(FVector& outNextPatrolDestination)
{
    if (m_PatrolPath)
    {
        m_PatrolPath->GetNextWayPoint(m_CurrentWayPoint, m_NextDestination, m_PathDirection);
        m_CurrentDestination = m_NextDestination;
        outNextPatrolDestination = m_NextDestination;
    }
}

void AAIBase::ProcessScaredDestination(FVector& outScaredDestination, float timeAgentIsScared)
{
    FVector AgentToTargetDir = (GetActorLocation() - m_targetPlayerPos).GetSafeNormal();
    outScaredDestination = GetActorLocation() + AgentToTargetDir * GetCharacterMovement()->MaxWalkSpeed * m_ScaredTimeLeft;

    UWorld* npcWorld = GetWorld();
    DrawDebugSphere(npcWorld, outScaredDestination, 30.0f, 32, FColor::Orange, false, timeAgentIsScared);
}

void AAIBase::ProcessSightedBomb(const FVector& bombPos)
{
    //Agent needs to retreat in safe position
    //Make sure that we stop investigating

    FVector AgentToBombDir = (GetActorLocation() - bombPos).GetSafeNormal();
    m_retreatDestination = GetActorLocation() + AgentToBombDir * GetCharacterMovement()->MaxWalkSpeed * 2.f;
   
    DrawDebugSphere(GetWorld(), m_retreatDestination, 30.0f, 32, FColor::Green);
}

void AAIBase::ProcessHeardBomb(const FVector& bombPos)
{
    //Agents needs to go investigate the noise
    m_isInvestigatingExplosion = true;

    m_lastExplosionPos = bombPos;

    //Bomb as exploded remove it's pos
    m_bombDropPos = FVector::ZeroVector;

    //DrawDebugSphere(GetWorld(), m_lastExplosionPos, 30.0f, 32, FColor::Blue);
}

void AAIBase::GoToNextPatrol()
{
    //Make sure that the agent will be able to return in patrol
    if (m_PatrolPath)
    {
        m_PatrolPath->GetNextWayPoint(m_CurrentWayPoint, m_NextDestination, m_PathDirection);
        m_CurrentDestination = m_NextDestination;
        SetNewMoveDestination(m_NextDestination);
    }
}

void AAIBase::DetectPlayer()
{
    m_IsPlayerDetected = false;
    AActor* targetPlayer = NULL;
    
    ReactionManager* reactionManager = ReactionManager::GetInstance();
    if (reactionManager)
    {
        targetPlayer = reactionManager->GetPlayer();
    }

    if (targetPlayer)
    {
        bool canSeePlayer = false;
        FVector npcPosition = GetActorLocation();
        m_targetPlayerPos = targetPlayer->GetActorLocation();
        FVector distToTarget = npcPosition - m_targetPlayerPos;
        if (distToTarget.Size() < 15000.0f)
        {
            FVector npcHead = npcPosition + FVector::UpVector * 200.0f;
            FVector playerHead  = m_targetPlayerPos + FVector::UpVector * 200.0f;
            FVector playerTorso = m_targetPlayerPos + FVector::UpVector * 100.0f;
            FVector playerFeet  = m_targetPlayerPos + FVector::UpVector * 25.0f;

            UWorld* npcWorld = GetWorld();
            int bodyPartSeen = 0;

            if (!SDTUtils::Raycast(npcWorld, npcHead, playerHead))
            {
                //DrawDebugLine(npcWorld, npcHead, playerHead, FColor::Blue, false, 0.066f);
                ++bodyPartSeen;
            }
            if (!SDTUtils::Raycast(npcWorld, npcHead, playerTorso))
            {
                //DrawDebugLine(npcWorld, npcHead, playerTorso, FColor::Blue, false, 0.066f);
                ++bodyPartSeen;
            }
            if (!SDTUtils::Raycast(npcWorld, npcHead, playerFeet))
            {
                //DrawDebugLine(npcWorld, npcHead, playerFeet, FColor::Blue, false, 0.066f);
                ++bodyPartSeen;
            }

            if (bodyPartSeen > 1)
            {
                m_IsPlayerDetected = true;
                m_isInvestigatingLKP = false;
            }
        }
    }
}

void AAIBase::DetectBomb()
{
    //hanlde cases that the agent see's the bomb a while after it was dropped
    if (m_bombDropPos != FVector::ZeroVector && !m_isRetreating)
    {
        bool canSeeBomb = false;
        FVector npcPosition = GetActorLocation();
        FVector distToBomb = npcPosition - m_bombDropPos;
        if (distToBomb.Size() < 15000.0f)
        {
            FVector npcHead = npcPosition + FVector::UpVector * 200.0f;

            UWorld* npcWorld = GetWorld();
            int bodyPartSeen = 0;

            if (!SDTUtils::Raycast(npcWorld, npcHead, m_bombDropPos))
            {
                ProcessSightedBomb(m_bombDropPos);
            }
        }
    }
}
