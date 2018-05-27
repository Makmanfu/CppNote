
#include "stdafx.h"
#include "WndOpenCV.h"

WndOpenCV::WndOpenCV(QWidget* parent) : QWidget(parent)
    , ImageShow(NULL)
    , Act_OpenImg(NULL), Act_NavtImg(NULL)
    , Act_gray(NULL), Act_blackw(NULL), Act_Equalize(NULL)
    , Act_erode(NULL), Act_dilate(NULL), Act_opend(NULL), Act_closed(NULL)
    , Act_Canny(NULL)
    , Act_Face(NULL), Act_Pep(NULL), Act_Speed(NULL), Act_Car(NULL), Act_Num(NULL)
    , Act_CutScreen(NULL), Act_Video(NULL), Act_Avi(NULL)
{
    this->setMinimumSize(600, 600);
    //设置背景颜色
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(242, 79, 101, 255));
    setPalette(pal);
    //设置为自动填充背景
    setAutoFillBackground(true);
    //设置字体
    Menuft = QFont("Modern", 13);
    ImageShow = new QLabel(this);
    ImageShow->move(0, 0);
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(ImageShow);
    ImageShowGrid = new QGridLayout;
    this->setLayout(ImageShowGrid);
    ImageShowGrid->setMargin(0);
    ImageShowGrid->addWidget(/*ImageShow*/scrollArea, 0, 0);
    connect(&theTimer, SIGNAL(timeout()), this, SLOT(updateImage()));
    CreateMenus();
}

WndOpenCV::~WndOpenCV()
{
    //if(theTimer)
    if (ImageShow != NULL)
    {
        delete ImageShow;
        ImageShow = NULL;
    }
    if (Act_OpenImg != NULL)
    {
        delete Act_OpenImg;
        Act_OpenImg = NULL;
    }
    if (Act_NavtImg != NULL)
    {
        delete Act_NavtImg;
        Act_NavtImg = NULL;
    }
    if (Act_gray != NULL)
    {
        delete Act_gray;
        Act_gray = NULL;
    }
    if (Act_blackw != NULL)
    {
        delete Act_blackw;
        Act_blackw = NULL;
    }
    if (Act_Equalize != NULL)
    {
        delete Act_Equalize;
        Act_Equalize = NULL;
    }
    if (Act_erode != NULL)
    {
        delete Act_erode;
        Act_erode = NULL;
    }
    if (Act_dilate != NULL)
    {
        delete Act_dilate;
        Act_dilate = NULL;
    }
    if (Act_opend != NULL)
    {
        delete Act_opend;
        Act_opend = NULL;
    }
    if (Act_closed != NULL)
    {
        delete Act_closed;
        Act_closed = NULL;
    }
    if (Act_Canny != NULL)
    {
        delete Act_Canny;
        Act_Canny = NULL;
    }
    if (Act_Face != NULL)
    {
        delete Act_Face;
        Act_Face = NULL;
    }
    if (Act_Pep != NULL)
    {
        delete Act_Pep;
        Act_Pep = NULL;
    }
    if (Act_Speed != NULL)
    {
        delete Act_Speed;
        Act_Speed = NULL;
    }
    if (Act_Car != NULL)
    {
        delete Act_Car;
        Act_Car = NULL;
    }
    if (Act_Num != NULL)
    {
        delete Act_Num;
        Act_Num = NULL;
    }
    if (Act_CutScreen != NULL)
    {
        delete Act_CutScreen;
        Act_CutScreen = NULL;
    }
}

void WndOpenCV::CreateMenus()
{
    //定义程序菜单
    Act_OpenImg = new QAction(TCSR("打开图片..."), this);
    connect(Act_OpenImg, SIGNAL(triggered()), this, SLOT(OnBtnOpenImgClicked()));
    Act_NavtImg = new QAction(TCSR("显示原图"), this);
    connect(Act_NavtImg, SIGNAL(triggered()), this, SLOT(OnBtnNavtImgClicked()));
    Act_gray = new QAction(TCSR("去色(灰度)"), this);
    connect(Act_gray, SIGNAL(triggered()), this, SLOT(OnBtnGrayClicked()));
    Act_blackw = new QAction(TCSR("阈值化"), this);
    connect(Act_blackw, SIGNAL(triggered()), this, SLOT(OnBtnBlackwClicked()));
    Act_Equalize = new QAction(TCSR("直方图均衡化"), this);
    connect(Act_Equalize, SIGNAL(triggered()), this, SLOT(OnBtnEqualizeClicked()));
    Act_erode = new QAction(TCSR("腐蚀"), this);
    Act_dilate = new QAction(TCSR("膨胀"), this);
    Act_opend = new QAction(TCSR("开运算"), this);
    Act_closed = new QAction(TCSR("闭运算"), this);
    Act_Canny = new QAction(TCSR("Canny"), this);
    Act_Face = new QAction(TCSR("人脸检测"), this);
    connect(Act_Face, SIGNAL(triggered()), this, SLOT(OnBtnFaceClicked()));
    Act_Pep = new QAction(TCSR("行人检测"), this);
    Act_Speed = new QAction(TCSR("限速牌识别"), this);
    connect(Act_Speed, SIGNAL(triggered()), this, SLOT(OnBtnSpeedClicked()));
    Act_Car = new QAction(TCSR("车牌识别"), this);
    Act_Num = new QAction(TCSR("数字识别"), this);
    Act_CutScreen = new QAction(TCSR("截屏"), this);
    connect(Act_CutScreen, SIGNAL(triggered()), this, SLOT(OnBtnCutScreenClicked()));
    Act_Video = new QAction(TCSR("摄像头检测"), this);
    Act_Video->setCheckable(true);
    Act_Video->setChecked(false);
    connect(Act_Video, SIGNAL(triggered()), this, SLOT(OnBtnVideoClicked()));
    Act_Avi = new QAction(TCSR("播放视频"), this);
    Act_Avi->setCheckable(true);
    Act_Avi->setChecked(false);
    connect(Act_Avi, SIGNAL(triggered()), this, SLOT(OnBtnAviClicked()));
}

void WndOpenCV::contextMenuEvent(QContextMenuEvent* ev)
{
    QCursor cur = this->cursor();
    //创建主菜单
    QMenu* menu = new QMenu(this);
    menu->setFont(Menuft);
    menu->addAction(Act_OpenImg);
    menu->addAction(Act_NavtImg);
    menu->addSeparator();
    QMenu* hist = new QMenu(TCSR("直方图处理..."), menu);
    hist->setFont(Menuft);
    menu->addMenu(hist);
    hist->addAction(Act_gray);
    hist->addAction(Act_blackw);
    hist->addAction(Act_Equalize);
    QMenu* morph = new QMenu(TCSR("形态学变化..."), menu);
    morph->setFont(Menuft);
    menu->addMenu(morph);
    morph->addAction(Act_erode);
    morph->addAction(Act_dilate);
    morph->addAction(Act_opend);
    morph->addAction(Act_closed);
    QMenu* edge = new QMenu(TCSR("边缘检测..."), menu);
    edge->setFont(QFont("Courier", 10));
    menu->addMenu(edge);
    edge->addAction(Act_Canny);
    QMenu* disting = new QMenu(TCSR("目标识别..."), menu);
    disting->setFont(Menuft);
    menu->addMenu(disting);
    disting->addAction(Act_Face);
    disting->addAction(Act_Pep);
    disting->addAction(Act_Speed);
    disting->addAction(Act_Car);
    disting->addAction(Act_Num);
    QMenu* feature = new QMenu(TCSR("特征算子..."), menu);
    feature->setFont(Menuft);
    menu->addMenu(feature);
    menu->addSeparator();
    menu->addAction(Act_CutScreen);
    menu->addAction(Act_Video);
    menu->addAction(Act_Avi);
    //关联到光标
    menu->exec(cur.pos());
    ev->accept();
}

void WndOpenCV::paintEvent(QPaintEvent* ev)
{
    if (videoopend == 0)
        return ;
    QImage image2 = QImage((uchar*)(Outimage.data), Outimage.cols, Outimage.rows, QImage::Format_RGB888);
    image2 = image2.scaled(ImageShow->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ImageShow->setPixmap(QPixmap::fromImage(image2));
    //ImageShow->resize(image2.size());
    ImageShow->show();
    ev->accept();
}

void WndOpenCV::MatImgShow(const Mat& _MatImg)
{
    if (_MatImg.data)
    {
        QImage img;
        Mat TmpMat = _MatImg.clone();
        if (TmpMat.channels() == 1)   //单通道数据
            img = QImage((const unsigned char*)(TmpMat.data), TmpMat.cols, TmpMat.rows, QImage::Format_Indexed8);
        else
        {
            //多通道彩色图
            cv::cvtColor(_MatImg, TmpMat, CV_BGR2RGB);       // change color channel ordering
            img = QImage((const unsigned char*)(TmpMat.data), TmpMat.cols, TmpMat.rows, QImage::Format_RGB888);
        }
        //图像缩放
        //img =img.scaled(ImageShow->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        ImageShow->setPixmap(QPixmap::fromImage(img));
        ImageShow->resize(img.size());
    }
}

void WndOpenCV::OnBtnOpenImgClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.bmp)"));
    Srcimage = cv::imread(fileName.toAscii().data());
    //cv::resize(Srcimage,Srcimage,Size(600, 400));
    Srcimage.copyTo(Outimage);
    MatImgShow(Outimage);
}

void WndOpenCV::OnBtnNavtImgClicked()
{
    Srcimage.copyTo(Outimage);
    MatImgShow(Outimage);
}

void WndOpenCV::OnBtnGrayClicked()
{
    Op.GetGrayMat(Outimage, Outimage);
    MatImgShow(Outimage);
}

void WndOpenCV::OnBtnBlackwClicked()
{
    Op.Binarization(Outimage, Outimage);
    MatImgShow(Outimage);
}

void WndOpenCV::OnBtnEqualizeClicked()
{
    Op.ColorHistEqualize(Outimage, Outimage);
    MatImgShow(Outimage);
}

void WndOpenCV::OnBtnFaceClicked()
{
    Op.FaceTestMat(Outimage, Outimage);
    MatImgShow(Outimage);
}

void WndOpenCV::OnBtnPepClicked()
{
    Op.PepTestMat(Outimage);
    MatImgShow(Outimage);
}

void WndOpenCV::OnBtnSpeedClicked()
{
    Op.SpeedTestMat(Outimage, Outimage);
    MatImgShow(Outimage);
}

void WndOpenCV::OnBtnCutScreenClicked()
{
    //截屏并显示在标签中
    QPixmap screen = QPixmap::grabWindow(QApplication::desktop()->winId());
    //screen.save("../myDrawing3/screen.png");
    //QLabel *label = new QLabel(this);
    //label->resize(400, 200);
    QPixmap pix = screen.scaled(ImageShow->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ImageShow->setPixmap(pix);
}

void WndOpenCV::OnBtnVideoClicked()
{
    if (!Act_Video->isChecked())
    {
        videoopend = 0;
        cap.release();
    }
    else
    {
        if (cap.open(0))
        {
            videoopend = 1;
            Outimage = Mat::zeros(cap.get(CV_CAP_PROP_FRAME_HEIGHT), cap.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC3);
            theTimer.start(20);
        }
        else
        {
            videoopend = 0;
            cap.release();
        }
    }
}

void WndOpenCV::OnBtnAviClicked()
{
    if (!Act_Avi->isChecked())
    {
        cap.release();
        videoopend = 0;
    }
    else
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Video"), ".", tr("Image Files (*.Avi *.wmv *.mp4)"));
        if (cap.open(fileName.toAscii().data()))
        {
            videoopend = 1;
            Outimage = Mat::zeros(cap.get(CV_CAP_PROP_FRAME_HEIGHT), cap.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC3);
            theTimer.start(1000);
        }
        else
        {
            cap.release();
            videoopend = 0;
            Act_Avi->setChecked(false);
        }
    }
}

void WndOpenCV::updateImage()
{
    cap >> Outimage;
    if (Outimage.data)
    {
        cvtColor(Outimage, Outimage, CV_BGR2RGB);    //Qt中支持的是RGB图像, OpenCV中支持的是BGR
        this->update();  //发送刷新消息
    }
}











