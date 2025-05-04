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
    Eigen::MatrixXd getTransform() const;
    void addRotation (const Point3D& start, const Point3D& end);
    void resetRotation();
private:
    void updateShiftScale();
    Dimensions getDimensions() const; //returns Xmin, Ymin, Zmin, Xmax,Ymax, Zmax

private:
    Eigen::MatrixXd shiftScaling;
    Matrix3d rotation;
    Eigen::MatrixXd transform;
    BSpline forming_;
    int cntFormingLines_ = 2;
    int cntPartsInSegments_ = 1;
};
