// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSAuroraCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"


ATPSAuroraCharacter::ATPSAuroraCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	BaseTurnRate = 45.0f;
	BaseLookUpRate = 45.0f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	attackMaxCount = 4;

	auroraInfo = new AuroraInfo();
	auroraInfo->attackCount = 0;
	auroraInfo->animPlayRateTime = 1.0f;
	auroraInfo->isDuringAttack = false;
	auroraInfo->auroraAttackSpeed = 0.4;
}

void ATPSAuroraCharacter::MoveForward(float value)
{
	if ((Controller != nullptr) && (value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
}

void ATPSAuroraCharacter::MoveRight(float value)
{
	if ((Controller != nullptr) && (value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);
	}
}

void ATPSAuroraCharacter::TurnAtRate(float rate)
{
	AddControllerYawInput(rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATPSAuroraCharacter::LookUpAtRate(float rate)
{
	AddControllerPitchInput(rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATPSAuroraCharacter::Attack()
{
	if (auroraInfo->isDuringAttack == true)
	{
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, TEXT("is During Attack"));

		return;
	}

	if (auroraInfo->attackCount >= attackMaxCount)
	{
		auroraInfo->attackCount = 0;
	}

	auroraInfo->attackCount++;

	switch (auroraInfo->attackCount)
	{
		case Attack_1:
		{
			if (IsValid(Attack_AnimMontage_1))
			{
				PlayAnimMontage(Attack_AnimMontage_1, auroraInfo->animPlayRateTime);
			}
		}
		break;
		case Attack_2:
		{
			if (IsValid(Attack_AnimMontage_2))
			{
				PlayAnimMontage(Attack_AnimMontage_2, auroraInfo->animPlayRateTime);
			}		
		}
		break;
		case Attack_3:
		{
			if (IsValid(Attack_AnimMontage_3))
			{
				PlayAnimMontage(Attack_AnimMontage_3, auroraInfo->animPlayRateTime);
			}
		}
		break;
		case Attack_4:
		{
			if (IsValid(Attack_AnimMontage_4))
			{
				PlayAnimMontage(Attack_AnimMontage_4, auroraInfo->animPlayRateTime);
			}		
		}
		break;
	default:
		break;
	}

	auroraInfo->isDuringAttack = true;

	FTimerHandle TH_Attack_End;

	GetWorldTimerManager().SetTimer(TH_Attack_End, this, &ATPSAuroraCharacter::Attack_End, auroraInfo->auroraAttackSpeed, false);
}

void ATPSAuroraCharacter::Attack_End()
{
	auroraInfo->isDuringAttack = false;
}

void ATPSAuroraCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ATPSAuroraCharacter::Attack);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSAuroraCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSAuroraCharacter::MoveRight);

	//추가 해야함
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATPSAuroraCharacter::TurnAtRate);
	//추가 해야함
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATPSAuroraCharacter::LookUpAtRate);

}
