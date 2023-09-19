// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGridManager.h"

// Sets default values
AMyGridManager::AMyGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the grid container
	SetSize(100, 100);
}

void AMyGridManager::SetSize(int width, int height)
{
	mWidth = width;
	mHeight = height;

	// Resize vectors
	mGridMap.clear();
	mGridMap.resize(mWidth);
	for (int i = 0; i < mGridMap.size(); i++) {
		mGridMap[i].resize(mHeight);
	}
}

// Called when the game starts or when spawned
void AMyGridManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

