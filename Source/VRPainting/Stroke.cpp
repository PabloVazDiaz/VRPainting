// Fill out your copyright notice in the Description page of Project Settings.


#include "Stroke.h"
#include "Components/SplineMeshComponent.h"

AStroke::AStroke()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	StrokeMeshes = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("StrokeMeshes"));
	StrokeMeshes->SetupAttachment(Root);
	JointMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("JointMesh"));
	JointMesh->SetupAttachment(Root);
}

void AStroke::Update(FVector CursorLocation)
{
	if (PreviousCursorLocation.IsNearlyZero()) 
	{
		PreviousCursorLocation = CursorLocation;
		JointMesh->AddInstance(GetNextJointTransform(CursorLocation));
		return;
	}
	
	StrokeMeshes->AddInstance(GetNextSegmentTransform(CursorLocation));
	JointMesh->AddInstance(GetNextJointTransform(CursorLocation));


	PreviousCursorLocation = CursorLocation;
}

FTransform AStroke::GetNextSegmentTransform(FVector CurrentLocation) const
{
	FTransform SegmentTransform;

	SegmentTransform.SetScale3D(GetNextSegmentScale(CurrentLocation));
	SegmentTransform.SetRotation(GetNextSegmentRotation(CurrentLocation));
	SegmentTransform.SetLocation(GetNextSegmentLocation(CurrentLocation));

	return SegmentTransform;
}

FVector AStroke::GetNextSegmentLocation(FVector CurrentLocation) const
{
	return GetTransform().InverseTransformPosition(PreviousCursorLocation);
}

FQuat AStroke::GetNextSegmentRotation(FVector CurrentLocation) const
{

	FVector Segment = CurrentLocation - PreviousCursorLocation;
	FVector SegmentNormal = Segment.GetSafeNormal();
	return FQuat::FindBetweenNormals(FVector::ForwardVector, SegmentNormal);

}

FVector AStroke::GetNextSegmentScale(FVector CurrentLocation) const
{

	FVector Segment = CurrentLocation - PreviousCursorLocation ;

	return FVector(Segment.Size(),1,1);
}

FTransform AStroke::GetNextJointTransform(FVector CurrentLocation) const
{
	FTransform JointTransform;

	JointTransform.SetLocation(GetTransform().InverseTransformPosition(CurrentLocation));

	return JointTransform;
}