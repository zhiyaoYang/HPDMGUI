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
#include <QTableWidget>
#include <QTabWidget>
#include <QSpinBox>
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
    void streamDirectionFlipped();
    void iterationDirectionFlipped();

    void vp1ComboChanged(QString str);
    void vp2ComboChanged(QString str);
private:

    void createStreamGroupBox();
    void createVariableGroupBox();
    void createIterationGroupBox();
    void createButtonGroupBox();

    void initialize();
    void readData();
    void loadStreamTable();
    void loadVariableTable();
    void loadIterationTable();

    QTabWidget *tabWidget;

    QGroupBox *streamGroupBox;
    QGroupBox *variableGroupBox;
    QGroupBox *iterationGroupBox;
    QGroupBox *buttonGroupBox;

    QLabel *streamCompLabel1;
    QLabel *streamCompLabel2;
    QPushButton *streamDirectionButton;
    QComboBox *streamSubstanceCombobox;
    QLabel *streamIOLabel1;
    QLabel *streamIOLabel2;
    QSpinBox *streamIOSpinBox1;
    QSpinBox *streamIOSpinBox2;
    QTableWidget *streamTable;
    QPushButton * streamAddButton;
    QPushButton * streamRemoveButton;
    QPushButton * streamReverseButton;

    QLabel *variableCompLabel1;
    QLabel *variableCompLabel2;
    QComboBox *variableMemberCombobox1;
    QComboBox *variableMemberCombobox2;
    QTableWidget *variableTable;
    QPushButton * variableAddButton;
    QPushButton * variableRemoveButton;


    QLabel *iterationCompLabel1;
    QLabel *iterationCompLabel2;
    QPushButton *iterationDirectionButton;
    QComboBox *iterationVPCombobox1;
    QComboBox *iterationVPCombobox2;
    QComboBox *iterationMemberCombobox1;
    QComboBox *iterationMemberCombobox2;
    QTableWidget *iterationTable;
    QPushButton * iterationAddButton;
    QPushButton * iterationRemoveButton;
    QPushButton * iterationReverseButton;

    QPushButton *doneButton;
    QPushButton *resetButton;
    QPushButton *cancelButton;

    component * myComp1;
    component * myComp2;

    bool streamRightDirection;
    bool iterationRightDirection;

    QStringList vars1;
    QStringList vars2;
    QStringList pars1;
    QStringList pars2;

    link * myLink;
};

#endif // MYLINKDIALOG_H
