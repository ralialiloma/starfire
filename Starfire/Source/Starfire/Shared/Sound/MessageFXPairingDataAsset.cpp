// Fill out your copyright notice in the Description page of Project Settings.

#include "MessageFXPairingDataAsset.h"

#if WITH_EDITOR
void UMessageFXPairingDataAsset::CleanUp()
{
	// Remove duplicates
	TArray<FMessageFXPairings> MappedFXsLocal = TArray<FMessageFXPairings>{};
	for (int32 i = 0; i< MappedFXs.Num(); ++i)
	{
		bool bFoundDuplicate = false;
		for (int32 j = 0; j< MappedFXsLocal.Num(); ++j)
		{
			if (MappedFXs[i].MessageFX == MappedFXsLocal[j].MessageFX && MappedFXs[i].MessageFX != FGameplayTag::EmptyTag)
			{
				bFoundDuplicate = true;
				break;
			}
		}
		if (!bFoundDuplicate)
			MappedFXsLocal.Add((MappedFXs[i]));
	}
	MappedFXs = MappedFXsLocal;

	//Sort
	MappedFXs.Sort([](const FMessageFXPairings& A, const FMessageFXPairings& B)
	{
		return A.MessageFX.ToString() < B.MessageFX.ToString();
	});
}
#endif


FGameplayTagContainer UMessageFXPairingDataAsset::GetMappedFX(FGameplayTag Tag)
{
	int Index = MappedFXs.Find(Tag);
	if  (!MappedFXs.IsValidIndex(Index))
		return FGameplayTagContainer();

	return MappedFXs[Index].Effects;
}


