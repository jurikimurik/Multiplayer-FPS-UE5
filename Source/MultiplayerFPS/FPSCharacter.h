// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Weapon.h"
#include "FPSCharacter.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AFPSCharacter : public ACharacter
{
public:
	virtual void Tick(float DeltaSeconds) override;
	
	void ApplyDamage(float Damage, AFPSCharacter* Causer);

private:
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FPS Character")
	class UCameraComponent* Camera;
	
protected:

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS Character")
	float Health = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	float MaxHealth = 100.0f;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "FPS Character")
	float Armor = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	float MaxArmor = 100.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	float ArmorAbsorption = 0.5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	USoundBase* SpawnSound = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	USoundBase* LandSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	USoundBase* HitSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	USoundBase* DamageSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS Character")
	USoundBase* WeaponChangeSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="FPS Character")
	TArray<TSubclassOf<AWeapon>> WeaponClasses;

	// --- INPUT ACTIONS -----------------------------------------------

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* LookInputAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* JumpInputAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* FireInputAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* PistolInputAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* MachineGunInputAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* RailgunInputAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* PreviousWeaponInputAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* NextWeaponInputAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* ToggleTableScoreInputAction;
	
	// TEST INPUT BUTTON
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* TestInputAction;

	// --- REPLICATED -----------------------------------------------------

	UPROPERTY(Replicated)
	TArray<int32> Ammo;

	UPROPERTY(Replicated)
	TArray<AWeapon*> Weapons;

	UPROPERTY(Replicated)
	AWeapon* CurrentWeapon;

	// --- FUNCTIONS ------------------------------------------------------
	
	AFPSCharacter();
	
	virtual void BeginPlay() override;	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartFire(const FInputActionValue& Value);
	void StopFire(const FInputActionValue& Value);

	void OnDeath();
	virtual void FellOutOfWorld(const UDamageType& dmgType) override;
	virtual void Landed(const FHitResult& Hit) override;

	void ShowScoretable(bool Visibility);
	// --- WEAPONS --------------------------------------------------
	
	UFUNCTION(NetMulticast, Reliable)
	void EquipWeapon(AWeapon* Weapon);
	
public:
	
	UFUNCTION(NetMulticast, Reliable)
	void AddWeapon(TSubclassOf<AWeapon> Weapon);

protected:

	UFUNCTION(Server, Reliable)
	void CycleWeapon(int32 Direction, bool UseAsEndIndex = false);
	void CycleToWeapon(int32 Number);
	void CycleForward();
	void CycleBackward();
	
	void DestroyWeapons();

public:

	// --- AMMO ------------------------------------------------------
	void AddAmmo(EWeaponType Weapon, int32 count);
	void RemoveAmmo(EWeaponType Weapon, int32 count);
	int32 GetAmmo(EWeaponType Weapon) const;
	
	
	// --- VISUALS ---------------------------------------------------

	// MULTICAST WYWOLYWAC TYLKO Z POZIOMU SERWERA!
	UFUNCTION(NetMulticast, Reliable)
	void PlayNewAnimMontage(UAnimMontage* Montage);

	UFUNCTION(Client, Unreliable)
	void PlaySound2D(USoundBase* Sound);
	
public:

	// --- HEALTH -----------------------------------------------------
	
	void AddHealth(float Amount) { SetHealth(Health + Amount); }
	void RemoveHealth(float Amount) { SetHealth(Health - Amount); }
	void SetHealth(float NewHealth) { Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth); }
	bool IsDead() const { return Health == 0.0f; }

	// --- ARMOR ------------------------------------------------------
	
	void AddArmor(float Amount) { SetArmor(Armor + Amount); }
	void SetArmor(float Amount) { Armor = FMath::Clamp(Amount, 0.0f, MaxArmor); }
	void ArmorAbsorbDamage(float & Damage);

	// --- GETTERS ----------------------------------------------------
	
	bool HasWeapon(TSubclassOf<AWeapon> Weapon) const;

	// --- UI GETTERS -------------------------------------------------
	
	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmo() const;
	UFUNCTION(BlueprintPure)
	FString GetCurrentWeapon() const;
	UFUNCTION(BlueprintPure)
	float GetCurrentHealth() const;
	UFUNCTION(BlueprintPure)
	float GetCurrentArmor() const;
};
