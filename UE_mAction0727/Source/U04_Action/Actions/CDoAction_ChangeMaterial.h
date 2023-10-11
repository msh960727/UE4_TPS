// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction_FireAttack.h"
#include "CDoAction_ChangeMaterial.generated.h"

/**
 * 
 */
UCLASS()
class U04_ACTION_API ACDoAction_ChangeMaterial : public ACDoAction_FireAttack
{
	GENERATED_BODY()

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;

protected:
	void OnBulletBeginOverlap(FHitResult InHitResult) override;
	
};
