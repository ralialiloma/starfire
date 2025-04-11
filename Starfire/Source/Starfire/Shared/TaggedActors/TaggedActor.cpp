#include "TaggedActor.h"
#include "Starfire/Shared/TaggedActors/AC_Tagged.h"


ATaggedActor::ATaggedActor(const FObjectInitializer& ObjectInitializer)
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	TaggedComp =  ObjectInitializer.CreateDefaultSubobject<UAC_Tagged>(this,"AC_Tagged");
}

void ATaggedActor::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TaggedComp->GetOwnedGameplayTags(TagContainer);
}

void ATaggedActor::GetGameplayTags(const EGameplayTagSourceMode SourceType, FGameplayTagContainer& TagContainer) const
{
	return TaggedComp->GetGameplayTags(SourceType,TagContainer);
}

bool ATaggedActor::HasTag(FGameplayTag Tag, bool bExactMatch, EGameplayTagSourceMode SourceType) const
{
	return TaggedComp->HasTag(Tag,bExactMatch,SourceType);
}

void ATaggedActor::AddTag(const FGameplayTag TagToAdd)
{
	TaggedComp->DynamicGameplayTags.AddTag(TagToAdd);
}

void ATaggedActor::RemoveTag(FGameplayTag TagToRemove)
{
	TaggedComp->DynamicGameplayTags.RemoveTag(TagToRemove);
}

