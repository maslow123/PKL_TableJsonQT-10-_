#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QVariant>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QJsonArray>
#include <QModelIndex>
#include <QMessageBox>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStandardItemModel *model = new QStandardItemModel(10,2,this); //3 record 2 field
        model -> setHorizontalHeaderItem(0, new QStandardItem(QString("Nis")));
        model -> setHorizontalHeaderItem(1, new QStandardItem(QString("Nama")));
    ui -> tableView -> setModel(model);

    QEventLoop eventLoop;
    // selesai
    QNetworkAccessManager mgr;
    QObject::connect(&mgr,SIGNAL(finished(QNetworkReply*)),&eventLoop,SLOT(quit()));

    //http request
    QNetworkRequest req(QUrl(QString("http://localhost/api/index.php?operasi=view")));
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec();

    if(reply -> error() == QNetworkReply::NoError){
        QByteArray strReply = reply ->readAll();

        //parse json
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply);
        QJsonObject object = jsonResponse.object();
        if(object.value("data_sekolah").isUndefined()){
            qDebug()<<"Gagal";
        }
        QJsonArray jsonArray = object.value("data_sekolah").toArray();
        int cnt = 0;

        QStandardItemModel *model = new QStandardItemModel(cnt,5,this); //3 record 2 field
        model -> setHorizontalHeaderItem(0,new QStandardItem(QString("NIS")));
        model -> setHorizontalHeaderItem(1,new QStandardItem(QString("Nama")));
        model -> setHorizontalHeaderItem(2,new QStandardItem(QString("Kelas")));
        model -> setHorizontalHeaderItem(3,new QStandardItem(QString("Alamat")));
        model -> setHorizontalHeaderItem(4,new QStandardItem(QString("Hobi")));

        ui -> tableView -> setModel(model);


        foreach(const QJsonValue &value,jsonArray){
             QJsonObject jsonObj = value.toObject();

             QStandardItem *nis =new QStandardItem(jsonObj["NIS"].toString());
             model->setItem(cnt,0,nis);


             QStandardItem *nama =new QStandardItem(jsonObj["Nama"].toString());
             model ->setItem(cnt,1,nama);
             QStandardItem *kelas =new QStandardItem(jsonObj["Kelas"].toString());
             model -> setItem(cnt,2,kelas);
             QStandardItem *alamat =new QStandardItem(jsonObj["Alamat"].toString());
             model -> setItem(cnt,3,alamat);
             QStandardItem *hobi =new QStandardItem(jsonObj["Hobi"].toString());
             model -> setItem(cnt,4,hobi);

             cnt++;

               }
    }else{
                QMessageBox msgBox;
                msgBox.setText("Harap periksa koneksi anda !");
                msgBox.exec();

            }


        }
MainWindow::~MainWindow()
{
    delete ui;
}

