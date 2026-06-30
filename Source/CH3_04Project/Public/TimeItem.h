// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "TimeItem.generated.h"

UCLASS()
class CH3_04PROJECT_API ATimeItem : public ABaseItem
{
	GENERATED_BODY()

public:
	ATimeItem();

protected:
	virtual void ActivateItem(AActor* Activator) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float TimeBonus;
	
};
