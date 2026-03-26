// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "ReactionEvent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI/FSM/AI_FiniteStateMachine.h"
#include "AI/TargetLKPInfo.h"
#include "AIBase.generated.h"

class UPatrolPathComponent;

UENUM(BlueprintType)
enum class EAIBrainMode : uint8
{
    IfElse          UMETA(DisplayName = "IfElse_Logic"),
    BehaviorTree 	UMETA(DisplayName = "BT_Logic"),
    FSM      	    UMETA(DisplayName = "FSM_Logic")
};

UCLASS(Blueprintable)
class SOFTDESIGNTRAINING_API AAIBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	//virtual void EndPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual void SetNewMoveDestination(const FVector DestLocation);
	virtual void ProcessReaction( ReactionEvent* reactionEvent);
	virtual void ProcessAvoidance();

    virtual void DetectPlayer();
	UBehaviorTree*	GetBehaviorTree() const { return m_aiBehaviorTree; }

    FVector              GetTargetPlayerPos() const { return m_targetPlayerPos; }
    bool                 IsTargetPlayerSeen() const { return m_IsPlayerDetected; }
    bool                 IsAgentBlind() const { return m_IsBlind; }
    bool                 IsAgentScared() const { return m_IsScared; }
    void                 SetAgentIsBlind(bool isBlind) { m_IsBlind = isBlind; }
    void                 SetAgentIsScared(bool isScared) { m_IsScared = isScared; }
    void                 SetCurrentPatrolDestination(FVector currentPatrolDestination) { m_CurrentDestination = currentPatrolDestination; }
    FVector              GetCurrentPatrolDestination() const { return m_CurrentDestination; }
    FVector              GetNextPatrolDestination() const { return m_NextDestination;  }
    void                 ProcessNextPatrolDestination(FVector& outNextPatrolDestination);
    void                 ProcessScaredDestination(FVector& outScaredDestination, float timeAgentIsScared);
    const TargetLKPInfo& GetCurrentTargetLKPInfo() const { return m_currentTargetLkpInfo; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AiBase")
    bool m_IsBlind;

	UPROPERTY(EditAnywhere, category = Behavior)
	UBehaviorTree* m_aiBehaviorTree;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision_Exercises")
    EAIBrainMode          m_currentBrainLogic;

private:

    void            UpdateCurrentLogic(float deltaTime);
    void            UpdateIfElseLogic(float deltaTime);
    void            UpdateBTLogic(float deltaTime);
    void            UpdateFSMLogic(float deltaTime);

    void            ProcessSightedBomb(const FVector& bombPos);
    void            ProcessHeardBomb(const FVector& bombPos);
    void            DetectBomb();

    void            GoToNextPatrol();

    //Patrolling
	unsigned long m_CurrentWayPoint;
	long m_PathDirection;
	UPatrolPathComponent* m_PatrolPath;
	FVector m_NextDestination;
    FVector m_CurrentDestination;
    
    //Player detection
    FVector m_targetPlayerPos;
    bool m_IsPlayerDetected;

    //Fleeing
    FVector m_scaredDestination;
    bool m_IsScared;
    float m_ScaredTimeLeft;

    //Blind
    float m_BlindTimeLeft;

    //Bomb reaction
    FVector m_lastExplosionPos;
    FVector m_bombDropPos;
    FVector m_sightedBombPos;
    FVector m_retreatDestination;
    bool    m_isInvestigatingExplosion;
    bool    m_isRetreating;

    //LKP 
    TargetLKPInfo m_currentTargetLkpInfo;
    bool          m_isInvestigatingLKP;

    //FSM
    AI_FiniteStateMachine m_aiFSM;
};
