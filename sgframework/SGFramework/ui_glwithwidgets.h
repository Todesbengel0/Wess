/********************************************************************************
** Form generated from reading UI file 'glwithwidgets.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GLWITHWIDGETS_H
#define UI_GLWITHWIDGETS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include "openglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_GLwithWidgets
{
public:
    QWidget *RootWidget;
    QHBoxLayout *horizontalLayout;
    OpenGLWidget *myGLWidget;

    void setupUi(QMainWindow *GLwithWidgets)
    {
        if (GLwithWidgets->objectName().isEmpty())
            GLwithWidgets->setObjectName(QString::fromUtf8("GLwithWidgets"));
        GLwithWidgets->resize(1000, 600);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GLwithWidgets->sizePolicy().hasHeightForWidth());
        GLwithWidgets->setSizePolicy(sizePolicy);
        GLwithWidgets->setMinimumSize(QSize(600, 400));
        RootWidget = new QWidget(GLwithWidgets);
        RootWidget->setObjectName(QString::fromUtf8("RootWidget"));
        sizePolicy.setHeightForWidth(RootWidget->sizePolicy().hasHeightForWidth());
        RootWidget->setSizePolicy(sizePolicy);
        RootWidget->setMaximumSize(QSize(16777215, 16777215));
        horizontalLayout = new QHBoxLayout(RootWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        myGLWidget = new OpenGLWidget(RootWidget);
        myGLWidget->setObjectName(QString::fromUtf8("myGLWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(20);
        sizePolicy1.setVerticalStretch(20);
        sizePolicy1.setHeightForWidth(myGLWidget->sizePolicy().hasHeightForWidth());
        myGLWidget->setSizePolicy(sizePolicy1);
        myGLWidget->setMinimumSize(QSize(0, 0));
        myGLWidget->setMaximumSize(QSize(10000, 10000));
        myGLWidget->setSizeIncrement(QSize(1, 1));
        myGLWidget->setBaseSize(QSize(600, 400));
        myGLWidget->setMouseTracking(true);
        myGLWidget->setFocusPolicy(Qt::StrongFocus);
        myGLWidget->setLayoutDirection(Qt::LeftToRight);

        horizontalLayout->addWidget(myGLWidget);

        GLwithWidgets->setCentralWidget(RootWidget);

        retranslateUi(GLwithWidgets);

        QMetaObject::connectSlotsByName(GLwithWidgets);
    } // setupUi

    void retranslateUi(QMainWindow *GLwithWidgets)
    {
        GLwithWidgets->setWindowTitle(QCoreApplication::translate("GLwithWidgets", "QtGL", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GLwithWidgets: public Ui_GLwithWidgets {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GLWITHWIDGETS_H
