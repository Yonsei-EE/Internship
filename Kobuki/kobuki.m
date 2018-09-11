function kobuki()
    %% Robot setup
    robot = serial( 'COM4', 'Baudrate',115200);
    fopen(robot);
    pause(2);

    cclockwise = [1 0];
    clockwise = [255 255];
    straight = [0 128];
    r = 50;
    initialized = false;
    findHome = false;
    goHome = false;
    avoid = false;
    dx = 0;
    dy = 0;
    s = false;
    
    %% Enviornment setup
    or = [40 30 20 40 30 20];
    ox = [20 300 620 150 500 640];
    oy = [30 80 100 150 300 400];

    timeStep = 0;
    %% Vision Initialization
    redThresh = 0.2; % Threshold for red detection(0.24)
    greenThresh = 0.2; % Threshold for green detection
    blueThresh = 0.2; % Threshold for blue detection

    cam = webcam(1);
    cam.Resolution = '640x480';
    hblob = vision.BlobAnalysis('AreaOutputPort', false, ... % Set blob analysis handling
            'CentroidOutputPort', true, ... 
            'BoundingBoxOutputPort', true', ...
            'MinimumBlobArea', 100, ...
            'MaximumBlobArea', 3000, ...
            'MaximumCount', 10);
    hshapeinsBox = vision.ShapeInserter('BorderColorSource', 'Input port', ... % Set box handling
                    'Fill', true, ...
                    'FillColorSource', 'Input port', ...
                    'Opacity', 0.4);
    hVideoIn = vision.VideoPlayer('Name', 'Final Video', ... % Output video player
            'Position', [100 100 1280+20 720+30]);

    cleanupObj = onCleanup(@() cleanup(cam, robot));
    v = VideoWriter('test3.avi');
    open(v);

    %% Vision
    tic;
    while(1)
        tmprgbFrame = snapshot(cam); % Acquire single frame
        rgbFrame = im2single(tmprgbFrame);
        original = rgbFrame;

        diffFrameRed = imsubtract(rgbFrame(:,:,1), rgb2gray(rgbFrame)); % Get red component of the image
        diffFrameRed = medfilt2(diffFrameRed, [3 3]); % Filter out the noise by using median filter
        binFrameRed = imbinarize(diffFrameRed, redThresh); % Convert the image into binary image with the red objects as white

        diffFrameGreen = imsubtract(rgbFrame(:,:,2), rgb2gray(rgbFrame)); % Get green component of the image
        diffFrameGreen = medfilt2(diffFrameGreen, [3 3]); % Filter out the noise by using median filter
        binFrameGreen = imbinarize(diffFrameGreen, greenThresh); % Convert the image into binary image with the green objects as white

        diffFrameBlue = imsubtract(rgbFrame(:,:,3), rgb2gray(rgbFrame)); % Get blue component of the image
        diffFrameBlue = medfilt2(diffFrameBlue, [3 3]); % Filter out the noise by using median filter
        binFrameBlue = imbinarize(diffFrameBlue, blueThresh); % Convert the image into binary image with the blue objects as white

        [centroidRed, bboxRed] = step(hblob, binFrameRed); % Get the centroids and bounding boxes of the red blobs
        [centroidGreen, bboxGreen] = step(hblob, binFrameGreen); % Get the centroids and bounding boxes of the green blobs
        [centroidBlue, bboxBlue] = step(hblob, binFrameBlue); % Get the centroids and bounding boxes of the blue blobs

        vidIn = step(hshapeinsBox, original, bboxRed, single([1 0 0])); % Insert the red box
        vidIn = step(hshapeinsBox, vidIn, bboxGreen, single([0 1 0])); % Insert the green box
        vidIn = step(hshapeinsBox, vidIn, bboxBlue, single([0 0 1])); % Insert the blue box

        for object = 1:1:length(bboxRed(:,1)) % Write the corresponding centroids for red
            centXRed = centroidRed(object,1); centYRed = centroidRed(object,2);
            redpointText = ['+  X: ' num2str(uint16(centXRed)) ', Y: ' num2str(uint16(centYRed))];     
            vidIn = insertText(vidIn,[centXRed-11 centYRed-17],redpointText, 'Font', 'Courier New','FontSize', 14,'TextColor', [1 1 0], 'BoxOpacity', 0);
        end
        for object = 1:1:length(bboxGreen(:,1)) % Write the corresponding centroids for green
            centXGreen = centroidGreen(object,1); centYGreen = centroidGreen(object,2);
            greenpointText = ['+  X: ' num2str(uint16(centXGreen)) ', Y: ' num2str(uint16(centYGreen))];
            vidIn = insertText(vidIn,[centXGreen-11 centYGreen-17], greenpointText, 'Font', 'Courier New','FontSize', 14,'TextColor', [1 1 0],'BoxOpacity', 0);
        end
        for object = 1:1:length(bboxBlue(:,1)) % Write the corresponding centroids for blue
            centXBlue = centroidBlue(object,1); centYBlue = centroidBlue(object,2);
            bluepointText = ['+  X: ' num2str(uint16(centXBlue)) ', Y: ' num2str(uint16(centYBlue))];
            vidIn = insertText(vidIn,[centXBlue-11 centYBlue-17], bluepointText, 'Font', 'Courier New','FontSize', 14,'TextColor', [1 1 0],'BoxOpacity', 0);
        end

        redcountText = ['Red  : ' num2str(length(bboxRed(:,1)))]; % Count the number of red blobs
        greencountText = ['Green  : ' num2str(length(bboxGreen(:,1)))]; % Count the number of green blobs
        bluecountText = ['Blue  : ' num2str(length(bboxBlue(:,1)))]; % Count the number of blue blobs
        vidIn = insertText(vidIn,[0 -3], redcountText, 'Font', 'Courier New','FontSize', 14,'TextColor', [1 0 0], 'BoxOpacity', 0);
        vidIn = insertText(vidIn,[0 11], greencountText, 'Font', 'Courier New','FontSize', 14,'TextColor', [0 1 0], 'BoxOpacity', 0); 
        vidIn = insertText(vidIn,[0 25], bluecountText, 'Font', 'Courier New','FontSize', 14,'TextColor', [0 0 1], 'BoxOpacity', 0); 

        %% Control
        if(isequal(size(centroidBlue),size(centroidRed)) && ~isempty(centroidBlue) && ~isempty(centroidRed))
            rvec = centroidBlue - centroidRed;
            centXRobot = (centXBlue+centXRed)/2;
            centYRobot = (centYBlue+centYRed)/2;
            theta = atan2(rvec(2),rvec(1));
            % initialize position, set home
            if(~initialized)
                initialp = [centXRobot, centYRobot];
                initialized = true;
            end
            % ir sensor
            ir = irsensor(centXRobot, centYRobot, r, theta, ox, oy, or);
            if(ir(1)<75||ir(2)<70||ir(8)<70||ir(7)<63||ir(3)<63)
                avoid = true;
            else
                avoid = false;
            end
            
            % time
            if(timeStep > 15)
                findHome = true;
            end
            
            if(~findHome)
                if(toc>0 && toc<0.2)
                    if(~s)
                        V = 200;
                        R = [200, -200];
                        i = randi([1,2],1,1); %right
                        R = R(i);
                        if(~avoid)
                            [velocity, radius] = getvr(V,R);     
                            control(robot,velocity,radius)
                            w = V/R;
                            dx = dx + 2*R*sin(-w/2)*cos(theta+w/2); % path integration for x
                            dy = dy + 2*R*sin(-w/2)*sin(theta+w/2); % path integration for y
                        else
                            [velocity, radius] = getvr(200,R);
                            avoidDir = randi([1,2],1,1);
                            if(avoidDir == 1)
                                control(robot,velocity,clockwise);
                            else
                                control(robot,velocity,cclockwise);
                            end
                        end
                        s = true;
                        disp(['move right ' num2str(dx) ' ' num2str(dy) ' ' num2str(theta)]);
                    end
                else
                    s = false;
                end
            else                
%                 homedir = atan2(initialp(2)-centYRobot,initialp(1)-centXRobot);
                homedir = atan2(dy,dx);
                if(abs(angdiff(theta,homedir))<0.6 || goHome)
                    if(~goHome)
                        control(robot,[0 0],[0 0]);
                        pause(1);
                    end
                    goHome = true;
                    if(sqrt((initialp(1)-centXRobot)^2+(initialp(2)-centYRobot)^2)>95)
 c
                    else
                        break;
                    end
                elseif(~goHome)
                    [velocity, radius] = getvr(200,R); 
                    control(robot,velocity,clockwise);
                end
            end
        end

        %% Write Video
        if(exist('centXRobot','var')&&exist('centYRobot','var'))
            vidIn = insertShape(vidIn,'circle',[initialp(1) initialp(2) r],'LineWidth',5,'Color','Green');
            vidIn = insertShape(vidIn,'circle',[ox(1),oy(1),or(1)],'LineWidth',5,'Color','Red');
            vidIn = insertShape(vidIn,'circle',[ox(2),oy(2),or(2)],'LineWidth',5,'Color','Red');
            vidIn = insertShape(vidIn,'circle',[ox(3),oy(3),or(3)],'LineWidth',5,'Color','Red');
            vidIn = insertShape(vidIn,'circle',[ox(4),oy(4),or(4)],'LineWidth',5,'Color','Red');
            vidIn = insertShape(vidIn,'circle',[ox(5),oy(5),or(5)],'LineWidth',5,'Color','Red');
            vidIn = insertShape(vidIn,'circle',[ox(6),oy(6),or(6)],'LineWidth',5,'Color','Red');

            vidIn = insertShape(vidIn,'Line',[centXRobot centYRobot centXRobot+r*cos(theta) centYRobot+r*sin(theta)],'LineWidth',5,'Color', 'Blue');
            vidIn = insertShape(vidIn,'circle',[centXRobot centYRobot r],'LineWidth',5,'Color','Black');
        end

        step(hVideoIn, vidIn); % Output video stream
        writeVideo(v,vidIn);
        if(toc>1 && ~goHome)
            tic;
            timeStep = timeStep + 1;
        end
    end
    control(robot,[0 0],[0 0]);
    pause(2);
    fclose(robot);
    pause(2);
    delete(robot);
end
%% Cleanup Function
function cleanup(cam, robot)
    disp('cleanup');
    control(robot,[0 0],[0 0]);
    pause(2);
    fclose(robot);
    pause(2);
    delete(robot);
    clear all;
end