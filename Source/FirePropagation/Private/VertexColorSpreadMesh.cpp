// Fill out your copyright notice in the Description page of Project Settings.


#include "VertexColorSpreadMesh.h"
#include "VertexColorSpread.h"
#include "StaticMeshResources.h"
#include "Engine/StaticMesh.h"
#include "StaticMeshDescription.h"
#include "Kismet/KismetMathLibrary.h"

AVertexColorSpreadMesh::AVertexColorSpreadMesh()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	ColorSpreadComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ColorSpreadMesh"));
	RootComponent = ColorSpreadComponent;
	// FireComponent = CreateDefaultSubobject<UFireComponent>(TEXT("FireComponent"));
	// FireComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	
	FireSimulationManager = CreateDefaultSubobject<UFireSimulation>(TEXT("FireSimulationManager"));
	// FireComponent->AddToRoot();
	// FScriptDelegate Delegate;
	// Delegate.BindUFunction(this, TEXT("TakePointDamageDelegate"));
	// OnTakePointDamage.AddUnique(Delegate);
	FirstInit = true;
	Distance = 50.f;
	TriggerIntensity = .2f;
	Interval = 1.0f;
	bActive = true;
	bTimerActive = false;
}

void AVertexColorSpreadMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bActive && !bTimerActive)
	{
		
		bTimerActive = true;
	}
}

void AVertexColorSpreadMesh::BeginPlay()
{
	Super::BeginPlay();
	ProcessingVertices.Add(FVertexID(0));
	MeshDescription = ColorSpreadComponent->GetStaticMesh()->GetMeshDescription(0);
	// Init the buffers and LOD data
	InitialiseLODInfoAndBuffers();
	
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &AVertexColorSpreadMesh::Spread, Interval, false);
		
}

void AVertexColorSpreadMesh::Spread()
{
	if (!bActive)
	{
		bTimerActive = false;
		return;
	}

	FStaticMeshLODResources& LODModel = ColorSpreadComponent->GetStaticMesh()->RenderData->LODResources[0];
	FStaticMeshComponentLODInfo* InstanceMeshLODInfo = &ColorSpreadComponent->LODData[0];
	
	BeginUpdateResourceRHI(InstanceMeshLODInfo->OverrideVertexColors);
	bool check_finished = SpreadIntenseColors(*InstanceMeshLODInfo, LODModel);
	NextProcessingVertices.Empty();

	if(!check_finished)
	{
		FTimerHandle Handle;
		GetWorldTimerManager().SetTimer(Handle, this, &AVertexColorSpreadMesh::Spread, Interval, false);
	}
}


bool AVertexColorSpreadMesh::SpreadIntenseColors(FStaticMeshComponentLODInfo& InstanceMeshLODInfo, FStaticMeshLODResources& LODModel)
{
	TArray<int32> newVertices;
	BeginInitResource(InstanceMeshLODInfo.OverrideVertexColors);
	ColorSpreadComponent->MarkRenderStateDirty();

	// 현재 processing vertices 단계 업
	for (auto ProcessingVertex : ProcessingVertices)
	{
		int id = ProcessingVertex.GetValue();
		VertexStruct* VertexStruct = startedVerticesMap.Find(id);
		if(!VertexStruct)
		{
			VertexStruct = new class VertexStruct(ProcessingVertex.GetValue(), FColor::Blue);
			startedVerticesMap.Add(ProcessingVertex.GetValue(), *VertexStruct);
		}
		StartedVertices.Add(ProcessingVertex);
		
		// FColor& vertexColor = InstanceMeshLODInfo.OverrideVertexColors->VertexColor(ProcessingVertex.GetValue());
		FColor vertexColor = VertexStruct->GetVertexColor();
		FColor ColorString = GetNextColor(vertexColor);
		VertexStruct->SetVertexColor(ColorString);
		
		FVector vertexLocation = LODModel.VertexBuffers.PositionVertexBuffer.VertexPosition(ProcessingVertex.GetValue());
		PaintVertexInstances(ProcessingVertex, InstanceMeshLODInfo, ColorString.ToString(), vertexLocation);
		//
		// UE_LOG(LogTemp, Log, TEXT("????: %d"), FVertexID(ProcessingVertex).GetValue());
		TSet<FVertexID> AdjacentSet = TSet<FVertexID>(MeshDescription->GetVertexAdjacentVertices(FVertexID(ProcessingVertex)));
		TSet<FVertexID> adjacentVertices = AdjacentSet.Difference(StartedVertices);
		
		// 인접한 버텍스 다음 processing 에 추가
		for(auto elem : adjacentVertices)
		{
			NextProcessingVertices.Add(elem);
		}
	}
	// ColorSpreadComponent->MarkRenderStateDirty();
	// BeginInitResource(InstanceMeshLODInfo->OverrideVertexColors);
	// 시작했지만 processing에서 다루지 않은 버텍스 단계 업
	TSet<FVertexID> StartedButNotProcessed = StartedVertices.Difference(ProcessingVertices);
	for (auto elem : StartedButNotProcessed)
	{
		VertexStruct* VertexStruct = startedVerticesMap.Find(elem.GetValue());
		FColor vertexColor = VertexStruct->GetVertexColor();
		FColor ColorString = GetNextColor(vertexColor);
		VertexStruct->SetVertexColor(ColorString);

		FVector vertexLocation = LODModel.VertexBuffers.PositionVertexBuffer.VertexPosition(elem.GetValue());
		PaintVertexInstances(elem, InstanceMeshLODInfo, ColorString.ToString(), vertexLocation);
	}
	ProcessingVertices.Empty();
	ProcessingVertices = NextProcessingVertices;
	
	if(ProcessingVertices.Num() != 0 || StartedButNotProcessed.Num() != 0)
		return false;
	return true;
}

void AVertexColorSpreadMesh::InitialiseLODInfoAndBuffers()
{
	ColorSpreadComponent->GetStaticMesh()->BuildFromMeshDescriptions({MeshDescription});
	if (ColorSpreadComponent->LODData.Num() == 0)
	{
		ColorSpreadComponent->SetLODDataCount(1, ColorSpreadComponent->LODData.Num());
	}

	FStaticMeshLODResources& LODModel = ColorSpreadComponent->GetStaticMesh()->RenderData->LODResources[0];
	FStaticMeshComponentLODInfo* InstanceMeshLODInfo = &ColorSpreadComponent->LODData[0];
	
	InstanceMeshLODInfo->OverrideVertexColors = new FColorVertexBuffer;
	
	if(FirstInit)
	{
		// If it doesn't, set all overridden vert colours to black
		InstanceMeshLODInfo->OverrideVertexColors->InitFromSingleColor(FColor::Blue, LODModel.GetNumVertices());
		FirstInit = false;
	}else
	{
		InstanceMeshLODInfo->OverrideVertexColors->InitFromColorArray(&LODModel.VertexBuffers.ColorVertexBuffer.VertexColor(0), LODModel.GetNumVertices());
	}

	ColorSpreadComponent->MarkRenderStateDirty();
}

void AVertexColorSpreadMesh::PaintVertexInstances(FVertexID id, FStaticMeshComponentLODInfo& LODInfo, FString ColorString, FVector& vertexLocation)
{
	// BeginInitResource(LODInfo.OverrideVertexColors);
	
	for(auto& item: MeshDescription->GetVertexVertexInstances(id))
	{
		LODInfo.OverrideVertexColors->VertexColor(item.GetValue()).InitFromString(ColorString);
	}
	if(ColorString == FColor::Green.ToString())
	{
		// UE_LOG(LogTemp, Log, TEXT("Before: %s"), *vertexLocation.ToString());
		vertexLocation = GetActorTransform().TransformPosition(vertexLocation);
		// UE_LOG(LogTemp, Log, TEXT("After: %s"), *vertexLocation.ToString());
		FireSimulationManager->SpawnSmoke(vertexLocation, FVertexID(id));
	}
	if(ColorString == FColor::Red.ToString())
	{
		// UE_LOG(LogTemp, Log, TEXT("Before: %s"), *vertexLocation.ToString());
		vertexLocation = GetActorTransform().TransformPosition(vertexLocation);
		// UE_LOG(LogTemp, Log, TEXT("After: %s"), *vertexLocation.ToString());
		FireSimulationManager->FindNearestFire(vertexLocation, FVertexID(id));
	}
}

FColor AVertexColorSpreadMesh::GetNextColor(FColor nowColor)
{
	FColor nextColor;
	if(nowColor == FColor::Blue)
		nextColor = FColor::Green;
	else if(nowColor == FColor::Green)
		nextColor = FColor::Yellow;
	else if(nowColor == FColor::Yellow)
		nextColor = FColor::Orange;
	else if(nowColor == FColor::Orange)
		nextColor = FColor::Red;
	else
		nextColor = FColor::Red;
	return nextColor;
}