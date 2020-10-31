#include "mainwindow.h"
#include "ui_glwithwidgets.h"
#include "scenemanager.h"

Ui::GLwithWidgets* MainWindow::sUi = 0;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    sUi = new Ui::GLwithWidgets();
    sUi->setupUi(this);

    SceneManager::instance()->setWindowReference(this, this->getUi());
    SceneManager::instance()->connectUiSignals(this->getUi());
}

MainWindow::~MainWindow()
{
    delete sUi;
}

Ui::GLwithWidgets* MainWindow::getUi()
{
    return sUi;
}
