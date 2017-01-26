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
    infoGroupBox = new QGroupBox(tr("Info"));
    QGridLayout *layout = new QGridLayout;

    indexLabel = new QLabel;
    nameLabel = new QLabel(tr("Component Name: "));
    nameEdit = new QLineEdit;
    descriptionLabel = new QLabel(tr("Component Description: "));
    descriptionEdit = new QTextEdit;

    picLabel = new QLabel(tr("Pic"));
    //todo: set picture

    layout->addWidget(indexLabel,0,0);
    layout->addWidget(nameLabel,1,0);
    layout->addWidget(nameEdit,2,0);
    layout->addWidget(descriptionLabel,3,0);
    layout->addWidget(descriptionEdit,4,0);
    layout->addWidget(picLabel,0,1,4,1);
    infoGroupBox->setLayout(layout);


}

void myCompDialog::createPropertyGroupBox()
{
    propertyGroupBox = new QGroupBox(tr("Properties"));



    parameterLabel = new QLabel(tr("Parameters:"));
    variableLabel = new QLabel(tr("Variables:"));
    //todo: add label, text edit, combo box, radio buttons in grid layout for each entry


    QVBoxLayout *layout = new QVBoxLayout;

    layout->addWidget(parameterLabel);
    layout->addWidget(variableLabel);


    propertyGroupBox->setLayout(layout);

}

void myCompDialog::createButtonGroupBox()
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

    connect(doneButton,SIGNAL(clicked(bool)),this, SLOT(doneClicked()));
    connect(resetButton,SIGNAL(clicked(bool)),this,SLOT(resetClicked()));
    connect(cancelButton,SIGNAL(clicked(bool)),this,SLOT(reject()));

}

void myCompDialog::doneClicked()
{
    //todo: perform validity check first
    //todo: update system with data input in each text edits
    accept();
}

void myCompDialog::resetClicked()
{
    //todo: overwrite text edits with data from system (same as when opening dialog)
    readData();
}

void myCompDialog::initialize()
{

    createInfoGroupBox();
    createPropertyGroupBox();
    createButtonGroupBox();

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(infoGroupBox);
    mainLayout->addWidget(propertyGroupBox);
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
    indexLabel->setText("Component Index: "+QString::number(myComponent->getIndex()));
    nameEdit->setText(myComponent->getCompName());
    descriptionEdit->setText(myComponent->getCompDescription());

}
