// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PullTrigger()
{
	UE_LOG(LogTemp, Warning, TEXT("You've been Shot"));

	UGameplayStatics::SpawnEmitterAttached(Particle, Mesh, TEXT("MuzzleFlashSocket"));

	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		if (AController* PawnController = OwnerPawn->GetController())
		{
			FVector Location;
			FRotator Rotation;
			PawnController->GetPlayerViewPoint(Location, Rotation);

			FVector EndPoint = Location + Rotation.Vector() * MaxRange;

			// TODO Line Trace
			FHitResult Hit;
			if (GetWorld()->LineTraceSingleByChannel(Hit, Location, EndPoint, ECC_GameTraceChannel1))
			{
				FVector ShotDirection = -Rotation.Vector();
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle ,Hit.Location, ShotDirection.Rotation());

				if (AActor* HitActor = Hit.GetActor())
				{
					FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
					HitActor->TakeDamage(Damage, DamageEvent, PawnController, OwnerPawn);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No HIt"));
			}
		}
	}
}

