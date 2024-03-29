// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UPlayerMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ToggleScoreboard();
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void SetScoreboardVisible(bool NewVisibility);

	
	UFUNCTION(BlueprintImplementableEvent)
	void NotifyPlayerKilled(const FString& VictimName, const float& Duration);
};
