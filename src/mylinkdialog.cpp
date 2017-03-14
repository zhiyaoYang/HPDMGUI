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

}

void myLinkDialog::streamDirectionFlipped()
{
    streamRightDirection = (!streamRightDirection);
    if(streamRightDirection){
        streamDirectionButton->setText(">>>>>");
    }
    else{
        streamDirectionButton->setText("<<<<<");
    }
}

void myLinkDialog::iterationDirectionFlipped()
{
    iterationRightDirection = (!iterationRightDirection);
    if(iterationRightDirection){
        iterationDirectionButton->setText(">>>>>");
    }
    else{
        iterationDirectionButton->setText("<<<<<");
    }

}

void myLinkDialog::createStreamGroupBox()
{
    //create and initiate items
    streamGroupBox = new QGroupBox;
    streamCompLabel1 = new QLabel(QString::number(myComp1->getIndex())+"-"+myComp1->getCompName());
    streamCompLabel2 = new QLabel(QString::number(myComp2->getIndex())+"-"+myComp2->getCompName());
    streamDirectionButton = new QPushButton(">>>>>");
    streamSubstanceCombobox = new QComboBox;
    streamSubstanceCombobox->insertItem(0,"Coolant");
    streamSubstanceCombobox->insertItem(0,"Air");
    streamSubstanceCombobox->insertItem(0,"Refrigerant");
    streamIOLabel1 = new QLabel("Inlet #");
    streamIOLabel2 = new QLabel("Outlet #");
    streamIOSpinBox1 = new QSpinBox;
    streamIOSpinBox2 = new QSpinBox;
    streamAddButton = new QPushButton(tr("Add"));
    streamRemoveButton = new QPushButton(tr("Remove"));
    streamReverseButton = new QPushButton(tr("Reverse"));

    streamTable = new QTableWidget;
    streamTable->setColumnCount(5);
    QStringList streamHeader;
    streamHeader<<"Fluid"<<"From Component"<<"Outlet#"<<"To Component"<<"Inlet#";
    streamTable->setHorizontalHeaderLabels(streamHeader);
    streamTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


    //setting layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *firstRowLayout = new QHBoxLayout;
    QHBoxLayout *thirdRowLayout = new QHBoxLayout;
    QHBoxLayout *buttonRowLayout = new QHBoxLayout;

    firstRowLayout->addWidget(streamCompLabel1);
    firstRowLayout->addWidget(streamDirectionButton);
    firstRowLayout->addWidget(streamCompLabel2);

    thirdRowLayout->addWidget(streamIOLabel1);
    thirdRowLayout->addWidget(streamIOSpinBox1);
    thirdRowLayout->addWidget(streamIOLabel2);
    thirdRowLayout->addWidget(streamIOSpinBox2);

    buttonRowLayout->insertStretch(0);
    buttonRowLayout->addWidget(streamAddButton);
    buttonRowLayout->addWidget(streamRemoveButton);
    buttonRowLayout->addWidget(streamReverseButton);

    mainLayout->addLayout(firstRowLayout);
    mainLayout->addWidget(streamSubstanceCombobox);
    mainLayout->addLayout(thirdRowLayout);
    mainLayout->addWidget(streamTable);
    mainLayout->addLayout(buttonRowLayout);

    streamGroupBox->setLayout(mainLayout);

    connect(streamDirectionButton,SIGNAL(clicked(bool)),this,SLOT(streamDirectionFlipped()));

}

void myLinkDialog::createVariableGroupBox()
{
    //create and initiate items
    variableGroupBox = new QGroupBox;
    variableCompLabel1 = new QLabel(QString::number(myComp1->getIndex())+"-"+myComp1->getCompName());
    variableCompLabel2 = new QLabel(QString::number(myComp2->getIndex())+"-"+myComp2->getCompName());
    variableVPCombobox1= new QComboBox;
    variableVPCombobox1->insertItem(0,"Variable");
    variableVPCombobox1->insertItem(0,"Parameter");
    variableVPCombobox2= new QComboBox;
    variableVPCombobox2->insertItem(0,"Variable");
    variableVPCombobox2->insertItem(0,"Parameter");
    variableMemberCombobox1 = new QComboBox;
    variableMemberCombobox2 = new QComboBox;
    variableAddButton = new QPushButton(tr("Add"));
    variableRemoveButton = new QPushButton(tr("Remove"));

    variableTable = new QTableWidget;
    variableTable->setColumnCount(5);
    QStringList variableHeader;
    variableHeader<<"Type"<<"Component"<<"Member"<<"Component"<<"Member";
    variableTable->setHorizontalHeaderLabels(variableHeader);
    variableTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


    //setting layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *firstRowLayout = new QHBoxLayout;
    QHBoxLayout *secondRowLayout = new QHBoxLayout;
    QHBoxLayout *thirdRowLayout = new QHBoxLayout;
    QHBoxLayout *buttonRowLayout = new QHBoxLayout;

    firstRowLayout->addWidget(variableCompLabel1);
    firstRowLayout->addWidget(variableCompLabel2);
    secondRowLayout->addWidget(variableVPCombobox1);
    secondRowLayout->addWidget(variableVPCombobox2);
    thirdRowLayout->addWidget(variableMemberCombobox1);
    thirdRowLayout->addWidget(variableMemberCombobox2);

    buttonRowLayout->insertStretch(0);
    buttonRowLayout->addWidget(variableAddButton);
    buttonRowLayout->addWidget(variableRemoveButton);

    mainLayout->addLayout(firstRowLayout);
    mainLayout->addLayout(secondRowLayout);
    mainLayout->addLayout(thirdRowLayout);
    mainLayout->addWidget(variableTable);
    mainLayout->addLayout(buttonRowLayout);

    variableGroupBox->setLayout(mainLayout);

}

void myLinkDialog::createIterationGroupBox()
{
    //create and initiate items
    iterationGroupBox = new QGroupBox;
    iterationCompLabel1 = new QLabel(QString::number(myComp1->getIndex())+"-"+myComp1->getCompName());
    iterationCompLabel2 = new QLabel(QString::number(myComp2->getIndex())+"-"+myComp2->getCompName());
    iterationDirectionButton = new QPushButton(">>>>>");
    iterationVPCombobox1= new QComboBox;
    iterationVPCombobox1->insertItem(0,"iteration");
    iterationVPCombobox1->insertItem(0,"Parameter");
    iterationVPCombobox2= new QComboBox;
    iterationVPCombobox2->insertItem(0,"iteration");
    iterationVPCombobox2->insertItem(0,"Parameter");
    iterationMemberCombobox1 = new QComboBox;
    iterationMemberCombobox2 = new QComboBox;
    iterationAddButton = new QPushButton(tr("Add"));
    iterationRemoveButton = new QPushButton(tr("Remove"));
    iterationReverseButton = new QPushButton(tr("Reverse"));

    iterationTable = new QTableWidget;
    iterationTable->setColumnCount(6);
    QStringList iterationHeader;
    iterationHeader<<"From Component"<<"V/P"<<"Name"<<"To Component"<<"V/P"<<"Name";
    iterationTable->setHorizontalHeaderLabels(iterationHeader);
    iterationTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


    //setting layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *firstRowLayout = new QHBoxLayout;
    QHBoxLayout *secondRowLayout = new QHBoxLayout;
    QHBoxLayout *thirdRowLayout = new QHBoxLayout;
    QHBoxLayout *buttonRowLayout = new QHBoxLayout;

    firstRowLayout->addWidget(iterationCompLabel1);
    firstRowLayout->addWidget(iterationDirectionButton);
    firstRowLayout->addWidget(iterationCompLabel2);
    secondRowLayout->addWidget(iterationVPCombobox1);
    secondRowLayout->addWidget(iterationVPCombobox2);
    thirdRowLayout->addWidget(iterationMemberCombobox1);
    thirdRowLayout->addWidget(iterationMemberCombobox2);

    buttonRowLayout->insertStretch(0);
    buttonRowLayout->addWidget(iterationAddButton);
    buttonRowLayout->addWidget(iterationRemoveButton);

    mainLayout->addLayout(firstRowLayout);
    mainLayout->addLayout(secondRowLayout);
    mainLayout->addLayout(thirdRowLayout);
    mainLayout->addWidget(iterationTable);
    mainLayout->addLayout(buttonRowLayout);

    iterationGroupBox->setLayout(mainLayout);

    connect(iterationDirectionButton,SIGNAL(clicked(bool)),this,SLOT(iterationDirectionFlipped()));
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
    layout->insertStretch(0);
    buttonGroupBox->setLayout(layout);

    connect(doneButton,SIGNAL(clicked(bool)),this,SLOT(doneClicked()));
    connect(resetButton,SIGNAL(clicked(bool)),this,SLOT(resetClicked()));
    connect(cancelButton,SIGNAL(clicked(bool)),this,SLOT(reject()));
}

void myLinkDialog::initialize()
{

    streamRightDirection = true;
    iterationRightDirection = true;

    readData();

    tabWidget = new QTabWidget;
    createStreamGroupBox();
    createVariableGroupBox();
    createIterationGroupBox();
    createButtonGroupBox();

    tabWidget->addTab(streamGroupBox,tr("Stream"));
    tabWidget->addTab(variableGroupBox,tr("Variable"));
    tabWidget->addTab(iterationGroupBox,tr("Successive"));


    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonGroupBox);

    setLayout(mainLayout);

    setWindowFlags(Qt::Tool);
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle(tr("Link Settings"));



}

void myLinkDialog::readData()
{
    //todo: load data from system into the dialog

    qDebug()<<"data read";

    myComp1 = myLink->getComp1();
    myComp2 = myLink->getComp2();

}

