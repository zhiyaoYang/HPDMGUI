#ifndef MYRESULTSDIALOG_H
#define MYRESULTSDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QTableWidget>
#include <QTabWidget>
#include <QGroupBox>
#include <QListWidget>
#include <QObject>
#include <QString>
#include <QTableWidgetItem>
#include <QStringList>
#include <QByteArray>
#include <QList>

class myResultsDialog : public QDialog
{

    Q_OBJECT
public:
    myResultsDialog(QString results);

private slots:

private:

    void createTabsAndBoxes();

    void initialize();

    void loadTable();

    void keyPressEvent(QKeyEvent * e);
    void onTableItemChanged();

    QTabWidget *tabWidget;

    QGroupBox * compKeyGroupBox;
    QGroupBox * paraKeyGroupBox;
    QGroupBox * keyGroupBox;
    QGroupBox * buttonGroupBox;

    QListWidget * compsList;
    QListWidget * parasList;

    QTableWidget * resultTable;

    QPushButton * closeButton;

    QStringList allResults;
    QString myComps;
    QString myParas;
    QStringList myResults;

    QList<QTableWidgetItem*> selected;
    QByteArray myByteArray;
};

#endif // MYRESULTSDIALOG_H
