#include "mainwindow.h"
#include "ui_mainwindow.h"

void task_1();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("milkfactory");
    db.setUserName("root");
    db.setPassword("qwe123qwe");
    if (db.open())
    {
        qDebug() << "Succ";
    }
    else
    {
        qDebug() << "Error";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QSqlQueryModel *mModel = new QSqlQueryModel;
    mModel->setQuery("select * from orders");
    ui->tableView->setModel(mModel);
}

void MainWindow::on_pushButton_2_clicked()
{
    QSqlQueryModel *mModel = new QSqlQueryModel;
    mModel->setQuery("select * from stores");
    ui->tableView->setModel(mModel);
}


void MainWindow::on_pushButton_4_clicked()
{
    QSqlQueryModel *mModel = new QSqlQueryModel;
    mModel->setQuery("select * from products");
    ui->tableView->setModel(mModel);
}


void MainWindow::on_pushButton_3_clicked()
{
    bool checked;
    QString date = QInputDialog::getText(this, "Вікно вводу", "Дата постачання", QLineEdit::Normal, "", &checked);
    if (checked)
    {
        QSqlQueryModel *mModel = new QSqlQueryModel;
        QString sqlCode = QString("SELECT s.store_name, o.order_id, o.order_date, p.product_name, o.ordered_quantity, o.delivered_quantity, o.delivery_date FROM Orders o JOIN Stores s ON o.store_code = s.store_code JOIN Products p ON o.product_code = p.product_code WHERE o.delivery_date = '%1';").arg(date);
        mModel->setQuery(sqlCode);
        ui->tableView->setModel(mModel);
    }
}


void MainWindow::on_pushButton_5_clicked()
{
    bool checked;
    QString nameProduct = QInputDialog::getText(this, "Вікно вводу", "Назва продукта", QLineEdit::Normal, "", &checked);
    if (checked)
    {
        QSqlQueryModel *beginChanged = new QSqlQueryModel;
        QSqlQueryModel *afterChanged = new QSqlQueryModel;
        QString sqlCode = QString("update products set unit_cost = unit_cost * 1.1 where product_name = '%1'").arg(nameProduct);

        beginChanged->setQuery("select * from products");
        QSqlQuery query(sqlCode);
        afterChanged->setQuery("select * from products");

        ui->tableView->setModel(beginChanged);
        ui->tableView_2->setModel(afterChanged);
    }
}


void MainWindow::on_pushButton_6_clicked()
{
    QSqlQueryModel *mModel = new QSqlQueryModel;
    mModel->setQuery("SELECT o.order_date, s.store_name, p.product_name, o.ordered_quantity * p.unit_cost AS total_cost, (o.ordered_quantity * p.unit_cost) * 1.20 AS total_payment FROM Orders o JOIN Stores s ON o.store_code = s.store_code JOIN Products p ON o.product_code = p.product_code; ");
    ui->tableView->setModel(mModel);

}


void MainWindow::on_pushButton_7_clicked()
{
    QDialog dgl;
    dgl.setWindowTitle("Вікно вводу");

    QVBoxLayout formDgl(&dgl);
    QLabel label_1("Введіть дату", &dgl);
    formDgl.addWidget(&label_1);

    QHBoxLayout horizotnalItems;
    QLineEdit dateFirst(&dgl);
    QLineEdit dateSecond(&dgl);
    QLabel label_2("-", &dgl);
    horizotnalItems.addWidget(&dateFirst);
    horizotnalItems.addWidget(&label_2);
    horizotnalItems.addWidget(&dateSecond);
    formDgl.addLayout(&horizotnalItems);

    QPushButton button_1("Ok", &dgl);
    QObject::connect(&button_1, &QPushButton::clicked, &dgl, [&](){
        QString sqlCode = QString("SELECT s.store_name, SUM(o.ordered_quantity * p.unit_cost) AS total_orders_cost FROM Orders o JOIN Stores s ON o.store_code = s.store_code JOIN Products p ON o.product_code = p.product_code WHERE o.order_date BETWEEN '%1' AND '%2' GROUP BY s.store_name;").arg(dateFirst.text(), dateSecond.text());
        QSqlQueryModel *mModel = new QSqlQueryModel;
        mModel->setQuery(sqlCode);
        ui->tableView->setModel(mModel);
        dgl.accept();
    });
    formDgl.addWidget(&button_1);

    dgl.setLayout(&formDgl);
    dgl.exec();
}

void MainWindow::on_pushButton_8_clicked()
{
    QSqlQueryModel *mModel = new QSqlQueryModel;
    mModel->setQuery("SELECT p.product_name FROM Products p LEFT JOIN Orders o ON p.product_code = o.product_code WHERE o.order_id IS NULL OR o.order_date < DATE_SUB(CURDATE(), INTERVAL 1 MONTH);");
    ui->tableView->setModel(mModel);
}


void MainWindow::on_pushButton_9_clicked()
{
    QSqlQueryModel *mModel = new QSqlQueryModel;
    mModel->setQuery("select product_name, unit_cost * 0.9 as discounted_cost from products");
    ui->tableView->setModel(mModel);
}

