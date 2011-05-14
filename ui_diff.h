/********************************************************************************
** Form generated from reading UI file 'diff.ui'
**
** Created: Sat May 14 12:55:18 2011
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
#include <QtGui/QCheckBox>
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
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Diff
{
public:
    QAction *actionExit;
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab_main;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *button_diff;
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
    QPushButton *button_parse;
    QPlainTextEdit *plainTextEdit_2;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QVBoxLayout *verticalLayout_3;
    QLineEdit *mall;
    QLineEdit *mvars;
    QLineEdit *mnums;
    QLineEdit *mops;
    QLineEdit *mrepl;
    QLineEdit *mvar0;
    QLineEdit *mops1;
    QTabWidget *tabWidget_2;
    QWidget *tab_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QTableWidget *tableWidget_2;
    QWidget *tab_4;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QTableWidget *tableWidget_3;
    QCheckBox *checkBox;
    QWidget *tab;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *pushButton;
    QPlainTextEdit *teste;
    QPlainTextEdit *testp;
    QPlainTextEdit *testr;
    QPlainTextEdit *mreplacers;
    QLabel *label_8;
    QPlainTextEdit *testr1;
    QPlainTextEdit *testr_2;
    QWidget *tab_2;
    QLineEdit *testv_src;
    QPlainTextEdit *testv_dest;
    QPushButton *testv_button;
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
        horizontalLayoutWidget_3->setGeometry(QRect(0, 90, 631, 84));
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
        groupBox_2->setGeometry(QRect(90, 200, 541, 111));
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
        verticalLayoutWidget_2->setGeometry(QRect(0, 170, 186, 31));
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
        button_parse = new QPushButton(tab_settings);
        button_parse->setObjectName(QString::fromUtf8("button_parse"));
        button_parse->setGeometry(QRect(-1, 376, 111, 41));
        plainTextEdit_2 = new QPlainTextEdit(tab_settings);
        plainTextEdit_2->setObjectName(QString::fromUtf8("plainTextEdit_2"));
        plainTextEdit_2->setGeometry(QRect(473, 26, 161, 171));
        plainTextEdit_2->setFrameShape(QFrame::NoFrame);
        plainTextEdit_2->setReadOnly(true);
        plainTextEdit_2->setTextInteractionFlags(Qt::NoTextInteraction);
        horizontalLayoutWidget = new QWidget(tab_settings);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(476, 198, 160, 219));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_5 = new QLabel(horizontalLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout->addWidget(label_5);

        label_6 = new QLabel(horizontalLayoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        verticalLayout->addWidget(label_6);

        label_7 = new QLabel(horizontalLayoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        verticalLayout->addWidget(label_7);

        label_9 = new QLabel(horizontalLayoutWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        verticalLayout->addWidget(label_9);

        label_10 = new QLabel(horizontalLayoutWidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        verticalLayout->addWidget(label_10);

        label_11 = new QLabel(horizontalLayoutWidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        verticalLayout->addWidget(label_11);

        label_12 = new QLabel(horizontalLayoutWidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        verticalLayout->addWidget(label_12);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        mall = new QLineEdit(horizontalLayoutWidget);
        mall->setObjectName(QString::fromUtf8("mall"));

        verticalLayout_3->addWidget(mall);

        mvars = new QLineEdit(horizontalLayoutWidget);
        mvars->setObjectName(QString::fromUtf8("mvars"));

        verticalLayout_3->addWidget(mvars);

        mnums = new QLineEdit(horizontalLayoutWidget);
        mnums->setObjectName(QString::fromUtf8("mnums"));

        verticalLayout_3->addWidget(mnums);

        mops = new QLineEdit(horizontalLayoutWidget);
        mops->setObjectName(QString::fromUtf8("mops"));

        verticalLayout_3->addWidget(mops);

        mrepl = new QLineEdit(horizontalLayoutWidget);
        mrepl->setObjectName(QString::fromUtf8("mrepl"));

        verticalLayout_3->addWidget(mrepl);

        mvar0 = new QLineEdit(horizontalLayoutWidget);
        mvar0->setObjectName(QString::fromUtf8("mvar0"));

        verticalLayout_3->addWidget(mvar0);

        mops1 = new QLineEdit(horizontalLayoutWidget);
        mops1->setObjectName(QString::fromUtf8("mops1"));

        verticalLayout_3->addWidget(mops1);


        horizontalLayout->addLayout(verticalLayout_3);

        tabWidget_2 = new QTabWidget(tab_settings);
        tabWidget_2->setObjectName(QString::fromUtf8("tabWidget_2"));
        tabWidget_2->setGeometry(QRect(0, 0, 471, 371));
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        pushButton_4 = new QPushButton(tab_3);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(0, 316, 31, 27));
        pushButton_5 = new QPushButton(tab_3);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(30, 316, 31, 27));
        tableWidget_2 = new QTableWidget(tab_3);
        if (tableWidget_2->columnCount() < 3)
            tableWidget_2->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        if (tableWidget_2->rowCount() < 1)
            tableWidget_2->setRowCount(1);
        tableWidget_2->setObjectName(QString::fromUtf8("tableWidget_2"));
        tableWidget_2->setGeometry(QRect(0, 0, 471, 311));
        tableWidget_2->setLineWidth(1);
        tableWidget_2->setMidLineWidth(0);
        tableWidget_2->setRowCount(1);
        tableWidget_2->horizontalHeader()->setDefaultSectionSize(123);
        tableWidget_2->horizontalHeader()->setStretchLastSection(true);
        tabWidget_2->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        pushButton_2 = new QPushButton(tab_4);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(0, 316, 31, 27));
        pushButton_3 = new QPushButton(tab_4);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(30, 316, 31, 27));
        tableWidget_3 = new QTableWidget(tab_4);
        if (tableWidget_3->columnCount() < 3)
            tableWidget_3->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(2, __qtablewidgetitem5);
        if (tableWidget_3->rowCount() < 1)
            tableWidget_3->setRowCount(1);
        tableWidget_3->setObjectName(QString::fromUtf8("tableWidget_3"));
        tableWidget_3->setGeometry(QRect(0, 0, 471, 311));
        tableWidget_3->setLineWidth(1);
        tableWidget_3->setMidLineWidth(0);
        tableWidget_3->setRowCount(1);
        tableWidget_3->horizontalHeader()->setDefaultSectionSize(123);
        tableWidget_3->horizontalHeader()->setStretchLastSection(true);
        tabWidget_2->addTab(tab_4, QString());
        checkBox = new QCheckBox(tab_settings);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(110, 387, 91, 19));
        tabWidget->addTab(tab_settings, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 30, 52, 16));
        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 70, 52, 13));
        pushButton = new QPushButton(tab);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(10, 150, 321, 41));
        teste = new QPlainTextEdit(tab);
        teste->setObjectName(QString::fromUtf8("teste"));
        teste->setGeometry(QRect(70, 30, 271, 31));
        testp = new QPlainTextEdit(tab);
        testp->setObjectName(QString::fromUtf8("testp"));
        testp->setGeometry(QRect(70, 70, 271, 31));
        testr = new QPlainTextEdit(tab);
        testr->setObjectName(QString::fromUtf8("testr"));
        testr->setGeometry(QRect(10, 200, 111, 31));
        mreplacers = new QPlainTextEdit(tab);
        mreplacers->setObjectName(QString::fromUtf8("mreplacers"));
        mreplacers->setGeometry(QRect(140, 200, 331, 171));
        label_8 = new QLabel(tab);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 110, 61, 16));
        testr1 = new QPlainTextEdit(tab);
        testr1->setObjectName(QString::fromUtf8("testr1"));
        testr1->setGeometry(QRect(350, 20, 251, 161));
        testr_2 = new QPlainTextEdit(tab);
        testr_2->setObjectName(QString::fromUtf8("testr_2"));
        testr_2->setGeometry(QRect(70, 110, 271, 31));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        testv_src = new QLineEdit(tab_2);
        testv_src->setObjectName(QString::fromUtf8("testv_src"));
        testv_src->setGeometry(QRect(0, 10, 291, 41));
        testv_dest = new QPlainTextEdit(tab_2);
        testv_dest->setObjectName(QString::fromUtf8("testv_dest"));
        testv_dest->setGeometry(QRect(0, 60, 431, 351));
        testv_button = new QPushButton(tab_2);
        testv_button->setObjectName(QString::fromUtf8("testv_button"));
        testv_button->setGeometry(QRect(300, 10, 131, 41));
        tabWidget->addTab(tab_2, QString());
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(0, 445, 61, 16));
        label_2->setTextFormat(Qt::RichText);
        Diff->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Diff);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 640, 19));
        menuDiff = new QMenu(menuBar);
        menuDiff->setObjectName(QString::fromUtf8("menuDiff"));
        Diff->setMenuBar(menuBar);

        menuBar->addAction(menuDiff->menuAction());
        menuDiff->addAction(actionExit);

        retranslateUi(Diff);
        QObject::connect(actionExit, SIGNAL(activated()), Diff, SLOT(close()));

        tabWidget->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Diff);
    } // setupUi

    void retranslateUi(QMainWindow *Diff)
    {
        Diff->setWindowTitle(QApplication::translate("Diff", "Diff", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("Diff", "\320\222\321\213\321\205\320\276\320\264", 0, QApplication::UnicodeUTF8));
        actionExit->setShortcut(QApplication::translate("Diff", "Alt+Q", 0, QApplication::UnicodeUTF8));
        button_diff->setText(QApplication::translate("Diff", "\320\237\321\200\320\276\320\270\320\267\320\262\320\276\320\264\320\275\320\260\321\217", 0, QApplication::UnicodeUTF8));
        button_easy->setText(QApplication::translate("Diff", "\320\243\320\277\321\200\320\276\321\201\321\202\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        button_show->setText(QApplication::translate("Diff", "\320\237\320\276\320\272\320\260\320\267\320\260\321\202\321\214", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("Diff", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202:", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Diff", "\320\222\321\213\321\200\320\260\320\266\320\265\320\275\320\270\320\265:", 0, QApplication::UnicodeUTF8));
        line_src->setPlainText(QApplication::translate("Diff", "sin(x+cos(x)^(x+3))+cos(tg(x))+erf(x)", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Diff", "\320\236\321\201\320\275\320\276\320\262\320\260\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8));
        line_base->setText(QApplication::translate("Diff", "x", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_main), QApplication::translate("Diff", "\320\236\321\201\320\275\320\276\320\262\320\275\320\276\320\265 \320\276\320\272\320\275\320\276", 0, QApplication::UnicodeUTF8));
        button_parse->setText(QApplication::translate("Diff", "\320\237\321\200\320\270\320\274\320\265\320\275\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        plainTextEdit_2->setPlainText(QApplication::translate("Diff", "\320\222\320\277\320\270\321\201\321\213\320\262\320\260\320\271\321\202\320\265 \320\277\321\200\320\260\320\262\320\270\320\273\320\260 \320\262 \321\201\320\276\320\276\321\202\320\262\320\265\321\202\321\201\321\202\320\262\321\203\321\216\321\211\321\203\321\216 \321\202\320\260\320\261\320\273\320\270\321\206\321\203.\n"
"\320\235\320\265\320\277\321\200\320\260\320\262\320\270\320\273\321\214\320\275\321\213\320\265 \320\261\321\203\320\264\321\203\321\202 \320\277\321\200\320\276\320\270\320\263\320\275\320\276\321\200\320\270\321\200\320\276\320\262\320\260\320\275\321\213\n"
"\n"
"\320\267\320\260\320\274\320\265\320\275\320\260 - \320\262\321\213\321\200\320\260\320\266\320\265\320\275\320\270\320\265 \320\262\320\270\320\264\320\260 [a]=[b]. \320\277\320\276\321\201\320\273\320\265 \320\262\321\213\320\277\320\276\320\273\320\275\320\265\320\275\320\270\321\217 \321\201\320\276\320\276\321\202\320\262\320\265\321\202\321\201\321\202\320\262\321\203\321\216\321\211\320\265\320\271 \320\276\320\277\320"
                        "\265\321\200\320\260\321\206\320\270\320\270 \320\262\321\201\320\265 \"a\" \320\262 \320\264\320\265\321\200\320\265\320\262\320\265 \320\261\321\203\320\264\321\203\321\202 \320\267\320\260\320\274\320\265\320\275\320\265\320\275\321\213 \320\275\320\260 \"b\"\n"
"\n"
"\321\202\320\260\320\272\320\266\320\265 \320\277\320\276\320\274\320\270\320\274\320\276 \320\276\320\261\321\213\321\207\320\275\321\213\321\205 \321\204\321\203\320\275\320\272\321\206\320\270\320\271 (sin, cos...) \320\274\320\276\320\266\320\275\320\276 \320\270\321\201\320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\321\214 \320\276\320\277\320\265\321\200\320\260\321\206\320\270\320\270 (\320\277\321\200. \320\264\320\270\321\204\321\204./\320\270\320\275\321\202.). \321\215\321\202\320\276 \321\201\320\276\320\267\320\264\320\260\321\201\321\202 \321\200\320\265\320\272\321\203\321\200\321\201\320\270\321\216 \320\270 \320\277\320\276\320\267\320\262\320\276\320\273\320\270\321\202 \320\277\320\276\321\201\321\207"
                        "\320\270\321\202\320\260\321\202\321\214, \320\275\320\260\320\277\321\200\320\270\320\274\320\265\321\200, \320\277\321\200\320\276\320\270\320\267\320\262\320\276\320\264\320\275\321\203\321\216 \321\201\321\203\320\274\320\274\321\213.\n"
"\320\264\320\273\321\217 \321\215\321\202\320\276\320\263\320\276 \320\275\320\260\320\277\320\270\321\210\320\270\321\202\320\265 diff(f) \320\270\320\273\320\270 diffA(f). \320\262 \320\277\320\265\321\200\320\262\320\276\320\274 \321\201\320\273\321\203\321\207\320\260\320\265 \320\276\321\201\320\275\320\276\320\262\320\260\320\275\320\270\320\265\320\274 \320\261\321\203\320\264\320\265\321\202 \"x\", \320\260 \320\262\320\276 \320\262\321\202\320\276\321\200\320\276\320\274 - A. \320\262 \320\272\320\260\321\207\320\265\321\201\321\202\320\262\320\265 A \320\274\320\276\320\266\320\265\321\202 \320\262\321\213\321\201\321\202\321\203\320\277\320\260\321\202\321\214 \321\202\320\276\320\273\321\214\320\272\320\276 \320\276\320\264\320\270\320\275 \321\201\320\270\320"
                        "\274\320\262\320\276\320\273, \320\277\321\200\320\270\321\207\320\265\320\274 \320\270\320\267 \320\275\320\260\320\261\320\276\321\200\320\260 \320\267\320\260\320\274\320\265\320\275 \320\270\320\273\320\270 \320\277\320\265\321\200\320\265\320\274\320\265\320\275\320\275\321\213\321\205.", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Diff", "\320\233\321\216\320\261\321\213\320\265", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Diff", "\320\237\320\265\321\200\320\265\320\274\320\265\320\275\320\275\321\213\320\265", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Diff", "\320\247\320\270\321\201\320\273\320\260", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("Diff", "\320\236\320\277\320\265\321\200\320\260\321\206\320\270\320\270", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("Diff", "\320\227\320\260\320\274\320\265\320\275\321\213", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("Diff", "\320\236\321\201\320\275. \320\277\320\265\321\200\320\265\320\274.", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("Diff", "\320\255\320\273\320\265\320\274. \320\276\320\277\320\265\321\200.", 0, QApplication::UnicodeUTF8));
        mall->setText(QApplication::translate("Diff", "fgh", 0, QApplication::UnicodeUTF8));
        mvars->setText(QApplication::translate("Diff", "xyz", 0, QApplication::UnicodeUTF8));
        mnums->setText(QApplication::translate("Diff", "abcdef", 0, QApplication::UnicodeUTF8));
        mops->setText(QApplication::translate("Diff", "diff integral", 0, QApplication::UnicodeUTF8));
        mrepl->setText(QApplication::translate("Diff", "mnop", 0, QApplication::UnicodeUTF8));
        mvar0->setText(QApplication::translate("Diff", "x", 0, QApplication::UnicodeUTF8));
        mops1->setText(QApplication::translate("Diff", "+-*/^_!@#$%&;:|\\", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        tab_3->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        pushButton_4->setText(QApplication::translate("Diff", "+", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("Diff", "-", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_2->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("Diff", "\320\235\320\260\321\207\320\260\320\273\321\214\320\275\320\276\320\265", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_2->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("Diff", "\320\232\320\276\320\275\320\265\321\207\320\275\320\276\320\265", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_2->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("Diff", "\320\227\320\260\320\274\320\265\320\275\321\213", 0, QApplication::UnicodeUTF8));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_3), QApplication::translate("Diff", "\320\243\320\277\321\200\320\276\321\211\320\265\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("Diff", "+", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("Diff", "-", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_3->horizontalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("Diff", "\320\235\320\260\321\207\320\260\320\273\321\214\320\275\320\276\320\265", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_3->horizontalHeaderItem(1);
        ___qtablewidgetitem4->setText(QApplication::translate("Diff", "\320\232\320\276\320\275\320\265\321\207\320\275\320\276\320\265", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_3->horizontalHeaderItem(2);
        ___qtablewidgetitem5->setText(QApplication::translate("Diff", "\320\227\320\260\320\274\320\265\320\275\321\213", 0, QApplication::UnicodeUTF8));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_4), QApplication::translate("Diff", "\320\237\321\200\320\276\320\270\320\267\320\262\320\276\320\264\320\275\320\260\321\217", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("Diff", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_settings), QApplication::translate("Diff", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Diff", "\320\222\321\213\321\200\320\260\320\266\320\265\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Diff", "\320\250\320\260\320\261\320\273\320\276\320\275", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Diff", "\320\242\320\265\321\201\321\202\320\270\321\200\320\276\320\262\320\260\321\202\321\214", 0, QApplication::UnicodeUTF8));
        teste->setPlainText(QApplication::translate("Diff", "3*x+x", 0, QApplication::UnicodeUTF8));
        testp->setPlainText(QApplication::translate("Diff", "a*f+f", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("Diff", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202", 0, QApplication::UnicodeUTF8));
        testr_2->setPlainText(QApplication::translate("Diff", "(a+1)*f", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Diff", "\320\242\320\265\321\201\321\202\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265 (\321\210\320\260\320\261\320\273\320\276\320\275\321\213)", 0, QApplication::UnicodeUTF8));
        testv_button->setText(QApplication::translate("Diff", "\320\222\320\260\321\200\320\270\320\260\320\275\321\202\321\213", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Diff", "\320\242\320\265\321\201\321\202\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265 (\320\262\320\260\321\200\320\270\320\260\320\275\321\202\321\213)", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Diff", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"http://i.179e.net\"><span style=\" text-decoration: underline; color:#0000ff;\">i.179e.net</span></a></p></body></html>", 0, QApplication::UnicodeUTF8));
        menuDiff->setTitle(QApplication::translate("Diff", "\320\237\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\260", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Diff: public Ui_Diff {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIFF_H
