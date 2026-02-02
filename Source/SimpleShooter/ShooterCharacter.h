// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;
class UInputMappingContext;
class UInputComponent;
class UInputConfig;
struct FInputActionValue;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         class AController* EventInstigator, AActor* DamageCauser) override;

    UFUNCTION(BlueprintPure)
    bool IsDead() const;
    
private:
	//void MoveForward(const float Value);
	//void LookUp(const float Value);

	UPROPERTY(EditAnywhere)
	float RotationRate = 10;
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
	float CurrentHealth;

public:
	/** The input config that maps Input Actions to Input Tags*/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputConfig* InputConfig;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	TSubclassOf<AGun> GunClass;

	UPROPERTY()
	AGun* Gun;

	/** Handles moving forward/backward */
	void Input_Move(const FInputActionValue& InputActionValue);

	/** Handles mouse and stick look */
	void Input_Look(const FInputActionValue& InputActionValue);

	/** Handles Jumping */
	void Input_Jump(const FInputActionValue& InputActionValue);

	/** Handles Pew Pew */
	void Input_Fire(const FInputActionValue& InputActionValue);

protected:
	/**
* Called via input to turn at a given rate.
* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
*/
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
};
