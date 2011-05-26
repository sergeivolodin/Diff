#include "diff.h"
#include "libdiff.cpp"
#include "ui_diff.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <math.h>
#include <string.h>
#include <strings.h>
using namespace std;
unsigned int sw=0;
rules* parsetable(QTableWidget* a,int type) {
    int i=0;
    char* buf=NULL;
    QTableWidgetItem *i1,*i2,*i3;
    rules* res=new rules;
    int c=a->rowCount();
    res->maxindex=-1;
    res->r=new rule*[c+1];
    while(i<c) {
        if(((i1=a->item(i,0))!=NULL)&&((i2=a->item(i,1))!=NULL)) {
            buf=i1->text().toAscii().data();
            buf=stradd(buf,MATH_CCOMMOND);
            buf=stradd(buf,print_num(type));
            buf=stradd(buf,MATH_CCOMMOND);
            buf=stradd(buf,i2->text().toAscii().data());
            i3=a->item(i,2);
            if(i3!=NULL&&i3->text()!=NULL) {
                buf=stradd(buf,MATH_CPOSTREPL);
                buf=stradd(buf,i3->text().toAscii().data());
            }
            if(buf!=NULL) {
                res->r[++(res->maxindex)]=parsestr(buf);
            }
        }
        i++;
    }
    return(res);
}
void rules_to_table(QTableWidget* a,int type,rules* R=FRULES) {
    if(R!=NULL&&a!=NULL&&R->r!=NULL) {
        if(R->maxindex>=0) {
            a->setRowCount(0);
            int i=0,si=0;
            a->setSortingEnabled(false);
            QTableWidgetItem* ti=NULL;
            while(i<=R->maxindex) {
                if(R->r[i]!=NULL&&R->r[i]->op==type) {
                    a->setRowCount(a->rowCount()+1);
                    ti=new QTableWidgetItem(QString(R->r[i]->src->display(TREE_DMATHB)));
                    if(ti!=NULL) {
                        a->setItem(si,0,ti);
                    }
                    ti=new QTableWidgetItem(QString(R->r[i]->dest->display(TREE_DMATHB)));
                    if(ti!=NULL) {
                        a->setItem(si,1,ti);
                    }
                    ti=new QTableWidgetItem(QString(postrepls_to_string(R->r[i]->postrepl,false)));
                    if(ti!=NULL) {
                        a->setItem(si,2,ti);
                        si++;
                    }
                }
                i++;
            }
        }
    }
}
void update_gui(Ui::Diff* UI) {
    UI->mnums->setText(QString(MATH_NUMS));
    UI->mall->setText(QString(MATH_FUNCS));
    UI->mvars->setText(QString(MATH_VARS));
    UI->mrepl->setText(QString(MATH_REPLS));
    UI->mvar0->setText(QString(MATH_DEFDIFF));
    UI->mops1->setText(QString(operations_to_string(MATH_OPS)));
    UI->mops->setText(QString(strings_merge(MATH_OPLIST)));
    rules_to_table(UI->tableWidget_2,0);
    rules_to_table(UI->tableWidget_3,1);
}
Diff::Diff(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::Diff)
{
    ui->setupUi(this);
}

Diff::~Diff() {
    delete ui;
};
void Diff::on_button_diff_clicked() {
    tree* src=parse(ui->line_src->toPlainText().toAscii().data());
    tree* dest=NULL;
    char* res=strcp(ERROR);
    const char* base=strcp(ui->line_base->text().toAscii().data());
    QString a;
    if(src!=NULL) {
        res="src ok";
        dest=operate(src,1,base);
        if(dest!=NULL) {
            res="dest ok";
            res=easy(dest)->display(TREE_DMATH);
        }
    }
    a=res;
    ui->line_dest->setPlainText(a);
}
void Diff::update_settings() {
    FRULES=rules_merge(parsetable(ui->tableWidget_3,1),parsetable(ui->tableWidget_2,0));
    MATH_NUMS=strcp(ui->mnums->text().toAscii().data());
    MATH_VARS=strcp(ui->mvars->text().toAscii().data());
    MATH_FUNCS=strcp(ui->mall->text().toAscii().data());
    MATH_REPLS=strcp(ui->mrepl->text().toAscii().data());
    MATH_DEFDIFF=strcp(ui->mvar0->text().toAscii().data());
    MATH_OPS=string_to_operations(strcp(ui->mops1->text().toAscii().data()));
    MATH_OPLIST=explode(strcp(ui->mops->text().toAscii().data()),' ');
    //printrules(FRULES);
}
void Diff::on_button_parse_clicked() {
    update_settings();
    if(ui->checkBox->isChecked()) {
        settings_to_file();
    }
}
void Diff::on_button_easy_clicked() {
    tree* src=parse(ui->line_src->toPlainText().toAscii().data());
    tree* dest=NULL;
    char* res=strcp(ERROR);
    QString a;
    if(src!=NULL) {
        res="src ok";
        dest=easy(src);
        if(dest!=NULL) {
            res=dest->display(TREE_DMATH);
        }
    }
    a=res;
    ui->line_dest->setPlainText(a);
}
void Diff::on_button_show_clicked() {
    tree* src=parse(ui->line_src->toPlainText().toAscii().data());
    char* res=strcp(ERROR);
    QString a;
    if(src!=NULL) {
        res=src->display(TREE_DMATH);
    }
    a=res;
    ui->line_dest->setPlainText(a);
}
void Diff::changeEvent(QEvent *e) {
    if(sw<=1) {
        if(sw==1) {
            settings_from_file();
            update_gui(ui);
            update_settings();
        }
        sw++;
    }
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
void Diff::on_pushButton_2_clicked() {
    ui->tableWidget_3->setRowCount(ui->tableWidget_3->rowCount()+1);
}

void Diff::on_pushButton_3_clicked() {
    ui->tableWidget_3->setRowCount(ui->tableWidget_3->rowCount()-1);
}

void Diff::on_pushButton_4_clicked() {
    ui->tableWidget_2->setRowCount(ui->tableWidget_2->rowCount()+1);
}

void Diff::on_pushButton_5_clicked() {
    ui->tableWidget_2->setRowCount(ui->tableWidget_2->rowCount()-1);
}
