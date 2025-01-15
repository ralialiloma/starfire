#include "ActionLogger.h"

#include "NativeGameplayTags.h"

UActionLoggerSubSystem* UActionLoggerSubSystem::Get(const UWorld* World)
{
	if (!IsValid(World))
		return nullptr;
		
	return World->GetSubsystem<UActionLoggerSubSystem>();
}

FActionLog::FActionLog(const FGameplayTag RelatedTagIn, const FVector& LocationIn)
{
	RelatedTag = RelatedTagIn;
	Location = LocationIn;
}

FActionLog::FActionLog(const FGameplayTag RelatedTagIn)
{
	RelatedTag = RelatedTagIn;
	Location = FVector::ZeroVector;
}

FActionLog::FActionLog(const FNativeGameplayTag& RelatedTagIn)
{
	RelatedTag = RelatedTagIn;
	Location = FVector::ZeroVector;
}

FActionLog::FActionLog()
{
	RelatedTag = FGameplayTag::EmptyTag;
	Location = FVector::ZeroVector;
}

FDateTime FCachedActionLogMessage::GetTimeStamp() const
{
	return Timestamp;
}

FGuid FCachedActionLogMessage::GetGuid() const
{
	return Guid;
}

void UActionLoggerSubSystem::ReportAction(const FActionLog ActionLogMessage)
{
	const FCachedActionLogMessage CachedActionLogMessage =  FCachedActionLogMessage(ActionLogMessage);
	SavedMessages.Add(CachedActionLogMessage);
	OnActionReport.Broadcast(ActionLogMessage,GetMessageCount(ActionLogMessage));
}

TArray<FCachedActionLogMessage> UActionLoggerSubSystem::GetMessages()
{
	return SavedMessages;
}

int UActionLoggerSubSystem::GetMessageCount(const FActionLog MessageToLookFor)
{
	int Counter = 0;
	for (int i = 0; i < SavedMessages.Num(); i++)
	{
		if (SavedMessages[i].Message == MessageToLookFor)
			Counter++;
	}
	return Counter;
}

int UActionLoggerSubSystem::GetMessageCountSince(const FActionLog MessageToLookFor, const FDateTime Time)
{
	int Counter = 0;
	for (int i = 0; i < SavedMessages.Num(); i++)
	{

		if (SavedMessages[i].Message == MessageToLookFor && SavedMessages[i].GetTimeStamp() <= Time)
		{
			Counter++;
		}
			
	}
	return Counter;
}
