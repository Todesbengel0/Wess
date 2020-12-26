#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class GLwithWidgets;
}

//! Ist der Startpunkt für die gesamte Applikation. Im Konstruktor muss die GUI und alle sonstigen Objekte instanziert
//! und initialisiert werden.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    virtual ~MainWindow();
    static Ui::GLwithWidgets* getUi();

private:
    static Ui::GLwithWidgets* sUi;
};

#endif // MAINWINDOW_H
