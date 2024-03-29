// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "WeaponPickup.generated.h"

enum class EWeaponType : uint8;
class AWeapon;
class AFPSCharacter;
/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AWeaponPickup : public APickup
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<AWeapon> Weapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	EWeaponType WeaponType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	int32 AmmoCount = 0;
	
public:
	void OnBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit) override;


protected:
	void AddWeapon(AFPSCharacter* Character);
};
