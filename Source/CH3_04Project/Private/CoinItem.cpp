
#include "CoinItem.h"
#include "CH3GameState.h"

ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "Default Coin";
	bCountsForLevelClear = true;
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			if (ACH3GameState* GameState = World->GetGameState<ACH3GameState>())
			{
				GameState->OnCoinCollected(bCountsForLevelClear, PointValue);
			}

		}
	}

	DestroyItem();
}


