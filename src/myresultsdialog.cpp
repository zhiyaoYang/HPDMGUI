#include "myresultsdialog.h"

#include <QDebug>
#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QApplication>
#include <QMimeData>
#include <QKeyEvent>
#include <QClipboard>

myResultsDialog::myResultsDialog(QString results)
{
    allResults = results.split("\n");

    initialize();

}

void myResultsDialog::createTabsAndBoxes()
{
    compKeyGroupBox = new QGroupBox;
    QHBoxLayout * compLayout = new QHBoxLayout;
    compsList = new QListWidget;
    compsList->addItems(myComps.split("\n"));
    compLayout->addWidget(compsList);
    compKeyGroupBox->setLayout(compLayout);

    paraKeyGroupBox = new QGroupBox;
    QHBoxLayout * paraLayout = new QHBoxLayout;
    parasList = new QListWidget;
    parasList->addItems(myParas.split("\n"));
    paraLayout->addWidget(parasList);
    paraKeyGroupBox->setLayout(paraLayout);

    keyGroupBox = new QGroupBox;
    QHBoxLayout * keyLayout = new QHBoxLayout;
    keyLayout->addWidget(compKeyGroupBox);
    keyLayout->addWidget(paraKeyGroupBox);
    keyGroupBox->setLayout(keyLayout);

    buttonGroupBox = new QGroupBox;
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    closeButton = new QPushButton(tr("Close"));
    QSpacerItem *sItem = new QSpacerItem(0,0,QSizePolicy::Minimum,QSizePolicy::Expanding);
    buttonLayout->addItem(sItem);
    buttonLayout->addWidget(closeButton);

    connect(closeButton,SIGNAL(clicked(bool)),this,SLOT(close()));
}

void myResultsDialog::initialize()
{
    //separate text

    QString str;
    QStringList list;

    int compRowEnd = allResults.indexOf("!	---	Output Variable List	---");
    myComps.clear();
    for(int i = 1; i < compRowEnd; i++){
        str = allResults.at(i);
        list = str.split("\t");
        list.removeAt(0);
        myComps.append(list.join("\t"));
        myComps.append("\n");
    }

    int paraRowEnd = allResults.indexOf("!	---	Output Data	---");
    str.clear();
    list.clear();
    myParas.clear();
    for(int i = compRowEnd+1; i < paraRowEnd; i++){
        str = allResults.at(i);
        list = str.split("\t");
        list.removeAt(0);
        myParas.append(list.join("\t"));
        myParas.append("\n");
    }

    str.clear();
    myResults.clear();
    for(int i = paraRowEnd+1; i < allResults.count(); i++){
        str = allResults.at(i);
        if(str!=""){
            myResults.append(str);
        }
    }

    //fill table
    loadTable();

    createTabsAndBoxes();

    tabWidget = new QTabWidget;
    tabWidget->addTab(resultTable,tr("Results"));
    tabWidget->addTab(keyGroupBox,tr("Index"));

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonGroupBox);

    setLayout(mainLayout);

}

void myResultsDialog::loadTable()
{
    resultTable = new QTableWidget;

    QStringList header = myResults.at(0).split("\t",QString::SkipEmptyParts);
    int columns = header.count();

    resultTable->setColumnCount(columns);
    resultTable->setHorizontalHeaderLabels(header);
    resultTable->setRowCount(myResults.count()-1);

    QStringList row;
    QTableWidgetItem * item = NULL;

    for(int i = 1; i < myResults.count(); i++){

        row = myResults.at(i).split("\t",QString::SkipEmptyParts);
        for(int j = 0; j < columns; j++){
            item = new QTableWidgetItem;
            item->setData(Qt::DisplayRole,row.at(j));
            item->setTextAlignment(Qt::AlignCenter);
            resultTable->setItem(i-1,j,item);

        }
    }

}

void myResultsDialog::keyPressEvent(QKeyEvent *e)
{
    if(e->matches(QKeySequence::Copy))
    {
        onTableItemChanged();
        QMimeData * mimeData = new QMimeData();
        mimeData->setData("text/plain",myByteArray);
        QApplication::clipboard()->setMimeData(mimeData);
    }
    else{
        QDialog::keyPressEvent(e);
    }

}

void myResultsDialog::onTableItemChanged()
{
    selected = resultTable->selectedItems();

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
