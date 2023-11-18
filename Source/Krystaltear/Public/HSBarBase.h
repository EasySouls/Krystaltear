// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetBase.h"
#include "HSBarBase.generated.h"

class UStatBarBase;

/**
 * 
 */
UCLASS()
class KRYSTALTEAR_API UHSBarBase : public UWidgetBase
{
public:
	UPROPERTY(BlueprintReadOnly, Category = "Constituent Controls", meta = (BindWidget))
	TObjectPtr<UStatBarBase> HealthBar = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Constituent Controls", meta = (BindWidget))
	TObjectPtr<UStatBarBase> StaminaBar = nullptr;

private:
	GENERATED_BODY()
};
