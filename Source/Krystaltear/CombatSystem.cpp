// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem.h"

#include "CharacterBase.h"
#include "DelayAction.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UCombatSystem::UCombatSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


void UCombatSystem::HandleTakeDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	
}

void UCombatSystem::LightAttack()
{
	if (bIsAttacking)
	{
		// If the character is currently attacking when the action is called, this variable keeps track of it,
		// so the next attack will be done automatically when the previous one finishes
		bSaveAttack = true;
	}
	else
	{
		bIsAttacking = true;


		// TODO Implement this based on the current weapon and its combos
		SwordCombo();
	}
}

void UCombatSystem::SwordCombo()
{
	switch (AttackIndex)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}
}

// Called when the game starts
void UCombatSystem::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
	{
		CharacterMesh = Cast<ACharacterBase>(Owner)->GetMesh();
		Owner->OnTakeAnyDamage.AddDynamic(this, &UCombatSystem::HandleTakeDamage);
	}
}


// Called every frame
void UCombatSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

