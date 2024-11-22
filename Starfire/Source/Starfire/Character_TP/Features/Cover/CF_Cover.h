#pragma once
#include "CoreMinimal.h"
#include "CF_Cover_Config.h"
#include "Starfire/Shared/CharacterFeature/Sf_CharacterFeature.h"
#include "CF_Cover.generated.h"



UCLASS(Blueprintable)
class STARFIRE_API UCF_Cover : public USf_CharacterFeature
{
	GENERATED_BODY()

public:
	virtual void Initialize(ASf_TP_Character* Holder, const USf_CharacterFeature_Config* InConfig) override;
	
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

	UPROPERTY()
	const UCF_Cover_Config* CoverConfig;
	

private:
	UPROPERTY()
	bool bIsInHighCoverState = false;
	UPROPERTY()
	bool bIsInLowCoverState = false;
#pragma endregion


};