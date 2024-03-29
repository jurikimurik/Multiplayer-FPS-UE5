// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class MULTIPLAYERFPS_API APickup : public AActor
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pickup")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pickup")
	class URotatingMovementComponent* RotatingMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup")
	USoundBase* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pickup")
	float RespawnTime;

	FTimerHandle TimerHandle;
	ECollisionEnabled::Type BasicMeshCollision;
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION(NetMulticast, Reliable)
	void HidePickup();

	UFUNCTION(NetMulticast, Reliable)
	void RespawnPickup();
};
