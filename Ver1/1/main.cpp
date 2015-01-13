#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <cmath>
#include <cstdlib>

#define MAXGRD 50
#define MAXNUM 10000
#define DATARANGE 100

const double MINNUM=1;
double d[MAXGRD+2][MAXNUM+2];
double pred[MAXGRD+2];
double a[MAXNUM+2];
int v[MAXNUM+2];
int k,n;
int nm[MAXGRD+2];

double random()
{
    return DATARANGE*(double)rand()/(double)RAND_MAX;
}

int MINk(int k,double x)
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
/*
void writeln()
{
    for (int i=1;i<=k;i++)
    {
        QDebug("%.5f %d\n",d[i][0],nm[i]);
        for (int j=1;j<=nm[i];j++)
            QDebug("%.5f ",d[i][j]);
        QDebug("\n");
    }
    for (int i=1;i<=n;i++)  cout<<v[i]<<" ";
    QDebug("\n \n");
}
*/
void write_finalresult()
{
    qDebug("This is the final result \n");
    for (int i=1;i<=k;i++)
    {
        qDebug("%.5f %d\n",d[i][0],nm[i]);
        for (int j=1;j<=nm[i];j++)
            qDebug("%.5f ",d[i][j]);
        qDebug("\n");
    }
    for (int i=1;i<=n;i++)  //cout<<v[i]<<" ";
        qDebug("%d ",v[i]);
    qDebug("\n \n");
}

bool goon()
{
    double tmp=0;
    for (int i=1;i<=k;i++)
        tmp+=fabs(pred[i]-d[i][0]);
    for (int i=1;i<=k;i++)
        pred[i]=d[i][0];
    qDebug("goon? >> %.5lf\n",fabs(tmp));
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

class MyMainWindow:public QWidget
{
public:
 MyMainWindow(QWidget *parent = 0);
 private:
 void paintEvent(QPaintEvent *);
 QPainter *paint;
};

void MyMainWindow::paintEvent(QPaintEvent *)
//paintEvent函数由系统自动调用，用不着我们人为的去调用。
{
 paint=new QPainter;
 paint->begin(this);
  //paint->drawPoint(100,100);

 paint->setPen(QPen(Qt::blue,4,Qt::DashLine)); //设置画笔形式
 for (int i=k+1;i<=k+n;i++)
     paint->drawPoint(i,(int)(a[i-k]));

 paint->setPen(QPen(Qt::red,4,Qt::DashLine)); //设置画笔形式
 for (int i=1;i<=k;i++)
 {
     int tm=(int)(d[i][0]);
     paint->drawPoint(i,tm);
 }

 paint->end();
}

MyMainWindow::MyMainWindow(QWidget *parent):QWidget(parent)
{
 setGeometry(100,100,400,400);
}

int main(int argc,char **argv)
{
    /*
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);

    cin>>k>>n;
    for (int i=1; i<=n; i++)
        cin>>a[i];          //the data of element i
    */
    QFile file("in.txt");
    if (!file.open(QIODevice::ReadOnly))    return 0;
    QTextStream S(&file);
    S>>k>>n;
    for (int i=1;i<=n;i++)
        S>>a[i];
    file.close();

    /*
    QFile file("out.txt");
    if (!file.open(IO_WriteOnly))   return;
    QTextStream S(&file);
    S<<k<<" "<<n<<endl;
         S.RealNumberPre
    */

    for (int i=1; i<=k; i++)
    {
        d[i][0]=random();   // the center data of grade i
        pred[i]=0;          // d[i][0] last time
        nm[i]=0;            // the number of elements in grade i
    }

    //writeln();
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
            if (nm[i]==0)   d[i][0]=random();
                else d[i][0]=sum/nm[i];
        }
        //writeln();
    }

    write_finalresult();


 QApplication aa(argc,argv);
 MyMainWindow w;
 w.show();
 return aa.exec();
}
