// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "CombatSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(
	FOnHealthChangedSignature,
	UCombatSystem*, CombatSystemComponent,
	float, Health,
	float, DamageAmount,
	const class UDamageType*, DamageType,
	class AController*, InstigatedBy,
	AActor*, DamageCauser);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class KRYSTALTEAR_API UCombatSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCombatSystem();

protected:
	UPROPERTY()
	bool bCanAttack = true;
	
	UPROPERTY()
	bool bIsAttacking = false;
	
	UPROPERTY()
	bool bSaveAttack = false;

	UPROPERTY()
	int AttackIndex = 0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> CharacterMesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<ACharacter> Character;

	UPROPERTY()
	FTimerHandle SwordTraceHandle;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Animations")
	TArray<UAnimMontage*> SwordAttackAnims;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Animations")
	TArray<UAnimMontage*> AxeAttackAnims;

	UFUNCTION(BlueprintCallable)
	void HandleTakeDamage(AActor* DamageActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;
	
	UFUNCTION()
	void LightAttack();

	UFUNCTION()
	void SwordCombo();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
