#include "BaseStateFeatureDefinition.h"

FBaseStateFeatureDefintion::FBaseStateFeatureDefintion()
{
}

void FBaseStateFeatureDefintion::OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName)
{
	FTableRowBase::OnDataTableChanged(InDataTable, InRowName);
}
