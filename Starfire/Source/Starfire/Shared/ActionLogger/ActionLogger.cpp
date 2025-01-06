#include "ActionLogger.h"

FDateTime FCachedActionLogMessage::GetTimeStamp() const
{
	return Timestamp;
}

FGuid FCachedActionLogMessage::GetGuid() const
{
	return Guid;
}

void UActionLoggerSubsSystem::ReportAction(const FActionLog ActionLogMessage)
{
	FCachedActionLogMessage CachedActionLogMessage =  FCachedActionLogMessage(ActionLogMessage);
	SavedMessages.Add(CachedActionLogMessage);
	OnActionReport.Broadcast(ActionLogMessage,GetMessageCount(ActionLogMessage));
}

TArray<FCachedActionLogMessage> UActionLoggerSubsSystem::GetMessages()
{
	return SavedMessages;
}

int UActionLoggerSubsSystem::GetMessageCount(const FActionLog MessageToLookFor)
{
	int Counter = 0;
	for (int i = 0; i < SavedMessages.Num(); i++)
	{
		if (SavedMessages[i].Message == MessageToLookFor)
			Counter++;
	}
	return Counter;
}

int UActionLoggerSubsSystem::GetMessageCountSince(const FActionLog MessageToLookFor, const FDateTime Time)
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
