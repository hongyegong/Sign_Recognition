
#ifndef ML_TRAIN_H
#define ML_TRAIN_H

#include <opencv/cv.h>

using namespace cv;

class ml_train
{
private:
	int num_train_samples;
	int num_attr_per_sample;
	int num_classes;
	string path;
	Mat training_data;
	Mat training_classes;

	void extract_features(vector<string>, vector<vector<string> >);

	void run_SVM();

public:
	ml_train(string _path);
	void train();
};

#endif // ML_TRAIN_H