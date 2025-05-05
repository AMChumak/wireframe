#pragma once
#include "BSpline.h"

#include <eigen3/Eigen/Dense>

using Eigen::Matrix3d;

struct Dimensions
{
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    double zmin;
    double zmax;
};

class Wireframe
{
public:
    Wireframe();
    explicit Wireframe(const BSpline &forming);
    void setForming(const BSpline &forming);
    std::vector<Polyline3D> getPolylines() const; // возвращает сырые ломанные
    Eigen::Matrix4d getTransform() const;
    void addRotation (const Point3D& start, const Point3D& end);
    void resetRotation();
    void setCntFormingLines(const int &newM);
    void setCntPartsInSegment(const int &newM1);
    int getPartsInSegment() const;
    int getCntFormingLines() const;
private:
    void updateShiftScale();
    Dimensions getDimensions() const; //returns Xmin, Ymin, Zmin, Xmax,Ymax, Zmax

private:
    Eigen::Matrix4d shiftScaling;
    Matrix4d rotation;
    Eigen::Matrix4d transform;
    BSpline forming_{};
    int cntFormingLines_ = 2;
    int cntPartsInSegments_ = 1;
};


