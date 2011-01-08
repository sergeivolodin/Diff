/********************************************************************************
** Form generated from reading UI file 'diff.ui'
**
** Created: Fri Jan 7 20:57:31 2011
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIFF_H
#define UI_DIFF_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Diff
{
public:
    QAction *actionExit;
    QAction *actionAbout;
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab_main;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *button_diff;
    QPushButton *button_integral;
    QPushButton *button_easy;
    QPushButton *button_show;
    QGroupBox *groupBox_2;
    QPlainTextEdit *line_dest;
    QGroupBox *groupBox;
    QPlainTextEdit *line_src;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *line_base;
    QWidget *tab_settings;
    QLabel *label_3;
    QLabel *label_2;
    QMenuBar *menuBar;
    QMenu *menuDiff;

    void setupUi(QMainWindow *Diff)
    {
        if (Diff->objectName().isEmpty())
            Diff->setObjectName(QString::fromUtf8("Diff"));
        Diff->resize(640, 480);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Diff->sizePolicy().hasHeightForWidth());
        Diff->setSizePolicy(sizePolicy);
        Diff->setMinimumSize(QSize(640, 480));
        Diff->setMaximumSize(QSize(640, 480));
        Diff->setFocusPolicy(Qt::TabFocus);
        actionExit = new QAction(Diff);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionAbout = new QAction(Diff);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        centralWidget = new QWidget(Diff);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 640, 445));
        tab_main = new QWidget();
        tab_main->setObjectName(QString::fromUtf8("tab_main"));
        horizontalLayoutWidget_3 = new QWidget(tab_main);
        horizontalLayoutWidget_3->setObjectName(QString::fromUtf8("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(0, 90, 631, 54));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetMaximumSize);
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        button_diff = new QPushButton(horizontalLayoutWidget_3);
        button_diff->setObjectName(QString::fromUtf8("button_diff"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(button_diff->sizePolicy().hasHeightForWidth());
        button_diff->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(button_diff);

        button_integral = new QPushButton(horizontalLayoutWidget_3);
        button_integral->setObjectName(QString::fromUtf8("button_integral"));
        sizePolicy1.setHeightForWidth(button_integral->sizePolicy().hasHeightForWidth());
        button_integral->setSizePolicy(sizePolicy1);
        button_integral->setFlat(false);

        horizontalLayout_3->addWidget(button_integral);

        button_easy = new QPushButton(horizontalLayoutWidget_3);
        button_easy->setObjectName(QString::fromUtf8("button_easy"));
        sizePolicy1.setHeightForWidth(button_easy->sizePolicy().hasHeightForWidth());
        button_easy->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(button_easy);

        button_show = new QPushButton(horizontalLayoutWidget_3);
        button_show->setObjectName(QString::fromUtf8("button_show"));
        button_show->setEnabled(true);
        sizePolicy1.setHeightForWidth(button_show->sizePolicy().hasHeightForWidth());
        button_show->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(button_show);

        groupBox_2 = new QGroupBox(tab_main);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(150, 150, 541, 111));
        line_dest = new QPlainTextEdit(groupBox_2);
        line_dest->setObjectName(QString::fromUtf8("line_dest"));
        line_dest->setEnabled(true);
        line_dest->setGeometry(QRect(10, 20, 474, 91));
        line_dest->setReadOnly(true);
        groupBox = new QGroupBox(tab_main);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(0, 0, 640, 91));
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        line_src = new QPlainTextEdit(groupBox);
        line_src->setObjectName(QString::fromUtf8("line_src"));
        line_src->setGeometry(QRect(0, 20, 634, 61));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(line_src->sizePolicy().hasHeightForWidth());
        line_src->setSizePolicy(sizePolicy2);
        verticalLayoutWidget_2 = new QWidget(tab_main);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(0, 150, 151, 31));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(verticalLayoutWidget_2);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        line_base = new QLineEdit(verticalLayoutWidget_2);
        line_base->setObjectName(QString::fromUtf8("line_base"));
        sizePolicy.setHeightForWidth(line_base->sizePolicy().hasHeightForWidth());
        line_base->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(line_base);


        verticalLayout_2->addLayout(horizontalLayout_2);

        tabWidget->addTab(tab_main, QString());
        tab_settings = new QWidget();
        tab_settings->setObjectName(QString::fromUtf8("tab_settings"));
        label_3 = new QLabel(tab_settings);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(270, 200, 81, 16));
        label_3->setAlignment(Qt::AlignCenter);
        tabWidget->addTab(tab_settings, QString());
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(0, 445, 61, 16));
        label_2->setTextFormat(Qt::RichText);
        Diff->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Diff);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 640, 21));
        menuDiff = new QMenu(menuBar);
        menuDiff->setObjectName(QString::fromUtf8("menuDiff"));
        Diff->setMenuBar(menuBar);

        menuBar->addAction(menuDiff->menuAction());
        menuDiff->addAction(actionExit);

        retranslateUi(Diff);
        QObject::connect(actionExit, SIGNAL(activated()), Diff, SLOT(close()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Diff);
    } // setupUi

    void retranslateUi(QMainWindow *Diff)
    {
        Diff->setWindowTitle(QApplication::translate("Diff", "Diff", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("Diff", "Exit", 0, QApplication::UnicodeUTF8));
        actionExit->setShortcut(QApplication::translate("Diff", "Alt+Q", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("Diff", "About", 0, QApplication::UnicodeUTF8));
        button_diff->setText(QApplication::translate("Diff", "Derivative", 0, QApplication::UnicodeUTF8));
        button_integral->setText(QApplication::translate("Diff", "Integral", 0, QApplication::UnicodeUTF8));
        button_easy->setText(QApplication::translate("Diff", "Easy", 0, QApplication::UnicodeUTF8));
        button_show->setText(QApplication::translate("Diff", "Show", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("Diff", "Result:", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Diff", "Equal:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Diff", "Base", 0, QApplication::UnicodeUTF8));
        line_base->setText(QApplication::translate("Diff", "x", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_main), QApplication::translate("Diff", "Main", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Diff", "Coming soon", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_settings), QApplication::translate("Diff", "Settings", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Diff", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"http://i.179e.net\"><span style=\" text-decoration: underline; color:#0000ff;\">i.179e.net</span></a></p></body></html>", 0, QApplication::UnicodeUTF8));
        menuDiff->setTitle(QApplication::translate("Diff", "Program", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Diff: public Ui_Diff {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIFF_H
