//
// Created by anton on 5/4/25.
//

#include "Polyline3D.h"

Point3D& Point3D::operator+=(const Point3D& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Point3D Point3D::operator+(const Point3D& other) const
{
    return Point3D(*this) += other;
}

Point3D& Point3D::operator-=(const Point3D& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Point3D Point3D::operator-(const Point3D& other) const
{
    return Point3D(*this) -= other;
}

Point3D& Point3D::operator*=(double q)
{
    x *= q;
    y *= q;
    z *= q;
    return *this;
}

Point3D& Point3D::operator*(double zoom) const
{
    return Point3D(*this) *= zoom;
}

Polyline3D::Polyline3D(std::vector<Point3D> points)
{
    points_ = std::move(points);
}

const std::vector<Point3D>& Polyline3D::points()
{
    return points_;
}
