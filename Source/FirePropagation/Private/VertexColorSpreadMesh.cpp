// Fill out your copyright notice in the Description page of Project Settings.


#include "VertexColorSpread.h"
#include "StaticMeshResources.h"
#include "VertexColorSpreadMesh.h"
#include "Engine/StaticMesh.h"
#include "StaticMeshDescription.h"

// // Sets default values
// AVertexColorSpreadMesh::AVertexColorSpreadMesh()
// {
//  	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
// 	PrimaryActorTick.bStartWithTickEnabled = true;
// 	PrimaryActorTick.bCanEverTick = true;
//
// 	ColorSpreadComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ColorSpreadMesh"));
// 	RootComponent = ColorSpreadComponent;
// 	
// }
//
// // Called every frame
// void AVertexColorSpreadMesh::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// 	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Verts %d: "), ColorSpreadComponent->GetStaticMesh()->GetNumVertices(0)));
//
// 	ColorSpreadComponent->SetLODDataCount(1, ColorSpreadComponent->LODData.Num());
//
// 	FStaticMeshComponentLODInfo* InstanceMeshLODInfo = &ColorSpreadComponent->LODData[0];
//
// 	InstanceMeshLODInfo->PaintedVertices.Empty();
//
// 	InstanceMeshLODInfo->OverrideVertexColors = new FColorVertexBuffer;
//
// 	FStaticMeshLODResources& LODModel = ColorSpreadComponent->GetStaticMesh()->RenderData->LODResources[0];
//
// 	if ((int32)LODModel.VertexBuffers.ColorVertexBuffer.GetNumVertices() >= LODModel.GetNumVertices())
// 	{
// 		InstanceMeshLODInfo->OverrideVertexColors->InitFromColorArray(&LODModel.VertexBuffers.ColorVertexBuffer.VertexColor(0), LODModel.GetNumVertices());
// 	}
// 	else
// 	{
// 		InstanceMeshLODInfo->OverrideVertexColors->InitFromSingleColor(FColor::Green, LODModel.GetNumVertices());
// 	}
//
// 	BeginInitResource(InstanceMeshLODInfo->OverrideVertexColors);
// 	ColorSpreadComponent->MarkRenderStateDirty();
//
// 	for (uint32 i = 0; i < InstanceMeshLODInfo->OverrideVertexColors->GetNumVertices(); i++)
// 	{
// 		InstanceMeshLODInfo->OverrideVertexColors->VertexColor(i) = FColor::Green;
// 	}
// }
//
AVertexColorSpreadMesh::AVertexColorSpreadMesh()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	ColorSpreadComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ColorSpreadMesh"));
	RootComponent = ColorSpreadComponent;

	// FScriptDelegate Delegate;
	// Delegate.BindUFunction(this, TEXT("TakePointDamageDelegate"));
	// OnTakePointDamage.AddUnique(Delegate);
	FirstInit = true;
	Distance = 50.f;
	TriggerIntensity = .2f;
	Interval = 0.1f;
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
	ProcessingVertices.Add(0);

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

	// BuildAdjacencyCache(LODModel);
	
	// Start our spread code
	bool check_finished = SpreadIntenseColors(InstanceMeshLODInfo, LODModel);

	// Notify the render thread about the buffer change
	// BeginUpdateResourceRHI(InstanceMeshLODInfo->OverrideVertexColors);

	// BeginInitResource(InstanceMeshLODInfo->OverrideVertexColors);
	// ColorSpreadComponent->MarkRenderStateDirty();
	BeginUpdateResourceRHI(InstanceMeshLODInfo->OverrideVertexColors);
	if(!check_finished)
	{
		FTimerHandle Handle;
		GetWorldTimerManager().SetTimer(Handle, this, &AVertexColorSpreadMesh::Spread, Interval, false);
	}
}


bool AVertexColorSpreadMesh::SpreadIntenseColors(FStaticMeshComponentLODInfo* InstanceMeshLODInfo, FStaticMeshLODResources& LODModel)
{
	TArray<int32> newVertices;
	
	for(int i = 0; i < ProcessingVertices.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("출발~: %d"), ProcessingVertices[i]);
		PaintVertexInstances(FVertexID(ProcessingVertices[i]), *InstanceMeshLODInfo, "R=255,G=0,B=0,A=255");
		TArray<FVertexID> adjacentVertices = MeshDescription->GetVertexAdjacentVertices(FVertexID(ProcessingVertices[i]));

		for(auto elem : adjacentVertices)
		{
			if(!AlreadyCheckedIndices.Contains(elem.GetValue()))
			{
				UE_LOG(LogTemp, Log, TEXT("id: %d, color: %s"), elem.GetValue(), *InstanceMeshLODInfo->OverrideVertexColors->VertexColor(elem.GetValue()).ToString())
				newVertices.AddUnique(elem.GetValue());
				PaintVertexInstances(elem, *InstanceMeshLODInfo, "R=255,G=0,B=0,A=255");
				AlreadyCheckedIndices.AddUnique(elem.GetValue());
			}
		}
	}
	ProcessingVertices.Empty();
	ProcessingVertices = newVertices;
	
	BeginInitResource(InstanceMeshLODInfo->OverrideVertexColors);
	ColorSpreadComponent->MarkRenderStateDirty();
	UE_LOG(LogTemp, Log, TEXT("다음~: %d"), ProcessingVertices.Num());
	if(ProcessingVertices.Num() != 0)
		return false;
	else
	{
		return true;
	}
}

// TArray<int32> AVertexColorSpreadMesh::FindNearVertices(FVector Position, FStaticMeshLODResources& LODModel, FStaticMeshComponentLODInfo* InstanceMeshLODInfo)
// {
// 	TArray<int32> NearVertices;
// 	auto ShortestDistance = -1;
// 	auto NearestVertexIndex = -1;
// 	auto LocalToWorld = ColorSpreadComponent->GetComponentToWorld().ToMatrixWithScale();
//
// 	for (auto i = 0; i < LODModel.GetNumVertices(); i++)
// 	{
// 		auto LocalVertexPosition = LODModel.VertexBuffers.PositionVertexBuffer.VertexPosition(i);
// 		auto WorldVertexPosition = LocalToWorld.TransformPosition(LocalVertexPosition);
// 		auto distance = FVector::DistSquared(WorldVertexPosition, Position);
//
// 		if(distance < Distance)
// 		{
// 			if(InstanceMeshLODInfo->OverrideVertexColors->VertexColor(i) != FColor::Red)
// 				NearVertices.Add(i);
// 		}
// 	}
// 	return NearVertices;
// }

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
	// if (LODModel.VertexBuffers.ColorVertexBuffer.VertexColor(0))
	// {
	// 	UE_LOG(LogTemp, Log, TEXT("있음"))
	// 	// If the mesh already has vertex colours, initialise OverrideVertexColors from them
	// 	InstanceMeshLODInfo->OverrideVertexColors->InitFromColorArray(&LODModel.VertexBuffers.ColorVertexBuffer.VertexColor(0), LODModel.GetNumVertices());
	// }
	if(FirstInit)
	{
		UE_LOG(LogTemp, Log, TEXT("없음"))
		// If it doesn't, set all overridden vert colours to black
		InstanceMeshLODInfo->OverrideVertexColors->InitFromSingleColor(FColor::Blue, LODModel.GetNumVertices());
		FirstInit = false;
	}else
	{
		InstanceMeshLODInfo->OverrideVertexColors->InitFromColorArray(&LODModel.VertexBuffers.ColorVertexBuffer.VertexColor(0), LODModel.GetNumVertices());
	}

	BeginInitResource(InstanceMeshLODInfo->OverrideVertexColors);
	ColorSpreadComponent->MarkRenderStateDirty();
}

void AVertexColorSpreadMesh::PaintVertexInstances(FVertexID id, FStaticMeshComponentLODInfo& LODInfo, FString ColorString)
{
	for(auto& item: MeshDescription->GetVertexVertexInstances(id))
	{
		LODInfo.OverrideVertexColors->VertexColor(item.GetValue()).InitFromString(ColorString);
		// LODInfo.OverrideVertexColors->VertexColor(item.GetValue()) = FColor::Red;
	}
}