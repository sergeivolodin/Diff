#ifndef DIFF_H
#define DIFF_H

#include <QMainWindow>

namespace Ui {
    class Diff;
}

class Diff : public QMainWindow {
    Q_OBJECT
public:
    Diff(QWidget *parent = 0);
    ~Diff();

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_button_parse_clicked();
    void on_button_diff_clicked();
    void on_button_show_clicked();
    void on_button_easy_clicked();
private:
    Ui::Diff *ui;
    void update_settings();

};

#endif // DIFF_H
