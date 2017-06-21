#include "myparametricdialog.h"

#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>

#include <QDebug>

myParametricDialog::myParametricDialog()
{
    
}

void myParametricDialog::initialize()
{
    optButtonGroupBox = new QGroupBox;
    QVBoxLayout * optButtonGBLayout = new QVBoxLayout;
    
    nRunLabel = new QLabel(tr("Number of Runs:"));
    nRunSpinBox = new QSpinBox;
    compListComboBox = new QComboBox;
    vpListComboBox = new QComboBox;
    addButton = new QPushButton(tr("Add to Table"));
    removeBotton = new QPushButton(tr("Remove from Table"));

    //add list of components into the compListComboBox

    connect(compListComboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(componentChanged(QString)));
    connect(nRunSpinBox,SIGNAL(valueChanged(int)),this,SLOT(nRunChanged(int)));
    connect(addButton,SIGNAL(clicked(bool)),this,SLOT(addClicked()));
    connect(removeBotton,SIGNAL(clicked(bool)),this,SLOT(removeClicked()));
}
