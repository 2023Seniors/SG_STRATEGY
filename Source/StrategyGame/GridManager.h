// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridCell.h"

#include <vector>

#include "GridManager.generated.h"

UCLASS()
class STRATEGYGAME_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

	//void SetSize(int width, int height);

protected:
	// Size of the Grid Map
	int mWidth;
	int mHeight;

	// Grid container
	//std::vector<std::vector<AMyGrid>> mGridMap;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
