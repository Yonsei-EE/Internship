%% initialize
cam = webcam(1);

%% get points
while(1)
    image = snapshot(cam);
    BW = rgb2gray(image);
    imshow(BW);

    [imagePoints,boardSize,imagesUsed] = detectCheckerboardPoints(BW);
    hold on;
    if(~isempty(imagePoints))
        plot(imagePoints(:,1),imagePoints(:,2)  ,'ro');
    end
    
    if(abs(imagePoints(1,2)-imagePoints(9,2))<1)
        disp('Good');
        if(abs(imagePoints(73,2)-imagePoints(81,2))<1)
            disp('Very Good');
        end
    end
end 

%% cal d
d = p1(:,2) - p2(:,2);
d = fliplr((reshape(d,9,9))');