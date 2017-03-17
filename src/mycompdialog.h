#ifndef MYCOMPDIALOG_H
#define MYCOMPDIALOG_H


#include "component.h"

#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QTabWidget>
#include <QTableWidget>


class myCompDialog : public QDialog
{

    Q_OBJECT
public:
    myCompDialog(component *comp);


private slots:

    void doneClicked();
    void resetClicked();

private:

    void createInfoGroupBox();
    void createParameterGroupBox();
    void createVariableGroupBox();
    void createButtonGroupBox();


    void initialize();
    void readData();

    QTabWidget *tabWidget;

    QGroupBox *infoGroupBox;
    QGroupBox *parameterGroupBox;
    QGroupBox *variableGroupBox;
    QGroupBox *buttonGroupBox;

    QLabel * infoIndexLabel;
    QLabel * infoNameLabel;
    QLineEdit * infoNameLineEdit;
    QLabel * infoDescriptionLabel;
    QTextEdit * infoDescriptionLineEdit;
    QLabel * picLabel;

    QTableWidget * parameterTable;
    QTableWidget * variableTable;

    QPushButton *doneButton;
    QPushButton *resetButton;
    QPushButton *cancelButton;

    component * myComponent;

};

#endif // MYCOMPDIALOG_H
