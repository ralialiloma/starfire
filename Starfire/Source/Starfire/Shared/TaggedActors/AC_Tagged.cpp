#include "AC_Tagged.h"

UAC_Tagged::UAC_Tagged(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UAC_Tagged::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(StaticGameplayTags);
	TagContainer.AppendTags(DynamicGameplayTags);
}

bool UAC_Tagged::HasTag(const FGameplayTag Tag, const bool bExactMatch, const EGameplayTagSourceMode SourceType) const
{
	FGameplayTagContainer Container;
	GetGameplayTags(SourceType,Container);
	return bExactMatch?Container.HasTag(Tag):Container.HasTagExact(Tag);
}

void UAC_Tagged::GetGameplayTags(EGameplayTagSourceMode SourceType, FGameplayTagContainer& TagContainer) const
{
	if (SourceType == EGameplayTagSourceMode::StaticOnly)
		GetStaticTags(TagContainer);
	else if (SourceType == EGameplayTagSourceMode::DynamicOnly)
		GetDynamicTags(TagContainer);
	else
		GetOwnedGameplayTags(TagContainer);
}

void UAC_Tagged::GetStaticTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = StaticGameplayTags;
}

void UAC_Tagged::GetDynamicTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = DynamicGameplayTags;
}
