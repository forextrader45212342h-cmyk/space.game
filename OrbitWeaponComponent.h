#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitWeaponComponent.generated.h"

UCLASS(ClassGroup=(Orbit), meta=(BlueprintSpawnableComponent))
class PROJECTORBIT_API UOrbitWeaponComponent
    : public UActorComponent
{
    GENERATED_BODY()

public:
    UOrbitWeaponComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
    int32 MagazineSize = 15;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
    int32 AmmoInMagazine = 15;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
    int32 ReserveAmmo = 60;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
    float Damage = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
    float FireCooldown = 0.15f;

    UFUNCTION(BlueprintCallable, Category="Weapon")
    bool TryFire(
        const FVector& Start,
        const FVector& Direction);

    UFUNCTION(BlueprintCallable, Category="Weapon")
    bool Reload();

protected:
    virtual void BeginPlay() override;

private:
    float LastFireTime = -1000.0f;
};
