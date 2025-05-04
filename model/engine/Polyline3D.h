//
// Created by anton on 5/4/25.
//

#ifndef POLYLINE3D_H
#define POLYLINE3D_H
#include <vector>


struct Point3D
{
    double x;
    double y;
    double z;

    Point3D &operator +=(const Point3D &other);
    Point3D operator +(const Point3D &other) const;
    Point3D &operator -=(const Point3D &other);
    Point3D operator -(const Point3D &other) const;
    Point3D& operator*=(double q);
    Point3D& operator*(double q) const;
    Point3D& operator/=(double q);
    Point3D& operator/(double q) const;

};


inline Point3D& Point3D::operator/=(double q)
{
    x /= q;
    y /= q;
    z /= q;
    return *this;
}

inline Point3D& Point3D::operator/(double q) const
{
    return Point3D(*this) /= q;
}

class Polyline3D
{
public:
    explicit Polyline3D(std::vector<Point3D> points);
    [[nodiscard]] const std::vector<Point3D>& points() const;

protected:
    std::vector<Point3D> points_;
};


#endif //POLYLINE3D_H
