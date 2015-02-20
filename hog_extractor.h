#ifndef HOG_EXTRACTOR_H
#define HOG_EXTRACTOR_H

#include "extractor.h"

class HogExtractor : public Extractor
{
public:
	HogExtractor(Mat _rawImage, SignType actualType);
	~HogExtractor();

protected:
	HogExtractor();

	void extract(Mat _rawImage);
};

#endif // HOG_EXTRACTOR_H