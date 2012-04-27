#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H (1)

#include <QVector3D>
#include "BoundingBox.h"
#include "Scalar.h"

class BoundingBox
{
	libthing::BoundingBox innerBox;

//private:
//    QVector3D m_min, m_max;
//
public:
    BoundingBox();
    BoundingBox(const QVector3D min, const QVector3D max);
    void addPoint(const QVector3D p);

    const QVector3D min() const {
    	return QVector3D(innerBox.xmin(), innerBox.ymin(),innerBox.zmin());
    }
    const QVector3D max() const {
    	return QVector3D(innerBox.xmax(), innerBox.ymax(),innerBox.zmax());
    }
//
    QVector3D center() const;
//
    BoundingBox& operator+=(const QVector3D& p);
    BoundingBox& operator+=(const BoundingBox& b);

    bool operator==(const BoundingBox& b) { return b.innerBox == innerBox; }
    bool isEmpty() const;

    double xmin() const { return innerBox.xmin(); }
    double xmax() const { return innerBox.xmax(); }
    double ymin() const { return innerBox.ymin(); }
    double ymax() const { return innerBox.ymax(); }
    double zmin() const { return innerBox.zmin(); }
    double zmax() const { return innerBox.zmax(); }
};

#endif // BOUNDINGBOX_H
