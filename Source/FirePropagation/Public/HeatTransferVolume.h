// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "HeatTransferVolume.generated.h"

/**
 * 
 */
UCLASS()
class FIREPROPAGATION_API AHeatTransferVolume : public ATriggerVolume
{
	GENERATED_BODY()
private:
	int id;
	TArray<int> NextIds;
public:
	int GetID() const {	return id;	}
	void PutNextId(int _id){NextIds.AddUnique(_id);	}
};
