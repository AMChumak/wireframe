#include "Circle.h"

#include <eigen3/Eigen/Dense>
#include <cmath>
using Eigen::Matrix3d;
using Eigen::Vector3d;

Circle::Circle(Point3D start, int cntSegments, int cntParts) : Polyline3D(
    std::vector<Point3D>())
{
    startPoint_ = start;
    cntSegments_ = cntSegments;
    cntParts_ = cntParts;
    updatePoints();
}


const Point3D& Circle::startPoint() const
{
    return startPoint_;
}

void Circle::setStartPoint(Point3D start)
{
    startPoint_ = start;
    updatePoints();
}


void Circle::setCntSegments(int cntSegments)
{
    cntSegments_ = cntSegments;
    updatePoints();
}

void Circle::setCntParts(int cntParts)
{
    cntParts_ = cntParts;
    updatePoints();
}

const int& Circle::cntSegments() const
{
    return cntSegments_;
}

const int& Circle::cntParts() const
{
    return cntParts_;
}

void Circle::updatePoints()
{
    points_.clear();
    int totalCnt = cntSegments_ * cntParts_;
    Vector3d  startVector{startPoint_.x, startPoint_.y, startPoint_.z};
    for (int i = 0; i <= totalCnt; i++)
    {
        using std::sin, std::cos;
        double phi = (double)i / totalCnt * M_PI * 2;
        Matrix3d rotation {{1, 0, 0}, {0, cos(phi), -sin(phi)}, {0, sin(phi), cos(phi)}};
        Vector3d result = rotation * startVector;
        Point3D point{result.x(), result.y(), result.z()};
        points_.push_back(point);
    }
}
