addpath(genpath('/Users/ahmetcanozbek/Desktop/Parking/MatlabImplementation/vlfeat-0.9.20'));

%% Parameters
%Number of Words (number of clusters) to represent an image
k = 800; 
n_train_im_empty = size(train_im_empty,1);
n_train_im_occupied = size(train_im_occupied,1);
n_test_im_empty = size(test_im_empty,1);
n_test_im_occupied = size(test_im_occupied,1);

n_train_im = n_train_im_empty + n_train_im_occupied;
n_test_im = n_test_im_empty + n_test_im_occupied;

%% Extracting SIFT features of training images
%Extract SIFT (train_im_empty)
for i=1:size(train_im_empty)
   I = single(rgb2gray(train_im_empty{i,1})); 
   [f,d] = vl_sift(I);
   d = d';
   train_im_empty{i,2} = d;    
end

%Extract SIFT (train_im_occupied)
for i=1:size(train_im_occupied)
   I = single(rgb2gray(train_im_occupied{i,1})); 
   [f,d] = vl_sift(I);
   d = d';
   train_im_occupied{i,2} = d;    
end

%% Do k-means clustuering (get cluster means)
%Same k-means function, remove path
rmpath(genpath('/Users/ahmetcanozbek/Desktop/Parking/MatlabImplementation/vlfeat-0.9.20'));

%Create train_feature matrix
train_sift_features = cell2mat([train_im_empty(:,2) ; train_im_occupied(:,2)]);

%Cluster centers (words)
[idx,cluster_means] = kmeans(double(train_sift_features),k);


addpath(genpath('/Users/ahmetcanozbek/Desktop/Parking/MatlabImplementation/vlfeat-0.9.20'));
%% Create train,test , train_label,test_label

%TRAIN
%train
train_im = [train_im_empty ; train_im_occupied];
train = [];
n_of_train_images = size(train_im,1);
for i=1:n_of_train_images
    %Get histogram
    train(i,:) = get_im_words(train_im{i,1} , cluster_means );   
end
%train_label
train_label = [zeros(n_train_im_empty,1);ones(n_train_im_occupied,1)];

%TEST
%test
test_im = [test_im_empty ; test_im_occupied];
test = [];
n_of_test_images = size(test_im,1);
for i=1:n_of_test_images 
    %Get histogram
    test(i,:) = get_im_words(test_im{i,1} , cluster_means);    
end
%test_label
test_label = [zeros(n_test_im_empty,1) ; ones(n_test_im_occupied,1)];


%% Parameter selection for SVM with grid search (5-fold cross-validation)
rmpath(genpath('/Users/ahmetcanozbek/Desktop/Parking/MatlabImplementation/vlfeat-0.9.20'));

% bestcv = 0;
% C = [10,50,100,200,500,750,1000];
% gamma = [0.00025,0.0003,0.001,0.005,0.01,0.05,0.1,1,10];
% for i = 1:size(C,2)
%   for j = 1:size(gamma,2)
%     cmd = ['-t 2 -v 5' ' -c ' num2str(C(i)) ' -g ', num2str(gamma(j)) ' -q'];
%     cv = svmtrain(train_label, train, cmd);
%     disp(['Result: gamma = ' num2str(gamma(j)) ' , C = ' num2str(C(i)) ' , Accuracy = ' num2str(cv)]);    
%   end
% end

%SVM Parameters: C = 55 , gamma = 0.002
%Train the SVM model with parameters
C = 500;
gamma = 0.00025;
disp(['SVM Parameters: ' 'C = ' num2str(C) ' , gamma = ' num2str(gamma)]);
model = svmtrain(train_label,train,['-t 2 -c ' num2str(C) ' -g ' num2str(gamma) ' -b 1 -q']);
disp('Training Data');
[predicted_train_label, accuracy_train, ~] = svmpredict(train_label, train, model);
disp('Testing Data');
[predicted_test_label, accuracy_test, ~] = svmpredict(test_label, test, model);


addpath(genpath('/Users/ahmetcanozbek/Desktop/Parking/MatlabImplementation/vlfeat-0.9.20'));

%%
%Add path again
addpath(genpath('/Users/ahmetcanozbek/Desktop/Parking/MatlabImplementation/vlfeat-0.9.20'));

%% Clean up
clear I;
clear d;
clear f;
clear i;


% rmpath(genpath('/Users/ahmetcanozbek/Desktop/Parking/MatlabImplementation/vlfeat-0.9.20'));
% C = 1000;
% gamma = 0.02;
% disp(['SVM Parameters: ' 'C = ' num2str(C) ' , gamma = ' num2str(gamma)]);
% model = svmtrain(train_label,train,['-t 2 -c ' num2str(C) ' -g ' num2str(gamma) ' -b 1 -q']);
% disp('Training Data');
% [predicted_train_label, accuracy_train, prob_estimates_train] = svmpredict(train_label, train, model, '-b 1');
% addpath(genpath('/Users/ahmetcanozbek/Desktop/Parking/MatlabImplementation/vlfeat-0.9.20'));
