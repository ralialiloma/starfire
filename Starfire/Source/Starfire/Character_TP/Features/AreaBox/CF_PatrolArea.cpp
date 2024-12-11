#include "CF_PatrolArea.h"

#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Starfire/Character_TP/Features/Locomotion/CF_Locomotion.h"
#include "Starfire/Character_TP/PatrolArea/Sf_PatrolAreaManager.h"
#include "Starfire/Character_TP/EQS/TetherPointSystem/Sf_TetherPointGen.h"


void UCF_PatrolArea::Initialize(ASf_TP_Character* Holder, const USf_CharacterFeature_Config* InConfig)
{
	Super::Initialize(Holder, InConfig);
	VALIDATE_CONFIG(UCF_PatrolArea_Config,AreaBox_Config)
}

void UCF_PatrolArea::OnBeginPlay()
{
	Super::OnBeginPlay();

	bool bFound = false;
	CurrentPatrolArea =  USf_PatrolAreaManager::Get(GetWorld())->FindPatrolAreaForLocation(GetOwnerLocation(),bFound);
	

/*	UCF_Locomotion* Locomotion = GetOwningCharacter()->GetFeatureByClass<UCF_Locomotion>();
	if (!IsValid(Locomotion))
		return;
	
	Locomotion->OnMoveStarted_CPP.AddLambda([Locomotion,this]()->void
	{
		ActivePatrolPointTags = FGameplayTagContainer::EmptyContainer;
		Locomotion->OnMoveFinished_CPP.AddLambda([this]()->void{OnMoveFinished();});
	});*/

}

void UCF_PatrolArea::OnMoveFinished()
{
	ASf_TetherPointGen* TetherPointGen = ASf_TetherPointGen::Get(this);
	if (!IsValid(TetherPointGen))
		return;
	ActivePatrolPointTags =  TetherPointGen->GetClosestTetherPointTags(GetOwnerLocation());
	if (ActivePatrolPointTags.HasTag(Sf_GameplayTags::Gameplay::PatrolAreaMarkerTypes::Cover::Low))
		GetOwningCharacter()->Crouch();
}

ASf_PatrolArea* UCF_PatrolArea::GetCurrentPatrolArea()
{
	return CurrentPatrolArea;
}

FGameplayTagContainer UCF_PatrolArea::GetActivePatrolPointTags() const
{
	return ActivePatrolPointTags;
}

