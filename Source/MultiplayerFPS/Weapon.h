// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumTypes.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class AFPSCharacter;

UCLASS()
class MULTIPLAYERFPS_API AWeapon : public AActor
{
public:
	virtual void SetOwner(AActor* NewOwner) override;

private:
	GENERATED_BODY()

public:

	// --- UE PROPERTIES ----------------------------------------------------
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	EWeaponFireMode WeaponFireMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	EAmmoType AmmoType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	float Range;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UAnimMontage* UseMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	USoundBase* NoAmmoSound;

	// ----------------------------------------------------------------------

private:
	AFPSCharacter* CharacterOwner;
	FTimerHandle RecoilTimer;
	bool bShouldFire = false;
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	UFUNCTION(Server, Reliable)
	void StartFire();
	UFUNCTION(Server, Reliable)
	void StopFire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	void FireWeapon();
	void HitScan(FVector Start, FVector Rotation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
