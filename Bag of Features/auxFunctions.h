//CSCI574 HW5
//Ahmet Can Ozbek
//8401860152
#ifndef __CSCI574HW5__auxFunctions__
#define __CSCI574HW5__auxFunctions__

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/nonfree/features2d.hpp>


using namespace cv;

namespace aux {
    //Functions
    //Reading Images and Constructing True Labels
    void readTrainImages(Mat* inputMatArray, int arraySize);
    void readTestImages(Mat* inputMatArray, int arraySize);
    void getTrueTrainingLabels(Mat& trueTrainingLabels, int size);
    void getTrueTestLabels(Mat& trueTestLabels, int size);
    String getClassName(int index);
    
    //Get the words
    Mat getWords(Mat* trainImages, Mat* descriptorsOfTrainImages, PCA& pcaSIFT, SIFT& siftExtractor, int pcaDimension, int numberOfClusters);
    Mat getHistogram(Mat& inputIm, Mat& codewords, SIFT& siftExtractor, PCA& pcaSIFT);
    int getClosestCodewordIndex(Mat& inputFeatureVector, Mat& codewords);
    
    //Get Error Rate
    double getErrorRate(Mat trueLabels, Mat predictedLabels);
    //Get Confusion Matrix
    Mat getConfusionMatrix(Mat trueLabels, Mat predictedLabels, int numberOfClasses);
}


#endif /* defined(__CSCI574HW5__auxFunctions__) */
