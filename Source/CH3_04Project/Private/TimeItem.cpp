
#include "TimeItem.h"
#include "CH3GameState.h"

ATimeItem::ATimeItem()
{
	TimeBonus = 10.0f;
	ItemType = "TimeItem";
}

void ATimeItem::ActivateItem(AActor* Activator)
{
	UE_LOG(LogTemp, Warning, TEXT("TimeItem ActivateItem Called"));
	if (Activator && Activator->ActorHasTag("Player"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Activator is Player"));
		if (UWorld* World = GetWorld())
		{
			if (ACH3GameState* GameState = World->GetGameState<ACH3GameState>())
			{
				GameState->AddTime(TimeBonus);
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("+ %.1f Bonus Time"),TimeBonus));
			}
		}
	}
	DestroyItem();
}
