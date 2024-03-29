// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumTypes.h"
#include "Pickup.h"
#include "AmmoPickup.generated.h"

class AFPSCharacter;
/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AAmmoPickup : public APickup
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ammo")
	EWeaponType WeaponType = EWeaponType::Pistol;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ammo")
	int32 AmmoCount = 0;
	
public:
	void OnBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit) override;


protected:
	void AddAmmo(AFPSCharacter* Character);
};
