#include "IVR_CameraTake.h"

CIVRCameraTake::CIVRCameraTake()
{
   IVR_TakeID       = 0;
   IVR_TakeState    = 0;
   IVR_FrameCounter = 0;
   IVR_DesiredFPS   = 60;
   IVR_FPS          = 0;
}

CIVRCameraTake::~CIVRCameraTake()
{
   IVR_TakeID       = 0;
   IVR_TakeState    = 0;
   IVR_FrameCounter = 0;
   IVR_DesiredFPS   = 60;
   IVR_FPS          = 0;

   if(IVR_RenderQueue)
   {
       delete IVR_RenderQueue;
       IVR_RenderQueue = NULL;
   }

   if(IVR_DataQueue)
   {
       delete IVR_DataQueue  ;
       IVR_DataQueue   = NULL;
   }

   if(IVR_EffectsEnabled)
   {
       if(IVR_ImageHandler)
       IVR_ImageHandler->IVR_CleanUpFilters();
       delete IVR_ImageHandler;
       IVR_ImageHandler=NULL;
   }
}

void CIVRCameraTake::IVR_StartRecord()
{
    IVR_RenderQueue = new LockFreeQueue<IVR_RenderBuffer, 6720 >();
    IVR_DataQueue   = new LockFreeQueue<IVR_FrameData   , 2880  >();

    if(!IVR_RenderQueue || !IVR_DataQueue)return;

    //Change the state to recording.
    IVR_TakeState = 1;
    IVR_TakeTimer.start();

    IVR_TakeDTThread = QtConcurrent::run(this,&CIVRCameraTake::IVR_DataLoop);
    IVR_TakeBFThread = QtConcurrent::run(this,&CIVRCameraTake::IVR_ImageLoop);
}

void CIVRCameraTake::IVR_DataLoop()
{

    qWarning() << "======================";
    qWarning() << "Data Loop Started";
    qWarning() << "======================";

    while(IVR_TakeState == 1)
    {
            auto data = IVR_DataQueue->front();
            if(data.IVR_IsValid)
            {
               CIVRIFFReader *fReader = new CIVRIFFReader();
               fReader->IVR_AppendData(data,CIVRConfig::IVR_RootFolder,IVR_CameraName,IVR_TakeID);

               IVR_DataQueue->clean();
               IVR_DataQueue->pop();

               delete fReader;
            }
    }

    if(IVR_DataQueue->size()>0)
    {

    while(IVR_DataQueue->size()>0)
    {
            auto data = IVR_DataQueue->front();
            if(data.IVR_IsValid)
            {
               CIVRIFFReader *fReader = new CIVRIFFReader();
               fReader->IVR_AppendData(data,CIVRConfig::IVR_RootFolder,IVR_CameraName,IVR_TakeID);

               IVR_DataQueue->clean();
               IVR_DataQueue->pop();

               delete fReader;
            }
    }

    }
}

void CIVRCameraTake::IVR_ImageLoop()
{

    qWarning() << "======================";
    qWarning() << "Image Loop Started    ";
    qWarning() << "======================";

    while(IVR_TakeState == 1)
    {
            //---------------------------------------------------
            //Buffer Processing
            //---------------------------------------------------
            auto buffer = IVR_RenderQueue->front();

            if(buffer.IVR_Width > 0 && buffer.IVR_Height > 0 && buffer.IVR_ColorChannels >0)
            {
               CIVRIIFReader *fReader = new CIVRIIFReader();
               fReader->IVR_AppendData(buffer,CIVRConfig::IVR_RootFolder,IVR_CameraName,IVR_TakeID,IVR_FrameCounter);

               IVR_FrameCounter++;

               buffer.IVR_Buffer.release();
               buffer.IVR_Buffer = Mat();

               IVR_RenderQueue->clean();
               IVR_RenderQueue->pop();

               delete fReader;
            }
    }

    if(IVR_RenderQueue->size()>0)
    {
    while(IVR_RenderQueue->size()>0)
    {
            //---------------------------------------------------
            //Buffer Processing
            //---------------------------------------------------
            auto buffer = IVR_RenderQueue->front();

            if(buffer.IVR_Width > 0 && buffer.IVR_Height > 0 && buffer.IVR_ColorChannels >0)
            {
               CIVRIIFReader *fReader = new CIVRIIFReader();

               fReader->IVR_AppendData(buffer,CIVRConfig::IVR_RootFolder,IVR_CameraName,IVR_TakeID,IVR_FrameCounter);

               IVR_FrameCounter++;

               buffer.IVR_Buffer.release();
               buffer.IVR_Buffer = Mat();

               IVR_RenderQueue->clean();
               IVR_RenderQueue->pop();

               delete fReader;
            }
    }
    }
}

uint CIVRCameraTake::IVR_GetRecFPS()
{
    CIVRIFFReader *fReader = new CIVRIFFReader();
    fReader->IVR_ReadFrameData(CIVRConfig::IVR_RootFolder,IVR_CameraName,IVR_TakeID);

    //We pass here the final Frame file to be deleted later.
    IVR_FrameName = fReader->filePath;

    uint FinalFPS=0;
    switch(IVR_RecordingMode)
    {
        case IVR_Recording_Mode_Average  :{FinalFPS = fReader->IVR_GetAverageFPS();}break;
        case IVR_Recording_Mode_Best     :{FinalFPS = fReader->IVR_GetBestFPS();}break;
        case IVR_Recording_Mode_Desired  :{FinalFPS = (uint)IVR_DesiredFPS;}break;
        case IVR_Recording_Mode_TimeAprox:{FinalFPS = fReader->IVR_GetFPSByTimeApprox();}break;
        case IVR_Recording_Mode_Stabilise:{FinalFPS = fReader->IVR_GetFPSByTimeApprox();}break;
        case IVR_Recording_Mode_SuperRes :{FinalFPS = fReader->IVR_GetFPSByTimeApprox();}break;
    }

    delete fReader;

    return FinalFPS;
}

void CIVRCameraTake::IVR_StartCompile()
{
    if(IVR_TakeState==0)return;

    //Change the state to compiling.
    IVR_TakeState   = 2;
    IVR_ElapsedTime = IVR_TakeTimer.elapsed();

    IVR_TakeCPThread = QtConcurrent::run([this]()
    {
        qWarning() << "----------IVR-------------";
        qWarning() << "Starting Compile Take";
        qWarning() << "----------IVR-------------";

        //-------------------------------------------------------------------------
        //Initial Recording Setup
        //-------------------------------------------------------------------------
        qWarning() << "----------IVR----------------";
        qWarning() << "Waiting frame Queues be empty";
        qWarning() << "----------IVR----------------";
        //Waits for the recording Threads Stop
        while(IVR_TakeBFThread.isRunning());
        while(IVR_TakeDTThread.isRunning());

        //Record the video for each Camera we have
        uint FrameNumber      = 0;
        uint rec_mode         = 0;

        qWarning() << "----------IVR-----------------------";
        qWarning() << "Setting Up Recording Paths          ";
        qWarning() << "----------IVR-----------------------";
        IVR_TakeName  = CIVRConfig::IVR_RootFolder + "/" + IVR_CameraName + "Take" + QString("%1").arg(IVR_TakeID) + ".mp4";

        qWarning() << "----------IVR-----------------------";
        qWarning() << "Computing Frames/Recording Values   ";
        qWarning() << "----------IVR-----------------------";
        //-------------------------------------------------------------------------
        //Check the recording smoothness...
        //-------------------------------------------------------------------------
        IVR_FPS = IVR_GetRecFPS();

        //If we not have frames to process stop here..
        if(IVR_FPS==0)
        {
            qWarning() << "----------IVR-----------------------------------";
            qWarning() << "Warning! The FPS calculation are returning Zero!";
            qWarning() << "Setting Up a default 60 FPS recordingf setup    ";
            qWarning() << "----------IVR-----------------------------------";
            IVR_FPS = 60;
        }

        //Set recording Mode...
        rec_mode  = IVR_RecordingMode;

        qWarning() << "----------IVR-----------------------";
        qWarning() << "Computing Frame Dimensions          ";
        qWarning() << "----------IVR-----------------------";
        //-------------------------------------------------------------------------
        //Check the Video Capture Parameters and Codecs
        //-------------------------------------------------------------------------
        int frame_width  = static_cast<int>(CIVRConfig::IVR_Width );
        int frame_height = static_cast<int>(CIVRConfig::IVR_Height);

        Size frame_size(frame_width, frame_height);

        qWarning() << "----------IVR-----------------------";
        qWarning() << "Setting Up Special Effects          ";
        qWarning() << "----------IVR-----------------------";
        if(IVR_EffectsEnabled)
        {
            if(!IVR_ImageHandler)IVR_ImageHandler = new CIVRImageHandler();
            IVR_ImageHandler->IVR_CleanUpFilters();
            if(!IVR_EffectsName.isEmpty())
            {
               IVR_ImageHandler->IVR_LoadEffect(IVR_EffectsName);
            }
        }

        qWarning() << "======================================================";
        qWarning() << "Starting Record Video : " << IVR_TakeName;
        qWarning() << "======================================================";
        qWarning() << "The Recording Parameters are:";
        qWarning() << "Camera Name: " <<  IVR_CameraName;
        qWarning() << "FPS        : " <<  IVR_FPS;
        qWarning() << "Rec Mode   : " <<  rec_mode;
        qWarning() << "Width      : " <<  frame_width;
        qWarning() << "Height     : " <<  frame_height;
        qWarning() << "Frames Read: " <<  IVR_FrameCounter+1;
        qWarning() << "Compression: " <<  IVR_CompressionEnabled;
        qWarning() << "======================================================";

        //Create and initialize the VideoWriter object
        cv::VideoWriter oVideoWriter(IVR_TakeName.toStdString(),
            cv::VideoWriter::fourcc('3', 'I', 'V', 'D'),
            IVR_FPS,
            frame_size,
            true);

        //If the VideoWriter object is not initialized successfully, exit the program
        if (oVideoWriter.isOpened() == false)
        {
            //It´s not possible record this video!
            qWarning() << "Fail to open Video File!";
            return;
        }

        Mat   uncompressedImage;
        //-------------------------------------------------------------------------
        //Start Record the Video
        //-------------------------------------------------------------------------
        while (true)
        {
            //qWarning() << "======================================================";
            //qWarning() << "Starting Open Buffer : " << FrameNumber;
            //qWarning() << "======================================================";
            //qWarning() << "Root          : " <<  CIVRConfig::IVR_RootFolder;
            //qWarning() << "IVR_CameraName: " <<  IVR_CameraName;
            //qWarning() << "Take ID       : " <<  IVR_TakeID;
            //qWarning() << "Frame         : " <<  FrameNumber;
            //qWarning() << "Compression   : " <<  IVR_CompressionEnabled;
            //qWarning() << "======================================================";

            CIVRIIFReader *localReader = new CIVRIIFReader();
            localReader->IVR_ReadImageData(CIVRConfig::IVR_RootFolder,IVR_CameraName,IVR_TakeID,FrameNumber,IVR_CompressionEnabled);
            if (!localReader->IVR_IsValid())
            {
                qWarning() << "Finishing the Video!";

                //Finish Write the Final Video
                oVideoWriter.release();
                break;
            }
            else
            {
                //qWarning() << "======================================================";
                //qWarning() << "Reading Frame : " << FrameNumber;
                //qWarning() << "======================================================";
                //qWarning() << "The Frame Data are:";
                //qWarning() << "IVR_Width : "           <<  localReader->ivrBuffer.IVR_Width;
                //qWarning() << "IVR_Height: "           <<  localReader->ivrBuffer.IVR_Height;
                //qWarning() << "IVR_ColorChannels   : " <<  localReader->ivrBuffer.IVR_ColorChannels;
                //qWarning() << "IVR_ShrinkSize      : " <<  localReader->ivrBuffer.IVR_ShrinkSize;
                //qWarning() << "======================================================";

                uncompressedImage = localReader->ivrBuffer.IVR_Buffer.clone();

                //qWarning() << "======================================================";
                //qWarning() << "Mat Format : " ;
                //qWarning() << "======================================================";
                //qWarning() << "Channels : "   <<  uncompressedImage.channels();
                //qWarning() << "rows     : "   <<  uncompressedImage.rows;
                //qWarning() << "cols     : "   <<  uncompressedImage.cols;
                //qWarning() << "Depth    : "   <<  uncompressedImage.depth();
                //qWarning() << "======================================================";

            }

            if(IVR_EffectsEnabled)
            {
                IVR_ImageHandler->IVR_LoadImageMAT(uncompressedImage);
                IVR_ImageHandler->IVR_ApplyFilters();
                uncompressedImage = IVR_ImageHandler->IVR_ImageData.clone();
            }

            if(CIVRConfig::IVR_DebbugMode)
            {
               putText(uncompressedImage, QString("FPS: %1").arg(IVR_FPS).toStdString(),
                        cv::Point(uncompressedImage.cols/12, uncompressedImage.rows/12 + 10), //top-left position
                        cv::FONT_HERSHEY_DUPLEX,
                        1.0,
                        CV_RGB(0, 0, 0), //font color
                        2);

               QString strRecMode;
               switch(rec_mode)
               {
                   case IVR_Recording_Mode_Average  :{strRecMode = QString("Average FPS");}break;
                   case IVR_Recording_Mode_Best     :{strRecMode = QString("Best FPS");}break;
                   case IVR_Recording_Mode_Desired  :{strRecMode = QString("Desired FPS");}break;
                   case IVR_Recording_Mode_TimeAprox:{strRecMode = QString("Time Approximation");}break;
                   case IVR_Recording_Mode_Stabilise:{strRecMode = QString("With Stabilization");}break;
                   case IVR_Recording_Mode_SuperRes :{strRecMode = QString("With Super Resolution");}break;
               }
               putText(uncompressedImage, QString(QString("Recording Mode: ") + strRecMode).toStdString(),
                        cv::Point(uncompressedImage.cols/12 , uncompressedImage.rows/12 + 50), //top-left position
                        cv::FONT_HERSHEY_DUPLEX,
                        1.0,
                        CV_RGB(0, 0, 0), //font color
                        2);
            }

            //write the video frame to the file
            oVideoWriter.write(uncompressedImage);

            //Delete the Image File
            localReader->IVR_DeleteFile();

            delete localReader;

            FrameNumber++;

        }

        qWarning() << "----------IVR-------------";
        qWarning() << "Finishing Compilation";
        qWarning() << "----------IVR-------------";
        //-------------------------------------------------------------------------
        //Final Recording Setup
        //-------------------------------------------------------------------------
        //Clean the buffer memory
        uncompressedImage = Mat();
        IVR_TakeState     = 3;
        delete IVR_RenderQueue;
        delete IVR_DataQueue  ;
        IVR_RenderQueue = nullptr;
        IVR_DataQueue = nullptr;

    });
}
