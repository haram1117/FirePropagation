// Fill out your copyright notice in the Description page of Project Settings.


#include "VertexColorSpread.h"
#include "StaticMeshResources.h"
#include "VertexColorSpreadMesh.h"
#include "Engine/StaticMesh.h"
#include "StaticMeshDescription.h"

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
	
	// 얘넨 아마 다른 시간대로 돌아가야댈듯
	// for(int i = 0; i < OrangeLevelIndices.Num(); i++)
	// {
	// }
	while(OrangeLevelIndices.Num() > 0)
	{
		int32 index = OrangeLevelIndices.Pop();
		// const FColor tempvertexColor = InstanceMeshLODInfo->OverrideVertexColors->VertexColor(index);
		// const FString tempColorString = GetNextColor(tempvertexColor);
		PaintVertexInstances(FVertexID(index), *InstanceMeshLODInfo, FColor::Red.ToString());
		AlreadyCheckedIndices.AddUnique(index);
	}
	while(YellowLevelIndices.Num() > 0)
	{
		int32 index = YellowLevelIndices.Pop();
		// const FColor tempvertexColor = InstanceMeshLODInfo->OverrideVertexColors->VertexColor(index);
		// const FString tempColorString = GetNextColor(tempvertexColor);
		PaintVertexInstances(FVertexID(index), *InstanceMeshLODInfo, FColor::Orange.ToString());
		OrangeLevelIndices.AddUnique(index);
	}
	while(GreenLevelIndices.Num() > 0)
	{
		int32 index = GreenLevelIndices.Pop();
		// const FColor tempvertexColor = InstanceMeshLODInfo->OverrideVertexColors->VertexColor(index);
		// const FString tempColorString = GetNextColor(tempvertexColor);
		PaintVertexInstances(FVertexID(index), *InstanceMeshLODInfo, FColor::Yellow.ToString());
		YellowLevelIndices.AddUnique(index);
	}
	// BuildAdjacencyCache(LODModel);
	
	// BeginInitResource(InstanceMeshLODInfo->OverrideVertexColors);
	// ColorSpreadComponent->MarkRenderStateDirty();
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
		const FColor vertexColor = InstanceMeshLODInfo->OverrideVertexColors->VertexColor(ProcessingVertices[i]);
		const FString ColorString = GetNextColor(vertexColor);
		PaintVertexInstances(FVertexID(ProcessingVertices[i]), *InstanceMeshLODInfo, ColorString);
		TArray<FVertexID> adjacentVertices = MeshDescription->GetVertexAdjacentVertices(FVertexID(ProcessingVertices[i]));

		for(auto elem : adjacentVertices)
		{
			if(!AlreadyCheckedIndices.Contains(elem.GetValue()))
			{
				newVertices.AddUnique(elem.GetValue());
				const FColor tempvertexColor = InstanceMeshLODInfo->OverrideVertexColors->VertexColor(elem.GetValue());
				
				if(tempvertexColor == FColor::Blue)
					GreenLevelIndices.AddUnique(elem.GetValue());
				else if(tempvertexColor == FColor::Green)
				{
					GreenLevelIndices.Remove(elem.GetValue());
					YellowLevelIndices.AddUnique(elem.GetValue());
				}
				else if(tempvertexColor == FColor::Yellow)
				{
					YellowLevelIndices.Remove(elem.GetValue());
					OrangeLevelIndices.AddUnique(elem.GetValue());
				}
				else if(tempvertexColor == FColor::Orange)
				{
					OrangeLevelIndices.Remove(elem.GetValue());
					AlreadyCheckedIndices.AddUnique(elem.GetValue());
				}
				const FString tempColorString = GetNextColor(tempvertexColor);
				PaintVertexInstances(elem, *InstanceMeshLODInfo, tempColorString);
			}
		}
	}
	ProcessingVertices.Empty();
	ProcessingVertices = newVertices;
	
	BeginInitResource(InstanceMeshLODInfo->OverrideVertexColors);
	ColorSpreadComponent->MarkRenderStateDirty();
	if(ProcessingVertices.Num() != 0)
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

FString AVertexColorSpreadMesh::GetNextColor(FColor nowColor)
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
	{
		nextColor = FColor::Red;
	}
	
	return nextColor.ToString();
}
