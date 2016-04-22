In this project, we are trying to predict whether the parking space either empty or occupied.

The procedure is as follows:
a) Extracting SIFT features of images in the training data  
b) Perform k-means clustering (cluster means will be the “words”)  
c) Represent images with the histogram of these words (histogram becomes
the feature vector)  
d) Train a classifier by using those feature vectors (SVM)  


Green: Empty, Red: Occupied
![alt tag](https://github.com/a-ozbek/Computer-Vision/blob/master/Parking%20Space%20Classification/result.png)