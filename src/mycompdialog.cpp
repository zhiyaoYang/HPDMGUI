#include "mycompdialog.h"
#include "mymainwindow.h"

#include <QDebug>
#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QGridLayout>

extern component *dummy;
extern myMainwindow* mWindow;

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
    parameterTable->setColumnCount(5);
    QStringList parameterHeader;
    parameterHeader<<"Index"<<"Name"<<"Value"<<"Description"<<"Type";
    parameterTable->setHorizontalHeaderLabels(parameterHeader);
    parameterTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    parameterTable->verticalHeader()->setVisible(false);

    parameterTable->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout;

    layout->addWidget(parameterTable);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    parameterResetButton = new QPushButton("Reset Parameters");

    QSpacerItem *sItem = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Maximum);
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
    variableTable->verticalHeader()->setVisible(false);

    variableTable->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout;

    layout->addWidget(variableTable);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    variableResetButton = new QPushButton("Reset Variables");
    variableHideButton = new QPushButton("Show Empty");
    variableEmptyHidden = true;

    QSpacerItem *sItem = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Maximum);
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
    showAllLinkButton = new QPushButton(tr("Show Related Links"));
    showAllLinkButton->setCheckable(true);

    QSpacerItem *sItem = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout->addItem(sItem);
    layout->addWidget(showAllLinkButton);
    layout->addWidget(doneButton);
    layout->addWidget(cancelButton);
    buttonGroupBox->setLayout(layout);

    buttonGroupBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    connect(doneButton,SIGNAL(clicked(bool)),this, SLOT(doneClicked()));
    connect(cancelButton,SIGNAL(clicked(bool)),this,SLOT(reject()));
    connect(showAllLinkButton,SIGNAL(toggled(bool)),this,SLOT(showAllLinkClicked(bool)));

}

void myCompDialog::doneClicked()
{
    //todo: perform validity check first

    myComponent->setCompName(infoNameLineEdit->text());
    myComponent->setCompDescription(infoDescriptionLineEdit->text());

    QTableWidgetItem * item = NULL;
    QComboBox * box = NULL;

    parameter par;
    for(int i = 0; i < parameterTable->rowCount();i++){

        par = myComponent->findPar(i);

        item = parameterTable->item(i,1);
        par.name = item->text();
        item = parameterTable->item(i,2);
        par.value = item->text();
        item = parameterTable->item(i,3);
        par.description = item->text();
        box = dynamic_cast<QComboBox*>(parameterTable->cellWidget(i,4));
        par.iORd = box->currentText();

        myComponent->myPar.replace(i,par);

    }

    variable var;
    for(int i = 0; i < variableTable->rowCount();i++){

        var = myComponent->findVar(i);

        item = variableTable->item(i,1);
        var.name = item->text();
        item = variableTable->item(i,2);
        var.value = item->text();
        item = variableTable->item(i,3);
        var.description = item->text();
        box = dynamic_cast<QComboBox*>(variableTable->cellWidget(i,4));
        var.solvingSetting = box->currentText();
        box = dynamic_cast<QComboBox*>(variableTable->cellWidget(i,5));
        var.enabled = box->currentText();

        myComponent->myVar.replace(i,var);
    }

    updateComponentLabels();

    accept();
}

void myCompDialog::variableResetClicked()
{
    QMessageBox *mb = new QMessageBox("Reset Variables",
                                      "Discard current changes and reset variables?",
                                      QMessageBox::Information,
                                      QMessageBox::Yes,
                                      QMessageBox::Cancel,
                                      QMessageBox::NoButton);
    if(mb->exec()==QMessageBox::Yes){
        loadVariableTable();
    }
}

void myCompDialog::parameterResetClicked()
{
    QMessageBox *mb = new QMessageBox("Reset Parameters",
                                      "Discard current changes and reset parameters?",
                                      QMessageBox::Information,
                                      QMessageBox::Yes,
                                      QMessageBox::Cancel,
                                      QMessageBox::NoButton);
    if(mb->exec()==QMessageBox::Yes){
        loadParameterTable();
    }
}

void myCompDialog::variableHideToggled()
{

    if(variableEmptyHidden){
        showEmptyVar();
    }
    else{
        hideEmptyVar();
    }

    variableEmptyHidden = (!variableEmptyHidden);
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
    hideEmptyVar();
}

void myCompDialog::readData()
{
    infoIndexLabel->setText("Component #"+QString::number(myComponent->getIndex()));
    infoNameLineEdit->setText(myComponent->getCompName());
    infoDescriptionLineEdit->setText(myComponent->getCompDescription());


    QPixmap pic("compPic/illustrations/Component"+QString::number(myComponent->getTypeIndex())+".png");

    pic = pic.scaled(200,200,Qt::KeepAspectRatio);
    infoPicLabel->setPixmap(pic);

    loadParameterTable();
    loadVariableTable();
}

void myCompDialog::loadParameterTable()
{
    parameterTable->clearContents();

    QTableWidgetItem *pIndex = NULL, *pName= NULL, *pValue= NULL, *pDescription= NULL;
    QComboBox * combo = NULL;
    QStringList types;
    types<<"s"<<"d"<<"i";
    parameter par;


    if(!myComponent->myPar.isEmpty()){

        parameterTable->setRowCount(myComponent->myPar.count());

        for(int i = 0; i < myComponent->myPar.count(); i++){
            par = myComponent->myPar.at(i);

            pIndex = new QTableWidgetItem;
            pIndex->setData(Qt::DisplayRole,par.index);
            pIndex->setTextAlignment(Qt::AlignCenter);
            pIndex->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            parameterTable->setItem(i,0,pIndex);

            pName = new QTableWidgetItem;
            pName->setData(Qt::DisplayRole,par.name);
            pName->setTextAlignment(Qt::AlignCenter);
            pName->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
            parameterTable->setItem(i,1,pName);

            pValue = new QTableWidgetItem;
            pValue->setData(Qt::DisplayRole,par.value);
            pValue->setTextAlignment(Qt::AlignCenter);
            pValue->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
            parameterTable->setItem(i,2,pValue);

            pDescription = new QTableWidgetItem;
            pDescription->setData(Qt::DisplayRole,par.description);
            pDescription->setTextAlignment(Qt::AlignLeft);
            pDescription->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
            parameterTable->setItem(i,3,pDescription);

            combo = new QComboBox;
            combo->insertItems(0,types);
            combo->setCurrentText(par.iORd);
            parameterTable->setCellWidget(i,4,combo);
        }
    }
}

void myCompDialog::loadVariableTable()
{
    variableTable->clearContents();
    QComboBox * combo = NULL;
    QStringList types;
    types<<"g"<<"r"<<"i"<<"o"<<"t";
    QStringList yn;
    yn<<"y"<<"n";


    QTableWidgetItem *vIndex = NULL, *vName= NULL, *vValue= NULL, *vDescription= NULL;
    variable var;
    if(!myComponent->myVar.isEmpty()){

        variableTable->setRowCount(myComponent->myVar.count());

        for(int i = 0; i < myComponent->myVar.count(); i++){

            var = myComponent->myVar.at(i);

            vIndex = new QTableWidgetItem;
            vIndex->setData(Qt::DisplayRole,var.index);
            vIndex->setTextAlignment(Qt::AlignCenter);
            vIndex->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            variableTable->setItem(i,0,vIndex);

            vName = new QTableWidgetItem;
            vName->setData(Qt::DisplayRole,var.name);
            vName->setTextAlignment(Qt::AlignCenter);
            vName->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
            variableTable->setItem(i,1,vName);

            vValue = new QTableWidgetItem;
            vValue->setData(Qt::DisplayRole,var.value);
            vValue->setTextAlignment(Qt::AlignCenter);
            vValue->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
            variableTable->setItem(i,2,vValue);

            vDescription = new QTableWidgetItem;
            vDescription->setData(Qt::DisplayRole,var.description);
            vDescription->setTextAlignment(Qt::AlignCenter);
            vDescription->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
            variableTable->setItem(i,3,vDescription);

            combo = new QComboBox;
            combo->insertItems(0,types);
            combo->setCurrentText(var.solvingSetting);
            variableTable->setCellWidget(i,4,combo);

            combo = new QComboBox;
            combo->insertItems(0,yn);
            combo->setCurrentText(var.enabled);
            variableTable->setCellWidget(i,5,combo);

        }
    }

}

void myCompDialog::hideEmptyVar()
{
    QTableWidgetItem* item = NULL;
    for(int i = 0; i < variableTable->rowCount();i++){
        item = variableTable->item(i,2);
        if(item->text()==""){
            variableTable->hideRow(i);
        }
    }
    variableHideButton->setText("Show Empty");
}

void myCompDialog::showEmptyVar()
{
    for(int i = 0; i < variableTable->rowCount();i++){
        variableTable->showRow(i);
    }
    variableHideButton->setText("Hide Empty");
}

void myCompDialog::updateComponentLabels()
{
    myComponent->componentLabel->setText(myComponent->getCompName());
}

void myCompDialog::showAllLinkClicked(bool show)
{
    mWindow->enableLink(false);
    foreach(link* l, myComponent->myLinks){
        l->setVisible(show);
    }
}
