// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSPlayerState.h"
#include "GameFramework/GameState.h"
#include "FPSGameState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AFPSGameState : public AGameState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 KillsLimit = 25;


protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	TArray<AFPSPlayerState*> GetPlayerStatesOrderedByKills() const;

public:
	bool HasWinner() const;
};
