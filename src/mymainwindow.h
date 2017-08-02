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
    void enableLink(bool eLink);

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
    bool openHPDM();
    bool saveHPDM();
    void exitProgram();

    //edit menu
    void newComponent();
    void newLink();
    void enableDrag(bool compDrag);
    void enableDock(bool eDock);
    void switchPan();
    void switchSelect();
    void zoomToFit();
    void run();
    void paraRunSetup();
    void showResults();

    //help menu
    void help();
    void about();

private:

    void initialize();
    void createActions();
    void createMenus();
    void createDockWindows();
    void createStatusBar();

    QString loadResultFile(QString name);
    bool loadHPDMFile(QString name);
    bool saveHPDMFile(QString name);
    void reportError(QString err);
    component* findComp(int i);

    void closeEvent();
    int askToSave();
    void clearScene();

    myScene *scene;
    myView *view;
    QDockWidget* sysPicDock;
    QDockWidget* compListDock;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    QAction *newAct;
    QAction *openXMLAct;
    QAction *openHPDMAct;
    QAction *saveHPDMAct;
    QAction *exitAct;
    QAction *newCompAct;
    QAction *newLinkAct;
    QAction *enableDragAct;
    QAction *helpAct;
    QAction *aboutAct;
    QAction *panAct;
    QAction *selectAct;
    QAction *zoomToFitAct;
    QAction *enableDockAct;
    QAction *enableLinkAct;
    QAction *runAct;
    QAction *paraRunAct;
    QAction *showResultAct;

    QLabel *sysPic;
    QToolBar *myToolBar;
    QListWidget *introduction;

    QStringList batchRunLines;
    QString caseDirectory;


};

#endif // MYMAINWINDOW_H
