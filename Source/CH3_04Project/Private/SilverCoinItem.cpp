// Fill out your copyright notice in the Description page of Project Settings.


#include "SilverCoinItem.h"
#include "CH3GameState.h"

ASilverCoinItem::ASilverCoinItem()
{
	PointValue = 5;
	ItemType = "GoldCoinItem";
}

void ASilverCoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}
