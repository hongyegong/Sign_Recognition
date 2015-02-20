/*
* ml_train.cpp
* SVM code adapted from code written by Toby Breckon, Cranfield University
* http://public.cranfield.ac.uk/c5354/teaching/ml/examples/c++/speech_ex/svm.cpp
*/

#include "ml_train.h"
#include <errno.h>
#include <dirent.h>
#include <iostream>
#include <vector>
#include <string>
#include <sys/types.h>
#include "hog_extractor.h"
#include <opencv/highgui.h>
#include <opencv/ml.h>

using namespace std;

//code from:  jsshaw @ http://www.linuxquestions.org/questions/programming-9/c-list-files-in-directory-379323/
int getDir(string dir, vector<string> &files)
{
	const string DOT(".");
	const string DOTDOT("..");
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(dir.c_str())) == NULL) {
		cout << "Error(" << errno << ") opening " << dir << endl;
		exit(1);
	}

	while ((dirp = readdir(dp)) != NULL)
	{
		string filename = string(dirp->d_name);
		if (filename.compare(DOT) != 0 && filename.compare(DOTDOT) != 0)
			files.push_back(filename);
	}
	closedir(dp);
	return 0;
}

ml_train::ml_train(string _path)
{
	num_train_samples = 0;
	path = _path;
}

void ml_train::train()
{
	vector<vector<string> > image_names = vector<vector<string> >();

	//go thru directory structure
	vector<string> classes = vector<string>();
	getDir(path, classes);
	num_classes = classes.size();

	//each subdir is a class number
	for (int c = 0; c < classes.size(); c++)
	{
		vector<string> files = vector<string>();
		getDir(path + classes[c], files);
		image_names.push_back(files);
		num_train_samples += files.size();
	}

	extract_features(classes, image_names);
	run_SVM();
}

void ml_train::extract_features(vector<string> classes, vector<vector<string> > image_names)
{
	string full_path = path + classes[0] + string("/") + image_names[0][0];
	SignType type = (SignType)atoi(classes[0].c_str());
	Mat image_src = imread(full_path, 0);
	HogExtractor hog(image_src, type);

	training_data.create(num_train_samples, hog.getNumFeatures(), CV_32FC1);
	training_classes.create(num_train_samples, 1, CV_32FC1);

	int row_num = 0;

	for (int c = 0; c < classes.size(); c++)
	{
		type = (SignType)atoi(classes[c].c_str());

		for (int i = 0; i < image_names[c].size(); i++)
		{
			full_path = path + classes[c] + string("/") + image_names[c][i];
			image_src = imread(full_path, 0);
			hog = HogExtractor(image_src, type);
			Mat features = hog.getFeatures();

			for (int f = 0; f < hog.getNumFeatures(); f++)
			{
				training_data.at<float>(row_num, f) = features.at<float>(0, f);
			}
			training_classes.at<float>(row_num, 0) = (int)type;
			row_num++;
		}
	}
}

void ml_train::run_SVM()
{
	// Load training data set
	CvSVMParams params = CvSVMParams(
		CvSVM::C_SVC,   // Type of SVM; using N classes here
		CvSVM::LINEAR,  // Kernel type
		0.0,            // Param (degree) for poly kernel only
		0.0,            // Param (gamma) for poly/rbf kernel only
		0.0,            // Param (coef0) for poly/sigmoid kernel only
		10,             // SVM optimization param C
		0,              // SVM optimization param nu (not used for N class SVM)
		0,              // SVM optimization param p (not used for N class SVM)
		NULL,           // class weights (or priors)
		/*
		* Optional weights, assigned to particular classes.
		* They are multiplied by C and thus affect the misclassification
		* penalty for different classes. The larger the weight, the larger
		* the penalty on misclassification of data from the corresponding
		* class.
		*/

		cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, 0.000001));
	// Termination criteria for the learning algorithm

	CvSVM* svm = new CvSVM;

	cout << "Starting SVM training...\n";

#ifdef AUTO_TRAIN_SVM
	cout << "Finding optimal parameters to use...\n";

	// Use auto-training parameter grid search (ignore params manually
	// specified above)

	cout << "trainingData height = " << trainingData.size().height << " width = " << trainingData.size().width << endl;
	cout << "trainingClass height = " << trainingClass.size().height << " width = " << trainingClass.size().width << endl;
	svm->train_auto(trainingData, trainingClass, Mat(), Mat(), params, 10);
	params = svm->get_params();

	printf("The optimal parameters are: degree = %f, gamma = %f, coef0 = %f, C = %f, nu = %f, p = %f\n",
		params.degree, params.gamma, params.coef0, params.C, params.nu, params.p);
#else
	cout << "Using default parameters...\n";

	// Use regular training with parameters manually specified above
	svm->train(training_data, training_classes, Mat(), Mat(), params);
#endif

	cout << "Number of support vectors in the SVM: " << svm->get_support_vector_count() << endl;

	//save to file
	svm->save("hogsvm.txt");
}

//the files in each class subdir are png training images
int main(int argc, char *argv[])
{
	ml_train trainer("/projects/cs378.mquinlan/kchang/samples/");
	trainer.train();
	return 0;
}