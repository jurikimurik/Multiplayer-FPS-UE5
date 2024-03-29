// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"

#include "MultiplayerFPSGameModeBase.h"

void AFPSGameState::BeginPlay()
{
	Super::BeginPlay();
	if(const auto MyGameMode = GetWorld()->GetAuthGameMode<AMultiplayerFPSGameModeBase>())
	{
		KillsLimit = MyGameMode->GetKillsNecessary();
	}
}

TArray<AFPSPlayerState*> AFPSGameState::GetPlayerStatesOrderedByKills() const
{
	TArray<AFPSPlayerState*> PlayerStates(PlayerArray);
	PlayerStates.Sort([](const AFPSPlayerState& A, const AFPSPlayerState& B)
	{
		return A.GetKills() > B.GetKills();
	});
	return PlayerStates;
}

bool AFPSGameState::HasWinner() const
{
	TArray<AFPSPlayerState*> PlayerStates = GetPlayerStatesOrderedByKills();

	if(PlayerStates.IsEmpty())
		return true;

	if(PlayerStates[0]->GetKills() >= KillsLimit)
	{
		return true;
	} else
	{
		return false;
	}
}
