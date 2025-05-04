#pragma once
#include "Polyline3D.h"

class Circle: public Polyline3D {
public:
    explicit Circle(Point3D start, int cntSegments= 2, int cntParts = 1);

public:
    const Point3D &startPoint() const;
    void setStartPoint(Point3D start);
    void setCntSegments(int cntSegments);
    void setCntParts(int cntParts);
    const int &cntSegments() const;
    const int &cntParts() const;

private:
    void updatePoints();

private:
    using Polyline3D::points_;
    Point3D startPoint_;
    int cntParts_;
    int cntSegments_;
};


