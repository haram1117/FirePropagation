// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMeshBaseActor.h"
#include "SimpleDynamicMeshComponent.h"
#include "DynamicSDMCActor.generated.h"

/**
 * 
 */
UCLASS()
class FIREPROPAGATION_API ADynamicSDMCActor : public ADynamicMeshBaseActor
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere)
	USimpleDynamicMeshComponent* MeshComponent = nullptr;
};
