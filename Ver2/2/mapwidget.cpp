#include "mapwidget.h"
#include "stditem.h"
#include <QSlider>
#include <QGridLayout>
#include <QFile>
#include <QTextStream>
#include <QGraphicsScene>
#include <math.h>

#define MAXGRD 50
#define MAXNUM 10000
#define DATARANGE 100
#define MINZOOM 1

double mimm=DATARANGE;
const double MINNUM=1;
double d[MAXGRD+2][MAXNUM+2];
double pred[MAXGRD+2];
double a[MAXNUM+2];
int v[MAXNUM+2];
int k,n;
int nm[MAXGRD+2];

void write_finalresult()
{
    qDebug("This is the final result \n");
    for (int i=1;i<=k;i++)
    {
        qDebug("%.5f %d\n",d[i][0],nm[i]);
    //    for (int j=1;j<=nm[i];j++)
    //        qDebug("%.5f ",d[i][j]);
    //    qDebug("\n");
    }
    //for (int i=1;i<=n;i++)  //cout<<v[i]<<" ";
    //    qDebug("%d ",v[i]);
    qDebug("\n \n");
}

double _random()
{
    return DATARANGE*(double)rand()/(double)RAND_MAX;
}

int MINk(int k,double x)        //找出中心值离元素x最近的一类
{
    double mn=fabs(x-d[1][0]);
    int mni=1;
    for (int i=2; i<=k; i++)
    {
        if (fabs(x-d[i][0])<mn)
        {
            mn=fabs(x-d[i][0]);
            mni=i;
        }
    }
    return mni;
}

bool goon()             //检测相邻两次处理的结果的误差是否小于某阈值MINNUM
{
    double tmp=0;
    for (int i=1;i<=k;i++)
        tmp+=fabs(pred[i]-d[i][0]);
    for (int i=1;i<=k;i++)
        pred[i]=d[i][0];
    for (int i=1;i<=k;i++)
        if (nm[k]==0)   return true;
    if (fabs(tmp)<MINNUM)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void calculate()
{
    for (int i=1; i<=k; i++)
    {
        d[i][0]=_random();   // the center data of grade i
        pred[i]=0;          // d[i][0] last time
        nm[i]=0;            // the number of elements in grade i
    }
    while (goon())
    {
        for (int i=1;i<=k;i++)  nm[i]=0;

        for (int i=1; i<=n; i++)
        {
            int tm=MINk(k,a[i]);
            v[i]=tm;            //the grade of element i
            nm[tm]++;
            d[tm][nm[tm]]=a[i];
        }
        for (int i=1; i<=k; i++)
        {
            double sum=0;
            for (int j=1; j<=nm[i]; j++)
                sum+=d[i][j];
            if (nm[i]==0)   d[i][0]=_random();
                else d[i][0]=sum/nm[i];
        }
    }
    write_finalresult();
}

StdItem::StdItem(double D)
{
    dat=D;
}

void StdItem::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    painter->setPen(QPen (Qt::black,2));
    QFont font("ARPL KaitiM GB",12);
    font.setBold(true);
    painter->setFont(font);
    painter->drawText((dat-mimm)*7/2,(dat-mimm)*7/2,QString::number(dat));      //GraphicItem坐标系
}

MapWidget::MapWidget()
{
    readMap();
    calculate();
    zoom=MINZOOM;

    QGraphicsScene *scene = new QGraphicsScene(this);       //scene场景类，用来放置所有元素
    setScene(scene);

    QSlider *slider = new QSlider;          //滚动条
    slider->setOrientation(Qt::Vertical);
    slider->setRange(1,100);
    slider->setTickInterval(10);
    slider->setValue(MINZOOM);
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(slotZoom(int)));

    //垂直布局
    QVBoxLayout *zoomLayout = new QVBoxLayout;
    zoomLayout->addWidget(slider);

    //水平布局
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(zoomLayout);
    mainLayout->addStretch();
    mainLayout->setMargin(30);
    mainLayout->setSpacing(10);
    setLayout(mainLayout);

    setWindowTitle("K-means Demo");
    setMinimumSize(800,600);

    QPen ps[100];
    QPen ipen(Qt::black,3);
    for (int i=1;i<=k;i++)
        ps[i]=QPen ((enum Qt::GlobalColor)(i+5),1);

    for (int i=1;i<=n;i++)
        if (a[i]<mimm)  mimm=a[i];
    mimm-=2;        //所有学生中的最低分（确定圆的半径用）

    for (int i=1;i<=k;i++)
    {
        for (int j=1;j<=nm[i];j++)
            scene->addEllipse((-(d[i][j]-mimm)*10/2),-((d[i][j]-mimm)*10/2),(d[i][j]-mimm)*10,(d[i][j]-mimm)*10,ps[i]);
        scene->addEllipse(-((d[i][0]-mimm)*10/2),-((d[i][0]-mimm)*10/2),(d[i][0]-mimm)*10,(d[i][0]-mimm)*10,ipen);
        //addEllipse：画圆，以矩形QRect为边界。        QRect(x,y,w,h)：以点(x,y)为左上角，w为宽，h为高
        //QGraphicScene坐标系
        qDebug("score: %.5f  %.5f\n",d[i][0],d[i][0]-mimm);
    }
    for (int i=1;i<=k;i++)
    {
        StdItem *stditem=new StdItem(d[i][0]);     //写成绩数字
        scene->addItem(stditem);
    }
}

void MapWidget::readMap()
{
    QFile File("in.txt");
    int ok = File.open(QIODevice::ReadOnly);
    if(ok)
    {
        QTextStream ts(&File);
        ts>>k>>n;
        for (int i=1;i<=n;i++)
            ts>>a[i];
        File.close();
    }
}

void MapWidget::slotZoom(int value)   	//缩放
{
    qreal s;
    if(value>zoom)                 		//放大
    {
        s=pow(1.03,(value-zoom));
    }
    else                             	//缩小
    {
        s=pow(1/1.03,(zoom-value));
    }
    scale(s,s);
    zoom = value;
}

