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
    myParametricDialog();
    
private slots:
    
    void addClicked();
    void removeClicked();
    void runClicked();
    void cancelClicked();
    void nRunChanged(int i);
    void componentChanged(QString s);
    
private:
    
    void initialize();
    void createOptButtonGroupBox();
    void createTableGroupBox();
    void createDialogButtonGroupBox();

    bool validCheck();
    
    QGroupBox * optButtonGroupBox;
    QGroupBox * tableGroupBox;
    QGroupBox * dialogButtonGroupBox;
    
    QLabel * nRunLabel;
    QSpinBox * nRunSpinBox;
    QLabel * componentLabel;
    QComboBox * compListComboBox;
    QComboBox * vpLabel;
    QComboBox * vpListComboBox;
    
    QPushButton * addButton;
    QPushButton * removeBotton;
    QPushButton * runButton;
    QPushButton * cancelButton;
    
    QTableWidget * paraTable;

};

#endif // MYPARAMETRICDIALOG_H
