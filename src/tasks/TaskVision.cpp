#include <auv_mission_control/TaskVision.h>



TaskVision::TaskVision(){

}

TaskVision::~TaskVision(){

}

TaskVision::TaskVision(Camera* cam) : cam_(*cam){

}


void TaskVision::findBuoy(int color){
  cam_.updateBottom();
  imgOrigBottom = cam_.getBottom();

  cv::cvtColor(imgOrigBottom, imgHlsBottom, CV_BGR2HLS);

  if (color == COLOR_RED){
    cv::inRange(imgHlsBottom, sRedMin, sRedMax, imgThreshBottom);
  } else if (color == COLOR_GREEN){
    cv::inRange(imgHlsBottom, sGreenMin, sGreenMax, imgThreshBottom);
  } else {
    ROS_ERROR("[TASK VISION] INVALID COLOR");
  }

  cv::dilate(imgThreshBottom, imgThreshBottom, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
  cv::erode(imgThreshBottom, imgThreshBottom, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));

  cv::erode(imgThreshBottom, imgThreshBottom, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
  cv::dilate(imgThreshBottom, imgThreshBottom, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));

  cv::dilate(imgThreshBottom, imgThreshBottom, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7,7)));

  momentsBuoy = cv::moments(imgThreshBottom);

  buoyArea = momentsBuoy.m00;

  buoyCoordX = momentsBuoy.m10 / buoyArea;
  buoyCoordY = momentsBuoy.m01 / buoyArea;

  //here for if we need it
  buoyCoordCorrectedX = buoyCoordX;
  buoyCoordCorrectedY = buoyCoordY;


}


double TaskVision::getBuoyArea(){
  return buoyArea;
}


double TaskVision::getBuoyCoordX(){
  return buoyCoordCorrectedX;
}


double TaskVision::getBuoyCoordY(){
  return buoyCoordCorrectedY;
}

void TaskVision::findMarker(){
  cam_.updateBottom();
  imgOrigBottom = cam_.getBottom();

  cv::cvtColor(imgOrigBottom, imgHlsBottom, CV_BGR2HLS);

  cv::inRange(imgHlsBottom, sRedMin, sRedMax, imgThreshBottom);

  cv::dilate(imgThreshBottom, imgThreshBottom, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
  cv::erode(imgThreshBottom, imgThreshBottom, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));

  cv::erode(imgThreshBottom, imgThreshBottom, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
  cv::dilate(imgThreshBottom, imgThreshBottom, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));

  cv::dilate(imgThreshBottom, imgContoursBottom, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7,7)));
  cv::dilate(imgThreshBottom, imgThreshBottom, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7,7)));

  momentsMarker = cv::moments(imgThreshBottom);

  markerArea = momentsMarker.m00;

  markerCoordX = momentsMarker.m10 / markerArea;
  markerCoordY = momentsMarker.m01 / markerArea;

  //here for if we need it
  markerCoordCorrectedX = markerCoordX;
  markerCoordCorrectedY = markerCoordY;

  cv::findContours( imgContoursBottom, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

  for( int i = 0; i < contours.size(); i++ )
  {
          double count = contours[i].size();

          if( count < 6 )
                  continue;

          numLargeContours++;

          double a = contourArea ( contours[i], false);
          if ( a > largestArea )
          {
                  largestArea = a;
                  largestContourIndex = i;
          }
  }

  if (numLargeContours >= 1) {
          markerRect = cv::fitEllipse(contours[largestContourIndex]);


          //cv::Point2f rect_points[4];

          //markerRect.points( rect_points );


          double currentAngle = markerRect.angle - 90;
          if (markerRect.size.width < markerRect.size.height)
                  currentAngle = 90 + currentAngle;

          averagedAngle = (currentAngle * .1) + (averagedAngle * .9);


  } else {
          //ROS_INFO("No Contours greater than 6");
  }
}


double TaskVision::getMarkerArea(){
  return markerArea;
}


double TaskVision::getMarkerCoordX(){
  return markerCoordCorrectedX;
}


double TaskVision::getMarkerCoordY(){
  return markerCoordCorrectedY;
}


double TaskVision::getMarkerAngle(){
  return averagedAngle;
}
