// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "FuelItem.generated.h"

UCLASS()
class CH3_04PROJECT_API AFuelItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AFuelItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fuel")
	float FuelRefillAmount;

	virtual void ActivateItem(AActor* Activator) override;


};
