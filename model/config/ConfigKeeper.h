#pragma once

#include "BSpline.h"

class ConfigKeeper
{
public:
    ConfigKeeper();
    std::string saveConfig(const std::string& fileName);
    std::string loadConfig(const std::string& fileName);

public:
    BSpline spline;
    int M;
    int M1;
    Point3D cameraCenter{};
    double cameraZoom;
    Eigen::Matrix<double,4,4> rotation;
    double zForward;
};
