// Copyright Epic Games, Inc. All Rights Reserved.


#include "MultiplayerFPSGameModeBase.h"

#include "FPSCharacter.h"
#include "FPSGameState.h"
#include "FPSPlayerController.h"
#include "Kismet/GameplayStatics.h"

void AMultiplayerFPSGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	AfpsGameState = GetGameState<AFPSGameState>();
}

bool AMultiplayerFPSGameModeBase::ShouldSpawnAtStartSpot(AController* Player)
{
	return false;
}

void AMultiplayerFPSGameModeBase::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "The game has started!");
}

void AMultiplayerFPSGameModeBase::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "The game has ended!");
	TArray<AActor*> Characters;
	UGameplayStatics::GetAllActorsOfClass(this, AFPSCharacter::StaticClass(), Characters);
	for(AActor* Character : Characters)
	{
		Character->Destroy();
	}
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMultiplayerFPSGameModeBase::RestartMap, 5.0f);
}

void AMultiplayerFPSGameModeBase::RestartMap() const
{
	GetWorld()->ServerTravel(GetWorld()->GetName(), false, false);
}

bool AMultiplayerFPSGameModeBase::ReadyToEndMatch_Implementation()
{
	return  AfpsGameState && AfpsGameState->HasWinner();
}

bool AMultiplayerFPSGameModeBase::ReadyToStartMatch_Implementation()
{
	return true;
}

void AMultiplayerFPSGameModeBase::PlayerKilled_Implementation(AFPSCharacter* Victim, AFPSCharacter* Killer)
{
	// Add kill to killer
	if(AFPSPlayerState* CauserState = Cast<AFPSPlayerState>(Killer->GetPlayerState()))
	{
		CauserState->AddKill();
		AFPSPlayerController* KillerController = Cast<AFPSPlayerController>(Killer->GetController());
		if(KillerController)
		{
			KillerController->ShowVictim(Victim->GetHumanReadableName(), 1.0f);
		}
	}
		
	PlayerDied(Victim);
}

void AMultiplayerFPSGameModeBase::PlayerDied(AFPSCharacter* Victim)
{
	// Add death to myself
	if(AFPSPlayerState* MyState = Cast<AFPSPlayerState>(Victim->GetPlayerState()))
		MyState->AddDeath();

	AController* TempController = Victim->GetController();
	Victim->Destroy();
	this->RestartPlayer(TempController);
}
