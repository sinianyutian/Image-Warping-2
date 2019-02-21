#include "ImageWrap.h"
#include <cmath>
#include <QImage>
#include <ANN/ANN.h>					// ANN declarations

int				k				= 1;			// number of nearest neighbors
int				dim				= 2;			// dimension
double			eps				= 0;			// error bound
int				maxPts			= 1000;			// maximum number of data points
int             sqRad           = 1;

Wrap::Wrap()
{

}

Wrap::~Wrap()
{

}
int Wrap::Init(QPolygon &StartPoints, QPolygon &EndPoints)
{
	return 0;
}

int Wrap::DoWrap(QImage &image, QPolygon &StartPoints, QPolygon &EndPoints)
{
	return 0;
}

//searchradius:R^2
int Wrap::FixImage(QImage &image, int searchradius, int searchcount)
{
	int colorred,colorgreen,colorblue;
	int usecount;
	QRgb color;

	int					nPts;					// actual number of data points
	ANNpointArray		dataPts;				// data points
	ANNpoint			queryPt;				// query point
	ANNidxArray			nnIdx;					// near neighbor indices
	ANNdistArray		dists;					// near neighbor distances
	ANNkd_tree*			kdTree;					// search structure

	maxPts = image.height() * image.width();
	k = searchcount;
	sqRad = searchradius;
	usecount = 0;

	colorred = colorblue = colorgreen = 0;

	queryPt = annAllocPt(dim);					// allocate query point
	dataPts = annAllocPts(maxPts, dim);			// allocate data points
	nnIdx = new ANNidx[k];						// allocate near neigh indices
	dists = new ANNdist[k];						// allocate near neighbor dists


	nPts = 0;									// read data points

	cout << "Data Points:\n";
	for (int i=0; i<image.width(); i++)
	{
		for (int j=0; j<image.height(); j++)
		{
			if (MatrixSet(j,i) == 1)
			{
				dataPts[nPts][0] = i;			//2D
				dataPts[nPts][1] = j;
				nPts++;	
			}
		}
	}

	kdTree = new ANNkd_tree(					// build search structure
		dataPts,								// the data points
		nPts,									// number of points
		dim);									// dimension of space


	for (int i=0; i<image.width(); i++)
	{
		for (int j=0; j<image.height(); j++)
		{
			if (MatrixSet(j,i) == 0)
			{
				queryPt[0] = i;					//2D
				queryPt[1] = j;

				kdTree->annkFRSearch(			// search
					queryPt,					// query point
					sqRad,
					k,							// number of near neighbors
					nnIdx,						// nearest neighbors (returned)
					dists,						// distance (returned)
					eps);						// error bound

				colorred = 0;
				colorblue = 0;
				colorgreen = 0;

				for (usecount = 0; usecount < k; usecount++)
				{
					if (ANN_DIST_INF == dists[usecount])
					{
						break;
					}
					else
					{
						color = image.pixel(dataPts[nnIdx[usecount]][0], dataPts[nnIdx[usecount]][1]);
						colorred = colorred + qRed(color);
						colorgreen = colorgreen + qGreen(color);
						colorblue = colorblue + qBlue(color);

					}

				}

				if(usecount != 0)
				{
					image.setPixel(i, j, qRgb(colorred/usecount, colorgreen/usecount, colorblue/usecount));
				}


			}
		}
	}

	delete [] nnIdx;							// clean things up
	delete [] dists;
	delete kdTree;
	annClose();									// done with ANN

	return 0;
}

double Wrap::GetDistance(QPoint PointA, QPoint PointB)
{
	double result;

	result = pow((double)(PointA.x() - PointB.x()), 2) + pow((double)(PointA.y() - PointB.y()), 2); //a^2 + b^2
	result = sqrt(result);                      

	return result;
}

