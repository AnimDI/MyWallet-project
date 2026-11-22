#include "redactor.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlQuery>
#include <QDebug>

redactor::redactor(int userId, int recordId, QWidget *parent)
    : QDialog(parent), m_userId(userId), m_recordId(recordId)
{
    setWindowTitle("Редактирование");
    setFixedSize(600, 400);

    QWidget *central = new QWidget(this);
    QGridLayout *mainLayout = new QGridLayout(central);
    setLayout(mainLayout);

    QLabel *dateLabel = new QLabel("Дата");
    dateLabel->setAlignment(Qt::AlignCenter);
    QLabel *summLabel = new QLabel("Сумма");
    summLabel->setAlignment(Qt::AlignCenter);
    QLabel *commentLabel = new QLabel("Комментарий");
    commentLabel->setAlignment(Qt::AlignCenter);
    QLabel *categoryLabel = new QLabel("Категория");
    categoryLabel->setAlignment(Qt::AlignCenter);



    enterDate = new QDateEdit();
    enterDate->setStyleSheet("QDateEdit { background-color: white; color: #1a3b2e; font-family: 'Segoe UI', 'Arial', sans-serif; font-size: 13px; font-weight: 500; border: 1.5px solid #2e7d5e; border-radius: 18px; padding: 4px 10px; min-width: 140px; min-height: 8px; max-height: 24px; }");

    enterSumm = new QLineEdit();
    enterSumm->setStyleSheet("QLineEdit { background-color: white; color: #1a3b2e; font-family: 'Segoe UI', 'Arial', sans-serif; font-size: 13px; font-weight: 500; border: 1.5px solid #2e7d5e; border-radius: 18px; padding: 4px 10px; min-width: 140px; min-height: 8px; max-height: 24px; }");

    etnerText = new QLineEdit();
    etnerText->setStyleSheet("QLineEdit { background-color: white; color: #1a3b2e; font-family: 'Segoe UI', 'Arial', sans-serif; font-size: 13px; font-weight: 500; border: 1.5px solid #2e7d5e; border-radius: 18px; padding: 4px 10px; min-width: 140px; min-height: 8px; max-height: 24px; }");

    saveButton = new QPushButton("Сохранить");
    saveButton->setStyleSheet("QPushButton { background-color: transparent; color: #2e7d5e; font-family: 'Segoe UI', 'Arial', sans-serif; font-size: 16px; font-weight: 700; border: 2px solid #2e7d5e; border-radius: 10px; padding: 10px 22px; min-width: 100px; } QPushButton:hover { background-color: #2e7d5e; color: white; } QPushButton:pressed { background-color: #1a5b42; border-color: #1a5b42; }");
    QDoubleValidator *summValidator =new QDoubleValidator(-999999999, 999999999,2, this);

    summValidator->setNotation(QDoubleValidator::StandardNotation);
    summValidator->setLocale(QLocale::c());
    enterSumm->setValidator(summValidator);


    categoryBox = new QComboBox(this);

    categoryBox->setEditable(true);

    QSqlQuery catQuery;
    catQuery.prepare("SELECT name, code FROM categories WHERE user_id = ? ORDER BY id");
    catQuery.addBindValue(m_userId);
    catQuery.exec();
    while (catQuery.next()) {
        categoryBox->addItem(catQuery.value(0).toString(), catQuery.value(1).toString());
    }



    mainLayout->addWidget(dateLabel, 0, 0);
    mainLayout->addWidget(summLabel, 1, 0);
    mainLayout->addWidget(commentLabel, 2, 0);
    mainLayout->addWidget(enterDate, 0, 2);
    mainLayout->addWidget(enterSumm, 1, 2);
    mainLayout->addWidget(etnerText, 2, 2);
    mainLayout->addWidget(saveButton, 4, 0, 1, 3);
    mainLayout->addWidget(categoryLabel,3,0);
    mainLayout->addWidget(categoryBox,3,1);
    QSqlQuery query;
    query.prepare("SELECT date, summ, comment, type, category FROM spending WHERE id = ? AND user_id = ?");
    query.addBindValue(m_recordId);
    query.addBindValue(m_userId);

    if (query.exec() && query.next()) {
        QDate d = QDate::fromString(query.value(0).toString(), "yyyy-MM-dd");
        if (!d.isValid())
            d = QDate::fromString(query.value(0).toString(), "dd.MM.yyyy");
        if (d.isValid())
            enterDate->setDate(d);
        //enterDate->setText(query.value(0).toString());
        enterSumm->setText(QString::number(query.value(1).toDouble()));
        etnerText->setText(query.value(2).toString());
        QString catValue = query.value(4).toString();
        int index = categoryBox->findData(catValue);
        if (index == -1)
            index = categoryBox->findText(catValue, Qt::MatchFixedString);
        if (index != -1)
            categoryBox->setCurrentIndex(index);
        else if (!catValue.isEmpty())
            categoryBox->setEditText(catValue);
    }

    connect(saveButton, &QPushButton::clicked, this, &redactor::on_saveButton_clicked);
}

redactor::~redactor() {}

void redactor::on_saveButton_clicked()
{
    QString date = enterDate->date().toString("yyyy-MM-dd");
    double summ = enterSumm->text().toDouble();
    QString comment = etnerText->text();
    QSqlQuery query;
    QString categoryCode = categoryBox->currentData().toString();
    QString categoryName = categoryBox->currentText().trimmed();

    if (categoryName.isEmpty()) {
        qDebug() << "Категория не может быть пустой";
        return;
    }

    if (categoryCode.isEmpty()) {
        QSqlQuery findCat;
        findCat.prepare("SELECT code FROM categories WHERE user_id = ? AND name = ?");
        findCat.addBindValue(m_userId);
        findCat.addBindValue(categoryName);
        if (findCat.exec() && findCat.next()) {
            categoryCode = findCat.value(0).toString();
        } else {
            categoryCode = categoryName;
            QSqlQuery insertCat;
            insertCat.prepare("INSERT INTO categories(user_id, name, code) VALUES(?, ?, ?)");
            insertCat.addBindValue(m_userId);
            insertCat.addBindValue(categoryName);
            insertCat.addBindValue(categoryCode);
            insertCat.exec();
        }
    }
    if (summ > 0) {
        query.prepare("UPDATE spending SET summ = ?, type = 1, date = ?, comment = ?, category = ? WHERE id = ? AND user_id = ?");
    } else {
        query.prepare("UPDATE spending SET summ = ?, type = 0, date = ?, comment = ?, category = ? WHERE id = ? AND user_id = ?");
    }

    query.addBindValue(summ);
    query.addBindValue(date);
    query.addBindValue(comment);
    query.addBindValue(categoryCode);
    query.addBindValue(m_recordId);
    query.addBindValue(m_userId);

    if (query.exec()) {
        accept();
    } else {
        qDebug() << "Ошибка обновления";
    }
}