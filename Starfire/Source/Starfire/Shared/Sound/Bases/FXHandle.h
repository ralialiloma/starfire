#pragma once

#include "FXHandle.generated.h"


USTRUCT(BlueprintType)
struct FFXHandle
{
	GENERATED_BODY()

public:
	
	FFXHandle()
		: Handle()
	{
	}
	
	explicit FFXHandle(const FGuid& InHandle)
		: Handle(InHandle)
	{
	}

	static FFXHandle GenerateNewHandle()
	{
		return FFXHandle(FGuid::NewGuid());
	}
	
	void Invalidate()
	{
		Handle.Invalidate();
	}

	bool IsValid() const
	{
		return Handle.IsValid();
	}

	bool operator==(const FFXHandle& Other) const
	{
		return Handle == Other.Handle;
	}

	bool operator!=(const FFXHandle& Other) const
	{
		return !(*this == Other);
	}

	friend uint32 GetTypeHash(const FFXHandle& FXHandle)
	{
		return GetTypeHash(FXHandle.Handle);
	}

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGuid Handle;
};