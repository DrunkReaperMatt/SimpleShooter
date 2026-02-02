// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "Gun.h"
#include "MyEnhancedInputComponent.h"
#include "ShooterGameplayTags.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	CurrentHealth = MaxHealth;
	
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_R"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("weapon_rSocket"));
	Gun->SetOwner(this);
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UMyEnhancedInputComponent* MyEnhancedInputComponent = Cast<UMyEnhancedInputComponent>(PlayerInputComponent);

	check(MyEnhancedInputComponent);

	const FShooterGameplayTags& GameplayTags = FShooterGameplayTags::Get();

	//Bind Input actions by tag
	MyEnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &AShooterCharacter::Input_Move);
	MyEnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &AShooterCharacter::Input_Look);
	MyEnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_Look_Stick, ETriggerEvent::Triggered, this, &AShooterCharacter::Input_Look);
	MyEnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_Fire, ETriggerEvent::Triggered, this, &AShooterCharacter::Input_Fire);
	MyEnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_Jump, ETriggerEvent::Triggered, this, &AShooterCharacter::Input_Jump);

}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageTaken = FMath::Min(DamageTaken, CurrentHealth);
	CurrentHealth -= DamageTaken;
	UE_LOG(LogTemp, Warning, TEXT("Remaining Health: %f"), CurrentHealth);

	return DamageTaken;
}

bool AShooterCharacter::IsDead() const
{
	return CurrentHealth <= 0;
}

/*
void AShooterCharacter::MoveForward(const float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}
*/

void AShooterCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	if (Controller != nullptr)
	{
		const FVector2D MoveValue = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
 
		if (MoveValue.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			AddMovementInput(MovementDirection, MoveValue.X);
		}
 
		if (MoveValue.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(MovementDirection, MoveValue.Y);
		}
	}
}

void AShooterCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	if (Controller != nullptr)
	{
		const FVector2D LookValue = InputActionValue.Get<FVector2D>();
 
		if (LookValue.X != 0.0f)
		{
			TurnAtRate(LookValue.X);
		}
 
		if (LookValue.Y != 0.0f)
		{
			LookUpAtRate(LookValue.Y);
		}
	}
}

void AShooterCharacter::Input_Jump(const FInputActionValue& InputActionValue)
{
	Jump();
}

void AShooterCharacter::Input_Fire(const FInputActionValue& InputActionValue)
{
	Gun->PullTrigger();
}

void AShooterCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookUpAtRate(float Rate)
{
	
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * RotationRate * GetWorld()->GetDeltaSeconds());
}

/*
void AShooterCharacter::LookUp(const float Value)
{
	AddControllerPitchInput(Value);
}
*/
