//#ifndef _FEATURE_SUSIFT_
#pragma once
#define _FEATURE_SUSIFT_

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"

#include <stdio.h>
#include <stdlib.h>
#include "opencv2/opencv_modules.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/ml/ml.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>
#include <numeric>
#include <fstream>
//#include <boost/thread.hpp>

#define CHECK() fprintf(stderr, "%s:%d:check\n", __FILE__, __LINE__)

using namespace cv;
using namespace std;

#include "iFeature.h"

bool sort_kp_aa(cv::KeyPoint a, cv::KeyPoint b);

// test
class feature_susift : public iFeature {
	// C&D
public:
	virtual ~feature_susift() {}

	// ACC
public:
	virtual Mat extract(Mat input) {
		return extract(input, 0, 150);
	}
	virtual Mat extract(Mat input, int from, int to) {
		Mat out;
		out=input;
		resize(input, out, Size(from, to), 0, 0, INTER_CUBIC); 
		
		// TO DO
		initModule_nonfree(); // co to je?
		detector = FeatureDetector::create("SURF");
		control_detector = FeatureDetector::create("SIFT");
		extractor = DescriptorExtractor::create("SIFT");

		// old init
		if(check_object_init()){
			return out;
		}
		img = input;
		if(img.empty()){
			cout<<"No input image\n";
			return out;
		}

		// old compute
		vector<KeyPoint> kp;
		kp = calc_kp();
		std::sort(kp.begin(), kp.end(),sort_kp_aa);
		extractor->compute(img, kp, desc);
		out=desc;
		//write_to_xls();
		clean_up();

		/*namedWindow( "in", 1 );
		namedWindow( "out", 1 );
		imshow( "in", input );
		imshow( "out", out );
		waitKey(0);*/
		
		//out=out.reshape(0, 1);
		out.convertTo(out, CV_32FC1);
		return out;
	}

	// ATR
	private:
		Ptr<FeatureDetector> detector;
		Ptr<FeatureDetector> control_detector;
		Ptr<DescriptorExtractor> extractor;
		Mat img;
		Mat desc;

	// HELP
private:
	float calc_distance(cv::Point2f bod1, cv::Point2f bod2)
	{
		float dx,dy;

		dx=bod1.x-bod2.x;
		dy=bod1.y-bod2.y;

		return sqrt(dx*dx + dy*dy);
	}

	

	int check_object_init()
	{
		if(detector.empty()){
			cout<< "Chyba vytvorenia deskriptora: SURF"<<endl;
			return -1;
		}
	
		if(extractor.empty()){
			cout<< "Chyba vytvorenia extraktora: SIFT"<<endl;
			return -1;
		}
		if(control_detector.empty()){
			cout<< "Chyba vytvorenia kontrol deskriptora: SIFT"<<endl;
			return -1;
		}
		return 0;
	}


	vector<KeyPoint> calc_kp()
	{
		vector<KeyPoint> kp, kp_control,kp_result;	
		int i,j,k;
		float distance,min;
		double mean_val;
		std::vector<float> all_dist;

		detector->detect(img, kp);
		control_detector->detect(img, kp_control);

		for(k=0; k<kp.size(); k++){
			min=1000;
			for(j=0; j< kp_control.size(); j++){
				distance=calc_distance(kp[k].pt,kp_control[j].pt);
				if(distance < min){
					min=distance;
				}
			}
			all_dist.push_back(min);
		}
		mean_val=cv::mean(all_dist).val[0];
		for(k=0; k<kp.size(); k++){
			if(all_dist[k] < mean_val){
				kp_result.push_back(kp[k]);
			}
		}
		return kp_result;
	}

	int write_to_xls()
	{
		FileStorage fs;

		fs.open("susift.yml", FileStorage::WRITE);

		if(fs.isOpened()){
			cout << "Saving desc to file: "<<"susift.yml"<<endl;
			fs <<"desc"<<desc;
		} else {
			fs.release();
			return -1;
		}
		fs.release();
	
		FILE *fw;
		fw= fopen("test.txt", "w");
		int i,j;
		float tmp;
		cout << "type: "<< desc.type()<<endl;
		for(i=0; i<desc.rows; i++){
			for(j=0; j< desc.cols; j++){
				tmp= desc.at<float>(i,j);
				cout << desc.at<float>(i,j)<<" ";
				fprintf(fw,"%f ",tmp);
			}
			cout<<endl;
			fprintf(fw,"\n");
		}

		fclose(fw);



		return 0;
	}

	void clean_up()
	{
		if(!detector->empty()){
			detector.release();
		}
		if(!control_detector->empty()){
			control_detector.release();
		}
		if(!extractor->empty()){
			extractor.release();
		}

	}
};

bool sort_kp_aa(cv::KeyPoint a, cv::KeyPoint b) {
	if(a.response > b.response){
		return true;
	} else {
		return false;
	}
}

//#endif