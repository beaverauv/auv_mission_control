#include <auv_mission_control/Camera.hpp>

Camera::Camera(){
        AUV_INFO("Constructor");
        capFront.open(0);

        if (!capFront.isOpened()) {
                AUV_ERROR("Unable to open Front Camera");
        } else {
                AUV_INFO("Opened Front Camera");
                isFrontOpened = true;
        }

        capBottom.open(1);
        if (!capBottom.isOpened()) {
                AUV_ERROR("Unable to open bottom Camera");
        } else {
                AUV_INFO("Opened Bottom Camera");
                isBottomOpened = true;
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
        strftime(buffer,80,"%d-%m-%Y-%I:%M:%S",timeinfo);
        std::string str(buffer);

        sprintf(filenameVideoFront, "FrontVid-%s.mp4", buffer);
        sprintf(filenameVideoBottom, "BottomVid-%s.mp4", buffer);

        AUV_INFO("Writing Video to %s and %s", filenameVideoFront, filenameVideoBottom);

        if (isFrontOpened) {
                writerFront.open(filenameVideoFront, codeFourcc, camFPS, cv::Size(imgWidth, imgHeight));
                isFrontRecording = true;
        }

        if (isBottomOpened) {
                writerBottom.open(filenameVideoBottom, codeFourcc, camFPS, cv::Size(imgWidth, imgHeight));
                isBottomRecording = true;
        }

}

void Camera::stopRecording(){
        if (isFrontRecording)
                writerFront.release();

        if (isBottomOpened)
                writerBottom.release();
}


void Camera::updateFrames(){
        if (isFrontOpened) {
                if (!capFront.read(lastFrontImage)) {
                        ROS_ERROR("Failed to read from front camera");
                } else if (isFrontRecording) {
                        writerFront.write(lastFrontImage);
                }
        }

        if (isBottomOpened) {
                if (!capBottom.read(lastBottomImage)) {
                        ROS_ERROR("Failed to read from front camera");
                } else if (isBottomRecording) {
                        writerBottom.write(lastBottomImage);
                }
        }
}

cv::Mat Camera::getFront(){
        return lastFrontImage;
}
cv::Mat Camera::getBottom(){
        return lastBottomImage;
}
