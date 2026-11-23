#include "secondwindow.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QStatusBar>
#include <QSqlQuery>
#include <QSqlError>
#include <QColor>
#include <QDate>
#include <QDebug>
#include <QProgressBar>
#include <QCheckBox>
#include <QTimeEdit>
#include <QTimer>
#include <QTime>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QStyle>

secondwindow::secondwindow(int userId, QString userLogin, QWidget* parent)
    : QMainWindow(parent), m_userId(userId), m_userLogin(userLogin)
{
    setWindowTitle("Мой кошелёк");
    resize(1010, 691);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout* rootLayout = new QHBoxLayout(central);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    QWidget* sidebar = new QWidget;
    sidebar->setFixedWidth(80);
    sidebar->setStyleSheet("background-color: #2e7d5e;");
    QVBoxLayout* sideLayout = new QVBoxLayout(sidebar);

    mainPage_button = new QPushButton("Траты");
    limitPage_button = new QPushButton("Лимиты");
    reminderPage_button = new QPushButton("Напом.");
    mainPage_button->setFixedHeight(50);
    limitPage_button->setFixedHeight(50);
    reminderPage_button->setFixedHeight(50);
    mainPage_button->setStyleSheet("color: white; background: transparent; border: none; font-weight: bold;");
    limitPage_button->setStyleSheet("color: white; background: transparent; border: none; font-weight: bold;");
    reminderPage_button->setStyleSheet("color: white; background: transparent; border: none; font-weight: bold;");

    sideLayout->addWidget(mainPage_button);
    sideLayout->addWidget(limitPage_button);
    sideLayout->addWidget(reminderPage_button);
    sideLayout->addStretch();

    stackedWidget = new QStackedWidget(this);
    mainPage = new QWidget(this);
    QGridLayout* mainLayout = new QGridLayout(mainPage);

    QHBoxLayout* topLayout = new QHBoxLayout();

    QGridLayout* balanceLayout = new QGridLayout();
    balance = new QLabel("0");
    balance->setStyleSheet("color: #2e7d5e; font-size: 30px; font-weight: 800;");
    QLabel* balance_label = new QLabel("Баланс:");
    balance_label->setStyleSheet("color: #2e7d5e; font-size: 30px; font-weight: 800;");
    balanceLayout->addWidget(balance_label, 0, 0);
    balanceLayout->addWidget(balance, 0, 1);

    doxod = new QLabel("0");
    doxod->setStyleSheet("color: #2e7d5e; font-size: 18px; font-weight: 700;");
    QLabel* doxod_label = new QLabel("Доходы:");
    doxod_label->setStyleSheet("color: #2e7d5e; font-size: 17px; font-weight: 700;");
    rasxod = new QLabel("0");
    rasxod->setStyleSheet("color: #d14545; font-size: 18px; font-weight: 700;");
    QLabel* rasxod_label = new QLabel("Расходы:");
    rasxod_label->setStyleSheet("color: #2e7d5e; font-size: 17px; font-weight: 700;");

    balanceLayout->addWidget(doxod_label, 1, 0);
    balanceLayout->addWidget(doxod, 1, 1);
    balanceLayout->addWidget(rasxod_label, 2, 0);
    balanceLayout->addWidget(rasxod, 2, 1);
    topLayout->addLayout(balanceLayout);

    QGridLayout* formLayout = new QGridLayout();
    QLabel* dateLabel = new QLabel("Дата");
    dateLabel->setAlignment(Qt::AlignCenter);
    QLabel* summLabel = new QLabel("Сумма");
    summLabel->setAlignment(Qt::AlignCenter);
    QLabel* commentLabel = new QLabel("Комментарий");
    commentLabel->setAlignment(Qt::AlignCenter);
    QLabel* categoryLabel = new QLabel("Категория");
    categoryLabel->setAlignment(Qt::AlignCenter);
    enterDate = new QDateEdit();
    enterDate->setDate(QDate::currentDate());
    enterSumm = new QLineEdit();
    QDoubleValidator* summValidator = new QDoubleValidator(-999999999, 999999999, 2, this);
    summValidator->setNotation(QDoubleValidator::StandardNotation);
    summValidator->setLocale(QLocale::c());
    enterSumm->setValidator(summValidator);
    etnerText = new QLineEdit();

    formLayout->addWidget(dateLabel, 0, 0);
    formLayout->addWidget(summLabel, 1, 0);
    formLayout->addWidget(commentLabel, 2, 0);
    formLayout->addWidget(categoryLabel, 3, 0);
    formLayout->addWidget(enterDate, 0, 2);
    formLayout->addWidget(enterSumm, 1, 2);
    formLayout->addWidget(etnerText, 2, 2);

    topLayout->addLayout(formLayout);

    pushButton = new QPushButton("+");
    pushButton->setFixedSize(80, 80);
    pushButton->setStyleSheet("background-color: #2e7d5e; color: white; font-size: 70px; font-weight: bold; border-radius: 40px; text-align: center;");
    topLayout->addWidget(pushButton);

    mainLayout->addLayout(topLayout, 0, 0);

    QHBoxLayout* bottomLayout = new QHBoxLayout();

    reload_2 = new QPushButton("Удалить");
    reload_2->setFixedSize(80, 80);
    reload_2->setStyleSheet("background-color: #2e7d5e; color: white; font-size: 14px; font-weight: bold; border-radius: 40px; text-align: center;");

    list = new QListWidget();
    list->setMaximumSize(1000, 450);
    list->setFont(QFont("Arial", 14));
    list->setStyleSheet("background-color: white; border: 2px solid #2e7d5e; padding: 15px;");

    reload = new QPushButton("Обновить");
    reload->setFixedSize(80, 80);
    reload->setStyleSheet("background-color: #2e7d5e; color: white; font-size: 14px; font-weight: bold; border-radius: 40px; text-align: center;");

    searchButton = new QPushButton("Поиск");
    searchButton->setFixedSize(80, 80);
    searchButton->setStyleSheet("background-color: #2e7d5e; color: white; font-size: 14px; font-weight: bold; border-radius: 40px; text-align: center;");

    bottomLayout->addWidget(reload_2);
    bottomLayout->addWidget(list);
    bottomLayout->addWidget(reload);

    mainLayout->addLayout(bottomLayout, 1, 0);

    QHBoxLayout* searchLayout = new QHBoxLayout();

    enterSearch = new QLineEdit();
    enterSearch->setStyleSheet("QLineEdit { background-color: white; border: 1px solid #2e7d5e; border-radius: 18px; padding: 8px 15px; min-width: 200px; }");

    searchLayout->addWidget(enterSearch);
    searchLayout->addWidget(searchButton);

    mainLayout->addLayout(searchLayout, 2, 0, Qt::AlignCenter);

    status = new QLabel(this);
    status->setVisible(false);
    status->setAlignment(Qt::AlignCenter);
    status->setStyleSheet("color: #d14545; font-size: 14px; font-weight: 500; margin-top: 15px;");

    mainLayout->addWidget(status, 3, 0, Qt::AlignCenter);

    connect(pushButton, &QPushButton::clicked, this, &secondwindow::on_pushButton_clicked);
    connect(reload, &QPushButton::clicked, this, &secondwindow::on_reload_clicked);
    connect(reload_2, &QPushButton::clicked, this, &secondwindow::on_reload_2_clicked);
    connect(list, &QListWidget::itemDoubleClicked, this, &secondwindow::on_list_itemDoubleClicked);
    connect(searchButton, &QPushButton::clicked, this, &secondwindow::on_search_cliked);

    connect(mainPage_button, &QPushButton::clicked, this, [this]() { stackedWidget->setCurrentWidget(mainPage); });
    connect(limitPage_button, &QPushButton::clicked, this, [this]() { stackedWidget->setCurrentWidget(limitPage); on_reload_limits_clicked(); });
    connect(reminderPage_button, &QPushButton::clicked, this, [this]() { stackedWidget->setCurrentWidget(reminderPage); on_reload_reminder_clicked(); });

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./MyWalletDB.db");
    if (!db.open()) {
        qDebug() << "Ошибка открытия БД";
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS users(id INTEGER PRIMARY KEY AUTOINCREMENT, login TEXT, password TEXT);");
    query.exec("CREATE TABLE IF NOT EXISTS spending(id INTEGER PRIMARY KEY AUTOINCREMENT, user_id INTEGER, summ DOUBLE, type BOOLEAN, date TEXT, comment TEXT, category TEXT);");
    query.exec("CREATE TABLE IF NOT EXISTS categories("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "user_id INTEGER, "
        "name TEXT, "
        "code TEXT, "
        "UNIQUE(user_id, code));");
    query.exec("CREATE TABLE IF NOT EXISTS limits(id INTEGER PRIMARY KEY AUTOINCREMENT,user_id INTEGER,summ DOUBLE, category TEXT, UNIQUE(user_id,category))");
    query.exec("CREATE TABLE IF NOT EXISTS reminder(user_id INTEGER PRIMARY KEY, enabled INTEGER, remind_time TEXT);");

    QSqlQuery seed;
    seed.prepare("SELECT COUNT(*) FROM categories WHERE user_id = ?");
    seed.addBindValue(m_userId);
    seed.exec();
    seed.next();
    if (seed.value(0).toInt() == 0) {
        QSqlQuery ins;
        ins.prepare("INSERT OR IGNORE INTO categories(user_id, name, code) VALUES(?, ?, ?)");
        const QList<QPair<QString, QString>> defaults = {
            {"Еда", "food"},
            {"Переводы", "translation"},
            {"Супермаркеты", "shops"},
            {"Цифровые товары", "digital_goods"},
            {"Красота", "beauty"},
            {"Транспорт", "transport"},
            {"Остальное", "rest"},
        };
        for (const auto& p : defaults) {
            ins.addBindValue(m_userId);
            ins.addBindValue(p.first);
            ins.addBindValue(p.second);
            ins.exec();
        }
    }
    categoryBox = new QComboBox(this);
    categoryBox->setEditable(true);

    QSqlQuery catQuery;
    catQuery.prepare("SELECT name, code FROM categories WHERE user_id = ? ORDER BY id");
    catQuery.addBindValue(m_userId);
    catQuery.exec();
    while (catQuery.next()) {
        QString name = catQuery.value(0).toString();
        QString code = catQuery.value(1).toString();
        categoryBox->addItem(name, code);
    }
    formLayout->addWidget(categoryBox, 3, 2);
    on_reload_clicked();

    limitPage = new QWidget(this);

    QVBoxLayout* limitsLayout = new QVBoxLayout(limitPage);

    QLabel* limitsTitle = new QLabel("Лимиты по категориям", limitPage);
    limitsTitle->setStyleSheet("color: #2e7d5e; font-size: 22px; font-weight: 800;");
    limitsLayout->addWidget(limitsTitle);

    QHBoxLayout* formLayout2 = new QHBoxLayout();

    QLabel* categoryLabel2 = new QLabel("Категория:", limitPage);
    categoryBoxLimit = new QComboBox(limitPage);
    categoryBoxLimit->setEditable(true);
    categoryBoxLimit->setMinimumWidth(160);
    QSqlQuery catQuery2;
    catQuery2.prepare("SELECT name, code FROM categories WHERE user_id = ? ORDER BY id");
    catQuery2.addBindValue(m_userId);
    catQuery2.exec();
    while (catQuery2.next())
        categoryBoxLimit->addItem(catQuery2.value(0).toString(), catQuery2.value(1).toString());

    QLabel* summLabel2 = new QLabel("Лимит:", limitPage);
    enterSummLimit = new QLineEdit(limitPage);
    enterSummLimit->setPlaceholderText("Сумма в месяц");
    enterSummLimit->setMinimumWidth(140);
    QDoubleValidator* summValidator2 = new QDoubleValidator(0, 999999999, 2, this);
    summValidator2->setNotation(QDoubleValidator::StandardNotation);
    summValidator2->setLocale(QLocale::c());
    enterSummLimit->setValidator(summValidator2);

    saveLimitBtn = new QPushButton("Сохранить", limitPage);
    saveLimitBtn->setStyleSheet(
        "QPushButton { background-color: #2e7d5e; color: white; font-weight: bold; "
        "padding: 8px 18px; border-radius: 8px; } "
        "QPushButton:hover { background-color: #256b4f; }");

    formLayout2->addWidget(categoryLabel2);
    formLayout2->addWidget(categoryBoxLimit);
    formLayout2->addWidget(summLabel2);
    formLayout2->addWidget(enterSummLimit);
    formLayout2->addWidget(saveLimitBtn);
    formLayout2->addStretch();

    limitsLayout->addLayout(formLayout2);

    limitsList = new QListWidget(limitPage);
    limitsList->setFont(QFont("Arial", 13));
    limitsList->setStyleSheet("background-color: white; border: 2px solid #2e7d5e; padding: 10px;");
    limitsLayout->addWidget(limitsList);

    limitsStatus = new QLabel(limitPage);
    limitsStatus->setVisible(false);
    limitsStatus->setAlignment(Qt::AlignCenter);
    limitsStatus->setStyleSheet("color: #d14545; font-size: 14px; font-weight: 500;");
    limitsLayout->addWidget(limitsStatus);

    connect(saveLimitBtn, &QPushButton::clicked, this, &secondwindow::on_save_limit_clicked);

    reminderPage = new QWidget(this);
    QVBoxLayout* reminderLayout = new QVBoxLayout(reminderPage);

    QLabel* reminderTitle = new QLabel("Напоминания о тратах", reminderPage);
    reminderTitle->setStyleSheet("color: #2e7d5e; font-size: 22px; font-weight: 800;");
    reminderLayout->addWidget(reminderTitle);

    reminderCheck = new QCheckBox("Включить напоминание о внесении трат", reminderPage);
    reminderCheck->setStyleSheet("QCheckBox { font-size: 15px; color: #2e7d5e; font-weight: 600; }");
    reminderLayout->addWidget(reminderCheck);

    QHBoxLayout* reminderTimeLayout = new QHBoxLayout();
    QLabel* reminderTimeLabel = new QLabel("Время напоминания:", reminderPage);
    reminderTimeLabel->setStyleSheet("font-size: 14px;");
    reminderTime = new QTimeEdit(reminderPage);
    reminderTime->setDisplayFormat("HH:mm");
    reminderTime->setTime(QTime(21, 0));
    reminderTime->setStyleSheet("QTimeEdit { background-color: white; border: 1px solid #2e7d5e; "
        "border-radius: 12px; padding: 4px 8px; min-width: 70px; }");
    reminderTimeLayout->addWidget(reminderTimeLabel);
    reminderTimeLayout->addWidget(reminderTime);
    reminderTimeLayout->addStretch();
    reminderLayout->addLayout(reminderTimeLayout);

    QHBoxLayout* reminderButtonsLayout = new QHBoxLayout();
    saveReminderBtn = new QPushButton("Сохранить", reminderPage);
    saveReminderBtn->setStyleSheet("background-color: #2e7d5e; color: white; "
        "font-size: 13px; font-weight: bold; border-radius: 12px; padding: 6px 16px;");
    reloadReminderBtn = new QPushButton("Обновить", reminderPage);
    reloadReminderBtn->setStyleSheet("background-color: #2e7d5e; color: white; "
        "font-size: 13px; font-weight: bold; border-radius: 12px; padding: 6px 16px;");
    reminderButtonsLayout->addWidget(saveReminderBtn);
    reminderButtonsLayout->addWidget(reloadReminderBtn);
    reminderButtonsLayout->addStretch();
    reminderLayout->addLayout(reminderButtonsLayout);

    reminderStatus = new QLabel(reminderPage);
    reminderStatus->setVisible(false);
    reminderStatus->setAlignment(Qt::AlignCenter);
    reminderStatus->setStyleSheet("color: #d14545; font-size: 14px; font-weight: 500;");
    reminderLayout->addWidget(reminderStatus);

    reminderLayout->addStretch();

    connect(saveReminderBtn, &QPushButton::clicked, this, &secondwindow::on_save_reminder_clicked);
    connect(reloadReminderBtn, &QPushButton::clicked, this, &secondwindow::on_reload_reminder_clicked);

    stackedWidget->addWidget(mainPage);
    stackedWidget->addWidget(limitPage);
    stackedWidget->addWidget(reminderPage);
    rootLayout->addWidget(sidebar);
    rootLayout->addWidget(stackedWidget, 1);

    on_reload_limits_clicked();

    m_reminderEnabled = false;
    m_reminderTimeValue = QTime(21, 0);
    m_lastReminderDate = QDate();

    QSqlQuery remQuery;
    remQuery.prepare("SELECT enabled, remind_time FROM reminder WHERE user_id = ?");
    remQuery.addBindValue(m_userId);
    remQuery.exec();
    if (remQuery.next()) {
        m_reminderEnabled = remQuery.value(0).toInt() == 1;
        m_reminderTimeValue = QTime::fromString(remQuery.value(1).toString(), "HH:mm");
        if (!m_reminderTimeValue.isValid())
            m_reminderTimeValue = QTime(21, 0);
    }

    reminderCheck->setChecked(m_reminderEnabled);
    reminderTime->setTime(m_reminderTimeValue);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(style()->standardIcon(QStyle::SP_MessageBoxInformation));
    trayIcon->setToolTip("Мой кошелёк");
    trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::messageClicked, this, [this]() {
        this->showNormal();
        this->raise();
        this->activateWindow();
        });

    reminderTimer = new QTimer(this);
    connect(reminderTimer, &QTimer::timeout, this, &secondwindow::checkReminder);
    reminderTimer->start(60 * 1000);
}

void secondwindow::on_save_limit_clicked()
{
    QSqlQuery query;
    double summ = enterSummLimit->text().toDouble();
    QString category = categoryBoxLimit->currentData().toString();

    if (summ == 0) {
        limitsStatus->setText("Заполните все поля");
        limitsStatus->setVisible(true);
        return;
    }
    query.prepare("INSERT INTO limits(user_id,summ,category) VALUES(?,?,?) ON CONFLICT(user_id, category) DO UPDATE SET summ = excluded.summ");
    query.addBindValue(m_userId);
    query.addBindValue(summ);
    query.addBindValue(category);
    if (query.exec()) {
        limitsStatus->setText("Лимит добавлен");
        limitsStatus->setStyleSheet("color: #2e7d5e; font-size: 14px; font-weight: 500;");
        limitsStatus->setVisible(true);
    }
    else {
        limitsStatus->setText("Ошибка:" + query.lastError().text());
        limitsStatus->setStyleSheet("color: #d14545; font-size: 14px; font-weight: 500;");
        limitsStatus->setVisible(true);
    }
    enterSummLimit->clear();
    on_reload_limits_clicked();
}

void secondwindow::on_reload_limits_clicked()
{
    limitsList->clear();
    QSqlQuery query;
    query.prepare("SELECT id, summ,category FROM limits WHERE user_id=?");
    query.addBindValue(m_userId);
    query.exec();

    while (query.next()) {
        int id = query.value(0).toInt();
        double summ = query.value(1).toDouble();
        QString code = query.value(2).toString();
        QString categoryName = returnCategory(code);
        QProgressBar* progresBar = new QProgressBar();
        QSqlQuery query2;
        query2.prepare("SELECT COALESCE(SUM(ABS(summ)), 0) FROM spending "
            "WHERE user_id = ? AND category = ? AND date LIKE ? AND type = 0");
        query2.addBindValue(m_userId);
        query2.addBindValue(code);
        query2.addBindValue(QDate::currentDate().toString("yyyy-MM") + "-%");
        query2.exec();
        double allSumm = 0;
        if (query2.next())
            allSumm = query2.value(0).toDouble();

        int procent = (summ > 0) ? qRound(allSumm / summ * 100.0) : 0;
        progresBar->setRange(0, 100);
        progresBar->setValue(qMin(procent, 100));
        progresBar->setTextVisible(true);

        QString color;
        if (procent >= 100) {
            color = "#d14545";
        }
        else if (procent >= 80) {
            color = "#e6a700";
        }
        else {
            color = "#2e7d5e";
        }

        progresBar->setStyleSheet(QString(
            "QProgressBar {"
            "    border: 1px solid #ccc;"
            "    border-radius: 6px;"
            "    background: #f0f0f0;"
            "    text-align: center;"
            "    color: %1;"
            "    font-weight: bold;"
            "}"
            "QProgressBar::chunk {"
            "    background-color: %1;"
            "    border-radius: 6px;"
            "}"
        ).arg(color));
        QListWidgetItem* item = new QListWidgetItem(limitsList);

        QString text = QString("%1: %2 / %3  (%4%)")
            .arg(categoryName)
            .arg(allSumm, 0, 'f', 2)
            .arg(summ, 0, 'f', 2)
            .arg(procent);

        QLabel* label = new QLabel(text);
        item->setData(Qt::UserRole, id);
        QWidget* row = new QWidget(limitsList);
        QVBoxLayout* rowLayout = new QVBoxLayout(row);
        rowLayout->addWidget(label);
        rowLayout->addWidget(progresBar);

        item->setData(Qt::UserRole, id);
        item->setSizeHint(row->sizeHint());
        limitsList->addItem(item);
        limitsList->setItemWidget(item, row);
    }
}

QString secondwindow::returnCategory(QString category)
{
    for (int i = 0; i < categoryBox->count(); i++) {
        if (category == categoryBox->itemData(i).toString()) {
            return categoryBox->itemText(i);
        }
    }
    return category;
}

secondwindow::~secondwindow()
{
}

void secondwindow::on_pushButton_clicked()
{
    QSqlQuery query;
    double summ = enterSumm->text().toDouble();
    QString coment = etnerText->text();
    QString date = enterDate->date().toString("yyyy-MM-dd");

    QString category = categoryBox->currentData().toString();
    if (category.isEmpty()) {
        category = categoryBox->currentText().trimmed();
    }

    if (summ == 0 || coment.isEmpty()) {
        status->setText("Заполните все поля");
        status->setVisible(true);
        return;
    }

    QString categoryCode = categoryBox->currentData().toString();
    QString categoryName = categoryBox->currentText().trimmed();

    if (categoryCode.isEmpty()) {
        QSqlQuery findCat;
        findCat.prepare("SELECT code FROM categories WHERE user_id = ? AND name = ?");
        findCat.addBindValue(m_userId);
        findCat.addBindValue(categoryName);
        if (findCat.exec() && findCat.next()) {
            categoryCode = findCat.value(0).toString();
        }
        else {
            categoryCode = categoryName;
            QSqlQuery insertCat;
            insertCat.prepare("INSERT INTO categories(user_id, name, code) VALUES(?, ?, ?)");
            insertCat.addBindValue(m_userId);
            insertCat.addBindValue(categoryName);
            insertCat.addBindValue(categoryCode);
            if (!insertCat.exec()) {
                qDebug() << "Ошибка добавления категории:" << insertCat.lastError().text();
            }
        }
    }

    qDebug() << "CategoryCode:" << categoryCode;

    QSqlQuery limitQuery;
    limitQuery.prepare("SELECT summ FROM limits WHERE user_id = ? AND category = ?");
    limitQuery.addBindValue(m_userId);
    limitQuery.addBindValue(categoryCode);

    double Summ = 0;
    if (limitQuery.exec() && limitQuery.next()) {
        Summ = limitQuery.value(0).toDouble();
    }
    else {
        qDebug() << "Ошибка или запись не найдена:" << limitQuery.lastError().text();
    }

    QSqlQuery query3;
    query3.prepare("SELECT COALESCE(SUM(ABS(summ)), 0) FROM spending "
        "WHERE user_id = ? AND category = ? AND date LIKE ? AND type = 0");
    query3.addBindValue(m_userId);
    query3.addBindValue(categoryCode);
    query3.addBindValue(QDate::currentDate().toString("yyyy-MM") + "-%");
    query3.exec();

    double allSumm = 0;
    if (query3.next())
        allSumm = query3.value(0).toDouble();

    if (Summ > 0 && (allSumm + qAbs(summ)) > Summ) {
        QMessageBox::warning(this, "Лимит", "Лимит превышен");
    }
    if (summ > 0) {
        query.prepare("INSERT INTO spending (user_id, summ, type, date, comment, category) VALUES (?, ?, 1, ?, ?, ?)");
    }
    else {
        query.prepare("INSERT INTO spending (user_id, summ, type, date, comment, category) VALUES (?, ?, 0, ?, ?, ?)");
    }

    query.addBindValue(m_userId);
    query.addBindValue(summ);
    query.addBindValue(date);
    query.addBindValue(coment);
    query.addBindValue(categoryCode);
    if (query.exec()) {
        status->setText("Операция добавлена");
        status->setStyleSheet("color: #2e7d5e; font-size: 14px; font-weight: 500; margin-top: 15px;");
        status->setVisible(true);
        enterSumm->clear();
        etnerText->clear();
        enterDate->setDate(QDate::currentDate());
        on_reload_clicked();

        categoryBox->clear();
        QSqlQuery reloadCats;
        reloadCats.prepare("SELECT name, code FROM categories WHERE user_id = ? ORDER BY id");
        reloadCats.addBindValue(m_userId);
        reloadCats.exec();
        while (reloadCats.next()) {
            categoryBox->addItem(reloadCats.value(0).toString(), reloadCats.value(1).toString());
        }

        int idx = categoryBox->findData(categoryCode);
        if (idx != -1) categoryBox->setCurrentIndex(idx);
    }
    else {
        status->setText("Ошибка: " + query.lastError().text());
        status->setStyleSheet("color: #d14545; font-size: 14px; font-weight: 500; margin-top: 15px;");
        status->setVisible(true);
    }
}

void secondwindow::on_reload_clicked()
{
    list->clear();
    QSqlQuery query;
    query.prepare("SELECT id, date, summ, comment, type, category FROM spending WHERE user_id = ? ORDER BY date DESC");
    query.addBindValue(m_userId);
    query.exec();

    double endSumm = 0, doxodSum = 0, rasxodSum = 0;

    while (query.next()) {
        int id = query.value(0).toInt();
        QString date = query.value(1).toString();
        double summ = query.value(2).toDouble();
        QString comment = query.value(3).toString();
        int type = query.value(4).toInt();
        QString category = returnCategory(query.value(5).toString());
        endSumm += summ;

        QListWidgetItem* item = new QListWidgetItem();
        item->setData(Qt::UserRole, id);
        item->setText("Дата: " + date + " Сумма: " + QString::number(summ) + " Комментарий: " + comment + " Категория: " + category);

        if (type == 1) {
            item->setForeground(QColor(46, 125, 94));
            doxodSum += summ;
        }
        else {
            item->setForeground(QColor(209, 69, 69));
            rasxodSum += summ;
        }

        list->addItem(item);
    }

    balance->setText(QString::number(endSumm));
    rasxod->setText(QString::number(rasxodSum));
    doxod->setText(QString::number(doxodSum));
}

void secondwindow::on_search_cliked()
{
    status->setVisible(false);
    QString comment = enterSearch->text();
    if (comment.isEmpty()) {
        status->setText("Введите данные для поиска");
        status->setVisible(true);
        return;
    }
    list->clear();
    QSqlQuery query;
    query.prepare("SELECT id, date, summ, comment, type, category FROM spending WHERE user_id = ? AND (comment=? OR summ=?) ORDER BY date DESC");
    query.addBindValue(m_userId);
    query.addBindValue(comment);
    query.addBindValue(comment.toDouble());
    query.exec();
    double endSumm = 0, doxodSum = 0, rasxodSum = 0;
    bool found = false;
    while (query.next()) {
        int id = query.value(0).toInt();
        found = true;
        QString date = query.value(1).toString();
        double summ = query.value(2).toDouble();
        QString comment = query.value(3).toString();
        int type = query.value(4).toInt();

        QString category = returnCategory(query.value(5).toString());

        endSumm += summ;

        QListWidgetItem* item = new QListWidgetItem();
        item->setData(Qt::UserRole, id);
        item->setText("Дата: " + date + " Сумма: " + QString::number(summ) + " Комментарий: " + comment + " Категория: " + category);

        if (type == 1) {
            item->setForeground(QColor(46, 125, 94));
            doxodSum += summ;
        }
        else {
            item->setForeground(QColor(209, 69, 69));
            rasxodSum += summ;
        }

        list->addItem(item);
    }

    if (found != true) {
        status->setText("Траты не найдены");
        status->setVisible(true);
    }

    balance->setText(QString::number(endSumm));
    rasxod->setText(QString::number(rasxodSum));
    doxod->setText(QString::number(doxodSum));
}

void secondwindow::on_reload_2_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Подтверждение удаления", "Удалить выбраную запись?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (reply != QMessageBox::Yes) {
        return;
    }

    QListWidgetItem* currentItem = list->currentItem();
    if (!currentItem) {
        status->setText("не выбран элемент");
        status->setVisible(true);
        return;
    }

    int id = currentItem->data(Qt::UserRole).toInt();
    QSqlQuery query;
    query.prepare("DELETE FROM spending WHERE id = ? AND user_id = ?");
    query.addBindValue(id);
    query.addBindValue(m_userId);

    if (query.exec()) {
        status->setText("Удалено");
        status->setVisible(true);
        on_reload_clicked();
    }
    else {
        status->setText("Ошибка удаления" + query.lastError().text());
        status->setVisible(true);
    }
}

void secondwindow::on_list_itemDoubleClicked(QListWidgetItem* item)
{
    int id = item->data(Qt::UserRole).toInt();
    editor = new redactor(m_userId, id, this);
    editor->show();
}

void secondwindow::on_save_reminder_clicked()
{
    m_reminderEnabled = reminderCheck->isChecked();
    m_reminderTimeValue = reminderTime->time();

    QSqlQuery query;
    query.prepare("INSERT OR REPLACE INTO reminder (user_id, enabled, remind_time) VALUES (?, ?, ?)");
    query.addBindValue(m_userId);
    query.addBindValue(m_reminderEnabled ? 1 : 0);
    query.addBindValue(m_reminderTimeValue.toString("HH:mm"));

    if (query.exec()) {
        reminderStatus->setText(m_reminderEnabled
            ? "Напоминание включено на " + m_reminderTimeValue.toString("HH:mm")
            : "Напоминание отключено");
        reminderStatus->setStyleSheet("color: #2e7d5e; font-size: 14px; font-weight: 500;");
        reminderStatus->setVisible(true);
    }
    else {
        reminderStatus->setText("Ошибка сохранения: " + query.lastError().text());
        reminderStatus->setStyleSheet("color: #d14545; font-size: 14px; font-weight: 500;");
        reminderStatus->setVisible(true);
    }
}

void secondwindow::on_reload_reminder_clicked()
{
    m_reminderEnabled = false;
    m_reminderTimeValue = QTime(21, 0);

    QSqlQuery query;
    query.prepare("SELECT enabled, remind_time FROM reminder WHERE user_id = ?");
    query.addBindValue(m_userId);
    query.exec();
    if (query.next()) {
        m_reminderEnabled = query.value(0).toInt() == 1;
        m_reminderTimeValue = QTime::fromString(query.value(1).toString(), "HH:mm");
        if (!m_reminderTimeValue.isValid())
            m_reminderTimeValue = QTime(21, 0);
    }

    reminderCheck->setChecked(m_reminderEnabled);
    reminderTime->setTime(m_reminderTimeValue);

    reminderStatus->setText(m_reminderEnabled
        ? "Напоминание включено на " + m_reminderTimeValue.toString("HH:mm")
        : "Напоминание отключено");
    reminderStatus->setStyleSheet("color: #2e7d5e; font-size: 14px; font-weight: 500;");
    reminderStatus->setVisible(true);
}

void secondwindow::checkReminder()
{
    if (!m_reminderEnabled)
        return;

    QTime now = QTime::currentTime();
    if (now.hour() != m_reminderTimeValue.hour() ||
        now.minute() != m_reminderTimeValue.minute())
        return;

    if (m_lastReminderDate == QDate::currentDate())
        return;

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM spending WHERE user_id = ? AND type = 0 AND date = ?");
    query.addBindValue(m_userId);
    query.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));
    query.exec();

    int count = 0;
    if (query.next())
        count = query.value(0).toInt();

    if (count == 0) {
        m_lastReminderDate = QDate::currentDate();
        trayIcon->showMessage(
            "Мой кошелёк",
            "Сегодня вы ещё не вносили траты.\nНе забудьте зафиксировать расходы!",
            QSystemTrayIcon::Information,
            10000);
    }
}