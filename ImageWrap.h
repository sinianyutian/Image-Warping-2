#ifndef WRAP_H
#define WRAP_H

#include <QPolygon>
#include <vector>
#include <QImage>
#include <iostream>
#include <Dense>

using namespace std;
using namespace Eigen;

class Wrap
{
public:
	Wrap(void);
	virtual ~Wrap(void);
	virtual int Init(QPolygon &StartPoints, QPolygon &EndPoints);
	virtual int DoWrap(QImage &image, QPolygon &StartPoints, QPolygon &EndPoints);
	int FixImage(QImage &image, int searchradius, int searchcount);                                       //Fix the image after wraping
	double GetDistance(QPoint PointA, QPoint PointB);

	MatrixXi MatrixSet;

private:


};

#endif