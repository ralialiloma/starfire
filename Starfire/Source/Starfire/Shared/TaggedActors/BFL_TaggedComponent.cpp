#include "BFL_TaggedComponent.h"
#include "AC_Tagged.h"
#include "Extensions/HelperFunctionLibrary.h"

void UBFl_TaggedComponent::GetAllActorsWithTag(UObject* WorldContextObject,TArray<AActor*>& OutActors, const FGameplayTag Tag, const 
EGameplayTagSourceMode SourceMode)
{
	OutActors.Empty();

	UWorld* World = GEngine ? GEngine->GetWorldFromContextObjectChecked(WorldContextObject) : nullptr;
	if (!World) return;

	TArray<AActor*> AllActors;
	UHelperFunctionLibrary::GetAllActorsWithComponent<UAC_Tagged>(World, AllActors);;

	for (AActor* Actor : AllActors)
	{
		if (!Actor)
			continue;

		UAC_Tagged* TaggedComp = Actor->FindComponentByClass<UAC_Tagged>();
		if (!TaggedComp)
			continue;

		FGameplayTagContainer Tags;
		TaggedComp->GetGameplayTags(SourceMode,Tags);

		if (Tags.HasTag(Tag))
		{
			OutActors.Add(Actor);
		}
	}
}

bool UBFl_TaggedComponent::GetFirstActorWithTag(UObject* WorldContextObject, AActor*& FoundActor, const FGameplayTag Tag,
	const EGameplayTagSourceMode SourceMode)
{
	TArray<AActor*> FoundActors = {};
	GetAllActorsWithTag(WorldContextObject,FoundActors,Tag,SourceMode);
	if (FoundActors.Num() > 0)
	{
		FoundActor = FoundActors[0];
		return true;
	}
	return false;
}
