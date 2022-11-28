// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "HeatTransferVolume.generated.h"

/**
 * 
 */
UCLASS()
class FIREPROPAGATION_API AHeatTransferVolume : public ATriggerBox
{
	GENERATED_BODY()
private:
	int id;
	TArray<int> NextIds;
public:
	int GetID() const {	return id;}
	void SetID(int __id){id = __id;}
	void PutNextId(int _id){NextIds.AddUnique(_id);	}
};
