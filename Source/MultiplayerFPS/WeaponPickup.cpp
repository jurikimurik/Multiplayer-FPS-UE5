// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"

#include "FPSCharacter.h"

void AWeaponPickup::OnBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	Super::OnBeginOverlap_Implementation(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, Hit);
	if(AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor))
		AddWeapon(Character);
}

void AWeaponPickup::AddWeapon(AFPSCharacter* Character)
{
	if(Character->HasWeapon(Weapon))
	{
		Character->AddAmmo(WeaponType, AmmoCount);
	} else
	{
		if(Weapon)
			Character->AddWeapon(Weapon);
	}
}
