#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QtWidgets>

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

    void fillDock(QStringList string){
        introduction->clear();
        introduction->addItems(string);
        compListDock->update();
    }


private slots:
    //file menu
    void newFile();
    void open();
    bool openHPDM();
    void save();
    void exitProgram();

    //edit menu
    void newComponent();
    void newLink();
    void enableDrag(bool compDrag);
    void switchPan();
    void switchSelect();
    void zoomToFit();

    //help menu
    void help();
    void about();

private:

    void initialize();
    void createActions();
    void createMenus();
    void createDockWindows();
    void createStatusBar();
    bool loadHPDMFile(QString name);
    void reportError(QString err);
    component* findComp(int i);

    myScene *scene;
    myView *view;
    QDockWidget* sysPicDock;
    QDockWidget* compListDock;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    QAction *newAct;
    QAction *openAct;
    QAction *openHPDMAct;
    QAction *saveAct;
    QAction *exitAct;
    QAction *newCompAct;
    QAction *newLinkAct;
    QAction *enableDragAct;
    QAction *helpAct;
    QAction *aboutAct;
    QAction *panAct;
    QAction *selectAct;
    QAction *zoomToFitAct;

    QLabel *sysPic;
    QToolBar *myToolBar;
    QListWidget *introduction;

    QStringList equationLines;


};

#endif // MYMAINWINDOW_H
