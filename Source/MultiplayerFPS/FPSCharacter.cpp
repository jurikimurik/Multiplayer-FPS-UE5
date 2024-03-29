#include "FPSCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FPSPlayerController.h"
#include "FPSPlayerState.h"
#include "MultiplayerFPS.h"
#include "MultiplayerFPSGameModeBase.h"

void AFPSCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	/*FString AmmoString = TEXT("Ammo = ");
	for(int32 ammo : Ammo)
	{
		AmmoString.Appendf(TEXT("%d, "), ammo);
	}
	AmmoString.Append("\n");
	
	DrawDebugString(GetWorld(),
		GetActorLocation(),
		FString::Printf(TEXT("Health = %f\nArmor = %f\nCurrent Weapon = %s\n%s"), Health,Armor,*GetCurrentWeapon(), *AmmoString),
		nullptr,
		FColor::White,
		0.0f,
		true);*/

	AFPSPlayerState* FpsPlayerState = Cast<AFPSPlayerState>(GetPlayerState());
	if(!FpsPlayerState)
		return;

	DrawDebugString(GetWorld(),
		GetActorLocation(),
		FString::Printf(TEXT("Kills = %i,\n Deaths = %i\n, Weapons = %d\n"), FpsPlayerState->GetKills(), FpsPlayerState->GetDeaths(), Weapons.Num()),
		nullptr,
		FColor::White,
		0.0f,
		true);
}

void AFPSCharacter::ApplyDamage(float Damage, AFPSCharacter* Causer)
{
	ArmorAbsorbDamage(Damage);
	RemoveHealth(Damage);

	//If character died
	if(IsDead())
	{
		GetWorld()->GetAuthGameMode<AMultiplayerFPSGameModeBase>()->PlayerKilled(this, Causer);
		OnDeath();
		return;
	}

	//If character is still alive
	if (Causer != nullptr)
	{
		Causer->PlaySound2D(HitSound);
		PlaySound2D(DamageSound);
	}
}

AFPSCharacter::AFPSCharacter()
{
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(GetMesh(), "Camera");

	GetCharacterMovement()->MaxWalkSpeed = 800.0f;
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	Ammo.Init(50, ENUM_TO_INT32(EWeaponType::MAX));

	PrimaryActorTick.bCanEverTick = true;
}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySound2D(GetWorld(), SpawnSound);

	for(auto Weapon : WeaponClasses)
		AddWeapon(Weapon);
	
	if (!HasAuthority())
	{
		return;
	}
	
	if(Weapons.IsValidIndex(0))
		EquipWeapon(Weapons[0]);

	SetHealth(MaxHealth);
}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedPlayerInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	const APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (EnhancedPlayerInputComponent == nullptr || PlayerController == nullptr)
	{
		return;
	}
	
	UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	if(EnhancedSubsystem == nullptr)
	{
		return;
	}

	EnhancedSubsystem->AddMappingContext(InputMappingContext, 1);

	EnhancedPlayerInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Move);
	EnhancedPlayerInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Look);
	EnhancedPlayerInputComponent->BindAction(JumpInputAction, ETriggerEvent::Started, this, &ACharacter::Jump);

	EnhancedPlayerInputComponent->BindAction(FireInputAction, ETriggerEvent::Started, this, &AFPSCharacter::StartFire);
	EnhancedPlayerInputComponent->BindAction(FireInputAction, ETriggerEvent::Completed, this, &AFPSCharacter::StopFire);

	EnhancedPlayerInputComponent->BindAction(PistolInputAction, ETriggerEvent::Triggered, this, &AFPSCharacter::CycleToWeapon, 0);
	EnhancedPlayerInputComponent->BindAction(MachineGunInputAction, ETriggerEvent::Triggered, this, &AFPSCharacter::CycleToWeapon, 1);
	EnhancedPlayerInputComponent->BindAction(RailgunInputAction, ETriggerEvent::Triggered, this, &AFPSCharacter::CycleToWeapon, 2);

	EnhancedPlayerInputComponent->BindAction(PreviousWeaponInputAction, ETriggerEvent::Triggered, this, &AFPSCharacter::CycleForward);
	EnhancedPlayerInputComponent->BindAction(NextWeaponInputAction, ETriggerEvent::Triggered, this, &AFPSCharacter::CycleBackward);

	EnhancedPlayerInputComponent->BindAction(ToggleTableScoreInputAction, ETriggerEvent::Started, this, &AFPSCharacter::ShowScoretable, true);
	EnhancedPlayerInputComponent->BindAction(ToggleTableScoreInputAction, ETriggerEvent::Completed, this, &AFPSCharacter::ShowScoretable, false);
	
	//EnhancedPlayerInputComponent->BindAction(TestInputAction, ETriggerEvent::Triggered, this, &AFPSCharacter::TestAnimMontage);
}

void AFPSCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	DestroyWeapons();
	Destroy();
}

void AFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(AFPSCharacter, Health, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacter, Armor, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacter, CurrentWeapon, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacter, Weapons, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSCharacter, Ammo, COND_OwnerOnly);
}

void AFPSCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();

	if(InputValue.X != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), InputValue.X);
	}

	if(InputValue.Y != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), InputValue.Y);	
	}
}

void AFPSCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();

	if(InputValue.X != 0.0f)
	{
		AddControllerYawInput(InputValue.X);
	}

	if(InputValue.Y != 0.0f)
	{
		AddControllerPitchInput(InputValue.Y);	
	}
}

void AFPSCharacter::StartFire(const FInputActionValue& Value)
{
	if(CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}
}

void AFPSCharacter::StopFire(const FInputActionValue& Value)
{
	if(CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void AFPSCharacter::OnDeath()
{
	GetWorld()->GetAuthGameMode<AMultiplayerFPSGameModeBase>()->PlayerDied(this);
}

void AFPSCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
	OnDeath();
}

void AFPSCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	if(LandSound)
		UGameplayStatics::PlaySound2D(GetWorld(), LandSound);
}

void AFPSCharacter::ShowScoretable(bool Visibility)
{
	AFPSPlayerController* PlayerController = Cast<AFPSPlayerController>(GetController());
	if(PlayerController)
	{
		PlayerController->ToogleScoreboard(Visibility);
	}
}

void AFPSCharacter::AddWeapon_Implementation(TSubclassOf<AWeapon> Weapon)
{
	UClass* WeaponClass = Weapon;

	if (!WeaponClass & HasWeapon(WeaponClass))
	{
		return;
	}

	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AWeapon* NewWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnParameters);

	if (NewWeapon == nullptr)
	{
		return;
	}

	NewWeapon->SetActorHiddenInGame(true);
	Weapons.Add(NewWeapon);
	NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "GripPoint");
}

void AFPSCharacter::EquipWeapon_Implementation(AWeapon* Weapon)
{
	if(Weapon)
	{
		if(CurrentWeapon)
			CurrentWeapon->SetActorHiddenInGame(true);
		
		CurrentWeapon = Weapon;
		CurrentWeapon->SetActorHiddenInGame(false);
	}
}

void AFPSCharacter::CycleWeapon_Implementation(int32 Direction, bool UseAsEndIndex)
{
	int32 CurrentIndex = 0;
	if(CurrentWeapon)
	{
		CurrentIndex = Weapons.Find(CurrentWeapon);
	}
	
	int32 newIndex = 0;
	if(UseAsEndIndex)
	{
		newIndex = GET_WRAPPED_ARRAY_INDEX(Direction, Weapons.Num());
	} else
	{
		newIndex = GET_WRAPPED_ARRAY_INDEX(CurrentIndex+Direction, Weapons.Num());
	}

	
	if(Weapons.IsValidIndex(newIndex))
	{
		EquipWeapon(Weapons[newIndex]);
	}
		
}

void AFPSCharacter::CycleToWeapon(int32 Number)
{
	CycleWeapon(Number, true);
}

void AFPSCharacter::CycleForward()
{
	CycleWeapon(1);
}

void AFPSCharacter::CycleBackward()
{
	CycleWeapon(-1);
}

void AFPSCharacter::DestroyWeapons()
{
	for(AWeapon* Weapon : Weapons)
	{
		Weapon->Destroy();
	}
		
	if(CurrentWeapon)
		CurrentWeapon->Destroy();
}

void AFPSCharacter::AddAmmo(EWeaponType Weapon, int32 count)
{
	if(Ammo.IsValidIndex(ENUM_TO_INT32(Weapon)))
	{
		Ammo[ENUM_TO_INT32(Weapon)] += count;
	}
}

void AFPSCharacter::RemoveAmmo(EWeaponType Weapon, int32 count)
{
	if(Ammo.IsValidIndex(ENUM_TO_INT32(Weapon)))
	{
		// 0 is the limit
		Ammo[ENUM_TO_INT32(Weapon)] = FMath::Max((Ammo[ENUM_TO_INT32(Weapon)] - count), 0);
	}
}

int32 AFPSCharacter::GetAmmo(EWeaponType Weapon) const
{
	if(Ammo.IsValidIndex(ENUM_TO_INT32(Weapon)))
	{
		return Ammo[ENUM_TO_INT32(Weapon)];
	}
	else
	{
		return 0;
	}
}

void AFPSCharacter::PlaySound2D_Implementation(USoundBase* Sound)
{
	if(Sound)
	{
		UGameplayStatics::PlaySound2D(this, Sound);
	}
}

void AFPSCharacter::PlayNewAnimMontage_Implementation(UAnimMontage* Montage)
{
	if(Montage)
	{
		PlayAnimMontage(Montage);
	}
}

void AFPSCharacter::ArmorAbsorbDamage(float & Damage)
{
	const float AbsorbedDamage = Damage * ArmorAbsorption;
	const float RemainingArmor = Armor - AbsorbedDamage;

	SetArmor(RemainingArmor);

	Damage = (Damage * (1 - ArmorAbsorption)) - FMath::Min(RemainingArmor, 0.0f);
	RemoveHealth(Damage);
}

bool AFPSCharacter::HasWeapon(TSubclassOf<AWeapon> Weapon) const
{
	if(Weapons.IsEmpty())
		return false;
	
	for(const AWeapon* WeaponInside : Weapons)
	{
		if(!WeaponInside)
			continue;
		
		if(WeaponInside->GetClass() == Weapon)
			return true;
	}
	return false;
}

int32 AFPSCharacter::GetCurrentAmmo() const
{
	if(!CurrentWeapon)
		return -1;
	
	int32 index = ENUM_TO_INT32(CurrentWeapon->WeaponType);
	if(Ammo.IsValidIndex(index))
		return Ammo[ENUM_TO_INT32(CurrentWeapon->WeaponType)];
	else
	{
		return -1;
	}
}

FString AFPSCharacter::GetCurrentWeapon() const
{
	if(CurrentWeapon)
		return CurrentWeapon->Name;
	else
		return FString("No Weapon");
}

float AFPSCharacter::GetCurrentHealth() const
{
	return Health;
}

float AFPSCharacter::GetCurrentArmor() const
{
	return Armor;
}
