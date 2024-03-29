// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"

#include "FPSCharacter.h"


void AAmmoPickup::OnBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	Super::OnBeginOverlap_Implementation(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, Hit);
	if(AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor))
		AddAmmo(Character);
}

void AAmmoPickup::AddAmmo(AFPSCharacter* Character)
{
	Character->AddAmmo(WeaponType, AmmoCount);
}
