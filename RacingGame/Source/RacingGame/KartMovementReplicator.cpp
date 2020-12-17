// Fill out your copyright notice in the Description page of Project Settings.


#include "KartMovementReplicator.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameStateBase.h"

// Sets default values for this component's properties
UKartMovementReplicator::UKartMovementReplicator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);
}


// Called when the game starts
void UKartMovementReplicator::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent = GetOwner()->FindComponentByClass<UKartMovementComponent>();

}


// Called every frame
void UKartMovementReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MovementComponent == nullptr) return;

	FKartMove Move = MovementComponent->GetLastMove();

	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{

		UnacknowledgedMoves.Add(Move);
		Server_SendMove(Move);
	}

	// We are the server and in control of the pawn.
	if (GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
	{
		UpdateServerState(Move);
	}

	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		MovementComponent->SimulateMove(ServerState.LastMove);
	}
}

void UKartMovementReplicator::UpdateServerState(const FKartMove& Move)
{
	ServerState.LastMove = Move;
	ServerState.Tranform = GetOwner()->GetActorTransform();
	ServerState.Velocity = MovementComponent->GetVelocity();
}

void UKartMovementReplicator::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UKartMovementReplicator, ServerState);
}

void UKartMovementReplicator::OnRep_ServerState()
{
	if (MovementComponent == nullptr) return;

	GetOwner()->SetActorTransform(ServerState.Tranform);
	MovementComponent->SetVelocity(ServerState.Velocity);

	ClearAcknowledgeMoves(ServerState.LastMove);

	for (const FKartMove& Move : UnacknowledgedMoves)
	{
		MovementComponent->SimulateMove(Move);
	}
}

void UKartMovementReplicator::ClearAcknowledgeMoves(FKartMove LastMove)
{
	TArray<FKartMove> NewMoves;

	for (const FKartMove& Move : UnacknowledgedMoves)
	{
		if (Move.Time > LastMove.Time)
		{
			NewMoves.Add(Move);
		}
	}

	UnacknowledgedMoves = NewMoves;
}

void UKartMovementReplicator::Server_SendMove_Implementation(FKartMove Move)
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SimulateMove(Move);

	UpdateServerState(Move);
}

bool UKartMovementReplicator::Server_SendMove_Validate(FKartMove Move)
{
	return true; //TODO: Make better validation
}