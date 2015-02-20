
#include "extractor.h"

Extractor::Extractor()
{ }

Extractor::Extractor(Mat _rawImage, SignType _actualType) :
actualType(_actualType),
predictedType(UNKNOWN)
{
	extract(_rawImage);
}

Extractor::~Extractor()
{ }

int Extractor::getNumFeatures()
{
	return featureVector.size().width;
}

Mat Extractor::getFeatures()
{
	return featureVector;
}

SignType Extractor::getPredictedType()
{
	return predictedType;
}

void Extractor::setPredictedType(SignType _predictedType)
{
	predictedType = _predictedType;
}

SignType Extractor::getActualType()
{
	return actualType;
}