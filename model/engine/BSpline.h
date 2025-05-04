//
// Created by anton on 5/4/25.
//

#ifndef BSPLINE_H
#define BSPLINE_H
#include "Polyline3D.h"
#include <eigen3/Eigen/Dense>

using Eigen::Matrix4d;
using Eigen::Vector4d;

class BSpline: public Polyline3D {
    public:
    explicit BSpline();
    explicit BSpline(std::vector<Point3D> keyPoints);
    [[nodiscard]]const std::vector<Point3D> &keyPoints() const;
    void addKeyPoint(const Point3D& point);
    void removeKeyPoint(const int& position);
    void updateKeyPoint(const int& position, const double& x, const double& y,const double &z);
    void updateKeyPoint(const int& position, const Point3D &newPoint);
    void setCntParts(int cntParts);
    [[nodiscard]] int getCntParts() const;

    private:
    void updatePoints();

private:
    using Polyline3D::points_;
    std::vector<Point3D> keyPoints_;
    int cntParts_ = 1; // count of parts of one segment in B-Spline
    Matrix4d coefficients {{-1,3,-3,1},{3,-6,3,0},{-3,0,3,0},{1,4,1,0}}; // matrix for bspline computation
};



#endif //BSPLINE_H
