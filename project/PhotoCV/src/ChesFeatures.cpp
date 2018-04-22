
#include "stdafx.h"
#include "ChesFeatures.h"


Mat MatDetection::CannyMat(const Mat& f_img, Mat& contoursInv)
{
    Mat t_img = f_img.clone();
    cvtColor(t_img, t_img, CV_RGB2GRAY);
    const double PI = 3.1415926;
    //Sobel����
    Mat sobelX = Mat(), sobelY = Mat(), sobelMagnitude, sobelOrientation, bin;
    int aperture(3);
    cv::Sobel(t_img, sobelX, CV_32F, 1, 0, aperture);
    cv::Sobel(t_img, sobelY, CV_32F, 0, 1, aperture);
    cv::cartToPolar(sobelX, sobelY, sobelMagnitude, sobelOrientation);    // Compute magnitude and orientation
    sobelOrientation.convertTo(bin, CV_8U, 90 / PI);
    imshow("Sobel (orientation)", bin);
    threshold(sobelMagnitude, bin, 125, 255, cv::THRESH_BINARY_INV);
    imshow("Sobel (low threshold)", bin);
    threshold(sobelMagnitude, bin, 350, 255, cv::THRESH_BINARY_INV);
    imshow("Sobel (high threshold)", bin);
    //canny����
    cv::Mat contours;
    cv::Canny(f_img, contours, 125, 350);
    //cv::Mat contoursInv;
    cv::threshold(contours, contoursInv, 128, 255, cv::THRESH_BINARY_INV);
    imshow("Canny Contours", contoursInv);
    waitKey(0);
    return contoursInv;
}

Mat MatDetection::CheckLines(const Mat& f_img, Mat& f_out)
{
    //����Canny��Ե
    Mat t_img = f_img.clone();
    cvtColor(t_img, t_img, CV_RGB2GRAY);
    cv::Mat contours, contoursInv;
    cv::Canny(t_img, contours, 125, 350);
    threshold(contours, contoursInv, 128, 255, cv::THRESH_BINARY_INV);
    //������
    std::vector<cv::Vec2f> lines;       // ���ֱ��
    HoughLines(contours, lines, 1, PI / 180, 60);    //ע������lines.size�͵õ�ֱ����
    //���滭��һЩ��Χ��ֱ��
    cv::Mat result(contoursInv.rows, contoursInv.cols, CV_8U, cv::Scalar(255));
    f_img.copyTo(result);
    std::cout << "Lines detected: " << lines.size() << std::endl;
    system("pause");
    std::vector<cv::Vec2f>::const_iterator it = lines.begin();
    while (it != lines.end())
    {
        float rho = (*it) [0];   // first element is distance rho
        float theta = (*it) [1];   // second element is angle theta
        if (theta < PI / 4. || theta > 3.*PI / 4.)   // ~vertical line
        {
            // point of intersection of the line with first row
            cv::Point pt1(rho / cos(theta), 0);
            // point of intersection of the line with last row
            cv::Point pt2((rho - result.rows * sin(theta)) / cos(theta), result.rows);
            // draw a white line
            cv::line(result, pt1, pt2, cv::Scalar(255), 1);
        }
        else     // ~horizontal line
        {
            // point of intersection of the line with first column
            cv::Point pt1(0, rho / sin(theta));
            // point of intersection of the line with last column
            cv::Point pt2(result.cols, (rho - result.cols * cos(theta)) / sin(theta));
            // draw a white line
            cv::line(result, pt1, pt2, cv::Scalar(255), 1);
        }
        std::cout << "line: (" << rho << "," << theta << ")\n";
        ++it;
    }
    // Display the detected line image
    cv::namedWindow("����任����ֱ��");
    cv::imshow("����任����ֱ��", result);
    waitKey(0);
    return result;
}

Mat MatDetection::GetOneLine(const Mat& f_img, Mat& f_out)
{
    /*  1.Canny����Ե����  */
    Mat contours, t_img = f_img.clone() , contoursInv;
    cvtColor(t_img, t_img, CV_RGB2GRAY);    //�ҶȻ�
    Canny(t_img, contours, 125, 350);    //�õ���Եͼ    //ShowMat(contours,"��Եͼ");
    threshold(contours, contoursInv, 128, 255, cv::THRESH_BINARY_INV);    //��ֵ��ȡ��
    /*  2.�ø��ʻ���任��ֱ��  */
    std::vector<cv::Vec4i> lines;                               // �����а�����⵽��ֱ�ߵĶ˵�
    double deltaRho = 1, deltaTheta = PI / 180;            // �ۼ����ķֱ���
    int minVote = 80;                                                // ֱ�߱�����ʱ����Ҫ����СͶƱ��
    double minLength = 100;                                    // ֱ�ߵ���С����
    double maxGap = 20;                                          // ����ֱ�߷�������ȱ��
    HoughLinesP(contours, lines, deltaRho, deltaTheta, minVote, minLength, maxGap);
    /*  3.������Щ����ֱ��   */
    f_out = f_img.clone();        //��ʾ��ɫͼ
    for (std::vector<cv::Vec4i>::const_iterator it2 = lines.begin(); it2 != lines.end(); ++it2)
    {
        std::cout << "(" << (*it2) [0] << "," << (*it2) [1] << ")-(" << (*it2) [2] << "," << (*it2) [3] << ")" << std::endl;         //��ӡ��Щֱ�ߵ�����
        Point pt1((*it2) [0], (*it2) [1]), pt2((*it2) [2], (*it2) [3]);               //lines������ľ��ǵ����Ϣ
        cv::line(f_out, pt1, pt2, Scalar(255));
    }
    cv::imshow("����ֱ�߽��", f_out);
    waitKey(0);
    /*----------------4.��չ ��ȡ����ֱ�ߵķ��� ����ͼ�㴦��--------------------- */
    int n = 0;    //�������ʾ ��һ��ֱ�ߵ�������
    Mat oneline(f_img.size(), CV_8U, Scalar(0));                                                                //��һ����ɫ��������ͼ�� ��contours����
    line(oneline, Point(lines[n][0], lines[n][1]), Point(lines[n][2], lines[n][3]), Scalar(255), 5);             //��ǻ�����Ӧ�ߵ�����
    bitwise_and(contours, oneline, oneline);                                                                      //������������
    threshold(oneline, oneline, 128, 255, THRESH_BINARY_INV);                                        //��ֵ��ȡ��һ��
    cv::imshow("��ʾһ������ֱ�ߵ�Ч��", oneline);
    waitKey(0);
    return f_out;
}

Mat MatDetection::CheckCircle(const Mat& f_img, Mat& f_out)
{
    /*  1.Canny����Ե����  */
    Mat contours, t_img = f_img.clone() , contoursInv;
    cvtColor(t_img, t_img, CV_RGB2GRAY);    //�ҶȻ�
    GaussianBlur(t_img, t_img, cv::Size(5, 5), 1.5);
    std::vector<cv::Vec3f> circles;
    HoughCircles(t_img, circles, CV_HOUGH_GRADIENT,
                 2,   // accumulator resolution (size of the image / 2)
                 50,  // minimum distance between two circles
                 200, // Canny high threshold
                 100, // minimum number of votes
                 5, 100);  // min and max radius
    cout << "Circles: " << circles.size() << endl;
    // Draw the circles
    std::vector<cv::Vec3f>::const_iterator itc = circles.begin();
    f_out = f_img.clone();    //���ڲ�ɫͼ��
    while (itc != circles.end())
    {
        cv::circle(f_out,
                   cv::Point((*itc) [0], (*itc) [1]),        // circle centre
                   (*itc) [2],                         // circle radius
                   cv::Scalar(255),                    // color
                   2);                                 // thickness
        ++itc;
    }
    //resize(image, image,Size(800,600) );
    imshow("���ͼƬ�е�Բ", f_out);
    waitKey(0);
    return f_out;
}

Mat MatDetection::CheckRoadEdge(const Mat& f_img, Mat& f_out)
{
    return f_out;
    ///*  1.Canny����Ե����  */
    //Mat contours, image;
    //cvtColor(mImgOld.clone(), image, CV_RGB2GRAY);    //�ҶȻ�
    //Canny(image, contours, 125, 350);    //�õ���Եͼ    //ShowMat(contours,"��Եͼ");
    ///*  2.�ø��ʻ���任��ֱ��  */
    //std::vector<cv::Vec4i> lines;                               // �����а�����⵽��ֱ�ߵĶ˵�
    //HoughLinesP(contours, lines, 1.0, PI / 180, 80, 100.0, 20.0);
    ///*----------------4.��չ ��ȡ����ֱ�ߵķ��� ����ͼ�㴦��--------------------- */
    //int n = 0;    //�������ʾ ��һ��ֱ�ߵ�������
    //Mat oneline(image.size(), CV_8U, Scalar(0));                                                                //��һ����ɫ��������ͼ�� ��contours����
    //line(oneline, Point(lines[n][0], lines[n][1]), Point(lines[n][2], lines[n][3]), Scalar(255), 5);     //��ǻ�����Ӧ�ߵ�����
    //bitwise_and(contours, oneline, oneline);                                                                      //������������
    ////����������ֱ�߽��------
    //std::vector<cv::Point> points;
    //// Iterate over the pixels to obtain all point positions
    //for (int y = 0; y < oneline.rows; y++)
    //{
    //    uchar* rowPtr = oneline.ptr<uchar>(y);
    //    for (int x = 0; x < oneline.cols; x++)     // if on a contour
    //        if (rowPtr[x])
    //            points.push_back(cv::Point(x, y));
    //}
    //
    //// find the best fitting line
    //cv::Vec4f line;
    //cv::fitLine(cv::Mat(points), line, CV_DIST_L2, 0, 0.01, 0.01);
    //std::cout << "line: (" << line[0] << "," << line[1] << ")(" << line[2] << "," << line[3] << ")\n";
    //
    //int x0 = line[2];                //ֱ���ϵĵ�
    //int y0 = line[3];
    //int x1 = x0 - 200 * line[0];     //ʹ�õ�Ԫ����
    //int y1 = y0 - 200 * line[1];     //��ӳ���Ϊ200������
    //image = mImgOld.clone();
    //cv::line(image, cv::Point(x0, y0), cv::Point(x1, y1), cv::Scalar(0), 3);
    //cv::namedWindow("Estimated line");
    //cv::imshow("Estimated line", image);
}

void MatDetection::GetConnectivity(const Mat& f_img)
{
    //ͨ��������̬ѧ����õ� ��ֵͼ
    cv::Mat image = cv::imread("binaryGroup.bmp", 0);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    cout << "Contours: " << contours.size() << endl;
    for (vector<vector<Point> >::const_iterator itContours = contours.begin() ; itContours != contours.end(); ++itContours)
        std::cout << "Size: " << itContours->size() << std::endl;
    // draw black contours on white image
    Mat result(image.size(), CV_8U, cv::Scalar(255));
    drawContours(result, contours,
                 -1, // draw all contours
                 cv::Scalar(0),    // in black
                 2);  // with a thickness of 2
    imshow("Contours", result);    //��ʾ����
    //�����������
    int cmin = 100, cmax = 1000;
    for (vector<vector<Point> >::const_iterator itc = contours.begin(); itc != contours.end(); ++itc)
        if (itc->size() < cmin || itc->size() > cmax)
            itc = contours.erase(itc);
    //ԭͼ�л�������
    Mat original = cv::imread("group.jpg");
    drawContours(original, contours, -1, Scalar(255, 255, 255), 2);
    imshow("Contours on Animals", original);
}



CharacteristicPt::CharacteristicPt()
{
}

CharacteristicPt::~CharacteristicPt()
{
}

void CharacteristicPt::SiftFeature(Mat& In_img) const
{
    Mat image = In_img.clone();
    Mat iamgeGray;
    cvtColor(image, iamgeGray, CV_BGR2GRAY);
    //��ȡsift������
    vector<KeyPoint> keypoints;
#if CV_MAJOR_VERSION<3
    //����0���������ķ�ֵ������ȥ���ͶԱȶȵĹؼ���   10�����ڽ���ֱ�����жȵķ�ֵ��ȥ�����ȵ�ı�Ե��Ӧ��
    SiftFeatureDetector sift2(0, 10);
    sift2.detect(iamgeGray, keypoints);
    //��ԭͼ����������
    Mat descriptors;
    //DrawMatchesFlags::DRAW_RICH_KEYPOINTS˵��
    //0ֻ���������㣬����������Χ��circle����������Ĵ�С�ͷ���.
    //1���ͼ�񽫱�������ƥ��㽫���������ͼ���������.
    //2�����ĵ㲻����.
    //4��ÿ����������Χ��circle������������Ĵ�С�ͷ��򽫱�����.
    //Scalar(255, 255, 255) �����ɫ
    drawKeypoints(image, keypoints, image, Scalar(0, 255, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    /*  Scalar::all(-1), //�������������ɫ��Ϊ���
        //��������Ϊ���Ļ�Բ��Բ�İ뾶��ʾ������Ĵ�С��ֱ�߱�ʾ������ķ���
        DrawMatchesFlags::DRAW_RICH_KEYPOINTS);*/
    imshow("sift������", image);
    waitKey();
#endif
}

void CharacteristicPt::SiftFeature(Mat& In_img, Mat& In_img2) const
{
#if CV_MAJOR_VERSION<3
    //���sift������
    SiftFeatureDetector detector;
    //������ʸ��
    vector<KeyPoint> keypoint1, keypoint2;
    detector.detect(In_img, keypoint1);
    detector.detect(In_img2, keypoint2);
    Mat output1, output2;
    drawKeypoints(In_img, keypoint1, output1);
    drawKeypoints(In_img2, keypoint2, output2);
    //imshow("ԭͼ1������.jpg", output1);
    //imshow("ԭͼ2������.jpg", output2);
    //waitKey(0);
    SiftDescriptorExtractor extractor;
    Mat descriptor1, descriptor2;
    extractor.compute(In_img, keypoint1, descriptor1);
    extractor.compute(In_img2, keypoint2, descriptor2);
    //ʵ��������ƥ����
    BruteForceMatcher<L2<float> > matcher;
    //����ƥ��������
    vector<DMatch> matches;
    //ʵ��������֮���ƥ�䣬�õ�����matches
    matcher.match(descriptor1, descriptor2, matches);
    //��ȡ��ǰ30�����ƥ����
    std::nth_element(matches.begin(),       // ƥ�������ӵĳ�ʼλ��
                     matches.begin() + 29,  // ���������
                     matches.end());        // ����λ��
    //�޳��������ƥ����
    matches.erase(matches.begin() + 30, matches.end());
    //��ʾ�Ա�ͼƬ
    Mat img_matches;
    drawMatches(In_img, keypoint1,   //��һ��ͼ�������������
                In_img2, keypoint2, //�ڶ���ͼ�������������
                matches,            //ƥ��������
                img_matches);       //ƥ�����ͼ��
    imshow("sift�Ա�ͼ", img_matches);
    //waitKey(0);
#endif
}

void CharacteristicPt::compareMat(Mat& In_img, Mat& In_img2) const
{
#if CV_MAJOR_VERSION<3
    Mat leftImage, rightImage;
    cvtColor(In_img, leftImage, CV_BGR2GRAY);
    cvtColor(In_img2, rightImage, CV_BGR2GRAY);
    //�������
    Mat disparityImage, disp8;
    //����StereoSGBM����
    StereoSGBM sgbm = StereoSGBM(0, 16, 3, 0, 0, 0, 0, 5, 50, 16);
    sgbm.preFilterCap = 63;
    int SADWindowSize = 9;
    sgbm.SADWindowSize = SADWindowSize > 0 ? SADWindowSize : 3;
    int cn = leftImage.channels();
    int numberOfDisparities = 64;
    sgbm.P1 = 8 * cn * sgbm.SADWindowSize * sgbm.SADWindowSize;
    sgbm.P2 = 32 * cn * sgbm.SADWindowSize * sgbm.SADWindowSize;
    sgbm.minDisparity = 0;
    sgbm.numberOfDisparities = numberOfDisparities;
    sgbm.uniquenessRatio = 10;
    sgbm.speckleWindowSize = 100;
    sgbm.speckleRange = 32;
    sgbm.disp12MaxDiff = 1;
    //��������ͼ��������ԣ����죩
    sgbm(leftImage, rightImage, disparityImage);
    //��ʾ���
    disparityImage.convertTo(disp8, CV_8U, 255 / (numberOfDisparities * 16.));
    imshow("��ʾ���", disp8);
    imwrite("result.jpg", disparityImage);
    waitKey(0);
#endif
}

