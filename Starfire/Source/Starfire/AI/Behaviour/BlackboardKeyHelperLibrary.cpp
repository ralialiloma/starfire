#include "BlackboardKeyHelperLibrary.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"

void UBlackboardKeyHelperLibrary::SetFloatValue(UBlackboardComponent* BlackboardComp, EFloatBlackboardKey FloatBlackboardKey, float Value)
{
    if (!BlackboardComp)
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Cannot set value for %s"), *USf_FunctionLibrary::GetEnumAsString<EFloatBlackboardKey>(FloatBlackboardKey));
        return;
    }

    const FName KeyName = USf_FunctionLibrary::GetEnumAsName<EFloatBlackboardKey>(FloatBlackboardKey);
    BlackboardComp->SetValueAsFloat(KeyName, Value);
}

void UBlackboardKeyHelperLibrary::SetIntValue(UBlackboardComponent* BlackboardComp, EIntBlackboardKey IntBlackboardKey, int32 Value)
{
    if (!BlackboardComp)
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Cannot set value for %s"), *USf_FunctionLibrary::GetEnumAsString<EIntBlackboardKey>(IntBlackboardKey));
        return;
    }

    const FName KeyName = USf_FunctionLibrary::GetEnumAsName<EIntBlackboardKey>(IntBlackboardKey);
    BlackboardComp->SetValueAsInt(KeyName, Value);
}

void UBlackboardKeyHelperLibrary::SetBoolValue(UBlackboardComponent* BlackboardComp, EBoolBlackboardKey BoolBlackboardKey, bool Value)
{
    if (!BlackboardComp)
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Cannot set value for %s"), *USf_FunctionLibrary::GetEnumAsString<EBoolBlackboardKey>(BoolBlackboardKey));
        return;
    }

    const FName KeyName = USf_FunctionLibrary::GetEnumAsName<EBoolBlackboardKey>(BoolBlackboardKey);
    BlackboardComp->SetValueAsBool(KeyName, Value);
}

void UBlackboardKeyHelperLibrary::SetActorValue(UBlackboardComponent* BlackboardComp, EActorBlackboardKey ActorBlackboardKey, AActor* Value)
{
    if (!BlackboardComp)
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Cannot set value for %s"), *USf_FunctionLibrary::GetEnumAsString<EActorBlackboardKey>(ActorBlackboardKey));
        return;
    }

    const FName KeyName = USf_FunctionLibrary::GetEnumAsName<EActorBlackboardKey>(ActorBlackboardKey);
    BlackboardComp->SetValueAsObject(KeyName, Value);
}

float UBlackboardKeyHelperLibrary::GetFloatValue(UBlackboardComponent* BlackboardComp, EFloatBlackboardKey FloatBlackboardKey)
{
    if (!BlackboardComp)
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Cannot get value for %s"), *USf_FunctionLibrary::GetEnumAsString<EFloatBlackboardKey>(FloatBlackboardKey));
        return 0.0f;
    }

    const FName KeyName = USf_FunctionLibrary::GetEnumAsName<EFloatBlackboardKey>(FloatBlackboardKey);
    return BlackboardComp->GetValueAsFloat(KeyName);
}

int32 UBlackboardKeyHelperLibrary::GetIntValue(UBlackboardComponent* BlackboardComp, EIntBlackboardKey IntBlackboardKey)
{
    if (!BlackboardComp)
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Cannot get value for %s"), *USf_FunctionLibrary::GetEnumAsString<EIntBlackboardKey>(IntBlackboardKey));
        return 0;
    }

    const FName KeyName = USf_FunctionLibrary::GetEnumAsName<EIntBlackboardKey>(IntBlackboardKey);
    return BlackboardComp->GetValueAsInt(KeyName);
}

bool UBlackboardKeyHelperLibrary::GetBoolValue(UBlackboardComponent* BlackboardComp, EBoolBlackboardKey BoolBlackboardKey)
{
    if (!BlackboardComp)
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Cannot get value for %s"), *USf_FunctionLibrary::GetEnumAsString<EBoolBlackboardKey>(BoolBlackboardKey));
        return false;
    }

    const FName KeyName = USf_FunctionLibrary::GetEnumAsName<EBoolBlackboardKey>(BoolBlackboardKey);
    return BlackboardComp->GetValueAsBool(KeyName);
}

AActor* UBlackboardKeyHelperLibrary::GetActorValue(UBlackboardComponent* BlackboardComp, EActorBlackboardKey ActorBlackboardKey)
{
    if (!BlackboardComp)
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Cannot get value for %s"), *USf_FunctionLibrary::GetEnumAsString<EActorBlackboardKey>(ActorBlackboardKey));
        return nullptr;
    }

    const FName KeyName = USf_FunctionLibrary::GetEnumAsName<EActorBlackboardKey>(ActorBlackboardKey);
    return Cast<AActor>(BlackboardComp->GetValueAsObject(KeyName));
}