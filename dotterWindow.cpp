#include "dotterWindow.h"
#include <QLineEdit>
#include <QFileDialog>
#include <QPushButton>
#include <QLabel>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTextStream>
#include <QString>
#include <QImageWriter>
#include <QList>
#include <QByteArray>

using namespace std;

DotterWindow::DotterWindow(map<string, string> optArgs, QWidget* parent)
    : QWidget(parent)
{
    ///////
    // This is silly but to find out to what formats I can save images to.
    QImageWriter writer;
    QList<QByteArray> formats = writer.supportedImageFormats();
    for(int i=0; i < formats.size(); ++i)
	cout << i << "\t" << formats[i].data() << endl;
    ///////////////
    
    wSize = new QSpinBox(this);
    wSize->setRange(10, 255);
    stepSize = new QSpinBox(this);
    infoBox = new QTextEdit(this);
    nameBox = new QLineEdit(this);

    QLabel* wsLabel = new QLabel("W. Size", this);
    QLabel* stepLabel = new QLabel("S. Size", this);
    
    scaleBox = new QSpinBox(this);
    biasBox = new QSpinBox(this);
    QLabel* scaleLabel = new QLabel("Scale", this);
    QLabel* biasLabel = new QLabel("Bias", this);

    scaleBox->setRange(1, 40);
    biasBox->setRange(-255, 255);

    connect(scaleBox, SIGNAL(valueChanged(int)), this, SLOT(setScaleAndBias(int)) );
    connect(biasBox, SIGNAL(valueChanged(int)), this, SLOT(setScaleAndBias(int)) );

    dotter = new Dotter();
    dotterWidget = new DotterWidget();
    dotterWidget->resize(1024, 1024);

    connect(dotterWidget, SIGNAL(markPosition(int, int)), this, SLOT(markPosition(int, int)) );
    connect(dotterWidget, SIGNAL(currentPosition(int, int)), this, SLOT(currentPosition(int, int)) );

    QPushButton* loadButton = new QPushButton("Load File", this);
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadSequence()) );

    QPushButton* compareButton = new QPushButton("Compare", this);
    connect(compareButton, SIGNAL(clicked()), this, SLOT(doComparison()) );

    xpos = new QLabel("0", this);
    ypos = new QLabel("0", this);
    QLabel* posLabel = new QLabel("Position", this);

    // At this point lets check optional arguments..
    for(map<string, string>::iterator it=optArgs.begin(); it != optArgs.end(); it++){
	cout << (*it).first << " : " << (*it).second << endl;
    }
    if(optArgs.count("windowSize")){
	int ws = atoi(optArgs["windowSize"].c_str());
	if(ws)
	    wSize->setValue(ws);
    }

    QVBoxLayout* vbox = new QVBoxLayout(this);
    vbox->addWidget(infoBox);
    QHBoxLayout* loadBox = new QHBoxLayout();
    vbox->addLayout(loadBox);
    loadBox->addWidget(nameBox);
    loadBox->addWidget(loadButton);
    
    QGridLayout* grid = new QGridLayout();
    vbox->addLayout(grid);
    grid->addWidget(wsLabel, 0, 0);
    grid->addWidget(wSize, 0, 1);
    grid->addWidget(stepLabel, 0, 2);
    grid->addWidget(stepSize, 0, 3);
    grid->addWidget(compareButton, 1, 3);
    
    grid->addWidget(scaleLabel, 2, 0);
    grid->addWidget(scaleBox, 2, 1);
    grid->addWidget(biasLabel, 2, 2);
    grid->addWidget(biasBox, 2, 3);

    grid->addWidget(posLabel, 3, 0);
    grid->addWidget(xpos, 3, 1);
    grid->addWidget(ypos, 3, 2);
    if(optArgs.count("die")){
	loadSequence(optArgs["die"].c_str());
	doComparison();
	string oname = optArgs["die"] + ".jpg";
	dotterWidget->saveImage(oname.c_str());
	exit(0);
    }

}

void DotterWindow::loadSequence(){
    QString fname = QFileDialog::getOpenFileName(this);
    if(fname.isNull()){
	return;
    } 
    loadSequence(fname.toAscii().data());
}

void DotterWindow::loadSequence(const char* fname){
//     QString fname = QFileDialog::getOpenFileName(this);
//     if(fname.isNull()){
// 	return;
//     }

    FastaReader fr(fname);
//    FastaReader fr(fname.toAscii().data());
    sequences = fr.sequences();
    
    QString sData;
    QTextStream ts(&sData);
    for(uint i=0; i < sequences.size(); ++i){
	ts << i << "\t" << sequences[i].name.c_str() << "\t" << sequences[i].sequence.length() << "\n";
    }
    infoBox->append(sData);
    // also calculate the settings for step size.
    // if we make a maximal comparison of 1000 x 1000 then we have one million comparisons to do...
    // -- it's a reasonable limit because my screen is 1200 x 3600.
    // However, that would mean only one comparison per 1000 bp for a million vs a million...
    // which seems a trifle small. Well, maybe not too bad.. well, let's give it a try anyway..
    int maxWidth = 1200;
    if(sequences.size()){
	int minStep = sequences[0].sequence.length() / maxWidth;
	minStep = minStep < 1 ? 1 : minStep;
	int maxStep = minStep * 10;
	stepSize->setRange(minStep, maxStep);
	stepSize->setValue(minStep);
    }
}

void DotterWindow::setScaleAndBias(int b){
    b = 0; // avoids warnings.
    dotterWidget->setScaleAndBias(float(scaleBox->value()), float(biasBox->value()) );
}

// void DotterWindow::doComparison(){
//     if(sequences.size()){
// 	dotter->compareSeqs(sequences[0].sequence, sequences[0].sequence, 
// 			    (unsigned char)wSize->value(),
// 			    (unsigned int)stepSize->value() );
// 	dotterWidget->setData(dotter);
// 	dotterWidget->show();
//     }
// }

void DotterWindow::doComparison(){
    if(sequences.size()){
	IndexDotter idotter;
	cout << "Created an indexDotter and am calling selfCompare" << endl;
	idotter.selfCompare(sequences[0].sequence, (unsigned char)wSize->value());
	cout << "and am getting the red points from the comparer" << endl;
	vector<coord_set> rpoints = idotter.selfPoints();
	cout << "and then the rev comp points" << endl;
	vector<coord_set> bpoints = idotter.secondPoints();
	cout << "Got those so will see if I can get the painter to pain them" << endl;
	dotterWidget->setData(rpoints, bpoints, sequences[0].sequence.length(), sequences[0].sequence.length());
	dotterWidget->show();
    }
    cout << "And that's that.. " << endl;
}

void DotterWindow::currentPosition(int x, int y){
    QString lstring;
    lstring.setNum(x);
    xpos->setText(lstring);
    lstring.setNum(y);
    ypos->setText(lstring);
}

void DotterWindow::markPosition(int x, int y){
    QString label;
    QTextStream l(&label);
    l << "Position :\t" << x << "\t" << y;
    infoBox->append(label);
}
