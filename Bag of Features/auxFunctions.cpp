//CSCI574 HW5
//Ahmet Can Ozbek
//8401860152

#include "auxFunctions.h"
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <vector>
#include <iostream>
#include <string>

#define N_OF_CLASSES 5
#define N_OF_TRAINING_IMAGES 100
#define N_OF_TRAINING_IMAGES_PERCLASS 20
#define N_OF_TEST_IMAGES_PERCLASS 10
#define N_OF_TEST_IMAGES 50

using namespace cv;
using namespace std;

void aux::readTrainImages(Mat* inputMatArray, int arraySize){
    
    String fileLocation = "/Users/ahmetcanozbek/Desktop/CSCI574/Homeworks/hw5/images/";
    String folderName = "/train/";
    String fileExtension = ".jpg";
    String fullPathFileName;
    
    for(int classIndex=0; classIndex<N_OF_CLASSES; classIndex++){
        for(int i=0; i<N_OF_TRAINING_IMAGES_PERCLASS; i++){
            //construct full path filename
            fullPathFileName = fileLocation + getClassName(classIndex) + folderName + to_string(i+1) + fileExtension;
            inputMatArray[classIndex*N_OF_TRAINING_IMAGES_PERCLASS + i] = imread(fullPathFileName,0);
        }
    }
}

void aux::readTestImages(Mat* inputMatArray, int arraySize){
    
    String fileLocation = "/Users/ahmetcanozbek/Desktop/CSCI574/Homeworks/hw5/images/";
    String folderName = "/test/";
    String fileExtension = ".jpg";
    String fullPathFileName;
    
    for(int classIndex=0; classIndex<N_OF_CLASSES; classIndex++){
        for(int i=0; i<N_OF_TEST_IMAGES_PERCLASS; i++){
            //construct full path filename
            fullPathFileName = fileLocation + getClassName(classIndex) + folderName + to_string(i+1) + fileExtension;
            inputMatArray[classIndex*N_OF_TEST_IMAGES_PERCLASS + i] = imread(fullPathFileName,0);
        }
    }
}

void aux::getTrueTrainingLabels(Mat& trueTrainingLabels, int size){
    
    trueTrainingLabels = Mat::zeros(N_OF_TRAINING_IMAGES, 1, CV_32S);
    
    for(int classNumber=0; classNumber<N_OF_CLASSES; classNumber++){
        for(int i=0; i<N_OF_TRAINING_IMAGES_PERCLASS; i++){
            trueTrainingLabels.at<int>(classNumber*N_OF_TRAINING_IMAGES_PERCLASS+i,0) = classNumber;
        }
    }
}

void aux::getTrueTestLabels(Mat& trueTestLabels, int size){
    
    trueTestLabels = Mat::zeros(N_OF_TEST_IMAGES, 1, CV_32S);
    
    for(int classNumber=0; classNumber<N_OF_CLASSES; classNumber++){
        for(int i=0; i<N_OF_TEST_IMAGES_PERCLASS; i++){
            trueTestLabels.at<int>(classNumber*N_OF_TEST_IMAGES_PERCLASS+i,0) = classNumber;
        }
    }
}


Mat aux::getWords(Mat* trainImages, Mat* descriptorsOfTrainImages, PCA& pcaSIFT, SIFT& siftExtractor, int pcaDimension, int numberOfClusters){
    
    //SIFT feature extraction
    //Get descriptors of training images
    vector<KeyPoint> keyPoints;    
    for(int i=0; i<N_OF_TRAINING_IMAGES; i++){
        siftExtractor.operator()(trainImages[i], noArray(), keyPoints, descriptorsOfTrainImages[i]);
    }   
    
    //Merge all SIFT features from all training images to apply PCA
    Mat mergedTrainDescriptors;
    vconcat(descriptorsOfTrainImages, N_OF_TRAINING_IMAGES, mergedTrainDescriptors);
        
    //Apply PCA to merged SIFT descriptors of all training images    
    pcaSIFT = PCA(mergedTrainDescriptors, Mat(), 0, pcaDimension);
    Mat projectedMergedTrainDescriptors;
    pcaSIFT.project(mergedTrainDescriptors, projectedMergedTrainDescriptors);
        
    //k-means clustering merged SIFT features of all training images
    Mat labels;
    int attempts = 5;
    Mat clusterMeans;
    kmeans(projectedMergedTrainDescriptors, numberOfClusters, labels, TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS,
           clusterMeans);
    
    
    return clusterMeans;
}

/**
    Returns the histogram feature of an image
 
    @param inputIm input image
    @param codeWords codewords from the training data (cluster centers)
    @return histogram of codewords of the input image
*/

Mat aux::getHistogram(Mat& inputIm, Mat& codewords, SIFT& siftExtractor, PCA& pcaSIFT){
    
    //Initialize Histogram
    int numberOfHistogramBins = codewords.rows;
    Mat histogram = Mat::zeros(1, numberOfHistogramBins, CV_32FC1);
    
    //Extract SIFT features of the input image
    vector<KeyPoint> keyPoints; //dummy keyPoints variable
    Mat inputImDescriptor;
    siftExtractor.operator()(inputIm, noArray(), keyPoints, inputImDescriptor); //extract SIFT features
    //Apply PCA to SIFT features of input image (reduce the dimensionality)
    Mat projectedInputImDescriptor;
    pcaSIFT.project(inputImDescriptor, projectedInputImDescriptor);
    
    //Construct the histogram
    int codewordIndex = 0;
    for(int i=0; i<projectedInputImDescriptor.rows; i++){
        //Quantize the vector, (get the index)
        Mat featureVectorToBeQuantized = projectedInputImDescriptor.row(i);
        codewordIndex = aux::getClosestCodewordIndex(featureVectorToBeQuantized, codewords);
        //Update the histogram
        histogram.at<float_t>(0,codewordIndex) = histogram.at<float_t>(0,codewordIndex) + 1;
    }
    
    //Normalize the histogram
    histogram = histogram / (sum(histogram)[0]);
        
    return histogram;
}


int aux::getClosestCodewordIndex(Mat& inputFeatureVector, Mat& codewords){
    
    vector<float> distanceArray;
    
    //Get all distance between inputFeatureVector and all the codewords
    int numberOfCodewords = codewords.rows;
    for(int i=0; i<numberOfCodewords; i++){
        distanceArray.push_back(norm(inputFeatureVector, codewords.row(i)));
    }
    //Find the minimum distance and return the index
    int minIndex = min_element(distanceArray.begin(), distanceArray.end()) - distanceArray.begin();
    return minIndex;
}

double aux::getErrorRate(Mat trueLabels, Mat predictedLabels){
    
    assert(("True Labels and Predicted Labels must be the same size", (trueLabels.rows == predictedLabels.rows) &&
                                                                      (trueLabels.cols == 1) &&
                                                                      (predictedLabels.cols == 1)));
    int numberOfSamples = trueLabels.rows;
    double numberOfErrors = 0;
    
    for(int i=0; i<numberOfSamples; i++){
        if(trueLabels.at<float_t>(0,i) != predictedLabels.at<float_t>(0,i)){
            numberOfErrors = numberOfErrors + 1;
        }
    }
    
    return numberOfErrors/numberOfSamples;
}

Mat aux::getConfusionMatrix(Mat trueLabels, Mat predictedLabels, int numberOfClasses){
    
    Mat confusionMatrix = Mat::zeros(numberOfClasses, numberOfClasses, CV_8UC1);
    int nPerClass = predictedLabels.rows / numberOfClasses;

    Mat predictedLabelsCopy = predictedLabels.clone();
    predictedLabelsCopy.convertTo(predictedLabelsCopy, CV_8UC1);
        
    for(int i=0; i<predictedLabels.rows; i++){
        confusionMatrix.at<uchar>(i/nPerClass, predictedLabelsCopy.at<uchar>(i,0)) += 1;
    }  
    
    
    return confusionMatrix;
}



String aux::getClassName(int index){
    /*
     label 0 --> car_side
     label 1 --> butterfly
     label 2 --> faces
     label 3 --> watch
     label 4 --> water_lilly
     */
    if(index == 0){return "car_side";}
    if(index == 1){return "butterfly";}
    if(index == 2){return "faces";}
    if(index == 3){return "watch";}
    if(index == 4){return "water_lilly";}
    
    return "";
}
