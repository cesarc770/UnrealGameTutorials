// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KartMovementComponent.h"
#include "KartMovementReplicator.generated.h"

USTRUCT()
struct FKartState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FTransform Tranform;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FKartMove LastMove;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACINGGAME_API UKartMovementReplicator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKartMovementReplicator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void ClearAcknowledgeMoves(FKartMove LastMove);
	void UpdateServerState(const FKartMove& Move);
	void ClientTick(float DeltaTime);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FKartMove Move);

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FKartState ServerState;

	UFUNCTION()
	void OnRep_ServerState();


	void AutonomousProxy_OnRep_ServerState();
	void SimulatedProxy_OnRep_ServerState();

	TArray<FKartMove> UnacknowledgedMoves;

	float ClientTimeSinceUpdate;
	float ClientTimeBetweenLastUpdate;
	FVector ClientStartLocation;


	UPROPERTY()
	UKartMovementComponent* MovementComponent;

		
};
