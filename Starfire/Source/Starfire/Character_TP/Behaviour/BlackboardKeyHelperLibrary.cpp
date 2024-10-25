#include "BlackboardKeyHelperLibrary.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"

void UBlackboardKeyHelperLibrary::SetFloatValue(UBlackboardComponent* BlackboardComp, EFloatBlackboardKey FloatBlackboardKey, float Value)
{
    if (!IsValid(BlackboardComp))
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Cannot set value for %s"), *USf_FunctionLibrary::GetEnumAsString<EFloatBlackboardKey>(FloatBlackboardKey));
        return;
    }

    const FName KeyName = USf_FunctionLibrary::GetEnumAsName<EFloatBlackboardKey>(FloatBlackboardKey);
    const FBlackboard::FKey KeyID = BlackboardComp->GetKeyID(KeyName);
    if (KeyID == FBlackboard::InvalidKey)
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Blackboard key %s does not exist in %s"), *KeyName.ToString(), *BlackboardComp->GetName());
        return;
    }
    
    BlackboardComp->SetValueAsFloat(KeyName, Value);
}

void UBlackboardKeyHelperLibrary::SetIntValue(UBlackboardComponent* BlackboardComp, EIntBlackboardKey IntBlackboardKey, int32 Value)
{
    if (!IsValid(BlackboardComp))
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Cannot set value for %s"), *USf_FunctionLibrary::GetEnumAsString<EIntBlackboardKey>(IntBlackboardKey));
        return;
    }

    const FName KeyName = USf_FunctionLibrary::GetEnumAsName<EIntBlackboardKey>(IntBlackboardKey);
    const FBlackboard::FKey KeyID = BlackboardComp->GetKeyID(KeyName);
    if (KeyID == FBlackboard::InvalidKey)
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Blackboard key %s does not exist in %s"), *KeyName.ToString(), *BlackboardComp->GetName());
        return;
    }
    
    BlackboardComp->SetValueAsInt(KeyName, Value);
}

void UBlackboardKeyHelperLibrary::SetBoolValue(UBlackboardComponent* BlackboardComp, EBoolBlackboardKey BoolBlackboardKey, bool Value)
{
    if (!IsValid(BlackboardComp))
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Cannot set value for %s"), *USf_FunctionLibrary::GetEnumAsString<EBoolBlackboardKey>(BoolBlackboardKey));
        return;
    }

    const FName KeyName = USf_FunctionLibrary::GetEnumAsName<EBoolBlackboardKey>(BoolBlackboardKey);
    const FBlackboard::FKey KeyID = BlackboardComp->GetKeyID(KeyName);
    if (KeyID == FBlackboard::InvalidKey)
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Blackboard key %s does not exist in %s"), *KeyName.ToString(), *BlackboardComp->GetName());
        return;
    }
    
    BlackboardComp->SetValueAsBool(KeyName, Value);
}

void UBlackboardKeyHelperLibrary::SetVectorValue(UBlackboardComponent* BlackboardComp, ELocationBlackboardKey ActorBlackboardKey, FVector Value)
{
    if (!IsValid(BlackboardComp))
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Cannot set value for %s"), *USf_FunctionLibrary::GetEnumAsString<ELocationBlackboardKey>(ActorBlackboardKey));
        return;
    }

    const FName KeyName = USf_FunctionLibrary::GetEnumAsName<ELocationBlackboardKey>(ActorBlackboardKey);
    const FBlackboard::FKey KeyID = BlackboardComp->GetKeyID(KeyName);
    if (KeyID == FBlackboard::InvalidKey)
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Blackboard key %s does not exist in %s"), *KeyName.ToString(), *BlackboardComp->GetName());
        return;
    }
    
    BlackboardComp->SetValueAsVector(KeyName, Value);
}

float UBlackboardKeyHelperLibrary::GetFloatValue(UBlackboardComponent* BlackboardComp, EFloatBlackboardKey FloatBlackboardKey)
{
    if (!IsValid(BlackboardComp))
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Cannot get value for %s"), *USf_FunctionLibrary::GetEnumAsString<EFloatBlackboardKey>(FloatBlackboardKey));
        return 0.0f;
    }

    const FName KeyName = USf_FunctionLibrary::GetEnumAsName<EFloatBlackboardKey>(FloatBlackboardKey);
    const FBlackboard::FKey KeyID = BlackboardComp->GetKeyID(KeyName);
    if (KeyID == FBlackboard::InvalidKey)
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Blackboard key %s does not exist in %s"), *KeyName.ToString(), *BlackboardComp->GetName());
        return 0;
    }
    
    return BlackboardComp->GetValueAsFloat(KeyName);
}

int32 UBlackboardKeyHelperLibrary::GetIntValue(UBlackboardComponent* BlackboardComp, EIntBlackboardKey IntBlackboardKey)
{
    if (!IsValid(BlackboardComp))
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Cannot get value for %s"), *USf_FunctionLibrary::GetEnumAsString<EIntBlackboardKey>(IntBlackboardKey));
        return 0;
    }

    const FName KeyName = USf_FunctionLibrary::GetEnumAsName<EIntBlackboardKey>(IntBlackboardKey);
    return BlackboardComp->GetValueAsInt(KeyName);
}

bool UBlackboardKeyHelperLibrary::GetBoolValue(UBlackboardComponent* BlackboardComp, EBoolBlackboardKey BoolBlackboardKey)
{
    if (!IsValid(BlackboardComp))
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Cannot get value for %s"), *USf_FunctionLibrary::GetEnumAsString<EBoolBlackboardKey>(BoolBlackboardKey));
        return false;
    }

    const FName KeyName = USf_FunctionLibrary::GetEnumAsName<EBoolBlackboardKey>(BoolBlackboardKey);
    return BlackboardComp->GetValueAsBool(KeyName);
}

FVector UBlackboardKeyHelperLibrary::GetVectorValue(UBlackboardComponent* BlackboardComp, ELocationBlackboardKey ActorBlackboardKey)
{
    if (!IsValid(BlackboardComp))
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Cannot get value for %s"), *USf_FunctionLibrary::GetEnumAsString<ELocationBlackboardKey>(ActorBlackboardKey));
        return FVector(0,0,0);
    }

    const FName KeyName = USf_FunctionLibrary::GetEnumAsName<ELocationBlackboardKey>(ActorBlackboardKey);
    const FBlackboard::FKey KeyID = BlackboardComp->GetKeyID(KeyName);
    if (KeyID == FBlackboard::InvalidKey)
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Blackboard key %s does not exist in %s"), *KeyName.ToString(), *BlackboardComp->GetName());
        return FVector(0,0,0);
    }
    
    return BlackboardComp->GetValueAsVector(KeyName);
}

void UBlackboardKeyHelperLibrary::ClearFloatValue(UBlackboardComponent* BlackboardComp, EFloatBlackboardKey FloatBlackboardKey)
{
    if (!IsValid(BlackboardComp))
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Cannot clear value for %s"), *USf_FunctionLibrary::GetEnumAsString<EFloatBlackboardKey>(FloatBlackboardKey));
        return;
    }

    const FName KeyName = USf_FunctionLibrary::GetEnumAsName<EFloatBlackboardKey>(FloatBlackboardKey);
    const FBlackboard::FKey KeyID = BlackboardComp->GetKeyID(KeyName);
    if (KeyID == FBlackboard::InvalidKey)
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Blackboard key %s does not exist in %s"), *KeyName.ToString(), *BlackboardComp->GetName());
        return;
    }
    
    BlackboardComp->ClearValue(KeyName);
}


void UBlackboardKeyHelperLibrary::ClearIntValue(UBlackboardComponent* BlackboardComp, EIntBlackboardKey IntBlackboardKey)
{
    if (!IsValid(BlackboardComp))
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Cannot clear value for %s"), *USf_FunctionLibrary::GetEnumAsString<EIntBlackboardKey>(IntBlackboardKey));
        return;
    }

    const FName KeyName = USf_FunctionLibrary::GetEnumAsName<EIntBlackboardKey>(IntBlackboardKey);
    const FBlackboard::FKey KeyID = BlackboardComp->GetKeyID(KeyName);
    if (KeyID == FBlackboard::InvalidKey)
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Blackboard key %s does not exist in %s"), *KeyName.ToString(), *BlackboardComp->GetName());
        return;
    }
    
    BlackboardComp->ClearValue(KeyName);
}

void UBlackboardKeyHelperLibrary::ClearBoolValue(UBlackboardComponent* BlackboardComp, EBoolBlackboardKey BoolBlackboardKey)
{
    if (!IsValid(BlackboardComp))
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Cannot clear value for %s"), *USf_FunctionLibrary::GetEnumAsString<EBoolBlackboardKey>(BoolBlackboardKey));
        return;
    }

    const FName KeyName = USf_FunctionLibrary::GetEnumAsName<EBoolBlackboardKey>(BoolBlackboardKey);
    const FBlackboard::FKey KeyID = BlackboardComp->GetKeyID(KeyName);
    if (KeyID == FBlackboard::InvalidKey)
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Blackboard key %s does not exist in %s"), *KeyName.ToString(), *BlackboardComp->GetName());
        return;
    }
    
    BlackboardComp->ClearValue(KeyName);
}

void UBlackboardKeyHelperLibrary::ClearVectorValue(UBlackboardComponent* BlackboardComp, ELocationBlackboardKey ActorBlackboardKey)
{
    if (!IsValid(BlackboardComp))
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Cannot clear value for %s"), *USf_FunctionLibrary::GetEnumAsString<ELocationBlackboardKey>(ActorBlackboardKey));
        return;
    }

    const FName KeyName = USf_FunctionLibrary::GetEnumAsName<ELocationBlackboardKey>(ActorBlackboardKey);
    const FBlackboard::FKey KeyID = BlackboardComp->GetKeyID(KeyName);
    if (KeyID == FBlackboard::InvalidKey)
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Blackboard key %s does not exist in %s"), *KeyName.ToString(), *BlackboardComp->GetName());
        return;
    }
    
    BlackboardComp->ClearValue(KeyName);
}

bool UBlackboardKeyHelperLibrary::IsSetVectorValue(UBlackboardComponent* BlackboardComp, ELocationBlackboardKey VectorBlackboardKey)
{
    if (!IsValid(BlackboardComp))
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Cannot check set vector value for %s"), *USf_FunctionLibrary::GetEnumAsString<ELocationBlackboardKey>(VectorBlackboardKey));
        return false;
    }

    const FName KeyName = USf_FunctionLibrary::GetEnumAsName<ELocationBlackboardKey>(VectorBlackboardKey);
    const FBlackboard::FKey KeyID = BlackboardComp->GetKeyID(KeyName);
    if (KeyID == FBlackboard::InvalidKey)
    {
        UE_LOG(SF_BlackboardKeyHelper, Error, TEXT("Blackboard key %s does not exist in %s"), *KeyName.ToString(), *BlackboardComp->GetName());
        return false;
    }

    return BlackboardComp->IsVectorValueSet(KeyName);
}
