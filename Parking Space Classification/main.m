% %% Initialize
% clc; clear;
% %Read the dataset
% read_images;
% disp('Reading Images Done;');
% %Get the model
% train_test;
% disp('train test matrix done.');

%% 
%Read the parking space image 
im = imread('p2.jpg');
im_xml = xml2struct('p2.xml');

%Get the number of parking spaces from xml file
%For each parking space, get cx,cy,angle
%Loop through parking spaces and make a prediction from the model
%From that prediction, draw green or red rectangle
n_parkingspaces = size(im_xml.parking.space,2);
parkingspace_array = {};
figure(); imshow(im);
for i=1:n_parkingspaces    
    %Get cx,cy
    for j=1:4
        cx(j) = str2num(im_xml.parking.space{1,i}.contour.point{1,j}.Attributes.x);
        cy(j) = str2num(im_xml.parking.space{1,i}.contour.point{1,j}.Attributes.y);
    end
    %Get angle
    angle = str2num(im_xml.parking.space{1,i}.rotatedRect.angle.Attributes.d);
    %Crop space
    cropped_space = crop_parkingspace(im,cx,cy,angle);
    
    %Extract features of the cropped space (get words histogram)
    feature_vector = get_im_words(cropped_space,cluster_means);
    
    %Predict using model (empty(0) or occupied(1))
    dummy_label = 0;
    prediction = 0;
    [predicted_label, ~, ~] = svmpredict(dummy_label, feature_vector, model, '-q');
    
    %draw the polygon to the parking space
    %empty = green, occupied = red
    if(predicted_label == 0), color = 'g';, else, color = 'r';, end
    
    draw_polygon(cx,cy,color);  
    
    
end

