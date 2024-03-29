// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "ArmorPickup.generated.h"

class AFPSCharacter;
/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AArmorPickup : public APickup
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Armor")
	int32 ArmorCount = 0;
	
public:
	void OnBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit) override;


protected:
	void AddArmor(AFPSCharacter* Character);
};
