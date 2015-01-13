#ifndef STDITEM
#define STDITEM

class StdItem : public QGraphicsItem
{
public:
    StdItem(double);
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    QRectF boundingRect() const     {return (QRectF(10,10,10,10));}
private:
    double dat;
};

#endif // STDITEM

