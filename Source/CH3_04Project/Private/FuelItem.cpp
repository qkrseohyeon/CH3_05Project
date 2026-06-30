
#include "FuelItem.h"
#include "FlyPawn.h"

AFuelItem::AFuelItem()
{
	FuelRefillAmount = 20.0f;
	ItemType = "FuelItem";

}

void AFuelItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (AFlyPawn* FlyPawn = Cast<AFlyPawn>(Activator))
		{
			FlyPawn->AddFuel(FuelRefillAmount);
		}
	}

	DestroyItem();
}

