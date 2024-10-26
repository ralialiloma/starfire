#pragma once
#include "CoreMinimal.h"
#include "Starfire/Sf_Bases/Sf_CharacterFeature.h"
#include "CF_Cover.generated.h"


UCLASS(Blueprintable)
class STARFIRE_API UCF_Cover : public USf_CharacterFeature
{
	GENERATED_BODY()

public:
	virtual void Initialize(ASf_TP_Character* Holder) override;
	
#pragma region Functions
public:
	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Cover")
	bool EnterCover();

	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Cover")
	bool VerifyCover(FVector LocationToVerify);

	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Cover")
	bool ExitCover();

	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Cover")
	bool IsInHighCoverState() const;

	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Cover")
	bool IsInCoverState() const;

	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Cover")
	bool IsInLowCoverState() const;


private:
	bool CanBeHitByPlayer(float HeightOffset) const;
	bool CanBeHitByPlayer(FVector LocationToVerify) const;
#pragma endregion
	
#pragma region Properties
protected:
	UPROPERTY(EditDefaultsOnly)
	float MinCoverHeight = 120;

	UPROPERTY(EditDefaultsOnly)
	float MaxCrouchCoverHeight = 60;
private:
	UPROPERTY()
	bool bIsInHighCoverState = false;
	UPROPERTY()
	bool bIsInLowCoverState = false;
#pragma endregion


};