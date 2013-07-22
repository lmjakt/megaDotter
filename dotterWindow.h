#ifndef DOTTERWINDOW_H
#define DOTTERWINDOW_H

#include "dotterWidget.h"
#include "fastaReader.h"
#include "dotter.h" 
#include "sequence.h"
#include "indexDotter.h"
#include <vector>
#include <map>
#include <string>
#include <QTextEdit>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>

class DotterWindow : public QWidget
{
    Q_OBJECT
	public :
	DotterWindow(std::map<std::string, std::string> optArgs, QWidget* parent=0);
    
    private slots:
	void loadSequence();
    void loadSequence(const char* fname);
    void doComparison();
    void setScaleAndBias(int b);
    void currentPosition(int x, int y);
    void markPosition(int x, int y);

 private:

    QSpinBox* wSize;
    QSpinBox* stepSize;
    QSpinBox* scaleBox;
    QSpinBox* biasBox;

    QTextEdit* infoBox;
    QLineEdit* nameBox;

    QLabel* xpos;
    QLabel* ypos;

    Dotter* dotter;
    DotterWidget* dotterWidget;
    std::vector<Sequence> sequences;
};
    

#endif
