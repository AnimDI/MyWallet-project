#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QStatusBar>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QIntValidator>
#include <QComboBox>
#include <QMessageBox>
#include <redactor.h>
#include <QStackedWidget>
#include <QDateEdit>
class secondwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit secondwindow(int userId, QString userLogin, QWidget *parent = nullptr);
    ~secondwindow();

private slots:
    void on_pushButton_clicked();
    void on_reload_clicked();
    void on_reload_2_clicked();
    void on_list_itemDoubleClicked(QListWidgetItem *item);
    void on_search_cliked();
    void on_save_limit_clicked();
    void on_reload_limits_clicked();
    QString returnCategory(QString category);

private:
    int m_userId;
    QString m_userLogin;

    QListWidget *list;
    QLineEdit *enterSumm;
    QLineEdit *etnerText;
    //QLineEdit *enterDate;
    QDateEdit *enterDate;

    QPushButton *pushButton;
    QPushButton *reload;
    QPushButton *reload_2;

    QLabel *balance;
    QLabel *rasxod;
    QLabel *doxod;
    QStatusBar *statusbar;
    QLabel *status;
    redactor *editor;
    QSqlDatabase db;
    QLineEdit *enterSearch;
    QPushButton *searchButton;
    QComboBox *categoryBox;
    QMessageBox *messageBox;
    QStackedWidget *stackedWidget;
    QWidget *sidebar;

    QLineEdit *enterSummLimit;
    QComboBox *categoryBoxLimit;
    QListWidget *limitsList;
    QLabel *limitsStatus;
    QPushButton *saveLimitBtn;
    QPushButton *reloadLimitBtn;
    QWidget *mainPage;
    QWidget *limitPage;

    QPushButton *mainPage_button;
    QPushButton *limitPage_button;
};

#endif // SECONDWINDOW_H