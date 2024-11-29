#include "CF_AreaBox.h"


void UCF_AreaBox::Initialize(ASf_TP_Character* Holder, const USf_CharacterFeature_Config* InConfig)
{
	Super::Initialize(Holder, InConfig);
	VALIDATE_CONFIG(UCF_AreaBox_Config,AreaBox_Config)
}
