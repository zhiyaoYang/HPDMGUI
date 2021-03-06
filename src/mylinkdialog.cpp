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

    //todo: check if there is still any link left
    int streamCount = 0, varCount = 0, iterCount = 0;
    streamCount = streamTable->rowCount();
    varCount = variableTable->rowCount();
    iterCount = iterationTable->rowCount();

    if(streamCount+varCount+iterCount==0){
        QMessageBox *mb = new QMessageBox("Delete the link",
                                          "There is no stream/variable/successive link between the two components\n"
                                          "Do you want to delete this link item?",
                                          QMessageBox::Information,
                                          QMessageBox::Yes,
                                          QMessageBox::Cancel,
                                          QMessageBox::NoButton);
        if(mb->exec()==QMessageBox::Yes){

            myComp1->myLinks.remove(myLink);
            myComp2->myLinks.remove(myLink);
            delete myLink;
            accept();
        }
        else{
            //stay in the dialog and do nothing
        }
    }
    else{
        //todo: update system with data input in each text edits

        myLink->myStream.clear();
        for(int i = 0; i < streamCount; i++){
            streamLink s;
            s.type = dynamic_cast<QComboBox*>(streamTable->cellWidget(i,0))->currentText();
            if(streamTable->item(i,1)->text()==myComp1->getCompName()){
                s.fromComp = myComp1;
                s.toComp = myComp2;
            }
            else{
                s.fromComp = myComp2;
                s.toComp = myComp1  ;
            }
            s.fromPortNum = dynamic_cast<QSpinBox*>(streamTable->cellWidget(i,2))->value();
            s.toPortNum = dynamic_cast<QSpinBox*>(streamTable->cellWidget(i,4))->value();
            s.description = streamTable->item(i,5)->text();

            myLink->myStream.append(s);
        }

        myLink->myVar.clear();
        for(int i = 0; i < varCount; i++){
            varLink v;
            if(variableTable->item(i,0)->text()==myComp1->getCompName()){
                v.fromComp = myComp1;
                v.toComp = myComp2;
            }
            else{
                v.fromComp = myComp2;
                v.toComp = myComp1  ;
            }

            QString varName;
            varName = variableTable->item(i,1)->text();
            for(int j = 0; j < myComp1->myVar.count();j++){
                if(varName == myComp1->myVar.at(j).name){
                    v.fromVarNum = j;
                    break;
                }
            }
            varName = variableTable->item(i,3)->text();
            for(int j = 0; j < myComp2->myVar.count();j++){
                if(varName == myComp2->myVar.at(j).name){
                    v.toVarNum = j;
                    break;
                }
            }
            v.description = variableTable->item(i,4)->text();
            myLink->myVar.append(v);
        }

        myLink->mySuccessive.clear();
        for(int i = 0; i < iterCount; i++){
            successiveLink s;
            if(iterationTable->item(i,0)->text()==myComp1->getCompName()){
                s.fromComp = myComp1;
                s.toComp = myComp2;
            }
            else{
                s.fromComp = myComp2;
                s.toComp = myComp1  ;
            }

            s.fromType = (iterationTable->item(i,1)->text()=="Variable")?"V":"P";
            s.toType = (iterationTable->item(i,4)->text()=="Variable")?"V":"P";


            QString varName;
            varName = iterationTable->item(i,2)->text();
            if(s.fromType=="V"){
                for(int j = 0; j < myComp1->myVar.count();j++){
                    if(varName == myComp1->myVar.at(j).name){
                        s.fromNum = j;
                        break;
                    }
                }
            }
            else if(s.fromType=="P"){
                for(int j = 0; j < myComp1->myPar.count();j++){
                    if(varName == myComp1->myPar.at(j).name){
                        s.fromNum = j;
                        break;
                    }
                }
            }
            varName = iterationTable->item(i,5)->text();
            if(s.toType=="V"){
                for(int j = 0; j < myComp2->myVar.count();j++){
                    if(varName == myComp2->myVar.at(j).name){
                        s.toNum = j;
                        break;
                    }
                }
            }
            else if(s.toType=="P"){
                for(int j = 0; j < myComp2->myPar.count();j++){
                    if(varName == myComp2->myPar.at(j).name){
                        s.toNum = j;
                        break;
                    }
                }
            }
            s.description = iterationTable->item(i,6)->text();
            myLink->mySuccessive.append(s);
        }

        accept();
    }

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
    if(str=="Variable"){
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

    bool sameFromPort, sameToPort;
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

        sameFromPort = streamTable->item(i,1)->text()==fromComp->getCompName()
                &&dynamic_cast<QSpinBox*>(streamTable->cellWidget(i,2))->value()== fromPort;
        sameToPort = streamTable->item(i,3)->text()==toComp->getCompName()
                &&dynamic_cast<QSpinBox*>(streamTable->cellWidget(i,4))->value()== toPort;
        if(sameFromPort||sameToPort){
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
    int row = streamTable->currentRow();
    if(row ==-1){
        QMessageBox *mb = new QMessageBox;
        mb->setText("Please select the stream to remove!");
        mb->exec();
        return;
    }
    else{
        streamTable->removeRow(row);
        return;
    }
}

void myLinkDialog::reverseStream()
{

    int row = streamTable->currentRow();
    if(row ==-1){
        QMessageBox *mb = new QMessageBox;
        mb->setText("Please select the stream to remove!");
        mb->exec();
        return;
    }
    else{
        QString fromComp;
        int fromValue;
        fromComp = streamTable->item(row,1)->text();
        fromValue = dynamic_cast<QSpinBox*>(streamTable->cellWidget(row,2))->value();

        streamTable->item(row,1)->setText(streamTable->item(row,3)->text());
        dynamic_cast<QSpinBox*>(streamTable->cellWidget(row,2))->setValue(dynamic_cast<QSpinBox*>(streamTable->cellWidget(row,4))->value());

        streamTable->item(row,3)->setText(fromComp);
        dynamic_cast<QSpinBox*>(streamTable->cellWidget(row,4))->setValue(fromValue);
        return;
    }
}

void myLinkDialog::addVariable()
{
    QString fromMember = variableMemberCombobox1->currentText();
    QString toMember = variableMemberCombobox2->currentText();

    bool fromSame, toSame;
    for(int i = 0; i< variableTable->rowCount(); i++){
        fromSame = (variableTable->item(i,1)->text()==fromMember);
        toSame = (variableTable->item(i,3)->text()==toMember);

        if(fromSame&&toSame){
            QMessageBox *mb = new QMessageBox;
            mb->setText("Variable link already exists!");
            mb->exec();
            return;
        }
    }

    QTableWidgetItem * item;
    int row = variableTable->rowCount();
    variableTable->insertRow(row);

    item = new QTableWidgetItem;
    item->setData(Qt::DisplayRole,myComp1->getCompName());
    item->setTextAlignment(Qt::AlignCenter);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    variableTable->setItem(row,0,item);

    item = new QTableWidgetItem;
    item->setData(Qt::DisplayRole,fromMember);
    item->setTextAlignment(Qt::AlignCenter);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    variableTable->setItem(row,1,item);

    item = new QTableWidgetItem;
    item->setData(Qt::DisplayRole,myComp2->getCompName());
    item->setTextAlignment(Qt::AlignCenter);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    variableTable->setItem(row,2,item);

    item = new QTableWidgetItem;
    item->setData(Qt::DisplayRole,toMember);
    item->setTextAlignment(Qt::AlignCenter);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    variableTable->setItem(row,3,item);

    item = new QTableWidgetItem;
    item->setData(Qt::DisplayRole,"Add variable link description");
    item->setTextAlignment(Qt::AlignCenter);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
    variableTable->setItem(row,4,item);
}

void myLinkDialog::removeVariable()
{
    int row = variableTable->currentRow();
    if(row ==-1){
        QMessageBox *mb = new QMessageBox;
        mb->setText("Please select the variable link to remove!");
        mb->exec();
        return;
    }
    else{
        variableTable->removeRow(row);
        return;
    }

}

void myLinkDialog::addIteration()
{
    QString fromMember, toMember, temp, fromVp, toVp;
    fromMember = iterationMemberCombobox1->currentText();
    toMember = iterationMemberCombobox2->currentText();
    fromVp = iterationVPCombobox1->currentText();
    toVp = iterationVPCombobox2->currentText();

    if(fromMember==""||toMember==""){
        QMessageBox *mb = new QMessageBox;
        mb->setText("Please select valid parameter/variable to link!");
        mb->exec();
        return;
    }
    else{
        component *fromComp = myComp1, *toComp = myComp2;
        if(iterationDirectionButton->text()=="<<<<<"){
            fromComp = myComp2;
            toComp = myComp1;
            temp = fromMember;
            fromMember = toMember;
            toMember = temp;
            temp = fromVp;
            fromVp = toVp;
            toVp = temp;
        }

        for(int i = 0; i < iterationTable->rowCount(); i++){
            if(fromMember==iterationTable->item(i,2)->text()
                    &&toMember==iterationTable->item(i,5)->text()){
                QMessageBox *mb = new QMessageBox;
                mb->setText("The iteration link already exists.");
                mb->exec();
                return;
            }
        }


        int index = iterationTable->rowCount();
        iterationTable->insertRow(index);

        QTableWidgetItem* item;
        item = new QTableWidgetItem;
        item->setData(Qt::DisplayRole,fromComp->getCompName());
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        iterationTable->setItem(index,0,item);

        item = new QTableWidgetItem;
        item->setData(Qt::DisplayRole,fromVp);
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        iterationTable->setItem(index,1,item);

        item = new QTableWidgetItem;
        item->setData(Qt::DisplayRole,fromMember);
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        iterationTable->setItem(index,2,item);

        item = new QTableWidgetItem;
        item->setData(Qt::DisplayRole,toComp->getCompName());
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        iterationTable->setItem(index,3,item);


        item = new QTableWidgetItem;
        item->setData(Qt::DisplayRole,toVp);
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        iterationTable->setItem(index,4,item);

        item = new QTableWidgetItem;
        item->setData(Qt::DisplayRole,toMember);
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        iterationTable->setItem(index,5,item);

        item = new QTableWidgetItem;
        item->setData(Qt::DisplayRole,"Please add iteration connection description.");
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        iterationTable->setItem(index,6,item);
    }
}

void myLinkDialog::removeIteration()
{
    int row = iterationTable->currentRow();
    if(row ==-1){
        QMessageBox *mb = new QMessageBox;
        mb->setText("Please select the iteration link to remove!");
        mb->exec();
        return;
    }
    else{
        iterationTable->removeRow(row);
        return;
    }

}

void myLinkDialog::reverseIteration()
{
    int row = iterationTable->currentRow();
    if(row ==-1){
        QMessageBox *mb = new QMessageBox;
        mb->setText("Please select the iteration link to remove!");
        mb->exec();
        return;
    }
    else{
        QString fromComp, fromVp, fromName;
        fromComp = iterationTable->item(row,0)->text();
        fromVp = iterationTable->item(row,1)->text();
        fromName = iterationTable->item(row,2)->text();

        iterationTable->item(row,0)->setText(iterationTable->item(row,3)->text());
        iterationTable->item(row,1)->setText(iterationTable->item(row,4)->text());
        iterationTable->item(row,2)->setText(iterationTable->item(row,5)->text());
        iterationTable->item(row,3)->setText(fromComp);
        iterationTable->item(row,4)->setText(fromVp);
        iterationTable->item(row,5)->setText(fromName);
        return;
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

    connect(variableAddButton,SIGNAL(clicked(bool)),this,SLOT(addVariable()));
    connect(variableRemoveButton,SIGNAL(clicked(bool)),this,SLOT(removeVariable()));

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
    buttonRowLayout->addWidget(iterationReverseButton);

    mainLayout->addLayout(firstRowLayout);
    mainLayout->addLayout(secondRowLayout);
    mainLayout->addLayout(thirdRowLayout);
    mainLayout->addWidget(iterationTable);
    mainLayout->addLayout(buttonRowLayout);

    iterationGroupBox->setLayout(mainLayout);

    connect(iterationDirectionButton,SIGNAL(clicked(bool)),this,SLOT(iterationDirectionFlipped()));

    connect(iterationVPCombobox1,SIGNAL(currentTextChanged(QString)),this,SLOT(vp1ComboChanged(QString)));
    connect(iterationVPCombobox2,SIGNAL(currentTextChanged(QString)),this,SLOT(vp2ComboChanged(QString)));

    connect(iterationReverseButton,SIGNAL(clicked(bool)),this,SLOT(reverseIteration()));
    connect(iterationAddButton,SIGNAL(clicked(bool)),this,SLOT(addIteration()));
    connect(iterationRemoveButton,SIGNAL(clicked(bool)),this,SLOT(removeIteration()));

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
            member1->setData(Qt::DisplayRole,myComp1->myVar.at(var.fromVarNum).name);
            member1->setTextAlignment(Qt::AlignCenter);
            member1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            variableTable->setItem(i,1,member1);

            comp2 = new QTableWidgetItem;
            comp2->setData(Qt::DisplayRole,var.toComp->getCompName());
            comp2->setTextAlignment(Qt::AlignCenter);
            comp2->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            variableTable->setItem(i,2,comp2);

            member2 = new QTableWidgetItem;
            member2->setData(Qt::DisplayRole,myComp2->myVar.at(var.toVarNum).name);
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

    QTableWidgetItem *item = NULL, *comp1 = NULL, *member1 = NULL, *comp2 = NULL, *member2 = NULL, *description = NULL;
    successiveLink suc;

    if(!myLink->mySuccessive.isEmpty()){
        iterationTable->setRowCount(myLink->mySuccessive.count());
        for(int i = 0; i < myLink->mySuccessive.count();i++){

            suc = myLink->mySuccessive.at(i);

            comp1 = new QTableWidgetItem;
            comp1->setData(Qt::DisplayRole,suc.fromComp->getCompName());
            comp1->setTextAlignment(Qt::AlignCenter);
            comp1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            iterationTable->setItem(i,0,comp1);

            item = new QTableWidgetItem;
            item->setData(Qt::DisplayRole,
                          (suc.fromType=="V")?"Variable":"Parameter");
            item->setTextAlignment(Qt::AlignCenter);
            item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            iterationTable->setItem(i,1,item);

            QString name =
                    (suc.fromType=="V")?
                        suc.fromComp->myVar.at(suc.fromNum).name
                      :suc.fromComp->myPar.at(suc.fromNum).name;


            member1 = new QTableWidgetItem;
            member1->setData(Qt::DisplayRole,name);
            member1->setTextAlignment(Qt::AlignCenter);
            member1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            iterationTable->setItem(i,2,member1);

            comp2 = new QTableWidgetItem;
            comp2->setData(Qt::DisplayRole,suc.toComp->getCompName());
            comp2->setTextAlignment(Qt::AlignCenter);
            comp2->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            iterationTable->setItem(i,3,comp2);

            item = new QTableWidgetItem;
            item->setData(Qt::DisplayRole,
                          (suc.toType=="V")?"Variable":"Parameter");
            item->setTextAlignment(Qt::AlignCenter);
            item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            iterationTable->setItem(i,4,item);


            name = (suc.toType=="V")?
                        suc.toComp->myVar.at(suc.toNum).name
                      :suc.toComp->myPar.at(suc.toNum).name;

            member2 = new QTableWidgetItem;
            member2->setData(Qt::DisplayRole,name);
            member2->setTextAlignment(Qt::AlignCenter);
            member2->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            iterationTable->setItem(i,5,member2);

            description = new QTableWidgetItem;
            description->setData(Qt::DisplayRole,suc.description);
            description->setTextAlignment(Qt::AlignCenter);
            description->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable);
            iterationTable->setItem(i,6,description);

        }
    }
}

