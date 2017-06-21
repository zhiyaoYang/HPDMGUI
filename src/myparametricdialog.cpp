#include "myparametricdialog.h"
#include "mymainwindow.h"

#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>

#include <QDebug>

extern component *dummy;


myParametricDialog::myParametricDialog()
{
    initialize();
}

void myParametricDialog::addClicked()
{

}

void myParametricDialog::removeClicked()
{

}

void myParametricDialog::runClicked()
{
    if(validCheck()){

    }
}

void myParametricDialog::cancelClicked()
{
    reject();
}

void myParametricDialog::nRunChanged(int i)
{
    qDebug()<<"run count changed to "<<i;
}

void myParametricDialog::componentChanged(QString s)
{
    vpListComboBox->clear();
    if(s!="select component"){
        QStringList vpList;
        component * iterator = dummy, *myComp;
        bool found = false;
        while((iterator->next!=NULL)&&(!found)){
            iterator = iterator->next;
            if(iterator->getCompName()==s){
                found = true;
                myComp = iterator;
            }
        }
        foreach(parameter p, myComp->myPar){
            vpList.append("P:"+p.name);
        }
        foreach(variable v, myComp->myVar){
            vpList.append("V:"+v.name);
        }

        vpListComboBox->addItems(vpList);
    }
}

void myParametricDialog::initialize()
{
    createOptButtonGroupBox();

    createDialogButtonGroupBox();

    createTableGroupBox();

    QGridLayout *gLayout = new QGridLayout;

    gLayout->addWidget(tableGroupBox,0,0);
    gLayout->addWidget(optButtonGroupBox,0,1);
    gLayout->addWidget(dialogButtonGroupBox,1,0,2,1);

    setLayout(gLayout);

}

void myParametricDialog::createOptButtonGroupBox()
{

    optButtonGroupBox = new QGroupBox;

    nRunLabel = new QLabel(tr("Number of Runs:"));
    nRunSpinBox = new QSpinBox;
    componentLabel = new QLabel(tr("Component:"));
    compListComboBox = new QComboBox;
    vpLabel = new QLabel(tr("Parameter/Variable:"));
    vpListComboBox = new QComboBox;
    addButton = new QPushButton(tr("Add to Table"));
    removeBotton = new QPushButton(tr("Remove from Table"));

    QVBoxLayout * vLayout = new QVBoxLayout;

    vLayout->addWidget(nRunLabel);
    vLayout->addWidget(nRunSpinBox);
    vLayout->addWidget(componentLabel);
    vLayout->addWidget(compListComboBox);
    vLayout->addWidget(vpLabel);
    vLayout->addWidget(vpListComboBox);
    vLayout->addWidget(addButton);
    vLayout->addWidget(removeBotton);

    QSpacerItem * sItem = new QSpacerItem(0,0,QSizePolicy::Maximum,QSizePolicy::Expanding);

    vLayout->addItem(sItem);

    optButtonGroupBox->setLayout(vLayout);

    //add list of components into the compListComboBox

    QStringList compList;
    compList.append("select component");
    component * iterator = dummy;
    while(iterator->next!=NULL){
        iterator = iterator->next;
        compList.append(iterator->getCompName());
    }

    compListComboBox->addItems(compList);

    //connect signal-slot
    connect(compListComboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(componentChanged(QString)));
    connect(nRunSpinBox,SIGNAL(valueChanged(int)),this,SLOT(nRunChanged(int)));
    connect(addButton,SIGNAL(clicked(bool)),this,SLOT(addClicked()));
    connect(removeBotton,SIGNAL(clicked(bool)),this,SLOT(removeClicked()));

}

void myParametricDialog::createTableGroupBox()
{
    tableGroupBox = new QGroupBox;

    paraTable = new QTableWidget;

    QHBoxLayout * hLayout = new QHBoxLayout;

    hLayout->addWidget(paraTable);

    tableGroupBox->setLayout(hLayout);
}

void myParametricDialog::createDialogButtonGroupBox()
{
    dialogButtonGroupBox = new QGroupBox;

    runButton = new QPushButton(tr("Run"));
    cancelButton = new QPushButton(tr("Cancel"));

    QHBoxLayout * hLayout = new QHBoxLayout;

    QSpacerItem * sItem = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Maximum);

    hLayout->addItem(sItem);
    hLayout->addWidget(runButton);
    hLayout->addWidget(cancelButton);

    dialogButtonGroupBox->setLayout(hLayout);

    //connect signal-slot
    connect(runButton,SIGNAL(clicked(bool)),this,SLOT(runClicked()));
    connect(cancelButton,SIGNAL(clicked(bool)),this,SLOT(cancelClicked()));
}

bool myParametricDialog::validCheck()
{

}
