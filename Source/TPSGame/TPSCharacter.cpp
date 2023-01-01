// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"

ATPSCharacter::ATPSCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	isDuringAttack = false;
	ComboAttack_Num = 0;
	health = 0.0f;
	maxHealth = 100.0f;

	health = maxHealth;
}

void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATPSCharacter::OnHit(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	//PlayAnimMontage(BeHit_AnimMontage);
	if (DamageTaken > 0.f)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
	}
}

void ATPSCharacter::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	health = FMath::Min(0.f, health);

	UDamageType const* const DamageType = DamageEvent.DamageTypeClass ? Cast<const UDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject()) : GetDefault<UDamageType>();

	Killer = GetDamageInstigator(Killer, *DamageType);

	GetWorldTimerManager().ClearAllTimersForObject(this);

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_Pawn, ECR_Ignore);
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
	}

	if (Controller != NULL)
	{
		Controller->UnPossess();
	}

	//float DeathAnimDuration = PlayAnimMontage(Death_AnimMontage);

	FTimerHandle TimerHandle;
	//GetWorldTimerManager().SetTimer(TimerHandle, this, &ATPSCharacter::DeathAnimationEnd, DeathAnimDuration, false);


}

void ATPSCharacter::DeathAnimationEnd()
{
	this->SetActorHiddenInGame(true);
	SetLifeSpan(0.1f);

}

void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATPSCharacter::Attack_Melee()
{

	//PlayAnimMontage(Attack_AnimMontage, 1.f);

	//if (!isDuringAttack)
	//{
	//	if (ComboAttack_Num < 3)
	//	{
	//		int tmp_Num = rand() % 3 + 1;
	//		FString PlaySection = "Attack_" + FString::FromInt(tmp_Num);
	//		PlayAnimMontage(Attack_AnimMontage, 1.f, FName(*PlaySection));
	//		ComboAttack_Num++;

	//		isDuringAttack = true;

	//	}
	//	else
	//	{
	//		PlayAnimMontage(Attack_AnimMontage, 1.f, FName("Attack_4"));
	//		ComboAttack_Num = 0;
	//	}
	//}
}

void ATPSCharacter::Attack_Melee_End()
{
	isDuringAttack = false;
}

void ATPSCharacter::ShowFX()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFX, GetActorLocation());

}

float ATPSCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (health <= 0.0f)
	{
		return 0.0f;
	}

	const float myGetDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (myGetDamage > 0.f)
	{
		health -= myGetDamage;
	}


	if (health <= 0)
	{
		//PlayAnimMontage(Death_AnimMontage, 1.0f);
		Die(myGetDamage, DamageEvent, EventInstigator, DamageCauser);
	}
	else
	{
		OnHit(myGetDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HP is : %f"), health));
	}


	return myGetDamage;
}

