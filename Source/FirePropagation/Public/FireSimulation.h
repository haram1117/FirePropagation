// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VertexStruct.h"
#include "Components/ActorComponent.h"
#include "FireSimulation.generated.h"

class UFireComponent;
class AVertexColorSpreadMesh;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIREPROPAGATION_API UFireSimulation : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFireSimulation();
	
	
	// TArray<UFireComponent*> FireComponents;
	// UPROPERTY(VisibleAnywhere)
	TMap<int, UFireComponent*> FireComponents;

	AVertexColorSpreadMesh* vertexColorSpreadMesh;
	
	void SpawnSmoke(FVector fireLocation, VertexStruct* VertexStruct);

	void FindNearestFire(FVector fireLocation, VertexStruct* vertexStruct);
	
	

	UPROPERTY(EditAnywhere)
	float boundary = 50;
	
	/**
	 * @brief Char contraction factor
	 */
	UPROPERTY(EditAnywhere, meta=(ClampMin=0.5, ClampMax=1.0))
	float charContractionFactor = 0.5;

	
	/**
	 * @brief Min value of c as a result of charring
	 */
	UPROPERTY(EditAnywhere, meta=(ClampMin=0.0, ClampMax=1.0))
	float cMin = 0.0;

	/**
	 * @brief Rate of insulation due to charring
	 */
	UPROPERTY(EditAnywhere, meta=(ClampMin=50, ClampMax=250))
	float cRate = 50;

	/**
	 * @brief Mass Loss Rate -> * 10^(-3)
	 */
	UPROPERTY(EditAnywhere, meta=(ClampMin=0.01, ClampMax=10.0))
	float massLossRate = 0.01;

	/**
	 * @brief Initial Moisture
	 */
	UPROPERTY(EditAnywhere, meta=(ClampMin=0.0, ClampMax=1.0))
	float initialMoisture = 0.0;

	/**
	 * @brief heat transfer coefficient
	 */
	UPROPERTY(EditAnywhere, meta=(ClampMin=0.03, ClampMax=0.1))
	float heatTransferCoeff = 0.03;

	/**
	 * @brief Amount of Smoke from combustion
	 */
	UPROPERTY(EditAnywhere, meta=(ClampMin=0, ClampMax = 400))
	float smokeAmount = 0;

	/**
	 * @brief Temperature Diffusion Coefficient
	 */
	UPROPERTY(EditAnywhere, meta=(ClampMin=0.1, ClampMax=1.0))
	float temperatureDiffusionCoeff_wall = 0.1;

	UPROPERTY(VisibleAnywhere)
	float temperatureDiffusionCoeff_air = 0.8;

	UPROPERTY(VisibleAnywhere)
	float radiactiveCoolingCoeff = -0.008;

	UPROPERTY(VisibleAnywhere)
	float densityOfWall = 800;

	UPROPERTY(VisibleAnywhere)
	float waterEvaporationRate = 0.3;

	UPROPERTY(VisibleAnywhere)
	float smokeAmountFromWaterEvaporation = 200;

	UPROPERTY(VisibleAnywhere)
	float heatGeneratedAmount = 1.2;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
