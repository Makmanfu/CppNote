
#include "stdafx.h"
#include "ChesFeatures.h"


Mat MatDetection::CannyMat(const Mat& f_img, Mat& contoursInv)
{
    Mat t_img = f_img.clone();
    cvtColor(t_img, t_img, CV_RGB2GRAY);
    const double PI = 3.1415926;
    //Sobel算子
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
    //canny算子
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
    //计算Canny边缘
    Mat t_img = f_img.clone();
    cvtColor(t_img, t_img, CV_RGB2GRAY);
    cv::Mat contours, contoursInv;
    cv::Canny(t_img, contours, 125, 350);
    threshold(contours, contoursInv, 128, 255, cv::THRESH_BINARY_INV);
    //霍夫检测
    std::vector<cv::Vec2f> lines;       // 检测直线
    HoughLines(contours, lines, 1, PI / 180, 60);    //注意这里lines.size就得到直线数
    //下面画出一些范围的直线
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
    cv::namedWindow("霍夫变换检测的直线");
    cv::imshow("霍夫变换检测的直线", result);
    waitKey(0);
    return result;
}

Mat MatDetection::GetOneLine(const Mat& f_img, Mat& f_out)
{
    /*  1.Canny做边缘处理  */
    Mat contours, t_img = f_img.clone() , contoursInv;
    cvtColor(t_img, t_img, CV_RGB2GRAY);    //灰度化
    Canny(t_img, contours, 125, 350);    //得到边缘图    //ShowMat(contours,"边缘图");
    threshold(contours, contoursInv, 128, 255, cv::THRESH_BINARY_INV);    //二值化取反
    /*  2.用概率霍夫变换找直线  */
    std::vector<cv::Vec4i> lines;                               // 向量中包含检测到的直线的端点
    double deltaRho = 1, deltaTheta = PI / 180;            // 累加器的分辨率
    int minVote = 80;                                                // 直线被接受时所需要的最小投票数
    double minLength = 100;                                    // 直线的最小长度
    double maxGap = 20;                                          // 沿着直线方向的最大缺口
    HoughLinesP(contours, lines, deltaRho, deltaTheta, minVote, minLength, maxGap);
    /*  3.画出这些检查的直线   */
    f_out = f_img.clone();        //显示彩色图
    for (std::vector<cv::Vec4i>::const_iterator it2 = lines.begin(); it2 != lines.end(); ++it2)
    {
        std::cout << "(" << (*it2) [0] << "," << (*it2) [1] << ")-(" << (*it2) [2] << "," << (*it2) [3] << ")" << std::endl;         //打印这些直线的坐标
        Point pt1((*it2) [0], (*it2) [1]), pt2((*it2) [2], (*it2) [3]);               //lines本来存的就是点的信息
        cv::line(f_out, pt1, pt2, Scalar(255));
    }
    cv::imshow("检测的直线结果", f_out);
    waitKey(0);
    /*----------------4.扩展 提取区域直线的方法 类似图层处理--------------------- */
    int n = 0;    //下面就显示 第一根直线的区域了
    Mat oneline(f_img.size(), CV_8U, Scalar(0));                                                                //建一个黑色区域类似图层 的contours背景
    line(oneline, Point(lines[n][0], lines[n][1]), Point(lines[n][2], lines[n][3]), Scalar(255), 5);             //标记画出对应线的区域
    bitwise_and(contours, oneline, oneline);                                                                      //剪出过滤区域
    threshold(oneline, oneline, 128, 255, THRESH_BINARY_INV);                                        //二值化取反一下
    cv::imshow("显示一个区域直线的效果", oneline);
    waitKey(0);
    return f_out;
}

Mat MatDetection::CheckCircle(const Mat& f_img, Mat& f_out)
{
    /*  1.Canny做边缘处理  */
    Mat contours, t_img = f_img.clone() , contoursInv;
    cvtColor(t_img, t_img, CV_RGB2GRAY);    //灰度化
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
    f_out = f_img.clone();    //画在彩色图中
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
    imshow("检测图片中的圆", f_out);
    waitKey(0);
    return f_out;
}

Mat MatDetection::CheckRoadEdge(const Mat& f_img, Mat& f_out)
{
    return f_out;
    ///*  1.Canny做边缘处理  */
    //Mat contours, image;
    //cvtColor(mImgOld.clone(), image, CV_RGB2GRAY);    //灰度化
    //Canny(image, contours, 125, 350);    //得到边缘图    //ShowMat(contours,"边缘图");
    ///*  2.用概率霍夫变换找直线  */
    //std::vector<cv::Vec4i> lines;                               // 向量中包含检测到的直线的端点
    //HoughLinesP(contours, lines, 1.0, PI / 180, 80, 100.0, 20.0);
    ///*----------------4.扩展 提取区域直线的方法 类似图层处理--------------------- */
    //int n = 0;    //下面就显示 第一根直线的区域了
    //Mat oneline(image.size(), CV_8U, Scalar(0));                                                                //建一个黑色区域类似图层 的contours背景
    //line(oneline, Point(lines[n][0], lines[n][1]), Point(lines[n][2], lines[n][3]), Scalar(255), 5);     //标记画出对应线的区域
    //bitwise_and(contours, oneline, oneline);                                                                      //剪出过滤区域
    ////下面检测最优直线结果------
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
    //int x0 = line[2];                //直线上的点
    //int y0 = line[3];
    //int x1 = x0 - 200 * line[0];     //使用单元向量
    //int y1 = y0 - 200 * line[1];     //添加长度为200的向量
    //image = mImgOld.clone();
    //cv::line(image, cv::Point(x0, y0), cv::Point(x1, y1), cv::Scalar(0), 3);
    //cv::namedWindow("Estimated line");
    //cv::imshow("Estimated line", image);
}

void MatDetection::GetConnectivity(const Mat& f_img)
{
    //通过各种形态学处理得到 二值图
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
    imshow("Contours", result);    //显示轮廓
    //下面过滤轮廓
    int cmin = 100, cmax = 1000;
    for (vector<vector<Point> >::const_iterator itc = contours.begin(); itc != contours.end(); ++itc)
        if (itc->size() < cmin || itc->size() > cmax)
            itc = contours.erase(itc);
    //原图中画出轮廓
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
    //提取sift特征点
    vector<KeyPoint> keypoints;
#if CV_MAJOR_VERSION<3
    //其中0代表特征的阀值：用于去除低对比度的关键点   10是用于降低直线敏感度的阀值：去除不稳点的边缘响应点
    SiftFeatureDetector sift2(0, 10);
    sift2.detect(iamgeGray, keypoints);
    //在原图画出特征点
    Mat descriptors;
    //DrawMatchesFlags::DRAW_RICH_KEYPOINTS说明
    //0只画出特征点，而不画出周围的circle包含特征点的大小和方向.
    //1输出图像将被创建，匹配点将被画在输出图像的内容上.
    //2单个的点不画出.
    //4对每个特征点周围的circle，包含特征点的大小和方向将被画出.
    //Scalar(255, 255, 255) 随机颜色
    drawKeypoints(image, keypoints, image, Scalar(0, 255, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    /*  Scalar::all(-1), //绘制特征点的颜色，为随机
        //以特征点为中心画圆，圆的半径表示特征点的大小，直线表示特征点的方向
        DrawMatchesFlags::DRAW_RICH_KEYPOINTS);*/
    imshow("sift特征点", image);
    waitKey();
#endif
}

void CharacteristicPt::SiftFeature(Mat& In_img, Mat& In_img2) const
{
#if CV_MAJOR_VERSION<3
    //检测sift特征点
    SiftFeatureDetector detector;
    //特征点矢量
    vector<KeyPoint> keypoint1, keypoint2;
    detector.detect(In_img, keypoint1);
    detector.detect(In_img2, keypoint2);
    Mat output1, output2;
    drawKeypoints(In_img, keypoint1, output1);
    drawKeypoints(In_img2, keypoint2, output2);
    //imshow("原图1特征点.jpg", output1);
    //imshow("原图2特征点.jpg", output2);
    //waitKey(0);
    SiftDescriptorExtractor extractor;
    Mat descriptor1, descriptor2;
    extractor.compute(In_img, keypoint1, descriptor1);
    extractor.compute(In_img2, keypoint2, descriptor2);
    //实例化暴力匹配器
    BruteForceMatcher<L2<float> > matcher;
    //定义匹配器算子
    vector<DMatch> matches;
    //实现描述符之间的匹配，得到算子matches
    matcher.match(descriptor1, descriptor2, matches);
    //提取出前30个最佳匹配结果
    std::nth_element(matches.begin(),       // 匹配器算子的初始位置
                     matches.begin() + 29,  // 排序的数量
                     matches.end());        // 结束位置
    //剔除掉其余的匹配结果
    matches.erase(matches.begin() + 30, matches.end());
    //显示对比图片
    Mat img_matches;
    drawMatches(In_img, keypoint1,   //第一幅图像和它的特征点
                In_img2, keypoint2, //第二幅图像和它的特征点
                matches,            //匹配器算子
                img_matches);       //匹配输出图像
    imshow("sift对比图", img_matches);
    //waitKey(0);
#endif
}

void CharacteristicPt::compareMat(Mat& In_img, Mat& In_img2) const
{
#if CV_MAJOR_VERSION<3
    Mat leftImage, rightImage;
    cvtColor(In_img, leftImage, CV_BGR2GRAY);
    cvtColor(In_img2, rightImage, CV_BGR2GRAY);
    //结果数据
    Mat disparityImage, disp8;
    //创建StereoSGBM对象
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
    //查找两幅图像间的相关性（差异）
    sgbm(leftImage, rightImage, disparityImage);
    //显示结果
    disparityImage.convertTo(disp8, CV_8U, 255 / (numberOfDisparities * 16.));
    imshow("显示结果", disp8);
    imwrite("result.jpg", disparityImage);
    waitKey(0);
#endif
}

