#include "Wireframe.h"
#include <cmath>
#include <iostream>
#include <set>

#include "Circle.h"

using Eigen::Vector3d;
using Eigen::Matrix4d;

Wireframe::Wireframe(const BSpline& forming)
{
    rotation = Matrix4d::Identity();
    setForming(forming);
}

void Wireframe::setForming(const BSpline& forming)
{
    forming_ = forming;
    updateShiftScale();
}

Dimensions Wireframe::getDimensions() const
{
    Dimensions dims{};
    auto& points = forming_.points();
    dims.xmax = dims.xmin = points[0].x;
    dims.ymax = dims.ymin = points[0].y;
    dims.zmax = dims.zmin = points[0].z;
    for (auto point : points)
    {
        if (point.x > dims.xmax)
        {
            dims.xmax = point.x;
        }
        else if (point.x < dims.xmin)
        {
            dims.xmin = point.x;
        }

        if (std::abs(point.y) > dims.ymax) //работает, если образующая лежит в плоскости XoY
        {
            dims.ymax = std::abs(point.y);
            dims.zmax = std::abs(point.y);
            dims.ymin = -std::abs(point.y);
            dims.zmin = -std::abs(point.y);
        }
    }
    return dims;
}

std::vector<Polyline3D> Wireframe::getPolylines() const
{
    //get all forming polylines
    std::vector<Polyline3D> polylines;
    const auto& kPoints = forming_.keyPoints();
    for (int i = 0; i < cntFormingLines_; i++)
    {
        using std::sin, std::cos;
        double phi = (double)i / cntFormingLines_ * M_PI * 2;
        Matrix3d rotation{{1, 0, 0}, {0, cos(phi), -sin(phi)}, {0, sin(phi), cos(phi)}};
        std::vector<Point3D> nextKPoints;
        for (auto point : kPoints)
        {
            Vector3d startVector{point.x, point.y, point.z};
            Vector3d result = rotation * startVector;
            Point3D rKeypoint{result.x(), result.y(), result.z()};
            nextKPoints.push_back(rKeypoint);
        }
        BSpline spline {nextKPoints};
        spline.setCntParts(forming_.getCntParts());
        polylines.push_back(spline);
    }

    //get circles
    BSpline sample = forming_;
    sample.setCntParts(1);
    auto& startCircles = sample.points();
    Point3D lastStart = startCircles[0];
    polylines.push_back(Circle(lastStart, cntFormingLines_, cntPartsInSegments_));
    for (auto point : startCircles)
    {
        if (point.x != lastStart.x || point.y != lastStart.y || point.z != lastStart.z)
        {
            polylines.push_back(Circle(point, cntFormingLines_, cntPartsInSegments_));
            lastStart = point;
        }
    }


    return polylines;
}

Eigen::Matrix4d Wireframe::getTransform() const
{
    return transform;
}

void Wireframe::addRotation(const Point3D& start, const Point3D& end)
{
    //будем считать, что дельта по времени небольшое => вращение шло по кратчайшему пути
    // значит start, end и центр фигуры находятся в одной плоскости вращения

    //1. Находим нормаль к плоскости вращения
    Vector3d a{start.x, start.y, start.z};
    Vector3d b{end.x, end.y, end.z};
    a.normalize();
    b.normalize();
    Vector3d norm = a.cross(b);
    //2. Находим угол вращения
    double cosA = a.dot(b) / (a.norm() * b.norm());
    double sinA = norm.norm() / (a.norm() * b.norm());

    //3. Находим матрицу вращения
    Matrix3d dual{{0, -norm.z(), norm.y()}, {norm.z(), 0, -norm.x()}, {-norm.y(), norm.x(), 0}};
    Matrix3d vvt = norm * norm.transpose();
    Matrix3d addRot = vvt + (Matrix3d::Identity() - vvt) * cosA + dual * sinA;

    Matrix4d rot4d{
        {addRot(0, 0), addRot(0, 1), addRot(0, 2), 0},
        {addRot(1, 0), addRot(1, 1), addRot(1, 2), 0},
        {addRot(2, 0), addRot(2, 1), addRot(2, 2), 0},
        {0, 0, 0, 1}
    };

    //4. Домножаем текущую матрицу на новую матрицу, чтобы обновить общую матрицу преобразований

    rotation = rot4d * rotation;


    transform = rotation * shiftScaling;
}

void Wireframe::resetRotation()
{
    rotation.setIdentity();
}

void Wireframe::setCntFormingLines(const int& newM)
{
    cntFormingLines_ = newM;
}

void Wireframe::setCntPartsInSegment(const int& newM1)
{
    cntPartsInSegments_ = newM1;
}

void Wireframe::updateShiftScale()
{
    Dimensions dims = getDimensions();
    double maxSize = std::max(dims.xmax - dims.xmin, dims.ymax);
    Eigen::Matrix4d shift{{1, 0, 0, -(dims.xmax + dims.xmin) / 2}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, maxSize}};
    shiftScaling = shift / maxSize;
    transform = rotation * shiftScaling;
}
