#ifndef MYCOMPDIALOG_H
#define MYCOMPDIALOG_H


#include "component.h"

#include <QDialog>
#include <QGroupBox>
#include <QLabel>
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
    void variableResetClicked();
    void parameterResetClicked();
    void variableHideToggled();

private:

    void createInfoGroupBox();
    void createParameterGroupBox();
    void createVariableGroupBox();
    void createButtonGroupBox();


    void initialize();
    void readData();
    void loadParameterTable();
    void loadVariableTable();

    void hideEmptyVar();
    void showEmptyVar();

    QTabWidget *tabWidget;

    QGroupBox *infoGroupBox;
    QGroupBox *parameterGroupBox;
    QGroupBox *variableGroupBox;
    QGroupBox *buttonGroupBox;

    QLabel * infoIndexLabel;
    QLabel * infoNameLabel;
    QLineEdit * infoNameLineEdit;
    QLabel * infoDescriptionLabel;
    QLineEdit * infoDescriptionLineEdit;
    QLabel * infoPicLabel;

    QTableWidget * parameterTable;
    QPushButton *parameterResetButton;
    QTableWidget * variableTable;
    QPushButton *variableResetButton;
    QPushButton *variableHideButton;

    QPushButton *doneButton;
    QPushButton *cancelButton;

    bool variableEmptyHidden;

    component * myComponent;

};

#endif // MYCOMPDIALOG_H
