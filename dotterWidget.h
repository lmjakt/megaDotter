#ifndef DOTTERWIDGET_H
#define DOTTERWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPoint>
#include <vector>
#include "dotter.h"
#include "indexDotter.h"   // but this only for the struct point (very ugly)

class DotterWidget : public QWidget
{
    Q_OBJECT
	public :
	DotterWidget(QWidget* parent=0);
    ~DotterWidget();
    void setData(Dotter* dot);
    void setData(std::vector<coord_set>& a, std::vector<coord_set>& b, unsigned int a_length, unsigned int b_length);
    void setScaleAndBias(float s, float b);
    void saveImage(const char* fname);

    signals :
	void markPosition(int, int);
    void currentPosition(int, int);

    private :
	void paintEvent(QPaintEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mousePressEvent(QMouseEvent* e);

    void paintArea(QPainter* p);
    void paintPoints(QPainter* p, unsigned int w, unsigned int h);
    void prepareImage(unsigned int w, unsigned int h);
    void incrementImage(unsigned char* data, unsigned int w, unsigned int h, std::vector<coord_set>& pts, int offset, unsigned int mult);
    QPoint translatePosition(const QPoint& p);  // translate from widget position to sequence position.. 
    Dotter* dotter;  // contains the data to plot..
    float scale, bias;  // calculate by *bias + scale, but where bias is usually set to negative values
    std::vector<coord_set> redPoints;
    std::vector<coord_set> bluePoints;
    unsigned int comp_height, comp_width;
    QImage qimage;
    unsigned char* imageData;
    bool rePaint;
};

#endif
