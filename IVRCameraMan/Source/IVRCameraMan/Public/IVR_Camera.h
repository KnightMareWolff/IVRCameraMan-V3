

#pragma once

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Engine/Texture2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "AudioCaptureComponent.h"
#include "AudioMixerBlueprintLibrary.h"
#include "IVR_FunctionLibrary.h" //The Function Library already have the OpenCV Includes
#include "Templates/SharedPointer.h"
#include "Framework/Application/SlateApplication.h"
#include "IVR_Camera.generated.h"

UCLASS()
class IVRCAMERAMAN_API AIVR_Camera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIVR_Camera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//-----------------------------------------
	//Base Camera Functionality
	//-----------------------------------------
	UFUNCTION(Category = "CameraMan|Objects|IVR_Camera", BlueprintCallable)
		bool IVR_StartRecord();
	UFUNCTION(Category = "CameraMan|Objects|IVR_Camera", BlueprintCallable)
		bool IVR_StopRecord();
	UFUNCTION(Category = "CameraMan|Objects|IVR_Camera", BlueprintCallable)
		bool IVR_CompileVideo();
	UFUNCTION(Category = "CameraMan|Objects|IVR_Camera", BlueprintCallable)
		bool IVR_LoadDefault();
	UFUNCTION(Category = "CameraMan|Objects|IVR_Camera", BlueprintCallable)
		bool IVR_SetTransform(FTransform pTransform);
	UFUNCTION(Category = "CameraMan|Objects|IVR_Camera", BlueprintCallable)
		bool IVR_SetRotation(FRotator    pRotation);
	UFUNCTION(Category = "CameraMan|Objects|IVR_Camera", BlueprintCallable)
		void IVR_RegisterCamera(FString CameraName, int32 LowLevelType, int32 LowLevelRecordingMode);

	//-----------------------------------------
	//Base Camera Blueprint Attributes
	//-----------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Objects|IVR_Camera")
		int32                     IVR_Width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Objects|IVR_Camera")
		int32                     IVR_Height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Objects|IVR_Camera")
		FLinearColor              IVR_ClearColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Objects|IVR_Camera")
		float                     IVR_Gamma;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Objects|IVR_Camera")
		bool                      IVR_EnableStabilization;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Objects|IVR_Camera")
		float                     IVR_StabilizationRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Objects|IVR_Camera")
		float                     IVR_StabilizationArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Objects|IVR_Camera")
		float                     IVR_LagDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Objects|IVR_Camera")
		FString                   IVR_CameraName;

	//-----------------------------------------------
	//Base Virtual Camera (Low Level) Attributes
	//-----------------------------------------------
	int32                     IVR_LowLevelIndex;
	int32                     IVR_LowLevelType;
	int32                     IVR_LowLevelRecordingType;
	int32                     IVR_BufferCache;
	float                     IVR_FPS;
	float                     IVR_DT;
	FString                   IVR_RecordingPath;
	FConfigFile               IVR_ProjectConfigFile;
	uint                      IVR_RecordingState;
	bool                      IVR_Enabled;
	bool                      IVR_LockedRendering;
	CIVRVirtualCamera* IVR_LowLevelCam;

	//Time Measuring
	float      IVR_ComponentStartTime;

	//-----------------------------------------------
	//Render Capture Attributes
	//-----------------------------------------------
	UPROPERTY(VisibleAnywhere, Category = "CameraMan|Objects|IVR_CameraActor")
	USceneComponent         * IVR_Root;
	UTextureRenderTarget2D  * IVR_RenderTarget;
	USceneCaptureComponent2D* IVR_CameraCapture;
	USphereComponent        * IVR_CollisionSphere;

	IVR_RenderBuffer IVR_FrameBuffer;
	IVR_FrameData    IVR_FrameInformation;
	TArray<FColor>   IVR_RawBuffer;

	//-----------------------------------------------
	//a Lock Free Queue for FrameBuffer recordings.
	//-----------------------------------------------
	// FRenderRequest* size 16 => ptr * 1000 = 16000 
	LockFreeQueue<TSharedPtr<FRenderRequest>, 16000  >* IVR_RenderQueue;
	CIVRShrinker* IVR_ShrinkerF;

	//-----------------------------------------------
	//Render Capture Functions
	//-----------------------------------------------
	void OnBackBufferReady(SWindow& SlateWindow, const FTexture2DRHIRef& BackBuffer);
	FDelegateHandle OnBackBufferReadyToPresent;

};
