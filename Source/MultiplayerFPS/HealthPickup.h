// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "HealthPickup.generated.h"

class AFPSCharacter;
/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AHealthPickup : public APickup
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health")
	int32 HealthCount = 0;
	
public:
	void OnBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit) override;


protected:
	void AddHealth(AFPSCharacter* Character);
};
