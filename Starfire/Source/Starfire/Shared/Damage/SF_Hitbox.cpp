
#include "SF_Hitbox.h"


USf_Hitbox::USf_Hitbox(): DamageMultiplier(1.0f)
{
	ShapeColor = FColor::Green;
	UPrimitiveComponent::SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	UPrimitiveComponent::SetCollisionObjectType(ECC_Pawn);
	UPrimitiveComponent::SetCollisionResponseToAllChannels(ECR_Overlap);
	UPrimitiveComponent::SetCollisionProfileName("Hitbox",true);
}
