#include "IVR_IIFReader.h"

CIVRIIFReader::CIVRIIFReader()
{
   isValid=false;
   shrinker = new CIVRShrinker();
}

void CIVRIIFReader::IVR_AppendData(IVR_RenderBuffer &pData,QString path)
{
    //---------------------------------------------------------------------
    //Append in the frames file a single realtime frame
    //---------------------------------------------------------------------
    filePath = path;
    imageFile = fopen(filePath.toStdString().c_str(),"ab");
    if (imageFile != NULL)
    {

        fwrite(&pData.IVR_Width        , sizeof(int)  , 1, imageFile);
        fwrite(&pData.IVR_Height       , sizeof(int)  , 1, imageFile);
        fwrite(&pData.IVR_ColorChannels, sizeof(int)  , 1, imageFile);
        fwrite(&pData.IVR_ShrinkSize   , sizeof(ulong), 1, imageFile);

        //fwrite(buffer.IVR_Buffer.data , buffer.IVR_Width * buffer.IVR_Height * buffer.IVR_ColorChannels , 1 , imageFile);
        fwrite(pData.IVR_Buffer.data , pData.IVR_ShrinkSize , 1 , imageFile);
    }

    fclose(imageFile);
}

void CIVRIIFReader::IVR_AppendData(IVR_RenderBuffer &pData,QString root,QString cam,uint take, uint frame)
{
    filePath = QString(root + "/" + cam + "Take" + QString("%1").arg(take) + QString("Frm%1").arg(frame) + ".iif");

    imageFile = fopen(filePath.toStdString().c_str(),"ab");

    if (imageFile != NULL)
    {

        fwrite(&pData.IVR_Width        , sizeof(int)  , 1, imageFile);
        fwrite(&pData.IVR_Height       , sizeof(int)  , 1, imageFile);
        fwrite(&pData.IVR_ColorChannels, sizeof(int)  , 1, imageFile);
        fwrite(&pData.IVR_ShrinkSize   , sizeof(ulong), 1, imageFile);

        //fwrite(buffer.IVR_Buffer.data , buffer.IVR_Width * buffer.IVR_Height * buffer.IVR_ColorChannels , 1 , imageFile);
        fwrite(pData.IVR_Buffer.data , pData.IVR_ShrinkSize , 1 , imageFile);
    }

    fclose(imageFile);
}

bool CIVRIIFReader::IVR_ReadImageData(QString path, bool compressionEnabled)
{
    char *pShrinkedImage=nullptr;

    CIVRShrinker shrinker;

    //---------------------------------------------------------------------
    //Open the frame file and fill the camera Frames
    //---------------------------------------------------------------------
    filePath = path;
    imageFile = fopen(filePath.toStdString().c_str(),"rb");

    if (imageFile != NULL)
    {
        fread(&ivrBuffer.IVR_Width        , sizeof(int)  , 1, imageFile);
        fread(&ivrBuffer.IVR_Height       , sizeof(int)  , 1, imageFile);
        fread(&ivrBuffer.IVR_ColorChannels, sizeof(int)  , 1, imageFile);
        fread(&ivrBuffer.IVR_ShrinkSize   , sizeof(ulong), 1, imageFile);

        ulong originalSize = ivrBuffer.IVR_Height * ivrBuffer.IVR_Width * ivrBuffer.IVR_ColorChannels;
        if(compressionEnabled)
        {
           pShrinkedImage = new char[ivrBuffer.IVR_ShrinkSize];
           fread(pShrinkedImage , ivrBuffer.IVR_ShrinkSize , 1, imageFile);

           //Decompress the image
           shrinker.DeShrink((char*)pShrinkedImage,originalSize);

           ivrBuffer.IVR_Buffer = Mat(ivrBuffer.IVR_Height , ivrBuffer.IVR_Width, CV_8UC4 , (char*)shrinker.GetData());
        }
        else
        {
           pShrinkedImage = new char[originalSize];
           fread(pShrinkedImage , originalSize , 1, imageFile);
           ivrBuffer.IVR_Buffer = Mat(ivrBuffer.IVR_Height , ivrBuffer.IVR_Width, CV_8UC4 , (char*)pShrinkedImage);
        }

    }
    else
    {
        qWarning() << "Problems to open the IIF File!";
        return false;
    }

    fclose(imageFile);
    return true;
}

bool CIVRIIFReader::IVR_ReadImageData(QString root, QString cam, uint take, uint frame, bool compressionEnabled)
{
    FILE *bufferFile;

    if(ivrBuffer.IVR_Buffer.total()>0)
    {
        ivrBuffer.IVR_Height = 0;
        ivrBuffer.IVR_Width  = 0;
        ivrBuffer.IVR_ColorChannels=0;
        ivrBuffer.IVR_ShrinkSize=0;
        ivrBuffer.IVR_Buffer = Mat();
    }

    filePath  = QString(root + "/" + cam + "Take" + QString("%1").arg(take) + QString("Frm%1").arg(frame) + ".iif");

    //qWarning() << "======================================================";
    //qWarning() << "Reading IIF File : " << filePath;
    //qWarning() << "======================================================";
    //---------------------------------------------------------------------
    //Open the frame file and fill the camera Frames
    //---------------------------------------------------------------------
    bufferFile = fopen(filePath.toStdString().c_str(),"rb");
    if (bufferFile != NULL)
    {
        fread(&ivrBuffer.IVR_Width        , sizeof(int)  , 1, bufferFile);
        fread(&ivrBuffer.IVR_Height       , sizeof(int)  , 1, bufferFile);
        fread(&ivrBuffer.IVR_ColorChannels, sizeof(int)  , 1, bufferFile);
        fread(&ivrBuffer.IVR_ShrinkSize   , sizeof(ulong), 1, bufferFile);

        //qWarning() << "IVR_Width         : " <<  ivrBuffer.IVR_Width;
        //qWarning() << "IVR_Height        : " <<  ivrBuffer.IVR_Height;
        //qWarning() << "IVR_ColorChannels : " <<  ivrBuffer.IVR_ColorChannels;
        //qWarning() << "IVR_ShrinkSize    : " <<  ivrBuffer.IVR_ShrinkSize;
        //qWarning() << "Compression       : " <<  compressionEnabled;
        //qWarning() << "======================================================";

        ulong originalSize = ivrBuffer.IVR_Width * ivrBuffer.IVR_Height * ivrBuffer.IVR_ColorChannels;
        if(compressionEnabled)
        {
           uchar *pShrinkedImage = new uchar[ivrBuffer.IVR_ShrinkSize];

           if(fread(pShrinkedImage , ivrBuffer.IVR_ShrinkSize , 1, bufferFile) == 1)
           {
               //Decompress the image
               if(shrinker->DeShrink((char*)pShrinkedImage,originalSize))
               {
                   ivrBuffer.IVR_Buffer = Mat(ivrBuffer.IVR_Height , ivrBuffer.IVR_Width, CV_8UC4 , (char*)shrinker->GetData());
               }
               else
               {
                   qWarning() << "Problems to open the IIF File!";
                   isValid=false;
                   return false;
               }
           }
           else
           {
               qWarning() << "Problems to open the IIF File!";
               isValid=false;
               return false;
           }
        }
        else
        {
           char *pShrinkedImage = new char[originalSize];
           if(fread(pShrinkedImage , originalSize , 1, bufferFile) == 1)
           {
               ivrBuffer.IVR_Buffer = Mat(ivrBuffer.IVR_Height , ivrBuffer.IVR_Width, CV_8UC4 , (char*)pShrinkedImage);
           }
           else
           {
               qWarning() << "Problems to open the IIF File!";
               isValid=false;
               return false;
           }
        }
        isValid = true;
    }
    else
    {
        qWarning() << "Problems to open the IIF File!";
        isValid=false;
        return false;
    }

    fclose(bufferFile);
    return true;
}
