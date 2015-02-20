
#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <opencv/cv.h>

using namespace cv;

enum SignType
{
	UNKNOWN,
	NONE,
	STOP,
	YIELD,
	SPEED_LIMIT
};

class Extractor
{
protected:
	Extractor();
	Extractor(Mat _rawImage, SignType _actualType = UNKNOWN);
	~Extractor();

	Mat featureVector;
	enum SignType actualType;
	enum SignType predictedType;

	virtual void extract(Mat _rawImage) {}

public:
	int getNumFeatures();
	Mat getFeatures();
	SignType getPredictedType();
	void setPredictedType(SignType _predictedType);
	SignType getActualType();
};

#endif // EXTRACTOR_H