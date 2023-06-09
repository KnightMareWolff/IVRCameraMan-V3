
/************************************************************************/
/*Project              :IVR CameraMan - Unreal Plugin                   */
/*Creation Date/Author :William Wolff - 17/09/2021                      */
/*                                                                      */
/*Copyright (c) 2021 William Wolff. All rights reserved                 */
/************************************************************************/
#pragma once

#include "Engine.h"
#include "Components/SceneComponent.h"
#include "Engine/Texture2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "AudioCaptureComponent.h"
#include "AudioMixerBlueprintLibrary.h"
#include "IVR_FunctionLibrary.h" //The Function Library already have the OpenCV Includes
#include "Templates/SharedPointer.h"
#include "Framework/Application/SlateApplication.h"
#include "IVR_CameraComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IVRCAMERAMAN_API UIVR_CameraComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UIVR_CameraComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	//-----------------------------------------
	//Base Camera Functionality
	//-----------------------------------------
	UFUNCTION(Category = "CameraMan|Components|IVR_CameraComponent", BlueprintCallable)
	bool IVR_StartRecord();
	UFUNCTION(Category = "CameraMan|Components|IVR_CameraComponent", BlueprintCallable)
	bool IVR_StopRecord();
	UFUNCTION(Category = "CameraMan|Components|IVR_CameraComponent", BlueprintCallable)
	bool IVR_CompileVideo();
	UFUNCTION(Category = "CameraMan|Components|IVR_CameraComponent", BlueprintCallable)
	bool IVR_LoadDefault();
	UFUNCTION(Category = "CameraMan|Components|IVR_CameraComponent", BlueprintCallable)
    bool IVR_SetTransform(FTransform pTransform);
	UFUNCTION(Category = "CameraMan|Components|IVR_CameraComponent", BlueprintCallable)
	bool IVR_SetRotation(FRotator    pRotation);
	UFUNCTION(Category = "CameraMan|Components|IVR_CameraComponent", BlueprintCallable)
	void IVR_CustomTick();
	UFUNCTION(Category = "CameraMan|Components|IVR_CameraComponent", BlueprintCallable)
	void IVR_RegisterCamera(FString CameraName, int32 LowLevelType, int32 LowLevelRecordingMode);
	
	//-----------------------------------------
	//Base Camera Blueprint Attributes
	//-----------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Components|IVR_CameraComponent")
		int32                     IVR_Width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Components|IVR_CameraComponent")
		int32                     IVR_Height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Components|IVR_CameraComponent")
		FLinearColor              IVR_ClearColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Components|IVR_CameraComponent")
		float                     IVR_Gamma;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Components|IVR_CameraComponent")
		bool                      IVR_EnableStabilization;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Components|IVR_CameraComponent")
		float                     IVR_StabilizationRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Components|IVR_CameraComponent")
		float                     IVR_StabilizationArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Components|IVR_CameraComponent")
		float                     IVR_LagDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMan|Components|IVR_CameraComponent")
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
	CIVRVirtualCamera*        IVR_LowLevelCam;
	
	//Time Measuring
	float      IVR_ComponentStartTime;
	
	//-----------------------------------------------
	//Render Capture Attributes
	//-----------------------------------------------
	UTextureRenderTarget2D  * IVR_RenderTarget;
	USceneCaptureComponent2D* IVR_CameraCapture;
	UAudioCaptureComponent  * IVR_SoundCapture;
	USpringArmComponent     * IVR_Stabilizer;
	USphereComponent*         IVR_CollisionSphere;

	IVR_RenderBuffer IVR_FrameBuffer;
	IVR_FrameData    IVR_FrameInformation;
	TArray<FColor>   IVR_RawBuffer;

	//-----------------------------------------------
	//a Lock Free Queue for FrameBuffer recordings.
	//-----------------------------------------------
	// FRenderRequest* size 16 => ptr * 1000 = 16000 
	LockFreeQueue<TSharedPtr<FRenderRequest>, 16000  > *IVR_RenderQueue;
	CIVRShrinker* IVR_ShrinkerF;

	//-----------------------------------------------
	//Render Capture Functions
	//-----------------------------------------------
	void OnBackBufferReady(SWindow & SlateWindow, const FTexture2DRHIRef & BackBuffer);
	FDelegateHandle OnBackBufferReadyToPresent;

};
