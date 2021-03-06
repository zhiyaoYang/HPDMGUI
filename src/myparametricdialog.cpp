#include "myparametricdialog.h"
#include "mymainwindow.h"

#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QMessageBox>
#include <QByteArray>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include "myresultsdialog.h"

#include <QDebug>

extern component *dummy;

myParametricDialog::myParametricDialog(QString caseDir)
{
    myCaseDir = caseDir;
    initialize();
}

void myParametricDialog::addClicked()
{
    if(compListComboBox->currentText()!="select component"){
        int i = paraTable->columnCount();

        QStringList header;
        for(int j = 0; j < paraTable->columnCount();j++){
            header.append(paraTable->horizontalHeaderItem(j)->text());
        }
        QString selectedVP = compListComboBox->currentText()+":"+vpListComboBox->currentText();
        if(header.contains(selectedVP)){
            QMessageBox * mBox = new QMessageBox(this);
            mBox->setText("The selected parameter/variable is already in the table.");
            mBox->exec();
        }
        else{
            header.append(selectedVP);
            paraTable->insertColumn(i);
            paraTable->setHorizontalHeaderLabels(header);

            paraTable->resizeColumnsToContents();

            component*iterator = dummy, *myComp = NULL;
            bool found = false;
            while((iterator->next!=NULL)&&(!found)){
                iterator = iterator->next;
                if(iterator->getCompName()==compListComboBox->currentText()){
                    found = true;
                    myComp = iterator;
                }
            }

            QString addStr;
            if(vpListComboBox->currentIndex()+1>myComp->myPar.count()){
                addStr = QString::number(myComp->getIndex()-1)
                              +";V;"
                              +QString::number(vpListComboBox->currentIndex()-myComp->myPar.count());
            }
            else{
                addStr = QString::number(myComp->getIndex()-1)
                              +";P;"
                              +QString::number(vpListComboBox->currentIndex());

                qDebug()<<"adding"<<addStr;
            }

            if(!vpList.contains(addStr)){
                vpList.append(addStr);
            }
        }


    }
    else{
        QMessageBox * mBox = new QMessageBox(this);
        mBox->setText("Please select a parameter/variable from a component.");
        mBox->exec();
    }
}

void myParametricDialog::removeClicked()
{
    int column = paraTable->currentColumn();
    if(column == -1){
        QMessageBox * mBox = new QMessageBox(this);
        mBox->setText("Please select the column of table to remove.");
        mBox->exec();
    }
    else{
        QMessageBox * wBox = new QMessageBox("Remove column",
                                             "Do you wish to remove column ["
                                                +paraTable->horizontalHeaderItem(column)->text()
                                                    +"]? The removed data will be lost.",
                                             QMessageBox::Information,
                                             QMessageBox::Ok,
                                             QMessageBox::Cancel,
                                             QMessageBox::NoButton);
        if(wBox->exec()==QMessageBox::Ok){
            paraTable->removeColumn(column);

            if(paraTable->columnCount()!=vpList.count()){
                qDebug()<<"list and table don't match!";
            }
            else{
                vpList.removeAt(column);
            }
        }
    }
}

void myParametricDialog::runClicked()
{
    if(writeBatchFile()){
        if(writeHPDMFile()){

            //running the calculation batch file
            QProcess p;
            p.setWorkingDirectory(myCaseDir);
            auto Command = QString("cmd.exe");
            auto Arguments = QStringList{
                    QString("/C"),
                    myCaseDir+"/RunHPDMFlex.bat"
            };

            p.start(Command,Arguments);
            p.waitForFinished(-1);

            if(p.exitCode()==0){
                QMessageBox * mBox = new QMessageBox(this);
                mBox->addButton("Show Results",QMessageBox::YesRole);
                mBox->addButton("Close",QMessageBox::NoRole);

                //copy of loadResultFile() from mainwindow
                QFile oFile(myCaseDir + "/out.xls");
                QString results;
                QString msg;

                if(!oFile.open(QIODevice::ReadOnly|QIODevice::Text)){
                    msg = "Failed to open result file.";
                    results = "";
                }
                else{
                    QTextStream stream(&oFile);
                    results = stream.readAll();
                    stream.flush();
                    oFile.close();
                    msg = "Calculation finished!";
                }


                mBox->setWindowTitle("Parametric run");
                mBox->setText(msg);
                mBox->setModal(true);
                mBox->exec();
                if(mBox->buttonRole(mBox->clickedButton())==QMessageBox::YesRole){

                    //copy of showResults() from mainwindow
                    if(results!=""){
                        myResultsDialog *rDialog
                                = new myResultsDialog(results);
                        rDialog->exec();
                    }

                }

            }
            else{
                QMessageBox::information(this,
                                         "Error",
                                         "Failed to run the calculation batch file.",
                                         QMessageBox::Ok);
                //check SysErrLog for further information

            }
        }
    }
}

void myParametricDialog::cancelClicked()
{
    reject();
}

void myParametricDialog::nRunChanged()
{
    QString warningText;
    int newNRun = nRunSpinBox->value();
    int diff = paraTable->rowCount() - newNRun;

    if(diff!=0){
        if(diff < 0){
            warningText = "Do you want to extend the table to "
                    +QString::number(newNRun)
                    +" rows?";
        }
        else if(diff > 0){
            warningText = "Do you want to truncate the last "
                    +QString::number(diff)
                    +" rows?\nAll truncated data will be lost.";
        }

        QMessageBox * mBox = new QMessageBox("Changing row count",
                                             warningText,
                                             QMessageBox::Information,
                                             QMessageBox::Ok,
                                             QMessageBox::Cancel,
                                             QMessageBox::NoButton);
        if(mBox->exec()==QMessageBox::Ok){
            paraTable->setRowCount(newNRun);
        }
        else{
            nRunSpinBox->setValue(paraTable->rowCount());
        }
    }

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

    loadExistingBatch();

}

void myParametricDialog::loadExistingBatch()
{
    //get batch line and read batch file to initialize the table

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
    connect(nRunSpinBox,SIGNAL(editingFinished()),this,SLOT(nRunChanged()));
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

void myParametricDialog::paste()
{
    QString str = QApplication::clipboard()->text();

    QStringList rows = str.split('\n');

    int numRows = rows.count();
    int numColumns = 0;
    for(int j = 0; j < numRows;j++){
        numColumns = (numColumns<rows.at(j).count('\t')+1)?rows.at(j).count('\t')+1:numColumns;
    }

    int lastRow = numRows+paraTable->currentRow();
    bool canceled = false;

    if(lastRow>paraTable->rowCount()){
        QMessageBox * mBox = new QMessageBox("Expand table?",
                                             "The dimension of the data in clipboard is larger than current table. "
                                             "Do you want to expand the table to "+QString::number(lastRow+1)+" rows?",
                                             QMessageBox::Information,
                                             QMessageBox::Ok,
                                             QMessageBox::No,
                                             QMessageBox::Cancel);
        int boxReturn = mBox->exec();
        if(boxReturn==QMessageBox::Ok){
            paraTable->setRowCount(lastRow);
        }
        else if(boxReturn==QMessageBox::No){
            numRows = paraTable->rowCount()-paraTable->currentRow();
        }
        else{
            canceled = true;
        }
    }

    if(!canceled){
        for(int i = 0; i < numRows;++i){
            QStringList columns = rows[i].split('\t');
            QTableWidgetItem * item = NULL;
            while(columns.count()<numColumns){
                columns.append("");
            }

            for(int j = 0; j < numColumns;++j){
                if(paraTable->currentColumn()+j<paraTable->columnCount()){

                    qDebug()<<paraTable->currentRow()+i<<paraTable->currentColumn()+j;

                    item = new QTableWidgetItem;
                    item->setData(Qt::DisplayRole,columns[j]);
                    item->setTextAlignment(Qt::AlignCenter);
                    paraTable->setItem(paraTable->currentRow()+i,
                                       paraTable->currentColumn()+j,
                                       item);
                }
            }
        }
    }
}

void myParametricDialog::onTableItemChanged()
{
    selected = paraTable->selectedItems();

    myByteArray.clear();

    int row0 = selected.first()->row();
    int row1;

    for(int i = 0; i < selected.size();i++)
    {
        row1 = selected.at(i)->row();

        if(row1 != row0)
        {
            myByteArray.remove(myByteArray.length()-1,1);
            myByteArray.append("\n");
            myByteArray.append(selected.at(i)->text());
            myByteArray.append("\t");
        }
        else
        {
            myByteArray.append(selected.at(i)->text());
            myByteArray.append("\t");
        }

        row0 = row1;
    }
    myByteArray.remove(myByteArray.length()-1,1);


}

bool myParametricDialog::writeBatchFile()
{

    validRun = paraTable->rowCount();

    QFile file(myCaseDir+"/BatchIn.dat");
    if(file.exists()){
        file.remove();
    }
    if(file.open(QIODevice::WriteOnly|QIODevice::Text)){

        int nRuns = paraTable->rowCount();
        QString row;
        QStringList rows;
        QTableWidgetItem* item;
        bool rowValid = true;
        for(int i = 0; i < nRuns; i++){

            rowValid = true;
            row.clear();

            for(int j = 0; j < paraTable->columnCount();j++){

                item = paraTable->item(i,j);
                if(item){
                    row.append(paraTable->item(i,j)->text()+"\t");
                }
                else{
                    rowValid = false;
                }
            }
            if(rowValid){
                rows<<row;
            }
            else{
                validRun --;
            }
        }

        QTextStream stream(&file);
        stream<<"!\tComment:\n";
        stream<<"N\t"<<validRun<<"\t\"number of runs for processing, and the first row for inputting titles\"\n";
        stream<<rows.join("\n");

        stream.flush();
        file.close();

        return true;
    }
    else{
        qDebug()<<"fail to write to batch file";
        return false;
    }
}

bool myParametricDialog::writeHPDMFile()
{
    QString batchLine;

    batchLine.append("B\tBatchIn.dat\t");
    for(int i = 0; i < vpList.count();i++){
        batchLine.append(vpList.at(i)+"\t");
    }
    batchLine.append("!\tParametric input def row");


    QString allText;
    QStringList lineList;
    QStringList toDel;


    QFile file(myCaseDir+"/hpdata.hpdm");
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream instream(&file);

        allText = instream.readAll();
        file.close();
    }
    else{
        qDebug()<<"fail to read original data";
        return false;
    }

    lineList = allText.split("\n");

    int batchRow = 0;
    for(int i = 0; i < lineList.count(); i++){
        if(lineList.at(i).at(0)=='B'){
            toDel.append(lineList.at(i));
        }
        if(lineList.at(i).contains("After this row, input batch run inputs")){
            batchRow = i;
        }

    }
    foreach(QString s,toDel){
        lineList.removeOne(s);
    }

    lineList.insert(batchRow+1,batchLine);
    allText = lineList.join("\n");

    QFile dFile(myCaseDir+"/hpdata1.hpdm");
    if(dFile.exists()){
        dFile.remove();
    }

    if(file.open(QIODevice::WriteOnly|QIODevice::Text)){
        QTextStream outstream(&file);
        outstream<<allText;
        outstream.flush();
        file.close();
        return true;
    }
    else{
        qDebug()<<"fail to write into hpdm file";
        return false;
    }
}

void myParametricDialog::keyPressEvent(QKeyEvent *event)
{

    if(event->matches(QKeySequence::Copy))
    {
        onTableItemChanged();
        QMimeData * mimeData = new QMimeData();
        mimeData->setData("text/plain",myByteArray);
        QApplication::clipboard()->setMimeData(mimeData);
    }
    else if(event->matches(QKeySequence::Paste)){
        paste();
    }
    else if(event->key()==Qt::Key_Escape){
    }
    else{
        QDialog::keyPressEvent(event);
    }
}
