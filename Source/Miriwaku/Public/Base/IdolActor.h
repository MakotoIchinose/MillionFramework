// Copyright (C) TheHoodieGuy02

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IdolList.h"
#include "IdolActor.generated.h"

class USkeletalMesh;
class USkeletalMeshComponent;
class UMaterialInterface;

UCLASS()
class MIRIWAKU_API AIdolActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AIdolActor(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the object is constructed
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Variables
public:
	
	// The visual mesh of the body part
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	USkeletalMesh* VisualBodyMesh;

	// The visual mesh of the head part
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	USkeletalMesh* VisualHeadMesh;

	// The visual mesh of the accessories
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	USkeletalMesh* VisualAccessoriesMesh;

	// The copy pose animation BP of the body part.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	TSubclassOf<UAnimInstance> VisualBodyAnimBP;

	// The copy pose animation BP of the head part
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	TSubclassOf<UAnimInstance> VisualHeadAnimBP;
	
	// Whether to use animation BP for accessories or not. If disabled, it will disable its own physics anim BP and set master pose component to head mesh.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	bool bAccessoriesUseAnimBP;

	// The copy pose animation BP of the accessories. Leaving it empty is the same as disabling animBP for accessories.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh", meta=(EditConditionHides="bAccessoriesUseAnimBP"))
	TSubclassOf<UAnimInstance> VisualAccessoriesAnimBP;

	UPROPERTY()
	FName BustShrinkMorphTarget;

	UPROPERTY()
	FName BustEnlargeMorphTarget;

	// Adjust the skin tone. At 0, the tone is peachy. At -1, the tone is slightly yellow. At +1, the tone is tan (e.g. Hibiki).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes", meta=(ClampMin=-1.0f, ClampMax=1.0f, UIMin=-1.0f, UIMax=1.0f))
	float SkinTone = 0.0f;

	// Adjust the idol's icon colour. Will affect some wildcard outfits.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	FLinearColor IconColour = FLinearColor(0.765f, 0.04f, 0.03f, 1.0f);
	// Defaults to Haruka's colour.

	/** Set the size of this idol's bust, in cm. Value ranges from 70 to 94, and only affect wildcard outfit/body meshes. 
	  * Outfit/body meshes that are specific to one idol have no effect. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes", Interp, meta=(ClampMin=70, ClampMax=94, UIMin=70, UIMax=94))
	int32 BustSize = 82;

	/*
	 * Toggle the idol's physics. Disable this before baking Control Rig animations to prevent
	 * the physics calculation baked in.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDisablePostProcessPhysics;

	// The proxy Control Rig mesh. This will not be visible, but used as pose reference for the other meshes.
	UPROPERTY(EditDefaultsOnly, Category = "Mesh", AdvancedDisplay)
	USkeletalMesh* ProxyMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Mesh", AdvancedDisplay)
	UMaterialInterface* OutlineMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh", AdvancedDisplay, meta=(DisplayName="Invisible Material"))
	UMaterialInterface* InvisoMaterial;

	UPROPERTY()
	int32 PreviousBustSize;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> BodyMeshMaterials;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> HeadMeshMaterials;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> AccessoriesMeshMaterials;

	// Components
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* ControlRigProxy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* VisualMeshBodyComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* VisualMeshHeadComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* VisualMeshAccessoriesComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* OutlineMeshBodyComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* OutlineMeshHeadComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* OutlineMeshAccessoriesComponent;

	// Functions
public:
	
	UFUNCTION(BlueprintCallable)
	void InitMeshes();

	UFUNCTION(BlueprintCallable)
	void UpdateBustSize();
	
};
