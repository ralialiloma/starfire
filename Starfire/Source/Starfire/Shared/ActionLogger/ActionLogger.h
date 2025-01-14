#pragma once
#include "GameplayTagContainer.h"
#include "ActionLogger.generated.h"

USTRUCT(BlueprintType)
struct  FActionLog
{
public:
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Gameplay.ActionLogger"))
	FGameplayTag RelatedTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	FActionLog(FGameplayTag RelatedTagIn, const FVector& LocationIn);
	FActionLog(FGameplayTag RelatedTagIn);
	FActionLog();

	bool operator==(const FActionLog& Other) const
	{
		return RelatedTag == Other.RelatedTag
		&& Location == Other.Location;
	}
	
	bool operator!=(const FActionLog& Other) const
	{
		return !(*this == Other);
	}
};

USTRUCT(BlueprintType)
struct  FCachedActionLogMessage
{
public:
	GENERATED_BODY()

public:
	FDateTime GetTimeStamp() const;
	FGuid GetGuid() const;
	
	FCachedActionLogMessage()
	{
		Message = FActionLog();
		Timestamp = FDateTime::Now();
	}

	FCachedActionLogMessage(FActionLog MessageIn)
	{
		Message =  MessageIn;
		Timestamp = FDateTime::Now();
	}
	
public:
	UPROPERTY(BlueprintReadOnly)
	FActionLog Message;
private:
	UPROPERTY()
	FDateTime Timestamp;
	UPROPERTY()
	FGuid Guid;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionReport, FCachedActionLogMessage, NewMessage, int, MessageCount);

UCLASS()
class UActionLoggerSubSystem: public UWorldSubsystem
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly)
	void ReportAction(FActionLog ActionLogMessage);

	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly)
	TArray<FCachedActionLogMessage>GetMessages();

	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly)
	int GetMessageCount(FActionLog MessageToLookFor);

	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly)
	int GetMessageCountSince(FActionLog MessageToLookFor, FDateTime Time);

	static UActionLoggerSubSystem* Get(const UWorld* World);

public:
	UPROPERTY(BlueprintAssignable)
	FOnActionReport OnActionReport;
protected:
	UPROPERTY()
	TArray<FCachedActionLogMessage> SavedMessages = TArray<FCachedActionLogMessage>{};
};
