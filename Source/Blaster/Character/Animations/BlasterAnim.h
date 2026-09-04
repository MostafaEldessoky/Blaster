// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BlasterAnim.generated.h"

class UAimOffsetBlendSpace;
class ABlasterCharacter;
/**
 * 
 */
UCLASS()
class BLASTER_API UBlasterAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	
	UPROPERTY(BlueprintReadOnly,Category="CPPVars")
	float XYSpeed;
	
	UPROPERTY(BlueprintReadOnly,Category="CPPVars")
    float XSpeed;
    	
    UPROPERTY(BlueprintReadOnly,Category="CPPVars")
    float YSpeed;
	
	UPROPERTY(BlueprintReadOnly,Category="CPPVars")
	float XYAngle;
	
	UPROPERTY(BlueprintReadOnly,Category="CPPVars")
	float XZAngle;
	
	UPROPERTY(BlueprintReadOnly,Category="CPPVars")
	bool bIsFalling;
	
	UPROPERTY(BlueprintReadOnly,Category="CPPVars")
	bool bHasWeapon;
	
	UPROPERTY(BlueprintReadOnly,Category="CPPVars")
	bool bIsCrouching;
	
	UPROPERTY(BlueprintReadOnly,Category="CPPVars")
	bool bISAiming;
	
	UPROPERTY(BlueprintReadOnly,Category="CPPVars")
	bool bISDead;

private:
                                                                    	
	UPROPERTY()
	ABlasterCharacter* BlasterPtr{nullptr};
	
	void GetOwnedCharacter();

};
