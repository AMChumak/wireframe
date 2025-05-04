//
// Created by anton on 5/4/25.
//

#include "BSpline.h"

#include <iostream>

BSpline::BSpline(): Polyline3D(std::vector<Point3D>())
{
}

BSpline::BSpline(std::vector<Point3D> keyPoints): Polyline3D(std::vector<Point3D>())
{
    keyPoints_ = std::move(keyPoints);
    updatePoints();
}

const std::vector<Point3D>& BSpline::keyPoints()
{
    return keyPoints_;
}

void BSpline::addKeyPoint(const Point3D& point)
{
    keyPoints_.push_back(point);
    updatePoints();
}

void BSpline::removeKeyPoint(const int &position)
{
    keyPoints_.erase(keyPoints_.begin() + position);
    updatePoints();
}

void BSpline::updateKeyPoint(const int& position, const double& x, const double& y, const double& z)
{
    keyPoints_.at(position).x = x;
    keyPoints_.at(position).y = y;
    keyPoints_.at(position).z = z;
    updatePoints();
}

void BSpline::updateKeyPoint(const int& position, const Point3D& newPoint)
{
    keyPoints_[position] = newPoint;
    updatePoints();
}

void BSpline::setCntParts(int cntParts)
{
    cntParts_ = cntParts;
    updatePoints();
}

int BSpline::getCntParts() const
{
    return cntParts_;
}

void BSpline::updatePoints()
{
    points_.clear();
    if (keyPoints_.size() < 4)
        return;

    for (int i = 1; i < keyPoints_.size() - 2; i++)
    {
        Vector4d xVector = { keyPoints_[i-1].x, keyPoints_[i].x , keyPoints_[i+1].x , keyPoints_[i+2].x};
        Vector4d yVector = { keyPoints_[i-1].y, keyPoints_[i].y , keyPoints_[i+1].y , keyPoints_[i+2].y};
        Vector4d zVector = { keyPoints_[i-1].z, keyPoints_[i].z , keyPoints_[i+1].z , keyPoints_[i+2].z};
        for (int j = 0; j <= cntParts_; j++)
        {
            double t = static_cast<double>(j)/(cntParts_ + 1);
            Point3D point{};

            Vector4d timeVector = {t*t*t, t*t, t , 1};


            point.x = (timeVector.transpose() * coefficients * xVector).value() / 6;
            point.y = (timeVector.transpose() * coefficients * yVector).value() / 6;
            point.z = (timeVector.transpose() * coefficients * zVector).value() / 6;
            points_.push_back(point);
        }
    }
}
