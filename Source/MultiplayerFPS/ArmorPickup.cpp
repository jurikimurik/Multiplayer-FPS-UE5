// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorPickup.h"

#include "FPSCharacter.h"

void AArmorPickup::OnBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	Super::OnBeginOverlap_Implementation(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, Hit);
	if(AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor))
		AddArmor(Character);
}

void AArmorPickup::AddArmor(AFPSCharacter* Character)
{
	Character->AddArmor(ArmorCount);
}
