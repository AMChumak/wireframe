#pragma once

#include <eigen3/Eigen/Dense>

using Eigen::Matrix4d;

#include "Polyline3D.h"


int comparePoints(const Point3D &p1, const Point3D &p2);

class Camera {
public:
    explicit Camera(double width, double height, double zForward, double zBack);
    std::vector <std::pair<Point3D, Point3D>> render(const Matrix4d &prescaling,const std::vector<Polyline3D> &polylines) const;
    void updateSizes(double width, double height);
    void zoom(double zoom);
    Point3D findPointOnSphere(const Point3D& point) const;

private:
    void updateProjection();

private:
    Matrix4d projection_;
    Point3D position_{};
    Point3D direction_{};
    Point3D up_{};
    double width_, height_;
    double zForward_;
    double zBack_;
};
