#ifndef MYLINKDIALOG_H
#define MYLINKDIALOG_H

#include "link.h"

#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>


class myLinkDialog : public QDialog
{

    Q_OBJECT
public:
    myLinkDialog(link * mlink);
    ~myLinkDialog();

private slots:
    void doneClicked();
    void resetClicked();

private:

    void createStreamGroupBox();
    void createParameterGroupBox();
    void createIterationGroupBox();
    void createButtonGroupBox();

    void initialize();
    void readData();

    QGroupBox *streamGroupBox;
    QGroupBox *parameterGroupBox;
    QGroupBox *iterationGroupBox;
    QGroupBox *buttonGroupBox;

    QLabel *streamLabel1;
    QLabel *streamLabel2;
    QComboBox *streamComboBox1;
    QComboBox *streamComboBox2;
    QLineEdit *streamLineEdit1;
    QLineEdit *streamLineEdit2;
    QListWidget *streamList1;
    QListWidget *streamList2;


    QLabel *parameterLabel1;
    QLabel *parameterLabel2;
    QComboBox *parameterComboBox1;
    QComboBox *parameterComboBox2;
    QLineEdit *parameterLineEdit1;
    QLineEdit *parameterLineEdit2;
    QListWidget *parameterList1;
    QListWidget *parameterList2;


    QLabel *iterationLabel1;
    QLabel *iterationLabel2;
    QComboBox *iterationComboBox1;
    QComboBox *iterationComboBox2;
    QLineEdit *iterationLineEdit1;
    QLineEdit *iterationLineEdit2;
    QListWidget *iterationList1;
    QListWidget *iterationList2;

    QPushButton *doneButton;
    QPushButton *resetButton;
    QPushButton *cancelButton;

    link * myLink;
};

#endif // MYLINKDIALOG_H
