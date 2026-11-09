#ifndef REDACTOR_H
#define REDACTOR_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QComboBox>

class redactor : public QDialog
{
    Q_OBJECT

public:
    explicit redactor(int userId, int recordId, QWidget *parent = nullptr);
    ~redactor();

private slots:
    void on_saveButton_clicked();

private:
    int m_userId;
    int m_recordId;

    QLineEdit *enterDate;
    QLineEdit *enterSumm;
    QLineEdit *etnerText;
    QPushButton *saveButton;
    QComboBox *categoryBox;
};

#endif // REDACTOR_H