#pragma once
#if WITH_EDITOR

struct EditorHelpers
{
public:
	template <typename T>
	static bool GetRawData(const TSharedPtr<IPropertyHandle>& PropertyHandle, T*& OutStruct);

	static void DisplayChildError(IDetailChildrenBuilder& ChildBuilder, const FString& Text = "An Error Occured.");
	static void DisplayEmptyChildren(IDetailChildrenBuilder& ChildBuilder);
	static void DisplayCustomTextChild(IDetailChildrenBuilder& ChildBuilder, const FString& NameContent = "An Error Occured.", const FString& ValueContent = "An Error Occured.");

	static void DisplayDefaultHeader(const TSharedRef<IPropertyHandle>& PropertyHandle, FDetailWidgetRow& HeaderRow);
	static void DisplayDefaultChildren(const TSharedRef<IPropertyHandle>& PropertyHandle, IDetailChildrenBuilder& ChildBuilder, const TArray<TSharedPtr<IPropertyHandle>>& ExcludedHandles = {});
};

template <typename T>
bool EditorHelpers::GetRawData(const TSharedPtr<IPropertyHandle>& PropertyHandle, T*& OutStruct)
{
	TArray<void*> RawData;
	PropertyHandle->AccessRawData(RawData);

	if (RawData.Num() > 0)
	{
		if (T* EditedStruct = static_cast<T*>(RawData[0]))
		{
			OutStruct = EditedStruct;
			return true;
		}
	}
	return false;
}

#endif
