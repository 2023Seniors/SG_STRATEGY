// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGrid.h"

// Sets default values
AMyGrid::AMyGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Start with no object
	mObject = nullptr;
}

// Called when the game starts or when spawned
void AMyGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyGrid::SetObject(UObject* obj)
{
	mObject = obj;
}

bool AMyGrid::Empty()
{
	return mObject == nullptr;
}

