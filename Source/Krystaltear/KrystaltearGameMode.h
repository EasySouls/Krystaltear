// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KrystaltearGameMode.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnPlayerDiedSignature,
	ACharacter*, Character);

UCLASS()
class KRYSTALTEAR_API AKrystaltearGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AKrystaltearGameMode();
	
	const FOnPlayerDiedSignature& GetOnPlayerDied() const { return OnPlayerDied; }

	// Tries to spawn the player's pawn
	virtual void RestartPlayer(AController* NewPlayer) override;

protected:
	virtual void BeginPlay() override;

	// Called when the Player's character has died
	UFUNCTION()
	virtual void PlayerDied(ACharacter *Character);

	// Signature to bind delegate
	UPROPERTY()
	FOnPlayerDiedSignature OnPlayerDied;
};
