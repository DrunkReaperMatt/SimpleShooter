// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAssetManager.h"
#include "ShooterGameplayTags.h"

UShooterAssetManager::UShooterAssetManager()
{
}

UShooterAssetManager& UShooterAssetManager::Get()
{
	check(GEngine);
 
	UShooterAssetManager* MyAssetManager = Cast<UShooterAssetManager>(GEngine->AssetManager);
	return *MyAssetManager;
}
 
void UShooterAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
 
	//Load Native Tags
	FShooterGameplayTags::InitializeNativeTags();
}