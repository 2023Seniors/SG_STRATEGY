// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h"

#include "GridManager.generated.h"


enum class GridCellModifiers
{
	SLOW = 0,
	FIRE,
	POISON
};

struct Cell
{
	Cell();
	AActor* mEntity;
	TArray<GridCellModifiers> mModifiers;

};

UCLASS()
class STRATEGYGAME_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

	UFUNCTION(BlueprintCallable)
		void SetMapSize(int width, int height);
	UFUNCTION(BlueprintCallable)
		void SetCellSize(int size);
	UFUNCTION(BlueprintCallable)
		void SetEntityFromWorldPos(AActor* Entity);
	UFUNCTION(BlueprintCallable)
		AActor* GetEntityFromWorldPos(FVector WorldPos);

protected:
	// Size of the Grid Map
	FUintVector2 mMapSize;
	FUintVector2 mCellSize;

	// Grid container
	TArray<Cell> mGridMap;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
