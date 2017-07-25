#ifndef MYPARAMETRICDIALOG_H
#define MYPARAMETRICDIALOG_H

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QGroupBox>
#include <QString>

#include "component.h"

class myParametricDialog : public QDialog
{
    
    Q_OBJECT
public:
    myParametricDialog(QString caseDir);
    
private slots:
    
    void addClicked();
    void removeClicked();
    void runClicked();
    void cancelClicked();
    void nRunChanged();
    void componentChanged(QString s);
    
private:
    
    void initialize();
    void createOptButtonGroupBox();
    void createTableGroupBox();
    void createDialogButtonGroupBox();

    void paste();
    void onTableItemChanged();
    void writeBatchFile();
    void writeHPDMFile();

    void keyPressEvent(QKeyEvent * event);
    
    QGroupBox * optButtonGroupBox;
    QGroupBox * tableGroupBox;
    QGroupBox * dialogButtonGroupBox;
    
    QLabel * nRunLabel;
    QSpinBox * nRunSpinBox;
    QLabel * componentLabel;
    QComboBox * compListComboBox;
    QLabel * vpLabel;
    QComboBox * vpListComboBox;
    
    QPushButton * addButton;
    QPushButton * removeBotton;
    QPushButton * runButton;
    QPushButton * cancelButton;
    
    QTableWidget * paraTable;

    QList<QTableWidgetItem*> selected;
    QByteArray myByteArray;

    QString myCaseDir;

    QStringList vpList;

    int validRun;

};

#endif // MYPARAMETRICDIALOG_H
