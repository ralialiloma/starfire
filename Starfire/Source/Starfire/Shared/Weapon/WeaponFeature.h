#pragma once
#include "WeaponFeatureConfig.h"
#include "WeaponFeature.generated.h"


#define VALIDATE_WF_CONFIG(Type, ConfigVar) \
ConfigVar = Cast<Type>(GetGenericConfig()); \
if (!IsValid(ConfigVar)) \
{ \
	USF_DebugFunctionLibrary::Sf_ThrowError(this,FString::Printf(TEXT("Invalid %s Config!"),*ConfigVar->GetName()));\
return; \
}

class AWeaponBase;
UCLASS(BlueprintType,Blueprintable,Abstract)
class UWeaponFeature: public UObject
{
	GENERATED_BODY()

public:
	void Init(AWeaponBase* WeaponBase, UWeaponFeatureConfig* Config);

	UFUNCTION(BlueprintNativeEvent)
	void OnInit();
	UFUNCTION(BlueprintNativeEvent)
	void OnBeginPlay();
	UFUNCTION(BlueprintNativeEvent)
	void OnTick(float OnTick);
	UFUNCTION(BlueprintNativeEvent)
	void OnEndPlay(const EEndPlayReason::Type EndPlayReason);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnFire();
	UFUNCTION(BlueprintNativeEvent)
	void OnReload();

protected:
	UWeaponFeatureConfig* GetGenericConfig();
	AWeaponBase* GetOwningWeapon();

private:
	UPROPERTY()
	AWeaponBase* OwningWeapon;

	UPROPERTY()
	UWeaponFeatureConfig* GenericConfig;
};