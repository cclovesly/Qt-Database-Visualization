#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        QSqlDatabase db= QSqlDatabase::database("qt_sql_default_connection");
    }else{
        QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("student.db");
        if(db.open()){
            QMessageBox::information(this,"infor","数据库连接成功",QMessageBox::Ok);
        }else{
            QMessageBox::information(this,"infor","连接失败",QMessageBox::No);
        }
        QSqlQuery query;
        query.exec("create table student(ID int,NAME char,age int);");
        //query.exec("insert into student values(1,'张三',10);");
        //query.exec("insert into student values(2,'李四',11);");
        //query.exec("delete from student where ID=1;");
    }
    model = new QSqlTableModel(this);
    model->setTable("student");
    model->setHeaderData(0,Qt::Horizontal,tr("学号"));
    model->setHeaderData(1,Qt::Horizontal,tr("姓名"));
    model->setHeaderData(2,Qt::Horizontal,tr("年龄"));
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    ui->tableView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_add_clicked()
{
    model=(QSqlTableModel*)ui->tableView->model();
    QSqlRecord record = model->record();
    qDebug().noquote() << "获取字段 ：" << record.fieldName(0);


    // 插入记录
    qDebug().noquote() << "插入 ：" << model->insertRecord(model->rowCount(), record);
}


void MainWindow::on_delete_2_clicked()
{
    //获取选中的行
    int curRow = ui->tableView->currentIndex().row();

    //删除该行
    model->removeRow(curRow);

    int ret = QMessageBox::warning(this, tr("删除当前行"), tr("你确定要删除该行吗"), QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
        //删除该行 提交到数据库
        model->submitAll();
    }
    else
    {
        //不删除  撤销操作
        model->revertAll();
    }
    model->select();
}


void MainWindow::on_sure_clicked()
{
    model=(QSqlTableModel*)ui->tableView->model();
    model->submitAll();
}


void MainWindow::on_cancel_clicked()
{
    this->close();
}


void MainWindow::on_find_clicked()
{
    QString NAME=ui->NameEdit->text();
    model->setFilter(QString("name='%2'").arg(NAME));
    model->select();
}


void MainWindow::on_Update_clicked()
{
    model=(QSqlTableModel*)ui->tableView->model();
    model->submitAll();
}


void MainWindow::on_Sel_clicked()
{
    model->setFilter("");
    model->select();
}

