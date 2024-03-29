// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"
#include "FPSCharacter.h"

void AHealthPickup::OnBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
												 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	Super::OnBeginOverlap_Implementation(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, Hit);
	if(AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor))
		AddHealth(Character);
}

void AHealthPickup::AddHealth(AFPSCharacter* Character)
{
	Character->AddHealth(HealthCount);
}
