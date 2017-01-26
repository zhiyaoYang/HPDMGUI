#ifndef MYCOMPDIALOG_H
#define MYCOMPDIALOG_H


#include "component.h"

#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>

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
    void createPropertyGroupBox();
    void createButtonGroupBox();


    void initialize();
    void readData();

    QGroupBox *infoGroupBox;
    QGroupBox *propertyGroupBox;
    QGroupBox *buttonGroupBox;

    QLabel * indexLabel;
    QLabel * nameLabel;
    QLineEdit * nameEdit;
    QLabel * descriptionLabel;
    QTextEdit * descriptionEdit;
    QLabel * picLabel;

    QLabel * parameterLabel;
    QLabel * variableLabel;

    QPushButton *doneButton;
    QPushButton *resetButton;
    QPushButton *cancelButton;

    component * myComponent;

};

#endif // MYCOMPDIALOG_H
