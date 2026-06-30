
#include "GoldCoinItem.h"
#include "CH3GameState.h"

AGoldCoinItem::AGoldCoinItem()
{
	PointValue = 10;
	ItemType = "GoldCoinItem";
}

void AGoldCoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	
}
