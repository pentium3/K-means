#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstdio>
using namespace std;
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

void writeln()
{
    for (int i=1;i<=k;i++)
    {
        printf("%.5f %d\n",d[i][0],nm[i]);
        for (int j=1;j<=nm[i];j++)
            printf("%.5f ",d[i][j]);
        printf("\n");
    }
    for (int i=1;i<=n;i++)  cout<<v[i]<<" ";
    printf("\n \n");
}

void write_finalresult()
{
    printf("This is the final result \n");
    for (int i=1;i<=k;i++)
    {
        printf("%.5f %d\n",d[i][0],nm[i]);
        for (int j=1;j<=nm[i];j++)
            printf("%.5f ",d[i][j]);
        printf("\n");
    }
    for (int i=1;i<=n;i++)  cout<<v[i]<<" ";
    printf("\n \n");
}

bool goon()
{
    double tmp=0;
    for (int i=1;i<=k;i++)
        tmp+=fabs(pred[i]-d[i][0]);
    for (int i=1;i<=k;i++)
        pred[i]=d[i][0];
    printf("goon? >> %.5lf\n",fabs(tmp));
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

int main()
{
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);

    cin>>k>>n;
    for (int i=1; i<=n; i++)
        cin>>a[i];          //the data of element i

    cout<<k<<" "<<n<<endl;

    for (int i=1; i<=k; i++)
    {
        d[i][0]=random();   // the center data of grade i
        pred[i]=0;          // d[i][0] last time
        nm[i]=0;            // the number of elements in grade i
    }

    writeln();
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
        writeln();
    }

    write_finalresult();

    return 0;
}


