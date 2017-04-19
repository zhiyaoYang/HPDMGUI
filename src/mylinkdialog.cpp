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
    myComp1 = myLink->getComp1();
    myComp2 = myLink->getComp2();


    for(int i = 0; i < myComp1->myVar.count();i++){
        vars1<<myComp1->myVar.at(i).name;
    }

    for(int i = 0; i < myComp2->myVar.count();i++){
        vars2<<myComp2->myVar.at(i).name;
    }

    for(int i = 0; i < myComp1->myPar.count();i++){
        pars1<<myComp1->myPar.at(i).name;
    }

    for(int i = 0; i < myComp2->myPar.count();i++){
        pars2<<myComp2->myPar.at(i).name;
    }
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
    QMessageBox *mb = new QMessageBox("Reset the links",
                                      "Discard current changes and reset all links?",
                                      QMessageBox::Information,
                                      QMessageBox::Yes,
                                      QMessageBox::Cancel,
                                      QMessageBox::NoButton);
    if(mb->exec()==QMessageBox::Yes){
        readData();
    }

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

void myLinkDialog::vp1ComboChanged(QString str)
{
    if(str=="Variable"){
        iterationMemberCombobox1->clear();
        iterationMemberCombobox1->insertItems(0,vars1);
    }
    else{
        iterationMemberCombobox1->clear();
        iterationMemberCombobox1->insertItems(0,pars1);
    }
}

void myLinkDialog::vp2ComboChanged(QString str)
{
    if(str=="Parameter"){
        iterationMemberCombobox2->clear();
        iterationMemberCombobox2->insertItems(0,vars2);
    }
    else{
        iterationMemberCombobox2->clear();
        iterationMemberCombobox2->insertItems(0,pars2);
    }
}

void myLinkDialog::addStream()
{
    QString fluid;
    if(streamSubstanceCombobox->currentText()=="Coolant"){
        fluid = "c";
    }
    else if(streamSubstanceCombobox->currentText()=="Air"){
        fluid = "a";
    }
    else if(streamSubstanceCombobox->currentText()=="Refrigerant"){
        fluid = "s";
    }

    bool sameFluid, sameFromPort, sameToPort;
    component* fromComp = myComp1, *toComp= myComp2;
    int fromPort = streamIOSpinBox1->value(), toPort = streamIOSpinBox2->value(), temp;
    if(streamDirectionButton->text()=="<<<<<"){
        fromComp = myComp2;
        toComp = myComp1;
        temp = fromPort;
        fromPort = toPort;
        toPort = temp;
    }

    for(int i = 0; i < streamTable->rowCount();i++){
        sameFluid = (dynamic_cast<QComboBox*>(streamTable->cellWidget(i,0))->currentText() == fluid);
        sameFromPort = streamTable->item(i,1)->text()==fromComp->getCompName()
                &&dynamic_cast<QSpinBox*>(streamTable->cellWidget(i,2))->value()== fromPort;
        sameToPort = streamTable->item(i,3)->text()==toComp->getCompName()
                &&dynamic_cast<QSpinBox*>(streamTable->cellWidget(i,4))->value()== toPort;
        if(/*sameFluid||*/sameFromPort||sameToPort){
            QMessageBox *mb = new QMessageBox;
            mb->setText("The new stream is in coflict to existing stream!");
            mb->exec();
            return;
        }
    }

    int index = streamTable->rowCount();
    streamTable->insertRow(index);

    QComboBox*combo = new QComboBox;
    QStringList list(QStringList()<<"s"<<"a"<<"c");
    combo->insertItems(0,list);
    combo->setCurrentText(fluid);
    streamTable->setCellWidget(index,0,combo);

    QTableWidgetItem* item;
    item = new QTableWidgetItem;
    item->setData(Qt::DisplayRole,fromComp->getCompName());
    item->setTextAlignment(Qt::AlignCenter);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    streamTable->setItem(index,1,item);

    QSpinBox * sBox;
    sBox = new QSpinBox;
    sBox->setValue(fromPort);
    streamTable->setCellWidget(index,2,sBox);

    item = new QTableWidgetItem;
    item->setData(Qt::DisplayRole,toComp->getCompName());
    item->setTextAlignment(Qt::AlignCenter);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    streamTable->setItem(index,3,item);

    sBox = new QSpinBox;
    sBox->setValue(toPort);
    streamTable->setCellWidget(index,4,sBox);

    item = new QTableWidgetItem;
    item->setData(Qt::DisplayRole,"add description");
    item->setTextAlignment(Qt::AlignCenter);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
    streamTable->setItem(index,5,item);

}

void myLinkDialog::removeStream()
{

}

void myLinkDialog::reverseStream()
{

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
    streamTable->setColumnCount(6);
    QStringList streamHeader;
    streamHeader<<"Fluid"<<"From Component"<<"Outlet#"<<"To Component"<<"Inlet#"<<"Description";
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
    connect(streamAddButton,SIGNAL(clicked(bool)),this,SLOT(addStream()));
    connect(streamRemoveButton,SIGNAL(clicked(bool)),this,SLOT(removeStream()));
    connect(streamReverseButton,SIGNAL(clicked(bool)),this,SLOT(reverseStream()));

}

void myLinkDialog::createVariableGroupBox()
{
    //create and initiate items
    variableGroupBox = new QGroupBox;
    variableCompLabel1 = new QLabel(QString::number(myComp1->getIndex())+"-"+myComp1->getCompName());
    variableCompLabel2 = new QLabel(QString::number(myComp2->getIndex())+"-"+myComp2->getCompName());

    variableMemberCombobox1 = new QComboBox;
    variableMemberCombobox2 = new QComboBox;

    variableMemberCombobox1->insertItems(0,vars1);
    variableMemberCombobox2->insertItems(0,vars2);


    variableAddButton = new QPushButton(tr("Add"));
    variableRemoveButton = new QPushButton(tr("Remove"));

    variableTable = new QTableWidget;
    variableTable->setColumnCount(5);
    QStringList variableHeader;
    variableHeader<<"Component"<<"Member"<<"Component"<<"Member"<<"Description";
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
    iterationVPCombobox1->insertItem(0,"Variable");
    iterationVPCombobox1->insertItem(0,"Parameter");
    iterationVPCombobox2= new QComboBox;
    iterationVPCombobox2->insertItem(0,"Variable");
    iterationVPCombobox2->insertItem(0,"Parameter");
    iterationAddButton = new QPushButton(tr("Add"));
    iterationRemoveButton = new QPushButton(tr("Remove"));
    iterationReverseButton = new QPushButton(tr("Reverse"));

    iterationMemberCombobox1 = new QComboBox;
    iterationMemberCombobox2 = new QComboBox;

    iterationTable = new QTableWidget;
    iterationTable->setColumnCount(7);
    QStringList iterationHeader;
    iterationHeader<<"From Component"<<"V/P"<<"Name"<<"To Component"<<"V/P"<<"Name"<<"Description";
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

    connect(iterationVPCombobox1,SIGNAL(currentTextChanged(QString)),this,SLOT(vp1ComboChanged(QString)));
    connect(iterationVPCombobox2,SIGNAL(currentTextChanged(QString)),this,SLOT(vp2ComboChanged(QString)));


    iterationVPCombobox1->setCurrentIndex(0);
    iterationVPCombobox2->setCurrentIndex(0);
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

    readData();


}

void myLinkDialog::readData()
{
    //todo: load data from system into the dialog

    qDebug()<<"data read";
    loadStreamTable();
    loadVariableTable();
    loadIterationTable();

}

void myLinkDialog::loadStreamTable()
{
    streamTable->clearContents();

    QComboBox *combo = NULL;
    QSpinBox *sBox = NULL;
    QStringList fluids;
    fluids<<"s"<<"a"<<"c";

    QTableWidgetItem *fromComp = NULL, *toComp = NULL, *description = NULL;
    streamLink str;
    if(!myLink->myStream.isEmpty()){
        streamTable->setRowCount(myLink->myStream.count());
        for(int i = 0; i < myLink->myStream.count();i++){

            str = myLink->myStream.at(i);

            combo = new QComboBox;
            combo->insertItems(0,fluids);
            combo->setCurrentText(str.type);
            streamTable->setCellWidget(i,0,combo);

            fromComp = new QTableWidgetItem;
            fromComp->setData(Qt::DisplayRole,str.fromComp->getCompName());
            fromComp->setTextAlignment(Qt::AlignCenter);
            fromComp->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            streamTable->setItem(i,1,fromComp);


            sBox = new QSpinBox;
            sBox->setValue(str.fromPortNum);
            streamTable->setCellWidget(i,2,sBox);

            toComp = new QTableWidgetItem;
            toComp->setData(Qt::DisplayRole,str.toComp->getCompName());
            toComp->setTextAlignment(Qt::AlignCenter);
            toComp->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            streamTable->setItem(i,3,toComp);

            sBox = new QSpinBox;
            sBox->setValue(str.toPortNum);
            streamTable->setCellWidget(i,4,sBox);

            description = new QTableWidgetItem;
            description->setData(Qt::DisplayRole,str.description);
            description->setTextAlignment(Qt::AlignCenter);
            description->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
            streamTable->setItem(i,5,description);
        }
    }

}

void myLinkDialog::loadVariableTable()
{
    variableTable->clearContents();

    QComboBox *combo = NULL;
    QStringList types;
    types<<"V"<<"P";

    QTableWidgetItem *comp1 = NULL, *member1 = NULL, *comp2 = NULL, *member2 = NULL, *description = NULL;
    varLink var;
    if(!myLink->myVar.isEmpty()){
        variableTable->setRowCount(myLink->myVar.count());
        for(int i = 0; i < myLink->myVar.count();i++){

            var = myLink->myVar.at(i);

            comp1 = new QTableWidgetItem;
            comp1->setData(Qt::DisplayRole,var.fromComp->getCompName());
            comp1->setTextAlignment(Qt::AlignCenter);
            comp1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            variableTable->setItem(i,0,comp1);

            member1 = new QTableWidgetItem;
            member1->setData(Qt::DisplayRole,var.fromVarNum);
            member1->setTextAlignment(Qt::AlignCenter);
            member1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            variableTable->setItem(i,1,member1);

            comp2 = new QTableWidgetItem;
            comp2->setData(Qt::DisplayRole,var.toComp->getCompName());
            comp2->setTextAlignment(Qt::AlignCenter);
            comp2->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            variableTable->setItem(i,2,comp2);

            member2 = new QTableWidgetItem;
            member2->setData(Qt::DisplayRole,var.toVarNum);
            member2->setTextAlignment(Qt::AlignCenter);
            member2->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            variableTable->setItem(i,3,member2);

            description = new QTableWidgetItem;
            description->setData(Qt::DisplayRole,var.description);
            description->setTextAlignment(Qt::AlignCenter);
            description->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
            variableTable->setItem(i,4,description);
        }
    }
}

void myLinkDialog::loadIterationTable()
{
    iterationTable->clearContents();

    QComboBox *combo = NULL;
    QStringList types;
    types<<"Variable"<<"Parameter";

    QTableWidgetItem *comp1 = NULL, *member1 = NULL, *comp2 = NULL, *member2 = NULL, *description = NULL;
    varLink var;
    if(!myLink->mySuccessive.isEmpty()){
        variableTable->setRowCount(myLink->myVar.count());
        for(int i = 0; i < myLink->myVar.count();i++){

            var = myLink->myVar.at(i);


            comp1 = new QTableWidgetItem;
            comp1->setData(Qt::DisplayRole,var.fromComp->getCompName());
            comp1->setTextAlignment(Qt::AlignCenter);
            comp1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            variableTable->setItem(i,0,comp1);

            combo = new QComboBox;
            combo->insertItems(0,types);
            combo->setCurrentText("Variable");
            variableTable->setCellWidget(i,1,combo);

            member1 = new QTableWidgetItem;
            member1->setData(Qt::DisplayRole,var.fromVarNum);
            member1->setTextAlignment(Qt::AlignCenter);
            member1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            variableTable->setItem(i,2,member1);

            comp2 = new QTableWidgetItem;
            comp2->setData(Qt::DisplayRole,var.toComp->getCompName());
            comp2->setTextAlignment(Qt::AlignCenter);
            comp2->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            variableTable->setItem(i,3,comp2);


            combo = new QComboBox;
            combo->insertItems(0,types);
            combo->setCurrentText("Variable");
            variableTable->setCellWidget(i,4,combo);

            member2 = new QTableWidgetItem;
            member2->setData(Qt::DisplayRole,var.toVarNum);
            member2->setTextAlignment(Qt::AlignCenter);
            member2->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            variableTable->setItem(i,5,member2);

            description = new QTableWidgetItem;
            description->setData(Qt::DisplayRole,var.description);
            description->setTextAlignment(Qt::AlignCenter);
            description->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
            variableTable->setItem(i,6,description);
        }
    }
}

