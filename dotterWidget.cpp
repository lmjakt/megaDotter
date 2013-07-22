#include "dotterWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QImage>
#include <iostream>

using namespace std;

DotterWidget::DotterWidget(QWidget* parent)
    : QWidget(parent)
{
    dotter = 0;
    imageData = 0;
    setPalette(QPalette(QColor(0, 0, 0), QColor(255, 255, 255)));
    scale = 1;
    bias = 0;
    setMouseTracking(true);
    rePaint = true;
    // we might want to set some things here, but I can't be bothered
}

DotterWidget::~DotterWidget(){
    delete imageData;
}

void DotterWidget::setData(Dotter* dot){
    dotter = dot;
    resize(dotter->compWidth(), dotter->compHeight());  // which is the simple way of doing it..
    update();
}

void DotterWidget::setData(vector<coord_set>& a, vector<coord_set>& b, unsigned int a_length, unsigned int b_length){
    dotter = 0;
    redPoints = a;
    bluePoints = b;
    comp_width = a_length;
    comp_height = b_length;
    rePaint = true;
    cout << "and calling update from setData" << endl;
    repaint();
}

void DotterWidget::setScaleAndBias(float s, float b){
    scale = s;
    bias = b;
    update();
}

void DotterWidget::saveImage(const char* fname){
    cout << "trying to same the image" << endl;
    if(!qimage.width())
	prepareImage(width(), height());
    if(!qimage.save(fname, "jpg", 90))
	cerr << "unable to save the image to : " << fname << endl;
}

void DotterWidget::paintEvent(QPaintEvent* e){
    // essentially we'll be plotting a thingy.. 
    // assume that size is correct.
    // use red for forward and b for back..
    cout << "making a painter" << endl;
    QPainter p(this);
    
    if(dotter){
	paintArea(&p);
	return;
    }
    cout << "and calling pait points " << endl;
    paintPoints(&p, width(), height());
    cout << " can setting rePaint to false" << endl;
    rePaint = false;
}

void DotterWidget::paintArea(QPainter* p){
    if(!dotter)
	return;
    
    unsigned int w = dotter->compWidth();
    unsigned int h = dotter->compHeight();
    
    unsigned char* fw = dotter->f_area();
    unsigned char* rv = dotter->r_area();

    p->setBrush(Qt::NoBrush);
    for(uint y=0; y < h; ++y){
	for(uint x=0; x < w; ++x){
	    int r = int( scale * fw[ y * w + x] + (scale * bias));
	    int b = int( scale * rv[ y * w + x] + (scale * bias));
	    r = r > 255 ? 255 : r;
	    r = r < 0 ? 0 : r;
	    b = b > 255 ? 255 : b;
	    b = b < 0 ? 0 : b;
	    int g = 0;
	    p->setPen(QPen(QColor(r, g, b), 1));
	    p->drawPoint(x, y);
	}
    }
}

void DotterWidget::paintPoints(QPainter* p, unsigned int w, unsigned int h){
    p->setBrush(QColor(0, 0, 0));
    p->drawRect(0, 0, w, h);
    // qt4 doesn't do raster or operations. so I cannot just simply add the points myself.
    // bummer. However, I can make a qimage from a set of data so.. 
    if((int)w == qimage.width() && (int)h == qimage.height() && !rePaint){
	p->drawImage(0, 0, qimage);
	return;
    }
    prepareImage(w, h);
//     cout << "Just before deleting the old image data" << endl;
//     delete imageData; // this could give us a crash ? 
//     imageData = new unsigned char[w * h * 4]; // 32 bit aligned.. the first bit is alpha and so needs setting to 255
//     memset((void*)imageData, 0, w * h * 4);
//     for(uint i=0; i < w * h; ++i)
// 	imageData[i * 4 + 3] = 255;
//     cout << "made some new data and will go ahead and increment" << endl;
//     // the format is ARGB, so then we can go through our points.. 
//     incrementImage(imageData, w, h, redPoints, 2, 4);
//     incrementImage(imageData, w, h, bluePoints, 1, 4);
//     cout << "incremented can I make a new Qimage ? " << endl;
//     qimage = QImage(imageData, w, h, QImage::Format_ARGB32);
    p->drawImage(0, 0, qimage);
}

void DotterWidget::prepareImage(unsigned int w, unsigned int h){
    delete imageData; // this could give us a crash ? 
    imageData = new unsigned char[w * h * 4]; // 32 bit aligned.. the first bit is alpha and so needs setting to 255
    memset((void*)imageData, 0, w * h * 4);
    for(uint i=0; i < w * h; ++i)
	imageData[i * 4 + 3] = 255;
    cout << "made some new data and will go ahead and increment" << endl;
    // the format is ARGB, so then we can go through our points.. 
    incrementImage(imageData, w, h, redPoints, 2, 4);
    incrementImage(imageData, w, h, bluePoints, 1, 4);
    cout << "incremented can I make a new Qimage ? " << endl;
    qimage = QImage(imageData, w, h, QImage::Format_ARGB32);
}	
		       
void DotterWidget::incrementImage(unsigned char* data, unsigned int w, unsigned int h, vector<coord_set>& pts, int offset, unsigned int mult){
    cout << "incrementing image offset " << offset << "  and mult " << mult << "  and w " << w << "  h: " << h << endl;
    int l = w * h * mult + offset;  // the maximum position.. 
    if(!comp_height || !comp_width)
	return;
    vector<int>::iterator it1;
    vector<int>::iterator it2;
    long counter = 0;
    for(uint i=0; i < pts.size(); ++i){
	for(it1 = pts[i].a.begin(); it1 != pts[i].a.end(); it1++){
	    for(it2 = pts[i].b.begin(); it2 != pts[i].b.end(); it2++){
		int x = int( float(w) * float(*it1) / float(comp_width) ); // everything converted to float first to allow big numbers
		int y = int( float(h) * float(*it2) / float(comp_height) ); // everything converted to float first to allow big numbers
		int p = (y * w + x) * mult + offset;
		++counter;
		if(p < l){
		    data[p] = 255;
		}
	    }
	}
//	if(!(i % 10000)){
//	    cout << i << " / " << pts.size() << "  counter : " << counter << endl;
//	}
    }
    cout << endl;
}

void DotterWidget::mouseMoveEvent(QMouseEvent* e){
    QPoint p = translatePosition(e->pos());
    emit currentPosition(p.x(), p.y());
}

void DotterWidget::mousePressEvent(QMouseEvent* e){
    QPoint p = translatePosition(e->pos());
    emit markPosition(p.x(), p.y());
}

QPoint DotterWidget::translatePosition(const QPoint& p){
    int x = (comp_width * p.x()) / width();
    int y = (comp_height * p.y()) / height();
    return(QPoint(x, y));
}
