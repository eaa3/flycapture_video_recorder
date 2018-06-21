#include <flycapture/FlyCapture2.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>


#include <cstdlib>
#include <cmath>


using namespace FlyCapture2;

int main()
{
    Error error;
    Camera camera;
    CameraInfo camInfo;

    // Connect the camera
    error = camera.Connect( 0 );
    if ( error != PGRERROR_OK )
    {
        std::cout << "Failed to connect to camera" << std::endl;     
        return false;
    }

    // Get the camera info and print it out
    error = camera.GetCameraInfo( &camInfo );
    if ( error != PGRERROR_OK )
    {
        std::cout << "Failed to get camera info from camera" << std::endl;     
        return false;
    }
    std::cout << camInfo.vendorName << " "
              << camInfo.modelName << " " 
              << camInfo.serialNumber << std::endl;
	
    error = camera.StartCapture();
    if ( error == PGRERROR_ISOCH_BANDWIDTH_EXCEEDED )
    {
        std::cout << "Bandwidth exceeded" << std::endl;     
        return false;
    }
    else if ( error != PGRERROR_OK )
    {
        std::cout << "Failed to start image capture" << std::endl;     
        return false;
    } 

    double MAX_FPS = 25;
    double period = (1000*1.0)/MAX_FPS;


    // Get the image
    Image rawImage;
    error = camera.RetrieveBuffer( &rawImage );
    if ( error != PGRERROR_OK )
    {
        std::cout << "capture error" << std::endl;
            return -1;
    }

    // convert to rgb
    Image rgbImage;
    rawImage.Convert( FlyCapture2::PIXEL_FORMAT_BGR, &rgbImage );

    std::cout << rgbImage.GetCols() << " x " << rgbImage.GetRows() << std::endl;
    cv::VideoWriter video("out.avi",CV_FOURCC('M','J','P','G'), MAX_FPS, cv::Size(rgbImage.GetCols(),rgbImage.GetRows()),true);

    // capture loop
    char key = 0;



    bool recording = false;
    while(key != 'q')
    {
        int64 t0 = cv::getTickCount();

        // Get the image
        error = camera.RetrieveBuffer( &rawImage );
        if ( error != PGRERROR_OK )
        {
                std::cout << "capture error" << std::endl;
                continue;
        }

        // convert to rgb
        rawImage.Convert( FlyCapture2::PIXEL_FORMAT_BGR, &rgbImage );

        //std::cout << rgbImage.GetCols() << " x " << rgbImage.GetRows() << std::endl;
        // convert to OpenCV Mat
        unsigned int rowBytes = (double)rgbImage.GetReceivedDataSize()/(double)rgbImage.GetRows();       
        cv::Mat image = cv::Mat(rgbImage.GetRows(), rgbImage.GetCols(), CV_8UC3, rgbImage.GetData(),rowBytes);

        cv::imshow("image", image);

        if(recording)
            video.write(image);
        

        int64 tf = cv::getTickCount();   

        double time_elapsed = (tf-t0)*1000.0/cv::getTickFrequency();
        int64 dt = int64(period - time_elapsed);
        int64 sleep_time = std::max(dt,int64(1));
        // std::cout << "dt: " << dt << std::endl;
        // std::cout << "time_elapsed: " << time_elapsed << std::endl;
        // std::cout << "sleep_time: " << sleep_time << std::endl;

        key = cv::waitKey(sleep_time); 

        if( key == 'r' ){
            recording = !recording;

            std::cout << "Recording: " << (recording? "True" : "False") << std::endl;
        }
    }

    error = camera.StopCapture();
    if ( error != PGRERROR_OK )
    {
        // This may fail when the camera was removed, so don't show 
        // an error message
    }  

    camera.Disconnect();

    return 0;
}