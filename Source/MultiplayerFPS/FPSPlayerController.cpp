// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerController.h"
#include "PlayerMenu.h"
#include "Net/UnrealNetwork.h"

void AFPSPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(AFPSPlayerController, ScoreboardMenu, COND_OwnerOnly);
}

void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if(IsLocalController() && ScoreboardMenuClass != nullptr)
	{
		ScoreboardMenu = CreateWidget<UPlayerMenu>(this, ScoreboardMenuClass);
		if(ScoreboardMenu != nullptr)
		{
			ScoreboardMenu->AddToViewport(0);
		}
	}
}

void AFPSPlayerController::ToogleScoreboard(bool ShouldBeVisible)
{
	ScoreboardMenu->SetScoreboardVisible(ShouldBeVisible);
}

void AFPSPlayerController::ShowVictim_Implementation(const FString& VictimName, const float& Duration)
{
	if(!ScoreboardMenu)
		return;
		
	
	ScoreboardMenu->NotifyPlayerKilled(VictimName, Duration);
}
