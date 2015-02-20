
#include "hog_extractor.h"

HogExtractor::HogExtractor()
{ }

HogExtractor::HogExtractor(Mat _rawImage, SignType actualType)
: Extractor(_rawImage, actualType)
{
	extract(_rawImage);
}

HogExtractor::~HogExtractor()
{
	featureVector.release();
}

void HogExtractor::extract(Mat _rawImage)
{
	if (!_rawImage.empty())
	{
		vector<float> hog_features;
		HOGDescriptor hog = HOGDescriptor();

		Mat image;
		resize(_rawImage, image, Size(50, 50));

		hog.compute(image, hog_features, Size(10, 10), Size(50, 50));

		featureVector.create(1, hog_features.size(), CV_32FC1);

		for (int c = 0; c < hog_features.size(); c++)
		{
			featureVector.at<float>(0, c) = hog_features[c];
		}

		image.release();
	}
}