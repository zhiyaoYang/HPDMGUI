#include "mycompdialog.h"
#include "mymainwindow.h"

#include <QDebug>
#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QGridLayout>

extern component *dummy;

myCompDialog::myCompDialog(component *comp)
{
    myComponent = comp;

    initialize();
}

void myCompDialog::createInfoGroupBox()
{
    infoGroupBox = new QGroupBox;
    QGridLayout *layout = new QGridLayout;

    infoIndexLabel = new QLabel;
    infoNameLabel = new QLabel(tr("Component Name: "));
    infoNameLineEdit = new QLineEdit;
    infoDescriptionLabel = new QLabel(tr("Component Description: "));
    infoDescriptionLineEdit = new QLineEdit;

    infoPicLabel = new QLabel(tr("Pic"));
    //todo: set picture

    layout->addWidget(infoIndexLabel,0,0);
    layout->addWidget(infoNameLabel,1,0);
    layout->addWidget(infoNameLineEdit,2,0);
    layout->addWidget(infoDescriptionLabel,3,0);
    layout->addWidget(infoDescriptionLineEdit,4,0);
    layout->addWidget(infoPicLabel,0,1,5,1);
    infoGroupBox->setLayout(layout);


}

void myCompDialog::createParameterGroupBox()
{
    parameterGroupBox = new QGroupBox;

    parameterTable = new QTableWidget;
    parameterTable->setColumnCount(4);
    QStringList parameterHeader;
    parameterHeader<<"Index"<<"Name"<<"Value"<<"Description";
    parameterTable->setHorizontalHeaderLabels(parameterHeader);
    parameterTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


    QVBoxLayout *layout = new QVBoxLayout;

    layout->addWidget(parameterTable);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    parameterResetButton = new QPushButton("Reset Parameters");

    QSpacerItem *sItem = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding);
    buttonLayout->addItem(sItem);
    buttonLayout->addWidget(parameterResetButton);

    layout->addLayout(buttonLayout);

    parameterGroupBox->setLayout(layout);

    connect(parameterResetButton,SIGNAL(clicked(bool)),this,SLOT(parameterResetClicked()));

}

void myCompDialog::createVariableGroupBox()
{
    variableGroupBox = new QGroupBox;

    variableTable = new QTableWidget;
    variableTable->setColumnCount(6);
    QStringList variableHeader;
    variableHeader<<"Index"<<"Name"<<"Value"<<"Description"<<"Type"<<"Enable";
    variableTable->setHorizontalHeaderLabels(variableHeader);
    variableTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


    QVBoxLayout *layout = new QVBoxLayout;

    layout->addWidget(variableTable);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    variableResetButton = new QPushButton("Reset Variables");
    variableHideButton = new QPushButton("Hide Empty");
    variableEmptyHidden = true;

    QSpacerItem *sItem = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding);
    buttonLayout->addItem(sItem);
    buttonLayout->addWidget(variableResetButton);
    buttonLayout->addWidget(variableHideButton);

    layout->addLayout(buttonLayout);

    variableGroupBox->setLayout(layout);

    connect(variableResetButton,SIGNAL(clicked(bool)),this,SLOT(variableResetClicked()));
    connect(variableHideButton,SIGNAL(clicked(bool)),this,SLOT(variableHideToggled()));

}

void myCompDialog::createButtonGroupBox()
{
    buttonGroupBox = new QGroupBox;
    QHBoxLayout *layout = new QHBoxLayout;

    doneButton = new QPushButton(tr("Done"));
    cancelButton = new QPushButton(tr("Cancel"));

    QSpacerItem *sItem = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout->addItem(sItem);
    layout->addWidget(doneButton);
    layout->addWidget(cancelButton);
    buttonGroupBox->setLayout(layout);

    connect(doneButton,SIGNAL(clicked(bool)),this, SLOT(doneClicked()));
    connect(cancelButton,SIGNAL(clicked(bool)),this,SLOT(reject()));

}

void myCompDialog::doneClicked()
{
    //todo: perform validity check first
    //todo: update system with data input in each text edits
    accept();
}

void myCompDialog::variableResetClicked()
{
    //todo: replace blanks with original variables
    qDebug()<<"reset var";
}

void myCompDialog::parameterResetClicked()
{
    //todo: replace blanks with original parameters
    qDebug()<<"reset par";
}

void myCompDialog::variableHideToggled()
{
    variableEmptyHidden = (!variableEmptyHidden);
    if(variableEmptyHidden){
        qDebug()<<"show empty!";
        variableHideButton->setText("Hide Empty");
    }
    else{
        qDebug()<<"hide empty!";
        variableHideButton->setText("Show Empty");
    }
}


void myCompDialog::initialize()
{

    tabWidget = new QTabWidget;
    createInfoGroupBox();
    createParameterGroupBox();
    createVariableGroupBox();
    createButtonGroupBox();
    tabWidget->addTab(parameterGroupBox,tr("Parameters"));
    tabWidget->addTab(variableGroupBox,tr("Variables"));

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(infoGroupBox);
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonGroupBox);
    setLayout(mainLayout);

    setWindowFlags(Qt::Tool);
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle(tr("Component Settings"));

    readData();
}

void myCompDialog::readData()
{
    //todo: load data from system into the dialog
    qDebug()<<"data read";

    infoIndexLabel->setText("Component #"+QString::number(myComponent->getIndex()));
    infoNameLineEdit->setText(myComponent->getCompName());
    infoDescriptionLineEdit->setText(myComponent->getCompDescription());

    QPixmap pic("testComponent.png");
    pic = pic.scaled(200,200,Qt::KeepAspectRatio);
    infoPicLabel->setPixmap(pic);
}
