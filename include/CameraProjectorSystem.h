#ifndef _ARGOS_CAMERAPROJECTORSYSTEM_H
#define _ARGOS_CAMERAPROJECTORSYSTEM_H

#include <opencv2/opencv.hpp>
#include "CameraModel.h"

using namespace std;

namespace argosClient {

  class CameraProjectorSystem{

  public:
    CameraProjectorSystem();

    void setCalibrationsPath(const string& path);
    void load(const string& cameraConfig = "calibrationCamera.xml",
              const string& projectorConfig  = "calibrationProjector.xml",
              const string& extrinsicsConfig = "CameraProjectorExtrinsics.xml");

    void saveExtrinsics(const string& filename) const;
    void loadExtrinsics(const string& filename);

    bool isValid() const;

    vector<cv::Point2f> getProjected(const vector<cv::Point3f>& objectPoints,
                                     const cv::Mat& rotObjToCam = cv::Mat::zeros(3, 1, CV_64F),
                                     const cv::Mat& transObjToCam = cv::Mat::zeros(3, 1, CV_64F));

    CameraModel& getCamera() { return camera; }
    CameraModel& getProjector() { return projector; }

    const cv::Mat& getCamToProjRotation() { return rotCamToProj; }
    const cv::Mat& getCamToProjTranslation() { return transCamToProj; }

  protected:
    CameraModel camera;
    CameraModel projector;

    cv::Mat rotCamToProj;
    cv::Mat transCamToProj;

    string calibrationsPath;
  };

}

#endif
