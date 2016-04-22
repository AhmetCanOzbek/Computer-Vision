%%
%TRAIN
%Read train_occupied images
files = dir('/Users/ahmetcanozbek/Desktop/Parking/MatlabImplementation/sub_dataset/train/occupied');
n_files = size(files,1) - 3; %(-3) to avoid ., .., '.DS_Store'
train_im_occupied = {};
for i=4:size(files,1)
    filename = files(i).name; 
    train_im_occupied{i-3,1} = imread(filename);    
end

%Read train_empty images
files = dir('/Users/ahmetcanozbek/Desktop/Parking/MatlabImplementation/sub_dataset/train/empty');
n_files = size(files,1) - 3; %(-3) to avoid ., .., '.DS_Store'
train_im_empty = {};
for i=4:size(files,1)
    filename = files(i).name; 
    train_im_empty{i-3,1} = imread(filename);    
end


%%
%TEST
%Read test_occupied images
files = dir('/Users/ahmetcanozbek/Desktop/Parking/MatlabImplementation/sub_dataset/test/occupied');
n_files = size(files,1) - 3; %(-3) to avoid ., .., '.DS_Store'
test_im_occupied = {};
for i=4:size(files,1)
    filename = files(i).name; 
    test_im_occupied{i-3,1} = imread(filename);    
end

%Read test_empty images
files = dir('/Users/ahmetcanozbek/Desktop/Parking/MatlabImplementation/sub_dataset/test/empty');
n_files = size(files,1) - 3; %(-3) to avoid ., .., '.DS_Store'
test_im_empty = {};
for i=4:size(files,1)
    filename = files(i).name; 
    test_im_empty{i-3,1} = imread(filename);    
end


%%
%Clean Up
clear n_files;
clear files;
clear i;
clear filename;