#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>

#include "myscene.h"
#include "myview.h"

class myMainwindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit myMainwindow(QWidget *parent = 0);

signals:

public slots:

protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU


private slots:
    //file menu
    void newFile();
    void open();
    void save();
    void exitProgram();

    //edit menu
    void newComponent();
    void newLink();
    void enableDrag(bool arg1);

    //help menu
    void help();
    void about();

private:

    void initialize();
    void createActions();
    void createMenus();

    myScene *scene;
    myView *view;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;
    QAction *newCompAct;
    QAction *newLinkAct;
    QAction *enableDragAct;
    QAction *helpAct;
    QAction *aboutAct;


};

#endif // MYMAINWINDOW_H
