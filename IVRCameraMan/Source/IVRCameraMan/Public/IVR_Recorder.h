

#pragma once

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "IVR_Camera.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "IVR_Recorder.generated.h"

UCLASS()
class IVRCAMERAMAN_API AIVR_Recorder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIVR_Recorder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//-----------------------------------------
	//Base Camera Functionality
	//-----------------------------------------
	UFUNCTION(Category = "CameraMan|Objects|IVR_Recorder", BlueprintCallable)
	void IVR_StartRecord();
	UFUNCTION(Category = "CameraMan|Objects|IVR_Recorder", BlueprintCallable)
	void IVR_StopRecord(bool IVR_AutoRecord = true);
	UFUNCTION(Category = "CameraMan|Objects|IVR_Recorder", BlueprintCallable)
	bool IVR_SetTransform(FTransform pTransform);
	UFUNCTION(Category = "CameraMan|Objects|IVR_Recorder", BlueprintCallable)
		void IVR_RegisterCamera(FString CameraName, ECameraType LowLevelType, ERecordingMode LowLevelRecordingMode, bool UseEffects, FString EffectName, bool UseCompression);

	void IVR_RegisterCameraML(FString CameraName, int32 LowLevelType, int32 LowLevelRecordingMode, bool UseEffects, FString EffectName, bool UseCompression);
	bool IVR_MoveCamera;
	bool IVR_SelfRegistered;

	FCriticalSection          IVR_UpdCamSection;

	UPROPERTY(VisibleAnywhere, Category = "CameraMan|Objects|IVR_Recorder")
	AIVR_Camera*     IVR_RecorderCamera;
	UPROPERTY(VisibleAnywhere, Category = "CameraMan|Objects|IVR_Recorder")
	USceneComponent* IVR_Root;

};
