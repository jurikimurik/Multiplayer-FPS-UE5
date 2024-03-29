// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FPSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AFPSPlayerState : public APlayerState
{

private:
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 Kills;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 Deaths;


public:
	void AddKill() { Kills++; }
	void AddKill(int32 NumberOfKills) { Kills += NumberOfKills; }

	void AddDeath() { Deaths++; }

	int32 GetKills() const
	{
		return Kills;
	}

	int32 GetDeaths() const
	{
		return Deaths;
	}
};
