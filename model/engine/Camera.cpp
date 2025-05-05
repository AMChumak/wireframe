//
// Created by anton on 5/5/25.
//

#include "Camera.h"

#include <iostream>

using Eigen::Matrix4d;

int comparePoints(const Point3D& p1, const Point3D& p2)
{
    if (p1.z - p2.z > 0) return 1;
    else if (p1.z - p2.z < 0) return -1;
    else return 0;
}

Camera::Camera(double width, double height, double zForward, double zBack)
{
    width_ = width;
    height_ = height;
    zForward_ = zForward;
    zBack_ = zBack;
    position_ = {0, 0, -10};
    up_ = {0, 1, 0};
    direction_ = {0, 0, 10};
    updateProjection();
}

std::vector<std::pair<Point3D, Point3D>> Camera::render(const Matrix4d& prescaling,
                                                        const std::vector<Polyline3D>& polylines) const
{
    const Matrix4d transform = projection_ * prescaling;
    std::vector<std::pair<Point3D, Point3D>> result;
    for (const auto& polyline : polylines)
    {
        auto& points = polyline.points();

        Point3D lastPoint = points[0];
        Eigen::Vector4d lastPointVec{lastPoint.x, lastPoint.y, lastPoint.z, 1};
        lastPointVec = transform * lastPointVec;
        lastPointVec = lastPointVec / lastPointVec.w();
        lastPoint.x = lastPointVec.x();
        lastPoint.y = lastPointVec.y();
        lastPoint.z = lastPointVec.z();

        for (int i = 1; i < points.size(); ++i)
        {
            Point3D nextPoint = points[i];
            Eigen::Vector4d nextPointVec{nextPoint.x, nextPoint.y, nextPoint.z, 1};
            nextPointVec = transform * nextPointVec;
            nextPointVec = nextPointVec / nextPointVec.w();
            nextPoint = {nextPointVec.x(), nextPointVec.y(), nextPointVec.z()};
            if (comparePoints(lastPoint, nextPoint) >= 0)
            {
                result.emplace_back(lastPoint, nextPoint);
            }
            else
            {
                result.emplace_back(nextPoint, lastPoint);
            }

            lastPoint = nextPoint;
        }
    }
    std::sort(result.begin(), result.end(), [](std::pair<Point3D, Point3D> a, std::pair<Point3D, Point3D> b) -> bool
    {
        if (comparePoints(a.first, b.first) < 0) return true;
        return false;
    });
    return result;
}

void Camera::updateSizes(double width, double height)
{
    width_ = width;
    height_ = height;
    updateProjection();
}

void Camera::zoom(double zoom)
{
    zForward_ *= zoom;
    if (zForward_ >= 9.7)
    {
        zForward_ = 9.7;
    }
    updateProjection();
}

Point3D Camera::findPointOnSphere(const Point3D& point) const
{
    //с одной стороны знаем прямую, на которой лежит точка, с другой стороны, точка лежит на сфере радиуса корень из 2 с центром в 0,0,10
    //считаем квадратное уравнение и берём точку с полусферы, которая ближе к камере
    double a = point.x * (width_ / 2);
    double b = point.y * (height_ / 2);
    double k = std::sqrt(a*a + b *b);
    double d = 100 - 97 * (1 + k*k/(zForward_*zForward_));
    if (d < 0)
    {
        return {-10,-10,-10};
    }
    double z = (10 - std::sqrt(d)) / (1 + k*k/(zForward_*zForward_));
    a *= z/zForward_;
    b *= z/zForward_;
    z = z - 10;
    return {a, b, z};
}

void Camera::updateProjection()
{
    Matrix4d shift{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, -(position_.z)}, {0, 0, 0, 1}};
    Matrix4d projection{
        {2 / width_ * zForward_, 0, 0, 0}, {0, 2 / height_ * zForward_, 0, 0},
        {0, 0, zBack_ / (zBack_ - zForward_), -zForward_ * zBack_ / (zBack_ - zForward_)}, {0, 0, 1, 0}
    };
    projection_ = projection * shift;
}
