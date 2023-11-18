// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetBase.h"
#include "OverloadLayoutBase.generated.h"

class UHSBarBase;
class UImage;
/**
 * 
 */
UCLASS(Abstract)
class KRYSTALTEAR_API UOverloadLayoutBase : public UWidgetBase
{
public:

	UPROPERTY(BlueprintReadOnly, Category = "Constinuent Controls", meta = (BindWidget))
	TObjectPtr<UHSBarBase> HSBar = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Constinuent Controls", meta = (BindWidget))
	TObjectPtr<UImage> Crosshair = nullptr;

private:
	GENERATED_BODY()
};
