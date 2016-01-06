cat = imread('baby_cat.tif'); %1
bear = imread('baby_bear.tif'); %2
dog = imread('baby_dog.tif'); %3
panda = imread('baby_panda.tif');
baby = imread('baby.tif');

%K
f = sqrt(3);
w = 200; cx = w/2;
h = 200; cy = h/2;
pixelSpacing = 0.0033;
f = f * (1/pixelSpacing);
K = [f 0 cx; 0 f cy; 0 0 1];

%Rt
Xc = [-1/sqrt(2); 1/sqrt(2); 0];
Yc = [1/sqrt(6); 1/sqrt(6); -2/sqrt(6)];
Zc = [-1/sqrt(3); -1/sqrt(3); -1/sqrt(3)];
r = [5; 5; 5];
Rt = [Xc' -dot(r,Xc); Yc' -dot(r,Yc); Zc' -dot(r,Zc);]; 

%Forward Mapping (World Coordinates --> Image Coordinates)
outForwardIm = zeros(w,h,3);
surfaceImages = {panda, bear, dog};
for surface = 1:3
    for rgb=1:3
        for i=1:size(cat,1)
            for j=1:size(cat,2)
                %get the coordinate values of the point in real world(on the
                %cube)
                ti = ((i/200) * 2) -1;
                tj = ((j/200) * 2) -1;
                %construct the point
                if(surface == 1), wP = [ti tj 1];  end
                if(surface == 2), wP = [1 ti tj];  end
                if(surface == 3), wP = [ti 1 tj];  end
                wP = [wP 1]; wP = wP';
                %Project onto image coordinates
                %Get image coordinates
                iP = (K*Rt) * wP;
                iP = iP / iP(3);        
                %Assign the values to the output image (camera view image)
                outForwardIm(round(iP(2)),round(iP(1)),rgb) = surfaceImages{surface}(i,j,rgb);         
            end
        end
    end
end
figure();imshow(uint8(outForwardIm));
title(['Pixel Spacing = ' num2str(pixelSpacing)]);