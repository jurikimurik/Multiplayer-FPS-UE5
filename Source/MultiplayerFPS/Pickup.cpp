// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "FPSCharacter.h"
#include "FPSGameState.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APickup::APickup()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetCollisionProfileName("OverlapAll");
	RootComponent = Mesh;

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>("RotatingMovement");
	RotatingMovement->RotationRate = FRotator(0.0, 90.0f, 0);

	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	BasicMeshCollision = Mesh->GetCollisionEnabled();
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnBeginOverlap);
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
}

void APickup::OnBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor);
	if(Character == nullptr || !HasAuthority())
	{
		if(!Character)
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't get character!"));
		}
		else if (!HasAuthority())
			UE_LOG(LogTemp, Warning, TEXT("No autority!"));
		return;
	}
	Character->PlaySound2D(PickupSound);

	HidePickup();

	GetWorldTimerManager().SetTimer(
   TimerHandle, // handle to cancel timer at a later time
   this, // the owning object
   &APickup::RespawnPickup, // function to call on elapsed
   RespawnTime, // float delay until elapsed
   false);
}

void APickup::HidePickup_Implementation()
{
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetHiddenInGame(true);
}

void APickup::RespawnPickup_Implementation()
{
	Mesh->SetCollisionEnabled(BasicMeshCollision);
	Mesh->SetHiddenInGame(false);
}
