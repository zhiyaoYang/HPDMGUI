#include "mylinkdialog.h"
#include "mymainwindow.h"

#include <QDebug>
#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>



myLinkDialog::myLinkDialog(link *mlink)
{
    myLink = mlink;

    initialize();
}

myLinkDialog::~myLinkDialog()
{

}

void myLinkDialog::doneClicked()
{
    //todo: perform validity check first
    //todo: update system with data input in each text edits
    accept();

}

void myLinkDialog::resetClicked()
{
    //todo: overwrite text edits with data from system (same as when opening dialog)
//    readData();

    qDebug()<<myLink->getComp1();
    qDebug()<<myLink->getComp2();
}

void myLinkDialog::createStreamGroupBox()
{
    streamGroupBox = new QGroupBox(tr("Stream"));
    QGridLayout *layout = new QGridLayout;

    streamLabel1 = new QLabel(tr("streamLabel1"));
    streamLabel2 = new QLabel(tr("streamLabel2"));
    streamComboBox1 = new QComboBox;
    streamComboBox1->insertItem(0,"streamCB1");
    streamComboBox2 = new QComboBox;
    streamComboBox2->insertItem(0,"streamCB2");
    streamLineEdit1 = new QLineEdit(tr("streamLE1"));
    streamLineEdit2 = new QLineEdit(tr("streamLE2"));
//    streamList1 = new QListWidget;
//    streamList2 = new QListWidget;


//    QStringList streams1;
//    streams1<<"stream1-1"<<"stream1-2"<<"stream1-3";

//    QStringList streams2;
//    streams2<<"stream2-1"<<"stream2-2"<<"stream2-3";


//    streamList1->insertItems(0,streams1);
//    streamList2->insertItems(0,streams2);


    layout->addWidget(streamLabel1,0,0);
    layout->addWidget(streamLabel2,0,1);
    layout->addWidget(streamComboBox1,1,0);
    layout->addWidget(streamComboBox2,1,1);
//    layout->addWidget(streamList1,0,2);
//    layout->addWidget(streamList2,1,2);
    layout->addWidget(streamLineEdit1,3,0);
    layout->addWidget(streamLineEdit2,3,1);

    streamGroupBox->setLayout(layout);

}

void myLinkDialog::createParameterGroupBox()
{
    parameterGroupBox = new QGroupBox(tr("parameter"));
    QGridLayout *layout = new QGridLayout;

    parameterLabel1 = new QLabel(tr("parameterLabel1"));
    parameterLabel2 = new QLabel(tr("parameterLabel2"));
    parameterComboBox1 = new QComboBox;
    parameterComboBox1->insertItem(0,"parameterCB1");
    parameterComboBox2 = new QComboBox;
    parameterComboBox2->insertItem(0,"parameterCB2");
    parameterLineEdit1 = new QLineEdit("parameterLE1");
    parameterLineEdit2 = new QLineEdit("parameterLE2");
//    parameterList1 = new QListWidget;
//    parameterList2 = new QListWidget;


//    QStringList parameters1;
//    parameters1<<"parameter1-1"<<"parameter1-2"<<"parameter1-3";

//    QStringList parameters2;
//    parameters2<<"parameter2-1"<<"parameter2-2"<<"parameter2-3";


//    parameterList1->insertItems(0,parameters1);
//    parameterList2->insertItems(0,parameters2);


    layout->addWidget(parameterLabel1,0,0);
    layout->addWidget(parameterLabel2,0,1);
    layout->addWidget(parameterComboBox1,1,0);
    layout->addWidget(parameterComboBox2,1,1);
//    layout->addWidget(parameterList1,0,2);
//    layout->addWidget(parameterList2,1,2);
    layout->addWidget(parameterLineEdit1,3,0);
    layout->addWidget(parameterLineEdit2,3,1);

    parameterGroupBox->setLayout(layout);

}

void myLinkDialog::createIterationGroupBox()
{
    iterationGroupBox = new QGroupBox(tr("iteration"));
    QGridLayout *layout = new QGridLayout;

    iterationLabel1 = new QLabel(tr("iterationLabel1"));
    iterationLabel2 = new QLabel(tr("iterationLabel2"));
    iterationComboBox1 = new QComboBox;
    iterationComboBox1->insertItem(0,"iterationCB1");
    iterationComboBox2 = new QComboBox;
    iterationComboBox2->insertItem(0,"iterationCB2");
    iterationLineEdit1 = new QLineEdit(tr("iterationLE1"));
    iterationLineEdit2 = new QLineEdit(tr("iterationLE2"));
//    iterationList1 = new QListWidget;
//    iterationList2 = new QListWidget;


//    QStringList iterations1;
//    iterations1<<"iteration1-1"<<"iteration1-2"<<"iteration1-3";

//    QStringList iterations2;
//    iterations2<<"iteration2-1"<<"iteration2-2"<<"iteration2-3";


//    iterationList1->insertItems(0,iterations1);
//    iterationList2->insertItems(0,iterations2);


    layout->addWidget(iterationLabel1,0,0);
    layout->addWidget(iterationLabel2,0,1);
    layout->addWidget(iterationComboBox1,1,0);
    layout->addWidget(iterationComboBox2,1,1);
//    layout->addWidget(iterationList1,0,2);
//    layout->addWidget(iterationList2,1,2);
    layout->addWidget(iterationLineEdit1,3,0);
    layout->addWidget(iterationLineEdit2,3,1);

    iterationGroupBox->setLayout(layout);

}

void myLinkDialog::createButtonGroupBox()
{
    buttonGroupBox = new QGroupBox;
    QHBoxLayout *layout = new QHBoxLayout;

    doneButton = new QPushButton(tr("Done"));
    resetButton = new QPushButton(tr("Reset"));
    cancelButton = new QPushButton(tr("Cancel"));

    QSpacerItem *sItem = new QSpacerItem(0,0,QSizePolicy::Minimum,QSizePolicy::Expanding);
    layout->addItem(sItem);
    layout->addWidget(doneButton);
    layout->addWidget(resetButton);
    layout->addWidget(cancelButton);
    buttonGroupBox->setLayout(layout);

    connect(doneButton,SIGNAL(clicked(bool)),this,SLOT(doneClicked()));
    connect(resetButton,SIGNAL(clicked(bool)),this,SLOT(resetClicked()));
    connect(cancelButton,SIGNAL(clicked(bool)),this,SLOT(reject()));
}

void myLinkDialog::initialize()
{
    createStreamGroupBox();
    createParameterGroupBox();
    createIterationGroupBox();
    createButtonGroupBox();

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(streamGroupBox);
    mainLayout->addWidget(parameterGroupBox);
    mainLayout->addWidget(iterationGroupBox);
    mainLayout->addWidget(buttonGroupBox);
    setLayout(mainLayout);

    setWindowFlags(Qt::Tool);
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle(tr("Link Settings"));


    readData();

}

void myLinkDialog::readData()
{
    //todo: load data from system into the dialog
    streamLabel1->setText("Component1 info"
                          +QString::number(myLink->getComp1()->getIndex())
                          +"-"+myLink->getComp1()->getCompName());
    streamLabel2->setText("Component2 info"
                          +QString::number(myLink->getComp2()->getIndex())
                          +"-"+myLink->getComp2()->getCompName());

    qDebug()<<"data read";

}

