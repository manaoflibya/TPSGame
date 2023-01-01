// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSCharacter.h"
#include "TPSAuroraCharacter.generated.h"

/**
 * 
 */
struct AuroraInfo
{
public:
	int32 attackCount;
	float animPlayRateTime;
	bool isDuringAttack;
	float auroraAttackSpeed;
};

UENUM()
enum AttackCount
{
	Attack_1 = 1,
	Attack_2 = 2,
	Attack_3 = 3,
	Attack_4 = 4,
};

UCLASS()
class TPSGAME_API ATPSAuroraCharacter : public ATPSCharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	ATPSAuroraCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

protected:
	void MoveForward(float value);

	void MoveRight(float value);

	void TurnAtRate(float rate);

	void LookUpAtRate(float rate);

	void Attack();

	void Attack_End();
private:
	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* Attack_AnimMontage_1;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* Attack_AnimMontage_2;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* Attack_AnimMontage_3;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* Attack_AnimMontage_4;

	//UPROPERTY(EditDefaultsOnly, Category = Pawn)
	//	UAnimMontage* BeHit_AnimMontage;

	//UPROPERTY(EditDefaultsOnly, Category = Pawn)
	//	UAnimMontage* Death_AnimMontage;
private:
	AuroraInfo* auroraInfo;
	int32 attackMaxCount;
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)override;
public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom;}
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera;}
};
