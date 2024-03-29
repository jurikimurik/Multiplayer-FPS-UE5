// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FPSCharacter.h"
#include "GameFramework/GameMode.h"
#include "MultiplayerFPSGameModeBase.generated.h"

class AFPSCharacter;
class AFPSGameState;
/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AMultiplayerFPSGameModeBase : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;

	void RestartMap() const;
	
public:
	virtual bool ReadyToEndMatch_Implementation() override;
	virtual bool ReadyToStartMatch_Implementation() override;

	UFUNCTION(Server, Reliable)
	virtual void PlayerKilled(AFPSCharacter* Victim, AFPSCharacter* Killer);
	virtual void PlayerDied(AFPSCharacter* Victim);
	
	int32 GetKillsNecessary() const  { return KillsNecessary; }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 KillsNecessary = 5;

protected:
	UPROPERTY()
	AFPSGameState* AfpsGameState;
};
