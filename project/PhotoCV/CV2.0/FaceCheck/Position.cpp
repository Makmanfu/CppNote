
#include "stdafx.h"
#include "Position.h"
#include "imageUtils.h"

//DetectObject *************************************************************************

void DetectObject::detectLargestObject(const Mat& img, CascadeClassifier& cascade, Rect& largestObject, int scaledWidth /*= 320*/)
{
    // Only search for just 1 object (the biggest in the image).
    int flags = CASCADE_FIND_BIGGEST_OBJECT;// | CASCADE_DO_ROUGH_SEARCH;
    // Smallest object size.
    Size minFeatureSize = Size(20, 20);
    // How detailed should the search be. Must be larger than 1.0.
    float searchScaleFactor = 1.1f;
    // How much the detections should be filtered out. This should depend on how bad false detections are to your system.
    // minNeighbors=2 means lots of good+bad detections, and minNeighbors=6 means only good detections are given but some are missed.
    int minNeighbors = 4;
    // Perform Object or Face Detection, looking for just 1 object (the biggest in the image).
    vector<Rect> objects;
    detectObjectsCustom(img, cascade, objects, scaledWidth, flags, minFeatureSize, searchScaleFactor, minNeighbors);
    if (objects.size() > 0)
        largestObject = (Rect) objects.at(0);       // Return the only detected object.
    else
        largestObject = Rect(-1, -1, -1, -1);       // Return an invalid rect.
}

void DetectObject::detectManyObjects(const Mat& img, CascadeClassifier& cascade, vector<Rect>& objects, int scaledWidth /*= 320*/)
{
    // Search for many objects in the one image.
    int flags = CASCADE_SCALE_IMAGE;
    // Smallest object size.p
    Size minFeatureSize = Size(20, 20);
    // How detailed should the search be. Must be larger than 1.0.
    float searchScaleFactor = 1.1f;
    // How much the detections should be filtered out. This should depend on how bad false detections are to your system.
    // minNeighbors=2 means lots of good+bad detections, and minNeighbors=6 means only good detections are given but some are missed.
    int minNeighbors = 4;
    // Perform Object or Face Detection, looking for many objects in the one image.
    detectObjectsCustom(img, cascade, objects, scaledWidth, flags, minFeatureSize, searchScaleFactor, minNeighbors);
}

void DetectObject::detectObjectsCustom(const Mat& img, CascadeClassifier& cascade, vector<Rect>& objects, int scaledWidth, int flags, Size minFeatureSize, float searchScaleFactor, int minNeighbors)
{
    // If the input image is not grayscale, then convert the BGR or BGRA color image to grayscale.
    Mat gray;
    if (img.channels() == 3)
        cvtColor(img, gray, CV_BGR2GRAY);
    else if (img.channels() == 4)
        cvtColor(img, gray, CV_BGRA2GRAY);
    else
        gray = img;     // Access the input image directly, since it is already grayscale.
    // Possibly shrink the image, to run much faster.
    Mat inputImg;
    float scale = img.cols / (float) scaledWidth;
    if (img.cols > scaledWidth)
    {
        // Shrink the image while keeping the same aspect ratio.
        int scaledHeight = cvRound(img.rows / scale);
        resize(gray, inputImg, Size(scaledWidth, scaledHeight));
    }
    else
    {
        // Access the input image directly, since it is already small.
        inputImg = gray;
    }
    // Standardize the brightness and contrast to improve dark images.
    Mat equalizedImg;
    equalizeHist(inputImg, equalizedImg);
    // Detect objects in the small grayscale image.
    cascade.detectMultiScale(equalizedImg, objects, searchScaleFactor, minNeighbors, flags, minFeatureSize);
    // Enlarge the results if the image was temporarily shrunk before detection.
    if (img.cols > scaledWidth)
    {
        for (int i = 0; i < (int) objects.size(); i++)
        {
            objects[i].x = cvRound(objects[i].x * scale);
            objects[i].y = cvRound(objects[i].y * scale);
            objects[i].width = cvRound(objects[i].width * scale);
            objects[i].height = cvRound(objects[i].height * scale);
        }
    }
    // Make sure the object is completely within the image, in case it was on a border.
    for (int i = 0; i < (int) objects.size(); i++)
    {
        if (objects[i].x < 0)           objects[i].x = 0;
        if (objects[i].y < 0)           objects[i].y = 0;
        if (objects[i].x + objects[i].width > img.cols)
            objects[i].x = img.cols - objects[i].width;
        if (objects[i].y + objects[i].height > img.rows)
            objects[i].y = img.rows - objects[i].height;
    }
}

//Recognition ***************************************************************************

Ptr<FaceRecognizer> Recognition::learnCollectedFaces(const vector<Mat> preprocessedFaces, const vector<int> faceLabels, const string facerecAlgorithm /*= "FaceRecognizer.Eigenfaces"*/)
{
    Ptr<FaceRecognizer> model;
    cout << "Learning the collected faces using the [" << facerecAlgorithm << "] algorithm ..." << endl;
    // Make sure the "contrib" module is dynamically loaded at runtime.
    // Requires OpenCV v2.4.1 or later (from June 2012), otherwise the FaceRecognizer will not compile or run!
    bool haveContribModule = initModule_contrib();
    if (!haveContribModule)
        cerr << "ERROR: The 'contrib' module is needed for FaceRecognizer but has not been loaded into OpenCV!" << endl;
    // Use the new FaceRecognizer class in OpenCV's "contrib" module:
    // Requires OpenCV v2.4.1 or later (from June 2012), otherwise the FaceRecognizer will not compile or run!
    model = Algorithm::create<FaceRecognizer> (facerecAlgorithm);
    if (model.empty())
        cerr << "ERROR: The FaceRecognizer algorithm [" << facerecAlgorithm << "] is not available in your version of OpenCV. Please update to OpenCV v2.4.1 or newer." << endl;
    // Do the actual training from the collected faces. Might take several seconds or minutes depending on input
    try
    {
        model->train(preprocessedFaces, faceLabels);
    }
    catch (cv::Exception& e)
    {
        cerr << "Error Training \"" << "    " << "\". Reason: " << e.msg << endl;
    }
    return model;
}

void Recognition::showTrainingDebugData(const Ptr<FaceRecognizer> model, const int faceWidth, const int faceHeight)
{
    try
    {
        // Surround the OpenCV calls by a try/catch block so we don't crash if some model parameters aren't available.
        // Show the average face (statistical average for each pixel in the collected images).
        Mat averageFaceRow = model->get<Mat> ("mean");
        printMatInfo(averageFaceRow, "averageFaceRow");
        // Convert the matrix row (1D float matrix) to a regular 8-bit image.
        Mat averageFace = getImageFrom1DFloatMat(averageFaceRow, faceHeight);
        printMatInfo(averageFace, "averageFace");
        imshow("averageFace", averageFace);
        // Get the eigenvectors
        Mat eigenvectors = model->get<Mat> ("eigenvectors");
        printMatInfo(eigenvectors, "eigenvectors");
        // Show the best 20 eigenfaces
        for (int i = 0; i < min(20, eigenvectors.cols); i++)
        {
            // Create a column vector from eigenvector #i.
            // Note that clone() ensures it will be continuous, so we can treat it like an array, otherwise we can't reshape it to a rectangle.
            // Note that the FaceRecognizer class already gives us L2 normalized eigenvectors, so we don't have to normalize them ourselves.
            Mat eigenvectorColumn = eigenvectors.col(i).clone();
            //printMatInfo(eigenvectorColumn, "eigenvector");
            Mat eigenface = getImageFrom1DFloatMat(eigenvectorColumn, faceHeight);
            //printMatInfo(eigenface, "eigenface");
            imshow(format("Eigenface%d", i), eigenface);
        }
        // Get the eigenvalues
        Mat eigenvalues = model->get<Mat> ("eigenvalues");
        printMat(eigenvalues, "eigenvalues");
        //int ncomponents = model->get<int>("ncomponents");
        //cout << "ncomponents = " << ncomponents << endl;
        vector<Mat> projections = model->get<vector<Mat> > ("projections");
        cout << "projections: " << projections.size() << endl;
        for (int i = 0; i < (int) projections.size(); i++)
            printMat(projections[i], "projections");
        //labels = model->get<Mat>("labels");
        //printMat(labels, "labels");
    }
    catch (cv::Exception e)
    {
        cout << "WARNING: Missing FaceRecognizer properties." << endl;
    }
}

cv::Mat Recognition::reconstructFace(const Ptr<FaceRecognizer> model, const Mat preprocessedFace)
{
    try
    {
        // Get some required data from the FaceRecognizer model.
        Mat eigenvectors = model->get<Mat> ("eigenvectors");
        Mat averageFaceRow = model->get<Mat> ("mean");
        int faceHeight = preprocessedFace.rows;
        // Project the input image onto the PCA subspace.
        Mat projection = subspaceProject(eigenvectors, averageFaceRow, preprocessedFace.reshape(1, 1));
        //printMatInfo(projection, "projection");
        // Generate the reconstructed face back from the PCA subspace.
        Mat reconstructionRow = subspaceReconstruct(eigenvectors, averageFaceRow, projection);
        //printMatInfo(reconstructionRow, "reconstructionRow");
        // Convert the float row matrix to a regular 8-bit image. Note that we
        // shouldn't use "getImageFrom1DFloatMat()" because we don't want to normalize
        // the data since it is already at the perfect scale.
        // Make it a rectangular shaped image instead of a single row.
        Mat reconstructionMat = reconstructionRow.reshape(1, faceHeight);
        // Convert the floating-point pixels to regular 8-bit uchar pixels.
        Mat reconstructedFace = Mat(reconstructionMat.size(), CV_8U);
        reconstructionMat.convertTo(reconstructedFace, CV_8U, 1, 0);
        //printMatInfo(reconstructedFace, "reconstructedFace");
        return reconstructedFace;
    }
    catch (cv::Exception e)
    {
        cout << "WARNING: Missing FaceRecognizer properties." << endl;
        return Mat();
    }
}

double Recognition::getSimilarity(const Mat A, const Mat B)
{
    if (A.rows > 0 && A.rows == B.rows && A.cols > 0 && A.cols == B.cols)
    {
        // Calculate the L2 relative error between the 2 images.
        double errorL2 = norm(A, B, CV_L2);
        // Convert to a reasonable scale, since L2 error is summed across all pixels of the image.
        double similarity = errorL2 / (double)(A.rows * A.cols);
        return similarity;
    }
    else
    {
        //cout << "WARNING: Images have a different size in 'getSimilarity()'." << endl;
        return 100000000.0;  // Return a bad value
    }
}

cv::Mat Recognition::getImageFrom1DFloatMat(const Mat matrixRow, int height)
{
    // Make it a rectangular shaped image instead of a single row.
    Mat rectangularMat = matrixRow.reshape(1, height);
    // Scale the values to be between 0 to 255 and store them as a regular 8-bit uchar image.
    Mat dst;
    normalize(rectangularMat, dst, 0, 255, NORM_MINMAX, CV_8UC1);
    return dst;
}

//PrepocessFace *************************************************************************

PrepocessFace::PrepocessFace()
    : DESIRED_LEFT_EYE_X(0.16)
    , DESIRED_LEFT_EYE_Y(0.14)
    , FACE_ELLIPSE_CY(0.40)
    , FACE_ELLIPSE_W(0.5)
    , FACE_ELLIPSE_H(0.8)
{
    mDetectObject.reset(new DetectObject());
}

PrepocessFace::~PrepocessFace()
{
}

void PrepocessFace::detectBothEyes(const Mat& face, CascadeClassifier& eyeCascade1, CascadeClassifier& eyeCascade2, Point& leftEye, Point& rightEye, Rect* searchedLeftEye /*= NULL*/, Rect* searchedRightEye /*= NULL*/)
{
    // Skip the borders of the face, since it is usually just hair and ears, that we don't care about.
    /*
        // For "2splits.xml": Finds both eyes in roughly 60% of detected faces, also detects closed eyes.
        const float EYE_SX = 0.12f;
        const float EYE_SY = 0.17f;
        const float EYE_SW = 0.37f;
        const float EYE_SH = 0.36f;
    */
    /*
        // For mcs.xml: Finds both eyes in roughly 80% of detected faces, also detects closed eyes.
        const float EYE_SX = 0.10f;
        const float EYE_SY = 0.19f;
        const float EYE_SW = 0.40f;
        const float EYE_SH = 0.36f;
    */
    // For default eye.xml or eyeglasses.xml: Finds both eyes in roughly 40% of detected faces, but does not detect closed eyes.
    const float EYE_SX = 0.16f;
    const float EYE_SY = 0.26f;
    const float EYE_SW = 0.30f;
    const float EYE_SH = 0.28f;
    int leftX = cvRound(face.cols * EYE_SX);
    int topY = cvRound(face.rows * EYE_SY);
    int widthX = cvRound(face.cols * EYE_SW);
    int heightY = cvRound(face.rows * EYE_SH);
    int rightX = cvRound(face.cols * (1.0 - EYE_SX - EYE_SW));      // Start of right-eye corner
    Mat topLeftOfFace = face(Rect(leftX, topY, widthX, heightY));
    Mat topRightOfFace = face(Rect(rightX, topY, widthX, heightY));
    Rect leftEyeRect, rightEyeRect;
    // Return the search windows to the caller, if desired.
    if (searchedLeftEye)
        *searchedLeftEye = Rect(leftX, topY, widthX, heightY);
    if (searchedRightEye)
        *searchedRightEye = Rect(rightX, topY, widthX, heightY);
    // Search the left region, then the right region using the 1st eye detector.
    mDetectObject->detectLargestObject(topLeftOfFace, eyeCascade1, leftEyeRect, topLeftOfFace.cols);
    mDetectObject->detectLargestObject(topRightOfFace, eyeCascade1, rightEyeRect, topRightOfFace.cols);
    // If the eye was not detected, try a different cascade classifier.
    if (leftEyeRect.width <= 0 && !eyeCascade2.empty())
    {
        mDetectObject->detectLargestObject(topLeftOfFace, eyeCascade2, leftEyeRect, topLeftOfFace.cols);
        //if (leftEyeRect.width > 0)
        //    cout << "2nd eye detector LEFT SUCCESS" << endl;
        //else
        //    cout << "2nd eye detector LEFT failed" << endl;
    }
    //else
    //    cout << "1st eye detector LEFT SUCCESS" << endl;
    // If the eye was not detected, try a different cascade classifier.
    if (rightEyeRect.width <= 0 && !eyeCascade2.empty())
    {
        mDetectObject->detectLargestObject(topRightOfFace, eyeCascade2, rightEyeRect, topRightOfFace.cols);
        //if (rightEyeRect.width > 0)
        //    cout << "2nd eye detector RIGHT SUCCESS" << endl;
        //else
        //    cout << "2nd eye detector RIGHT failed" << endl;
    }
    //else
    //    cout << "1st eye detector RIGHT SUCCESS" << endl;
    if (leftEyeRect.width > 0)
    {
        // Check if the eye was detected.
        leftEyeRect.x += leftX;    // Adjust the left-eye rectangle because the face border was removed.
        leftEyeRect.y += topY;
        leftEye = Point(leftEyeRect.x + leftEyeRect.width / 2, leftEyeRect.y + leftEyeRect.height / 2);
    }
    else
    {
        leftEye = Point(-1, -1);    // Return an invalid point
    }
    if (rightEyeRect.width > 0)
    {
        // Check if the eye was detected.
        rightEyeRect.x += rightX; // Adjust the right-eye rectangle, since it starts on the right side of the image.
        rightEyeRect.y += topY;  // Adjust the right-eye rectangle because the face border was removed.
        rightEye = Point(rightEyeRect.x + rightEyeRect.width / 2, rightEyeRect.y + rightEyeRect.height / 2);
    }
    else
    {
        rightEye = Point(-1, -1);    // Return an invalid point
    }
}

void PrepocessFace::equalizeLeftAndRightHalves(Mat& faceImg)
{
    int w = faceImg.cols;
    int h = faceImg.rows;
    // 1) First, equalize the whole face.
    Mat wholeFace;
    equalizeHist(faceImg, wholeFace);
    // 2) Equalize the left half and the right half of the face separately.
    int midX = w / 2;
    Mat leftSide = faceImg(Rect(0, 0, midX, h));
    Mat rightSide = faceImg(Rect(midX, 0, w - midX, h));
    equalizeHist(leftSide, leftSide);
    equalizeHist(rightSide, rightSide);
    // 3) Combine the left half and right half and whole face together, so that it has a smooth transition.
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int v;
            if (x < w / 4)   // Left 25%: just use the left face.
                v = leftSide.at<uchar> (y, x);
            else if (x < w * 2 / 4)   // Mid-left 25%: blend the left face & whole face.
            {
                int lv = leftSide.at<uchar> (y, x);
                int wv = wholeFace.at<uchar> (y, x);
                // Blend more of the whole face as it moves further right along the face.
                float f = (x - w * 1 / 4) / (float)(w * 0.25f);
                v = cvRound((1.0f - f) * lv + (f) * wv);
            }
            else if (x < w * 3 / 4)   // Mid-right 25%: blend the right face & whole face.
            {
                int rv = rightSide.at<uchar> (y, x - midX);
                int wv = wholeFace.at<uchar> (y, x);
                // Blend more of the right-side face as it moves further right along the face.
                float f = (x - w * 2 / 4) / (float)(w * 0.25f);
                v = cvRound((1.0f - f) * wv + (f) * rv);
            }
            else
            {
                v = rightSide.at<uchar> (y, x - midX);   // Right 25%: just use the right face.
            }
            faceImg.at<uchar> (y, x) = v;
        }// end x loop
    }//end y loop
}

cv::Mat PrepocessFace::getPreprocessedFace(Mat& srcImg, int desiredFaceWidth, CascadeClassifier& faceCascade, CascadeClassifier& eyeCascade1, CascadeClassifier& eyeCascade2, bool doLeftAndRightSeparately, Rect* storeFaceRect /*= NULL*/, Point* storeLeftEye /*= NULL*/, Point* storeRightEye /*= NULL*/, Rect* searchedLeftEye /*= NULL*/, Rect* searchedRightEye /*= NULL*/)
{
    // Use square faces.
    int desiredFaceHeight = desiredFaceWidth;
    // Mark the detected face region and eye search regions as invalid, in case they aren't detected.
    if (storeFaceRect)
        storeFaceRect->width = -1;
    if (storeLeftEye)
        storeLeftEye->x = -1;
    if (storeRightEye)
        storeRightEye->x = -1;
    if (searchedLeftEye)
        searchedLeftEye->width = -1;
    if (searchedRightEye)
        searchedRightEye->width = -1;
    // Find the largest face.
    Rect faceRect;
    mDetectObject->detectLargestObject(srcImg, faceCascade, faceRect);
    // Check if a face was detected.
    if (faceRect.width > 0)
    {
        // Give the face rect to the caller if desired.
        if (storeFaceRect)
            *storeFaceRect = faceRect;
        Mat faceImg = srcImg(faceRect);    // Get the detected face image.
        // If the input image is not grayscale, then convert the BGR or BGRA color image to grayscale.
        Mat gray;
        if (faceImg.channels() == 3)
            cvtColor(faceImg, gray, CV_BGR2GRAY);
        else if (faceImg.channels() == 4)
            cvtColor(faceImg, gray, CV_BGRA2GRAY);
        else
            gray = faceImg;     // Access the input image directly, since it is already grayscale.
        // Search for the 2 eyes at the full resolution, since eye detection needs max resolution possible!
        Point leftEye, rightEye;
        detectBothEyes(gray, eyeCascade1, eyeCascade2, leftEye, rightEye, searchedLeftEye, searchedRightEye);
        // Give the eye results to the caller if desired.
        if (storeLeftEye)
            *storeLeftEye = leftEye;
        if (storeRightEye)
            *storeRightEye = rightEye;
        // Check if both eyes were detected.
        if (leftEye.x >= 0 && rightEye.x >= 0)
        {
            // Make the face image the same size as the training images.
            // Since we found both eyes, lets rotate & scale & translate the face so that the 2 eyes
            // line up perfectly with ideal eye positions. This makes sure that eyes will be horizontal,
            // and not too far left or right of the face, etc.
            // Get the center between the 2 eyes.
            Point2f eyesCenter = Point2f((leftEye.x + rightEye.x) * 0.5f, (leftEye.y + rightEye.y) * 0.5f);
            // Get the angle between the 2 eyes.
            double dy = (rightEye.y - leftEye.y);
            double dx = (rightEye.x - leftEye.x);
            double len = sqrt(dx * dx + dy * dy);
            double angle = atan2(dy, dx) * 180.0 / CV_PI;    // Convert from radians to degrees.
            // Hand measurements shown that the left eye center should ideally be at roughly (0.19, 0.14) of a scaled face image.
            const double DESIRED_RIGHT_EYE_X = (1.0f - DESIRED_LEFT_EYE_X);
            // Get the amount we need to scale the image to be the desired fixed size we want.
            double desiredLen = (DESIRED_RIGHT_EYE_X - DESIRED_LEFT_EYE_X) * desiredFaceWidth;
            double scale = desiredLen / len;
            // Get the transformation matrix for rotating and scaling the face to the desired angle & size.
            Mat rot_mat = getRotationMatrix2D(eyesCenter, angle, scale);
            // Shift the center of the eyes to be the desired center between the eyes.
            rot_mat.at<double> (0, 2) += desiredFaceWidth * 0.5f - eyesCenter.x;
            rot_mat.at<double> (1, 2) += desiredFaceHeight * DESIRED_LEFT_EYE_Y - eyesCenter.y;
            // Rotate and scale and translate the image to the desired angle & size & position!
            // Note that we use 'w' for the height instead of 'h', because the input face has 1:1 aspect ratio.
            Mat warped = Mat(desiredFaceHeight, desiredFaceWidth, CV_8U, Scalar(128));       // Clear the output image to a default grey.
            warpAffine(gray, warped, rot_mat, warped.size());
            //imshow("warped", warped);
            // Give the image a standard brightness and contrast, in case it was too dark or had low contrast.
            if (!doLeftAndRightSeparately)
            {
                // Do it on the whole face.
                equalizeHist(warped, warped);
            }
            else
            {
                // Do it seperately for the left and right sides of the face.
                equalizeLeftAndRightHalves(warped);
            }
            //imshow("equalized", warped);
            // Use the "Bilateral Filter" to reduce pixel noise by smoothing the image, but keeping the sharp edges in the face.
            Mat filtered = Mat(warped.size(), CV_8U);
            bilateralFilter(warped, filtered, 0, 20.0, 2.0);
            //imshow("filtered", filtered);
            // Filter out the corners of the face, since we mainly just care about the middle parts.
            // Draw a filled ellipse in the middle of the face-sized image.
            Mat mask = Mat(warped.size(), CV_8U, Scalar(0));       // Start with an empty mask.
            Point faceCenter = Point(desiredFaceWidth / 2, cvRound(desiredFaceHeight * FACE_ELLIPSE_CY));
            Size size = Size(cvRound(desiredFaceWidth * FACE_ELLIPSE_W), cvRound(desiredFaceHeight * FACE_ELLIPSE_H));
            ellipse(mask, faceCenter, size, 0, 0, 360, Scalar(255), CV_FILLED);
            //imshow("mask", mask);
            // Use the mask, to remove outside pixels.
            Mat dstImg = Mat(warped.size(), CV_8U, Scalar(128));       // Clear the output image to a default gray.
            /*
                namedWindow("filtered");
                imshow("filtered", filtered);
                namedWindow("dstImg");
                imshow("dstImg", dstImg);
                namedWindow("mask");
                imshow("mask", mask);
            */
            // Apply the elliptical mask on the face.
            filtered.copyTo(dstImg, mask);    // Copies non-masked pixels from filtered to dstImg.
            //imshow("dstImg", dstImg);
            return dstImg;
        }
        /*
            else {
            // Since no eyes were found, just do a generic image resize.
            resize(gray, tmpImg, Size(w,h));
            }
        */
    }
    return Mat();
}

void PrepocessFace::getPreprocessedCheckFace(Mat& srcImg, int desiredFaceWidth, CascadeClassifier& faceCascade, CascadeClassifier& eyeCascade1, CascadeClassifier& eyeCascade2, Rect* storeFaceRect /*= NULL*/, Point* storeLeftEye /*= NULL*/, Point* storeRightEye /*= NULL*/, Rect* searchedLeftEye /*= NULL*/, Rect* searchedRightEye /*= NULL*/)
{
    int desiredFaceHeight = desiredFaceWidth;
    if (storeFaceRect)
        storeFaceRect->width = -1;
    if (storeLeftEye)
        storeLeftEye->x = -1;
    if (storeRightEye)
        storeRightEye->x = -1;
    if (searchedLeftEye)
        searchedLeftEye->width = -1;
    if (searchedRightEye)
        searchedRightEye->width = -1;
    Rect faceRect;
    mDetectObject->detectLargestObject(srcImg, faceCascade, faceRect);
    if (faceRect.width > 0)
    {
        if (storeFaceRect)
            *storeFaceRect = faceRect;
        Mat faceImg = srcImg(faceRect);
        Mat gray;
        if (faceImg.channels() == 3)
            cvtColor(faceImg, gray, CV_BGR2GRAY);
        else if (faceImg.channels() == 4)
            cvtColor(faceImg, gray, CV_BGRA2GRAY);
        else
            gray = faceImg;
        Point leftEye, rightEye;
        detectBothEyes(gray, eyeCascade1, eyeCascade2, leftEye, rightEye, searchedLeftEye, searchedRightEye);
        if (storeLeftEye)
            *storeLeftEye = leftEye;
        if (storeRightEye)
            *storeRightEye = rightEye;
    }
}

cv::Mat PrepocessFace::getFace(Mat& srcImg, int desiredFaceWidth, CascadeClassifier& faceCascade, CascadeClassifier& eyeCascade1, CascadeClassifier& eyeCascade2, Point* storeLeftEye /*= NULL*/, Point* storeRightEye /*= NULL*/, Rect* searchedLeftEye /*= NULL*/, Rect* searchedRightEye /*= NULL*/)
{
    int desiredFaceHeight = desiredFaceWidth;
    if (storeLeftEye)
        storeLeftEye->x = -1;
    if (storeRightEye)
        storeRightEye->x = -1;
    if (searchedLeftEye)
        searchedLeftEye->width = -1;
    if (searchedRightEye)
        searchedRightEye->width = -1;
    Mat faceImg;
    srcImg.copyTo(faceImg);
    Mat gray;
    if (faceImg.channels() == 3)
        cvtColor(faceImg, gray, CV_BGR2GRAY);
    else if (faceImg.channels() == 4)
        cvtColor(faceImg, gray, CV_BGRA2GRAY);
    else
        gray = faceImg;
    imwrite("D:\\1.jpg", gray);
    Point leftEye, rightEye;
    detectBothEyes(gray, eyeCascade1, eyeCascade2, leftEye, rightEye, searchedLeftEye, searchedRightEye);
    if (storeLeftEye)
        *storeLeftEye = leftEye;
    if (storeRightEye)
        *storeRightEye = rightEye;
    if (leftEye.x >= 0 && rightEye.x >= 0)
    {
        Point2f eyesCenter = Point2f((leftEye.x + rightEye.x) * 0.5f, (leftEye.y + rightEye.y) * 0.5f);
        // Get the angle between the 2 eyes.
        double dy = (rightEye.y - leftEye.y);
        double dx = (rightEye.x - leftEye.x);
        double len = sqrt(dx * dx + dy * dy);
        double angle = atan2(dy, dx) * 180.0 / CV_PI;    // Convert from radians to degrees.
        const double DESIRED_RIGHT_EYE_X = (1.0f - DESIRED_LEFT_EYE_X);
        double desiredLen = (DESIRED_RIGHT_EYE_X - DESIRED_LEFT_EYE_X) * desiredFaceWidth;
        double scale = desiredLen / len;
        /*Mat rot_mat = getRotationMatrix2D(eyesCenter, angle, scale);*/
        Mat rot_mat = getRotationMatrix2D(eyesCenter, angle, 1.6);
        rot_mat.at<double> (0, 2) += desiredFaceWidth * 0.5f - eyesCenter.x;
        rot_mat.at<double> (1, 2) += desiredFaceHeight * DESIRED_LEFT_EYE_Y - eyesCenter.y;
        Mat warped = Mat(desiredFaceHeight, desiredFaceWidth, CV_8U, Scalar(128));
        warpAffine(gray, warped, rot_mat, warped.size());
        equalizeLeftAndRightHalves(warped);
        Mat filtered = Mat(warped.size(), CV_8U);
        bilateralFilter(warped, filtered, 0, 20.0, 2.0);
        //      Mat mask = Mat(warped.size(), CV_8U, Scalar(0));
        //      Point faceCenter = Point( desiredFaceWidth/2, cvRound(desiredFaceHeight * FACE_ELLIPSE_CY) );
        //      Size size = Size( cvRound(desiredFaceWidth * FACE_ELLIPSE_W), cvRound(desiredFaceHeight * FACE_ELLIPSE_H) );
        //      ellipse(mask, faceCenter, size, 0, 0, 360, Scalar(255), CV_FILLED);
        Mat dstImg = Mat(warped.size(), CV_8U, Scalar(128));
        filtered.copyTo(dstImg/*, mask*/);
        return dstImg;
    }
    else
        return Mat();
}

//TrainFaceOpertae **********************************************************************

TrainFaceOpertae::TrainFaceOpertae()
    : faceCascadeFilename(".\\lbpcascade_frontalface.xml")
    , eyeCascadeFilename1(".\\haarcascade_eye.xml")
    , eyeCascadeFilename2(".\\haarcascade_eye_tree_eyeglasses.xml")
    , CHANGE_IN_IMAGE_FOR_COLLECTION(0.3)
    , CHANGE_IN_SECONDS_FOR_COLLECTION(1.0)
{
    mTrainPrepocess.reset(new PrepocessFace());
    mTrainRecognition.reset(new Recognition());
}

TrainFaceOpertae::~TrainFaceOpertae()
{
}

Face_Result_E TrainFaceOpertae::initTrainFaceOpertae()
{
    try
    {
        mfaceCascade.load(faceCascadeFilename);
    }
    catch (cv::Exception& e)
    {
        cerr << "Error Init Dectectors \"" << "    " << "\". Reason: " << e.msg << endl;
    }
    if (mfaceCascade.empty())
        return RESULT_CHECK_INIT_FALURE;
    try
    {
        meyeCascade1.load(eyeCascadeFilename1);
    }
    catch (cv::Exception& e)
    {
        cerr << "Error Init Dectectors \"" << "    " << "\". Reason: " << e.msg << endl;
    }
    if (meyeCascade1.empty())
        return RESULT_CHECK_INIT_FALURE;
    try
    {
        meyeCascade2.load(eyeCascadeFilename2);
    }
    catch (cv::Exception& e)
    {
        cerr << "Error Init Dectectors \"" << "    " << "\". Reason: " << e.msg << endl;
    }
    if (meyeCascade2.empty())
        return RESULT_CHECK_INIT_FALURE;
    return RESULT_SUCCESS;
}

Face_Result_E TrainFaceOpertae::facePretreatment(char* facePath, Mat& Face)
{
    Mat cameraFrame;
    cameraFrame = imread(facePath, CV_LOAD_IMAGE_COLOR);
    if (cameraFrame.empty())
        return RESULT_FAILURE;
    if (FacePretreatment(cameraFrame, Face))
        return RESULT_SUCCESS;
    else
        return RESULT_FAILURE;
}

Face_Result_E TrainFaceOpertae::facePretreatment(Mat& soure, Mat& dec)
{
    if (soure.empty())
        return RESULT_FAILURE;
    if (FacePretreatment(soure, dec))
        return RESULT_SUCCESS;
    else
        return RESULT_FAILURE;
}

void TrainFaceOpertae::setData(int fw, int fh)
{
    facewidth = fw;
    faceheight = fh;
}

bool TrainFaceOpertae::FacePretreatment(Mat& sourceFace, Mat& dec)
{
    Mat old_prepreprocessedFace;
    double old_time = 0;
    if (sourceFace.empty())
    {
        cerr << "ERROR: Couldn't grab the next camera frame." << endl;
        return false;
    }
    Mat displayedFrame;
    sourceFace.copyTo(displayedFrame);
    Rect faceRect;
    Rect searchedLeftEye, searchedRightEye; // top-left and top-right regions of the face, where eyes were searched.
    Point leftEye, rightEye;    // Position of the detected eyes.
    Mat preprocessedFace = mTrainPrepocess->getFace(displayedFrame, facewidth, mfaceCascade, meyeCascade1, meyeCascade2, &leftEye, &rightEye, &searchedLeftEye, &searchedRightEye);
    bool gotFaceAndEyes = false;
    if (preprocessedFace.data)
        gotFaceAndEyes = true;
    if (gotFaceAndEyes)
    {
        // Check if this face looks somewhat different from the previously collected face.
        double imageDiff = 10000000000.0;
        if (old_prepreprocessedFace.data)
            imageDiff = mTrainRecognition->getSimilarity(preprocessedFace, old_prepreprocessedFace);
        // Also record when it happened.
        double current_time = (double) getTickCount();
        double timeDiff_seconds = (current_time - old_time) / getTickFrequency();
        // Only process the face if it is noticeably different from the previous frame and there has been noticeable time gap.
        if ((imageDiff > CHANGE_IN_IMAGE_FOR_COLLECTION) && (timeDiff_seconds > CHANGE_IN_SECONDS_FOR_COLLECTION))
        {
            // Also add the mirror image to the training set, so we have more training data, as well as to deal with faces looking to the left or right.
            Mat mirroredFace;
            flip(preprocessedFace, mirroredFace, 1);
            // Keep a copy of the processed face, to compare on next iteration.
            old_prepreprocessedFace = preprocessedFace;
            old_time = current_time;
            preprocessedFace.copyTo(dec);
            imwrite("D:\\3.jpg", preprocessedFace);
        }
    }
    return true;
}

Ptr<FaceRecognizer> TrainFaceOpertae::faceTrainlearnCollected(std::vector<Mat>& image, std::vector<int>& label, char* name)
{
    return mTrainRecognition->learnCollectedFaces(image, label, name);
}

//PositionFaceOperate ******************************************************************

PositionFaceOperate::PositionFaceOperate()
    : faceCascadeFilename(".\\lbpcascade_frontalface.xml")
    , eyeCascadeFilename1(".\\haarcascade_eye.xml")
    , eyeCascadeFilename2(".\\haarcascade_eye_tree_eyeglasses.xml")
    , UNKNOWN_PERSON_THRESHOLD(0.10f)
    , CHANGE_IN_IMAGE_FOR_COLLECTION(0.3)
    , CHANGE_IN_SECONDS_FOR_COLLECTION(1.0)
{
    mRecognition.reset(new Recognition());
    mPrepocessFace.reset(new PrepocessFace());
}

PositionFaceOperate::~PositionFaceOperate()
{
    createModelDate();
}

Face_Result_E PositionFaceOperate::initPositionFaceOperate()
{
    try
    {
        mfaceCascade.load(faceCascadeFilename);
    }
    catch (cv::Exception& e)
    {
        cerr << "Error Init Dectectors \"" << "    " << "\". Reason: " << e.msg << endl;
    }
    if (mfaceCascade.empty())
        return RESULT_CHECK_INIT_FALURE;
    try
    {
        meyeCascade1.load(eyeCascadeFilename1);
    }
    catch (cv::Exception& e)
    {
        cerr << "Error Init Dectectors \"" << "    " << "\". Reason: " << e.msg << endl;
    }
    if (meyeCascade1.empty())
        return RESULT_CHECK_INIT_FALURE;
    try
    {
        meyeCascade2.load(eyeCascadeFilename2);
    }
    catch (cv::Exception& e)
    {
        cerr << "Error Init Dectectors \"" << "    " << "\". Reason: " << e.msg << endl;
    }
    if (meyeCascade2.empty())
        return RESULT_CHECK_INIT_FALURE;
    return RESULT_SUCCESS;
}

Face_Result_E PositionFaceOperate::predictFace(Mat& predictmat, int& label, double& confidence)
{
    if (positionFaces(predictmat, label, confidence) != RESULT_SUCCESS)
        return RESULT_FAILURE;
    return RESULT_SUCCESS;
}

void PositionFaceOperate::setDate(int fw, int fh, int vw, int vh)
{
    facewidth = fw;
    faceheight = fh;
    videowidth = vw;
    videoheight = vh;
}

Face_Result_E PositionFaceOperate::positionFaces(Mat& predictmat, int& label1, double& confidence1)
{
    label = -1;
    confidence = -1.0;
    for (int j = 0; j < (int) model.size(); j++)
    {
        Mat reconstructedFace;
        reconstructedFace = mRecognition->reconstructFace(model[j], predictmat);
        double similarity = mRecognition->getSimilarity(predictmat, reconstructedFace);
        imwrite("d:\\a_4.bmp", reconstructedFace);
        if (similarity < UNKNOWN_PERSON_THRESHOLD)
        {
            model[j]->predict(predictmat, label, confidence);
            imwrite("d:\\a_3.bmp", predictmat);
            label1 = label;
            confidence1 = confidence;
        }
    }
    return RESULT_SUCCESS;
}

Face_Result_E PositionFaceOperate::getFace(uint8_t* buf, int sizeo, Mat& face)
{
    Mat _tempface;
    CharBufToCVMat(_tempface, buf, sizeo);
    if (Faces(_tempface, face))
        return RESULT_SUCCESS;
    else
        return RESULT_FAILURE;
}

Face_Result_E PositionFaceOperate::getFace(IplImage* image, Mat& face)
{
    Mat _tempFace(image);
    if (Faces(_tempFace, face))
        return RESULT_SUCCESS;
    else
        return RESULT_FAILURE;
}

Face_Result_E PositionFaceOperate::facePretreatmen(Mat& source, Mat& dec)
{
    if (Pretreatmen(source, dec))
        return RESULT_SUCCESS;
    else
        return RESULT_FAILURE;
}

bool PositionFaceOperate::Faces(Mat& sourFace, Mat& decFace)
{
    Mat displayedFrame;
    sourFace.copyTo(displayedFrame);
    Rect faceRect;
    Rect searchedLeftEye, searchedRightEye;
    Point leftEye, rightEye;
    mPrepocessFace->getPreprocessedCheckFace(displayedFrame, facewidth, mfaceCascade, meyeCascade1, meyeCascade2, &faceRect, &leftEye, &rightEye, &searchedLeftEye, &searchedRightEye);
    if (faceRect.width > 0 && leftEye.x >= 0 && rightEye.x >= 0)
        decFace = displayedFrame(faceRect);
    return true;
}

bool PositionFaceOperate::Pretreatmen(Mat& source, Mat& dec)
{
    Mat old_prepreprocessedFace;
    double old_time = 0;
    if (source.empty())
    {
        cerr << "ERROR: Couldn't grab the next camera frame." << endl;
        return false;
    }
    Mat displayedFrame;
    source.copyTo(displayedFrame);
    Rect faceRect;
    Rect searchedLeftEye, searchedRightEye; // top-left and top-right regions of the face, where eyes were searched.
    Point leftEye, rightEye;    // Position of the detected eyes.
    Mat preprocessedFace = mPrepocessFace->getFace(displayedFrame, facewidth, mfaceCascade, meyeCascade1, meyeCascade2, &leftEye, &rightEye, &searchedLeftEye, &searchedRightEye);
    bool gotFaceAndEyes = false;
    if (preprocessedFace.data)
        gotFaceAndEyes = true;
    if (gotFaceAndEyes)
    {
        // Check if this face looks somewhat different from the previously collected face.
        double imageDiff = 10000000000.0;
        if (old_prepreprocessedFace.data)
            imageDiff = mRecognition->getSimilarity(preprocessedFace, old_prepreprocessedFace);
        // Also record when it happened.
        double current_time = (double) getTickCount();
        double timeDiff_seconds = (current_time - old_time) / getTickFrequency();
        // Only process the face if it is noticeably different from the previous frame and there has been noticeable time gap.
        if ((imageDiff > CHANGE_IN_IMAGE_FOR_COLLECTION) && (timeDiff_seconds > CHANGE_IN_SECONDS_FOR_COLLECTION))
        {
            // Also add the mirror image to the training set, so we have more training data, as well as to deal with faces looking to the left or right.
            Mat mirroredFace;
            flip(preprocessedFace, mirroredFace, 1);
            // Keep a copy of the processed face, to compare on next iteration.
            old_prepreprocessedFace = preprocessedFace;
            old_time = current_time;
            preprocessedFace.copyTo(dec);
        }
    }
    return true;
}

void PositionFaceOperate::CharBufToCVMat(Mat& source, uint8_t* buf, int leng)
{
    IplImage* iplImg;
    bool isLowerLeft = videoheight > 0;
    CvSize Size;
    Size.height = (videoheight > 0) ? videoheight : -videoheight;
    Size.width = videowidth;
    iplImg = cvCreateImageHeader(Size, IPL_DEPTH_8U, 24 / 8);
    cvSetData(iplImg, (char*) buf, leng / videoheight);
    source = iplImg;
    cvSaveImage("D:\\charBufTOmat.bmp", iplImg);
}

void PositionFaceOperate::loadModelDate(std::vector<ModelXmlInfo_t>& modeItem)
{
    for (int i = 0; i < modeItem.size(); i++)
        model.push_back(createFisherFaceRecognizer());
    for (int i = 0; i < (int) modeItem.size(); i++)
        model[i]->load(modeItem[i].modelPath);
}

void PositionFaceOperate::createModelDate()
{
    model.clear();
}

//CheckFaceOperatec ******************************************************************

CheckFaceOperatec::CheckFaceOperatec()
    : faceCascadeFilename(".\\lbpcascade_frontalface.xml")
    , eyeCascadeFilename1(".\\haarcascade_eye.xml")
    , eyeCascadeFilename2(".\\haarcascade_eye_tree_eyeglasses.xml")
    , faceWidth(150)
    , faceHeight(150)
    , VideoWidth(720)
    , VideoHeight(576)
{
    mPrepocessFacePtr.reset(new PrepocessFace());
}

CheckFaceOperatec::~CheckFaceOperatec()
{
}

Face_Result_E CheckFaceOperatec::initCheckface()
{
    try
    {
        mfaceCascade.load(faceCascadeFilename);
    }
    catch (cv::Exception& e)
    {
        cerr << "Error Init Dectectors \"" << "    " << "\". Reason: " << e.msg << endl;
    }
    if (mfaceCascade.empty())
        return RESULT_CHECK_INIT_FALURE;
    try
    {
        meyeCascade1.load(eyeCascadeFilename1);
    }
    catch (cv::Exception& e)
    {
        cerr << "Error Init Dectectors \"" << "    " << "\". Reason: " << e.msg << endl;
    }
    if (meyeCascade1.empty())
        return RESULT_CHECK_INIT_FALURE;
    try
    {
        meyeCascade2.load(eyeCascadeFilename2);
    }
    catch (cv::Exception& e)
    {
        cerr << "Error Init Dectectors \"" << "    " << "\". Reason: " << e.msg << endl;
    }
    if (meyeCascade2.empty())
        return RESULT_CHECK_INIT_FALURE;
    return RESULT_SUCCESS;
}

void CheckFaceOperatec::setDate(int vw, int vh, int fw, int fh)
{
    VideoHeight = vh;
    VideoWidth = vw;
    faceWidth = fw;
    faceHeight = fh;
}

void CheckFaceOperatec::CharBufToCVMat(Mat& source, uint8_t* buf, int leng)
{
    IplImage* iplImg;
    bool isLowerLeft = VideoHeight > 0;
    CvSize Size;
    Size.height = (VideoHeight > 0) ? VideoHeight : -VideoHeight;
    Size.width = VideoWidth;
    iplImg = cvCreateImageHeader(Size, IPL_DEPTH_8U, 24 / 8);
    cvSetData(iplImg, (char*) buf, leng / VideoHeight);
    source = iplImg;
    cvSaveImage("D:\\charBufTOmat.bmp", iplImg);
}

bool CheckFaceOperatec::checkFace(uint8_t* buf, int leng, Mat& face, Check_Face_Model_t mod_)
{
    Mat _bufMat;
    CharBufToCVMat(_bufMat, buf, leng);
    if (_bufMat.empty())
    {
        face = Mat();
        return false;
    }
    if (FaceDetection(_bufMat, face, mod_))
        return true;
    else
        return false;
}

bool CheckFaceOperatec::checkFace(IplImage* img, Mat& face, Check_Face_Model_t mod_)
{
    Mat cameraFrame(img);
    if (cameraFrame.empty())
    {
        face = Mat();
        return false;
    }
    if (FaceDetection(cameraFrame, face, mod_))
        return true;
    else
        return false;
}

bool CheckFaceOperatec::checkFace(char* path, Mat& face)
{
    Mat cameraFrame;
    cameraFrame = cvLoadImage(path, CV_LOAD_IMAGE_UNCHANGED);
    if (cameraFrame.empty())
    {
        face = Mat();
        return false;
    }
    if (FaceDetection(cameraFrame, face, MODEL_FACEANDEYES))
        return true;
    else
        return false;
}

bool CheckFaceOperatec::FaceDetection(Mat& sour, Mat& dec , Check_Face_Model_t mod)
{
    Mat displayedFrame;
    sour.copyTo(displayedFrame);
    Rect faceRect;
    Rect searchedLeftEye, searchedRightEye;
    Point leftEye, rightEye;
    mPrepocessFacePtr->getPreprocessedCheckFace(displayedFrame, faceWidth, mfaceCascade, meyeCascade1, meyeCascade2, &faceRect, &leftEye, &rightEye, &searchedLeftEye, &searchedRightEye);
    switch (mod)
    {
        case  MODEL_FACE:
            if (faceRect.width > 0)
                dec = displayedFrame(faceRect);
            break;
        case MODEL_FACEANDLEFTEYE:
            if (faceRect.width > 0 && leftEye.x >= 0)
                dec = displayedFrame(faceRect);
            break;
        case MODEL_FACEANDRIGHTEYE:
            if (faceRect.width > 0 && rightEye.x >= 0)
                dec = displayedFrame(faceRect);
            break;
        case MODEL_FACEANDEYES:
            if (faceRect.width > 0 && leftEye.x >= 0 && rightEye.x >= 0)
                dec = displayedFrame(faceRect);
            break;
    }
    return true;
}


