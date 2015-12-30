//CSCI574 HW5
//Ahmet Can Ozbek
//8401860152
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/ml/ml.hpp>
#include <vector>
#include "auxFunctions.h"

using namespace std;
using namespace cv;

#define N_OF_CLASSES 5
#define N_OF_TRAINING_IMAGES 100
#define N_OF_TEST_IMAGES 50


int main(int argc, const char * argv[]) {
   
    /*
    label 0 --> car_side
    label 1 --> butterfly
    label 2 --> faces
    label 3 --> watch
    label 4 --> water_lilly     
    */
    
    //1)Read the images
    //Reading Training Images
    Mat trainImages[N_OF_TRAINING_IMAGES];
    aux::readTrainImages(trainImages, N_OF_TRAINING_IMAGES);
    //Reading Test Images
    Mat testImages[N_OF_TEST_IMAGES];
    aux::readTestImages(testImages, N_OF_TEST_IMAGES);
    
    //Construct true labels for training and testing images
    Mat trueTrainLabels;
    Mat trueTestLabels;
    aux::getTrueTrainingLabels(trueTrainLabels, N_OF_TRAINING_IMAGES);
    aux::getTrueTestLabels(trueTestLabels, N_OF_TEST_IMAGES);
    
    int pcaDimension = 25;
    int numberOfClusters = 150;
    int kNeighbors = 20;
    
    int pcaDimArray[3] = {15,20,25};
    int kArray[3] = {50,100,150};
    int kNNArray[3] = {3,10,20};
    

    //2)
    PCA pcaSift;
    SIFT siftExtractor;
    Mat descriptorsOfTrainImages[N_OF_TRAINING_IMAGES]; //SIFT descriptor matrix for each training image
    
    
    //Get codewords
    Mat codewords = aux::getWords(trainImages, descriptorsOfTrainImages,
                                  pcaSift, siftExtractor, pcaDimension, numberOfClusters); //Returns the cluster means (codewords)
    
    
    //Get the histograms of training images
    Mat trainingHistogramData;
    for(int i=0; i<N_OF_TRAINING_IMAGES; i++){
        trainingHistogramData.push_back(aux::getHistogram(trainImages[i], codewords, siftExtractor, pcaSift));
    }
    
    //Get the histograms of testing images
    Mat testingHistogramData;
    for(int i=0; i<N_OF_TEST_IMAGES; i++){
        testingHistogramData.push_back(aux::getHistogram(testImages[i], codewords, siftExtractor, pcaSift));
    }
    trainingHistogramData.convertTo(trainingHistogramData, CV_32F);
    testingHistogramData.convertTo(testingHistogramData, CV_32F);
    
   
    //Classification
    //KNN    
    cout << "KNN Classification with parameters: " << endl;
    cout << "*PCA dimension: " << pcaDimension << endl;
    cout << "*k(Number Of Clusters in k-means): " << numberOfClusters << endl;
    cout << "*k nearest neighbors in kNN classification: " << kNeighbors << endl;
    
    //Train kNN classifier
    CvKNearest knnClassifier;
    knnClassifier.train(trainingHistogramData, trueTrainLabels);    
    
    //Results
    cout << "Results: " << endl;
    //Error on training data
    Mat predictedTrainingLabelsKNN;
    knnClassifier.find_nearest(trainingHistogramData, kNeighbors, &predictedTrainingLabelsKNN);
    predictedTrainingLabelsKNN.convertTo(predictedTrainingLabelsKNN, CV_32S);
    cout << "-Training Error Rate: " << aux::getErrorRate(trueTrainLabels, predictedTrainingLabelsKNN) << endl;
    
    //Error on testing data
    Mat predictedTestingLabelsKNN;
    knnClassifier.find_nearest(testingHistogramData, kNeighbors, &predictedTestingLabelsKNN);
    predictedTestingLabelsKNN.convertTo(predictedTestingLabelsKNN, CV_32S);
    cout << "-Testing Error Rate: " << aux::getErrorRate(trueTestLabels, predictedTestingLabelsKNN) << endl;
    
    //Training Confusion Matrix
    Mat confusionMatrixTraining;
    confusionMatrixTraining = aux::getConfusionMatrix(trueTrainLabels, predictedTrainingLabelsKNN, N_OF_CLASSES);
    cout << "-Training Confusion Matrix: \n" << confusionMatrixTraining << endl;

    //Test Confusion Matrix
    Mat confusionMatrixTest;
    confusionMatrixTest = aux::getConfusionMatrix(trueTestLabels, predictedTestingLabelsKNN, N_OF_CLASSES);
    cout << "-Testing Confusion Matrix: \n" << confusionMatrixTest << endl;
    
    cout << "End." << endl;
    return 0;
}
