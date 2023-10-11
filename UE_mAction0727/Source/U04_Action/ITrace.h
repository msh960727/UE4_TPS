
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ITrace.generated.h"

UINTERFACE(MinimalAPI)
class UITrace : public UInterface
{
	GENERATED_BODY()
};


class U04_ACTION_API IITrace
{
	GENERATED_BODY()

public:
	virtual void GetLocationAndDirection(FVector& OutStart, FVector& OutEnd, FVector& OutDirection) = 0;
};
