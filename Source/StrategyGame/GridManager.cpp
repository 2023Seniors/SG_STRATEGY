// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"

// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the grid container
	SetSize(100, 100);
}

void AGridManager::SetSize(int width, int height)
{
	mWidth = width;
	mHeight = height;

	//// Clear the array vectors
	//for (auto& it : mGridMap)
	//{
	//	it.Empty();
	//}
	//mGridMap.Empty();
	//
	//// Resize to the new size
	//mGridMap.SetNum(width);
	//for (auto& it : mGridMap)
	//{
	//	it.SetNum(height);
	//}
}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

