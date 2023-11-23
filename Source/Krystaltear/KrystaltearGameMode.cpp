// Fill out your copyright notice in the Description page of Project Settings.


#include "KrystaltearGameMode.h"

#include "GameFramework/Character.h"

AKrystaltearGameMode::AKrystaltearGameMode()
{
	// Set the default pawn class
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBP(TEXT(""));
	if (PlayerPawnBP.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBP.Class;
	}
}

void AKrystaltearGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Bind our Player died delegate to the Gamemode's PlayerDied function
	if (!OnPlayerDied.IsBound())
	{
		OnPlayerDied.AddDynamic(this, &AKrystaltearGameMode::PlayerDied);
	}
}

void AKrystaltearGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}

void AKrystaltearGameMode::PlayerDied(ACharacter* Character)
{
	// Get a reference to our Character's Player Controller
	AController* CharacterController = Character->GetController();
	RestartPlayer(CharacterController);
}


