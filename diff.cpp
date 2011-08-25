#include "diff.h"
#include "libdiff.cpp"
#include "ui_diff.h"
using namespace std;
unsigned int sw=0;
rules* parsetable(QTableWidget* a,int type) {
    int i=0;
    char* buf=NULL;
    QTableWidgetItem *i1,*i2=NULL,*i3;
    rules* res=new rules;
    int c=a->rowCount();
    res->maxindex=-1;
    res->r=new rule*[c+1];
    while(i<c) {
        if(((i1=a->item(i,0))!=NULL)&&((i2=a->item(i,1))!=NULL)&&((i1->text())!=NULL)&&((i2->text())!=NULL)) {
            buf=i1->text().toAscii().data();
            buf=stradd(buf,MATH_CCOMMOND);
            buf=stradd(buf,print_num(type));
            buf=stradd(buf,MATH_CCOMMOND);
            buf=stradd(buf,i2->text().toAscii().data());
            i3=a->item(i,2);
            if(i3&&i3->text()!=NULL) {
                buf=stradd(buf,MATH_CPOSTREPL);
                buf=stradd(buf,i3->text().toAscii().data());
            }
            if(buf) res->r[++(res->maxindex)]=parsestr(buf);
        }
        i++;
    }
    return(res);
}
void rules_to_table(QTableWidget* a,int type,rules* R=FRULES) {
    if(R&&a&&R->r) {
        if(R->maxindex>=0) {
            a->setRowCount(0);
            int i=0,si=0;
            a->setSortingEnabled(false);
            QTableWidgetItem* ti=NULL;
            while(i<=R->maxindex) {
                if(R->r[i]&&R->r[i]->op==type) {
                    a->setRowCount(a->rowCount()+1);
                    ti=new QTableWidgetItem(QString(R->r[i]->src->display(TREE_DMATHB)));
                    if(ti) a->setItem(si,0,ti);
                    ti=new QTableWidgetItem(QString(R->r[i]->dest->display(TREE_DMATHB)));
                    if(ti) a->setItem(si,1,ti);
                    ti=new QTableWidgetItem(QString(postrepls_to_string(R->r[i]->postrepl,false)));
                    if(ti) {
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
    UI->line_base->setText(QString(MATH_DEFDIFF));
    UI->mops1->setText(QString(operations_to_string(MATH_OPS)));
    UI->mops->setText(QString(strings_merge(MATH_OPLIST)));
    UI->checkBox_2->setChecked(FULLEASY);
    UI->comboBox->setCurrentIndex(DEBUG);
    UI->spinBox->setValue(MAXITER);
    rules_to_table(UI->tableWidget_2,0);
    rules_to_table(UI->tableWidget_3,1);
}
Diff::Diff(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::Diff)
{
    ui->setupUi(this);
}

Diff::~Diff() {delete ui;};
void Diff::on_button_diff_clicked() {
    long int t=mtime();
    tree* src=parse(ui->line_src->toPlainText().toAscii().data());
    tree* dest=NULL;
    char* res=strcp(ERROR);
    const char* base=strcp(ui->line_base->text().toAscii().data());
    QString a;
    if(src) {
        res=strcp("src ok");
        dest=operate(src,1,base);
        if(dest) {
            res=strcp("dest ok");
            res=dest->display(TREE_DMATH);
        }
    }
    a=res;
    ui->line_dest->setPlainText(a);
    ui->label_13->setText(QString(stradd(print_num(mtime()-t)," ms")));
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
    MAXITER=ui->spinBox->value();
    FULLEASY=ui->checkBox_2->isChecked();
    DEBUG=ui->comboBox->currentIndex();
}
void Diff::on_button_parse_clicked() {
    update_settings();
    if(ui->checkBox->isChecked()) settings_to_file();
}
void Diff::on_button_easy_clicked() {
    long int t=mtime();
    tree* src=parse(ui->line_src->toPlainText().toAscii().data());
    tree* dest=NULL;
    char* res=strcp(ERROR);
    QString a;
    if(src) {
        res=strcp("src ok");
        dest=easy(src);
        if(dest) res=dest->display(TREE_DMATH);
    }
    a=res;
    ui->line_dest->setPlainText(a);
    ui->label_13->setText(QString(stradd(print_num(mtime()-t)," ms")));
}
void Diff::on_button_show_clicked() {
    long int t=mtime();
    tree* src=parse(ui->line_src->toPlainText().toAscii().data());
    char* res=strcp(ERROR);
    QString a;
    if(src) res=src->display(TREE_DMATHB);
    a=res;
    ui->line_dest->setPlainText(a);
    ui->label_13->setText(QString(stradd(print_num(mtime()-t)," ms")));
}
void Diff::changeEvent(QEvent *e) {
    if(sw<=1) {
        if(sw==1) {
            settings_from_file();
            update_gui(ui);
        }
        sw++;
    }
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default: break;
    }
}
void Diff::on_pushButton_2_clicked() {ui->tableWidget_3->setRowCount(ui->tableWidget_3->rowCount()+1);}
void Diff::on_pushButton_3_clicked() {ui->tableWidget_3->setRowCount(ui->tableWidget_3->rowCount()-1);}
void Diff::on_pushButton_4_clicked() {ui->tableWidget_2->setRowCount(ui->tableWidget_2->rowCount()+1);}
void Diff::on_pushButton_5_clicked() {ui->tableWidget_2->setRowCount(ui->tableWidget_2->rowCount()-1);}
