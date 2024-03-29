// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AFPSPlayerController : public APlayerController
{

	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FPS Player Controller")
	TSubclassOf<class UPlayerMenu> ScoreboardMenuClass;

	UPROPERTY(Replicated)
	class UPlayerMenu* ScoreboardMenu;

protected:
	virtual void BeginPlay() override;

public:
	virtual void ToogleScoreboard(bool ShouldBeVisible);
	UFUNCTION(Client, Reliable)
	virtual void ShowVictim(const FString& VictimName, const float& Duration);
};
