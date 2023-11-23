// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Krystaltear/CharacterBase.h"
#include "CharacterStatsComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class KRYSTALTEAR_API UCharacterStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterStatsComponent();
	
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Health")
	float Health;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Health")
	float MaxHealth;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Stamina")
	float Stamina;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Stamina")
	float MaxStamina;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Level and Xp")
	int Level;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Level and Xp")
	int Xp;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Level and Xp")
	int MaxXp;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bIsPlayer;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<ACharacterBase> PlayerCharacter;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<ACharacter> Dummy;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
