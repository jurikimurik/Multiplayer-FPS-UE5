// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "FPSCharacter.h"
#include "Camera/CameraComponent.h"

void AWeapon::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	CharacterOwner = Cast<AFPSCharacter>(NewOwner);
}

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	// Additional
	SetOwner(GetOwner());
}


void AWeapon::FireWeapon()
{
	// No owner
	if(!CharacterOwner)
	{
		return;
	}

	// No ammo
	if(CharacterOwner->GetAmmo(WeaponType) == 0)
	{
		if(NoAmmoSound)
			CharacterOwner->PlaySound2D(NoAmmoSound);
		return;
	}

	//Recoil time 
	if(GetWorldTimerManager().GetTimerRemaining(RecoilTimer) > 0.0f)
	{
		return;
	}

	if(!bShouldFire)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't fire!"));
		return;
	}
		

	// All good!
	CharacterOwner->RemoveAmmo(WeaponType, 1);
	
	FVector Location = CharacterOwner->Camera->GetComponentLocation();
	FVector Rotation = CharacterOwner->GetControlRotation().Vector();
	HitScan(Location, Rotation);
	if(UseMontage)
		CharacterOwner->PlayNewAnimMontage(UseMontage);
	
	if (WeaponFireMode == EWeaponFireMode::Automatic && bShouldFire)
	{
		GetWorldTimerManager().SetTimer(RecoilTimer, this, &AWeapon::FireWeapon, FireRate);
	}
	else if (WeaponFireMode == EWeaponFireMode::Single)
	{
		GetWorldTimerManager().SetTimer(RecoilTimer, FireRate, false);
	}
}

void AWeapon::HitScan(FVector Start, FVector Rotation)
{
	FHitResult Hit(ForceInit);
	FCollisionQueryParams TraceParams("Fire Trace", false, CharacterOwner);
	
	const FVector End = Start + Rotation * Range;

	//DrawDebugLine(GetWorld(),Start,End,FColor::Red,true, 10);
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	AFPSCharacter* HitCharacter = Cast<AFPSCharacter>(Hit.GetActor());

	if (HitCharacter != nullptr)
	{
		HitCharacter->ApplyDamage(Damage, CharacterOwner);
	}
}

void AWeapon::StartFire_Implementation()
{
	bShouldFire = true;

	FireWeapon();
}

void AWeapon::StopFire_Implementation()
{
	bShouldFire = false;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

