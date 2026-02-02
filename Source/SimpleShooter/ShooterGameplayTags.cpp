// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameplayTags.h"
#include "GameplayTagsManager.h"
#include "Engine/EngineTypes.h"

FShooterGameplayTags FShooterGameplayTags::GameplayTags;

void FShooterGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();
 
	GameplayTags.AddAllTags(GameplayTagsManager);
 
	GameplayTagsManager.DoneAddingNativeTags();
}

void FShooterGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(InputTag_Move, "InputTag.Move", "Move input.");
	AddTag(InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look (mouse) input.");
	AddTag(InputTag_Look_Stick, "InputTag.Look.Stick", "Look (stick) input.");
	AddTag(InputTag_Jump, "InputTag.Jump", "Jump input");
	AddTag(InputTag_Fire, "InputTag.Fire", "Fire input.");
}
 
void FShooterGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native)  ")).Append(FString(TagComment)));
}