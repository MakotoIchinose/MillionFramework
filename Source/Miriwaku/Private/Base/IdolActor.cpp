// Copyright (C) TheHoodieGuy02


#include "Base/IdolActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AIdolActor::AIdolActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//////////////////////////////////////////////////////////////////////////
    // Initialise components
    
	ControlRigProxy = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Control Rig Proxy"));
    VisualMeshBodyComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Visual Mesh Body Component"));
	VisualMeshHeadComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Visual Mesh Head Component"));
	VisualMeshAccessoriesComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Visual Mesh Accessories Component"));
	OutlineMeshBodyComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Outline Mesh Body Component"));
	OutlineMeshHeadComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Outline Mesh Head Component"));
	OutlineMeshAccessoriesComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Outline Mesh Accessories Component"));

	SetRootComponent(ControlRigProxy);
	VisualMeshBodyComponent->SetupAttachment(ControlRigProxy);
	VisualMeshHeadComponent->SetupAttachment(ControlRigProxy);
	VisualMeshAccessoriesComponent->SetupAttachment(ControlRigProxy);
	OutlineMeshBodyComponent->SetupAttachment(VisualMeshBodyComponent);
	OutlineMeshHeadComponent->SetupAttachment(VisualMeshHeadComponent);
	OutlineMeshAccessoriesComponent->SetupAttachment(VisualMeshAccessoriesComponent);

	// Make sure proxy mesh hidden by itself while propagating anims to visual meshes
	ControlRigProxy->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	ControlRigProxy->SetVisibility(false, false);

	OutlineMeshBodyComponent->SetDisablePostProcessBlueprint(true);
	OutlineMeshBodyComponent->CastShadow = false;
	OutlineMeshHeadComponent->SetDisablePostProcessBlueprint(true);
	OutlineMeshHeadComponent->CastShadow = false;
	OutlineMeshAccessoriesComponent->SetDisablePostProcessBlueprint(true);
	OutlineMeshAccessoriesComponent->CastShadow = false;
}



// Called on construction
void AIdolActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	InitMeshes();
	UpdateBustSize();
}


// Called every frame
void AIdolActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	// Real time bust adjustment 
	if (PreviousBustSize != BustSize)
	{
		UpdateBustSize();
		PreviousBustSize = BustSize;
	}
}

void AIdolActor::InitMeshes()
{
	{
		if (ProxyMesh != nullptr && ControlRigProxy->SkeletalMesh != ProxyMesh)
		{
			ControlRigProxy->SetSkeletalMesh(ProxyMesh);
		}
	}
	
	{
		// Check if the mesh is already the same to reduce instructions
		if (VisualMeshBodyComponent->SkeletalMesh != VisualBodyMesh)
		{
			VisualMeshBodyComponent->SetSkeletalMesh(VisualBodyMesh, true);
			
			// Reset materials to the skelmesh asset defaults
			BodyMeshMaterials.Empty();
			TArray<UMaterialInterface*> Materials = VisualMeshBodyComponent->GetMaterials();
			for (int i = 0; i < Materials.Num(); i++)
			{
				BodyMeshMaterials.AddUnique(UMaterialInstanceDynamic::Create(VisualBodyMesh->GetMaterials()[i].MaterialInterface, GetOuter()));
				VisualMeshBodyComponent->SetMaterial(i, BodyMeshMaterials[i]);
			}

			// Clear name caches, then find for bust morph targets
			BustShrinkMorphTarget = FName();
			BustEnlargeMorphTarget = FName();
			TArray<FString> BodyMorphTargets = VisualMeshBodyComponent->SkeletalMesh->K2_GetAllMorphTargetNames();
			for (int i = 0; i < BodyMorphTargets.Num(); i++)
			{
				// Set morph target names
				if (BodyMorphTargets[i].Contains(TEXT("body_gla"), ESearchCase::IgnoreCase, ESearchDir::FromStart))
					BustShrinkMorphTarget = FName(BodyMorphTargets[i]);
				else if (BodyMorphTargets[i].Contains(TEXT("body_sle"), ESearchCase::IgnoreCase, ESearchDir::FromStart))
					BustEnlargeMorphTarget = FName(BodyMorphTargets[i]);
			}
		}

		for (int i = 0; i < BodyMeshMaterials.Num(); i++)
		{
			if (SkinTone < 0.f)
			{
				BodyMeshMaterials[i]->SetVectorParameterValue(TEXT("SkinTone"),
					UKismetMathLibrary::LinearColorLerp(
						FLinearColor(0.947f, 0.705f, 0.620f, 0.7f),
						FLinearColor(0.947f, 0.706f, 0.386f, 0.7f),
						FMath::Abs(SkinTone)
					)
				);
			}
			else
			{
				BodyMeshMaterials[i]->SetVectorParameterValue(TEXT("SkinTone"),
					UKismetMathLibrary::LinearColorLerp(
						FLinearColor(0.947f, 0.705f, 0.620f, 0.7f),
						FLinearColor(0.745f, 0.478f, 0.350f, 0.7f),
						FMath::Abs(SkinTone)
					)
				);
			}
			BodyMeshMaterials[i]->SetVectorParameterValue(TEXT("ClothTint"), IconColour);
		}

		VisualMeshBodyComponent->SetAnimInstanceClass(VisualBodyAnimBP);
	}

	{
		// Check if the mesh is already the same to reduce instructions
		if (VisualMeshHeadComponent->SkeletalMesh != VisualHeadMesh)
		{
			VisualMeshHeadComponent->SetSkeletalMesh(VisualHeadMesh, true);
			// Reset materials
			HeadMeshMaterials.Empty();
			TArray<UMaterialInterface*> Materials = VisualMeshHeadComponent->GetMaterials();
			for (int i = 0; i < Materials.Num(); i++)
			{
				HeadMeshMaterials.AddUnique(UMaterialInstanceDynamic::Create(VisualHeadMesh->GetMaterials()[i].MaterialInterface, GetOuter()));
				VisualMeshHeadComponent->SetMaterial(i, HeadMeshMaterials[i]);
			}
		}

		for (int i = 0; i < HeadMeshMaterials.Num(); i++)
		{
			if (SkinTone < 0.f)
			{
				HeadMeshMaterials[i]->SetVectorParameterValue(TEXT("SkinTone"),
					UKismetMathLibrary::LinearColorLerp(
						FLinearColor(0.947f, 0.705f, 0.620f, 0.7f),
						FLinearColor(0.947f, 0.706f, 0.386f, 0.7f),
						FMath::Abs(SkinTone)
					)
				);
			}
			else
			{
				HeadMeshMaterials[i]->SetVectorParameterValue(TEXT("SkinTone"),
					UKismetMathLibrary::LinearColorLerp(
						FLinearColor(0.947f, 0.705f, 0.620f, 0.7f),
						FLinearColor(0.745f, 0.478f, 0.350f, 0.7f),
						FMath::Abs(SkinTone)
					)
				);
			}
		}

		VisualMeshHeadComponent->SetAnimInstanceClass(VisualHeadAnimBP);
	}

	{
		// Check if the mesh is already the same to reduce instructions
		if (VisualMeshAccessoriesComponent->SkeletalMesh != VisualAccessoriesMesh)
		{
			VisualMeshAccessoriesComponent->SetSkeletalMesh(VisualAccessoriesMesh, true);
			// Reset materials
			AccessoriesMeshMaterials.Empty();
			TArray<UMaterialInterface*> Materials = VisualMeshAccessoriesComponent->GetMaterials();
			for (int i = 0; i < Materials.Num(); i++)
			{
				AccessoriesMeshMaterials.AddUnique(UMaterialInstanceDynamic::Create(VisualAccessoriesMesh->GetMaterials()[i].MaterialInterface, GetOuter()));
				VisualMeshAccessoriesComponent->SetMaterial(i, AccessoriesMeshMaterials[i]);
			}
		}

		for (int i = 0; i < AccessoriesMeshMaterials.Num(); i++)
		{
			AccessoriesMeshMaterials[i]->SetVectorParameterValue(TEXT("ClothTint"), IconColour);
		}

		if (bAccessoriesUseAnimBP || VisualAccessoriesAnimBP)
			VisualMeshAccessoriesComponent->SetAnimInstanceClass(VisualAccessoriesAnimBP);
		else
			VisualMeshAccessoriesComponent->SetMasterPoseComponent(VisualMeshHeadComponent);

	}

	// Fallback to hidden if any outline materials are invalid
	if (OutlineMaterial && InvisoMaterial)
	{
		if (OutlineMeshBodyComponent->SkeletalMesh != VisualMeshBodyComponent->SkeletalMesh)
		{
			OutlineMeshBodyComponent->SetSkeletalMesh(VisualMeshBodyComponent->SkeletalMesh);
		}
		for (int i = 0; i < OutlineMeshBodyComponent->GetMaterials().Num(); i++)
		{
			// Only enable outline for material slots that used for alpha cutout, as inverted hull trick doesn't work with masked opacity.
			FName MaterialName = OutlineMeshBodyComponent->GetMaterialSlotNames()[i];
			if (MaterialName.ToString().Contains(TEXT("alp")) || MaterialName.ToString().Contains(TEXT("cut")))
			{
				OutlineMeshBodyComponent->SetMaterial(i, InvisoMaterial);
			}
			else
			{
				OutlineMeshBodyComponent->SetMaterial(i, OutlineMaterial);
			}
		}

		if (OutlineMeshHeadComponent->SkeletalMesh != VisualMeshHeadComponent->SkeletalMesh)
		{
			OutlineMeshHeadComponent->SetSkeletalMesh(VisualMeshHeadComponent->SkeletalMesh);
		}
		for (int i = 0; i < OutlineMeshHeadComponent->GetMaterials().Num(); i++)
		{
			// Only enable outline on face mesh.
			FName MaterialName = OutlineMeshHeadComponent->GetMaterialSlotNames()[i];
			if (MaterialName.ToString().Contains(TEXT("face")))
			{
				OutlineMeshHeadComponent->SetMaterial(i, OutlineMaterial);
			}
			else
			{
				OutlineMeshHeadComponent->SetMaterial(i, InvisoMaterial);
			}
		}

		if (OutlineMeshAccessoriesComponent->SkeletalMesh != VisualMeshAccessoriesComponent->SkeletalMesh)
		{
			OutlineMeshAccessoriesComponent->SetSkeletalMesh(VisualMeshAccessoriesComponent->SkeletalMesh);
		}
		for (int i = 0; i < OutlineMeshAccessoriesComponent->GetMaterials().Num(); i++)
		{
			// Only enable outline for material slots that used for alpha cutout, as inverted hull trick doesn't work with masked opacity.
			FName MaterialName = OutlineMeshAccessoriesComponent->GetMaterialSlotNames()[i];
			if (MaterialName.ToString().Contains(TEXT("alp")))
			{
				OutlineMeshAccessoriesComponent->SetMaterial(i, OutlineMaterial);
			}
			else
			{
				OutlineMeshAccessoriesComponent->SetMaterial(i, InvisoMaterial);
			}
		}
	}
	else
	{
		OutlineMeshBodyComponent->SetVisibility(false);
		OutlineMeshHeadComponent->SetVisibility(false);
		OutlineMeshAccessoriesComponent->SetVisibility(false);
	}
	
	OutlineMeshBodyComponent->SetMasterPoseComponent(VisualMeshBodyComponent);
	OutlineMeshHeadComponent->SetMasterPoseComponent(VisualMeshHeadComponent);
	OutlineMeshAccessoriesComponent->SetMasterPoseComponent(VisualMeshAccessoriesComponent);

}

void AIdolActor::UpdateBustSize()
{
	if (!BustShrinkMorphTarget.IsNone())
	{
		VisualMeshBodyComponent->SetMorphTarget(
			BustShrinkMorphTarget,
			1.f - FMath::GetMappedRangeValueClamped(FVector2D(70, 82), FVector2D(0.f, 1.f), BustSize)
		);
		OutlineMeshBodyComponent->SetMorphTarget(
			BustShrinkMorphTarget,
			1.f - FMath::GetMappedRangeValueClamped(FVector2D(70, 82), FVector2D(0.f, 1.f), BustSize)
		);
	}
	
	if (!BustEnlargeMorphTarget.IsNone())
	{
		VisualMeshBodyComponent->SetMorphTarget(
			BustEnlargeMorphTarget,
			FMath::GetMappedRangeValueClamped(FVector2D(82, 94), FVector2D(0.f, 1.f), BustSize)
		);
		OutlineMeshBodyComponent->SetMorphTarget(
			BustEnlargeMorphTarget,
			FMath::GetMappedRangeValueClamped(FVector2D(82, 94), FVector2D(0.f, 1.f), BustSize)
		);
	}
}

