function[cropped] = crop_parkingspace(im,cx,cy,angle)

    %For each corner point
    cx_r = [];
    cy_r = [];
    for i=1:4
        %Initialize an emptry matrix the same size as input im
        zeros_im = zeros(size(im,1),size(im,2));
        %Insert the markers (4 corner points)
        zeros_im(cy(i),cx(i)) = 1;
        %Rotate the empty matrix
        zeros_im_r = imrotate(zeros_im,angle,'bilinear');        
        %index = round(mean(find(zeros_im_r)));
        %Find the points in the rotated im
        [temp_cy_r,temp_cx_r] = find(zeros_im_r); 
        cy_r(i) = round(mean(temp_cy_r));
        cx_r(i) = round(mean(temp_cx_r));
    end   
    
    %Rotate the original image
    im_r = imrotate(im,angle,'bilinear');
    %Crop
    x_left = min(cx_r);
    x_right = max(cx_r);
    y_up = min(cy_r);
    y_down = max(cy_r);
    
    cropped = im_r(y_up:y_down,x_left:x_right,:); 
    
      

end