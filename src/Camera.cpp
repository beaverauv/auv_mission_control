#include <auv_mission_control/Camera.h>




Camera::Camera(){
        capFront.open(0);
        if (!capFront.isOpened()) {
                ROS_ERROR("Unable to open Front Camera");
        }

        capBottom.open(1);
        if (!capBottom.isOpened()) {
                ROS_ERROR("Unable to open bottom Camera");
        }

        capFront.set(CV_CAP_PROP_FRAME_WIDTH, imgWidth);
        capFront.set(CV_CAP_PROP_FRAME_HEIGHT, imgHeight);
        capBottom.set(CV_CAP_PROP_FRAME_WIDTH, imgWidth);
        capBottom.set(CV_CAP_PROP_FRAME_HEIGHT, imgHeight);

        capFront.set(CV_CAP_PROP_FPS, camFPS);
        capBottom.set(CV_CAP_PROP_FPS, camFPS);


//
}



Camera::~Camera(){
        stopRecording();
}

void Camera::startRecording(){
        time_t rawtime;
        struct tm * timeinfo;
        char buffer[80];

        time (&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(buffer,80,"%d-%m-%Y %I:%M:%S",timeinfo);
        std::string str(buffer);

        filenameVideoFront = 'FrontVid-' + buffer;
        filenameVideoBottom = 'BottomVid-' + buffer;
        AUV_INFO("Writing Video to " + filenameVideoFront + " and " + filenameVideoFront);

        writerFront.open(filenameVideoFront, codeFourcc, camFPS, cv::Size(imgWidth, imgHeight));
        writerBottom.open(filenameVideoBottom, codeFourcc, camFPS, cv::Size(imgWidth, imgHeight));

        isRecording = true;
}

void Camera::stopRecording(){
        if (isRecording) {
                writerFront.release();
                writerBottom.release();
        }
}


void Camera::updateFrames(){
        if (!capFront.read(lastFrontImage)) {
                ROS_ERROR("Failed to read from front camera");
        }

        if (!capBottom.read(lastBottomImage)) {
                ROS_ERROR("Failed to read from bottom Camera");
        }

        if (isRecording) {
                writerFront.write(lastFrontImage);
                writerBottom.write(lastBottomImage);
        }
        fpsRate.sleep();

}

cv::Mat Camera::getFront(){
        return lastFrontImage;
}
cv::Mat Camera::getBottom(){
        return lastBottomImage;
}
