#include "calculator.h"
#include "ui_calculator.h"


#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QNetworkReply>
#include <QListWidgetItem>



QString filepath = "C:/Users/navodk/Documents/data.json";

float price,fee,spent =0;

float sellPrice,sellFee,sellAmount=0;

QString BTCbal,ETHbal,XRPbal,USDTbal;

QString BTCprice, ETHprice,XRPprice,USDTprice;

QString BTCval,ETHval,XRPval,USDTval;

QJsonObject jsonOb;

QList<QString> ledgerList;


calculator::calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::calculator)
{
    ui->setupUi(this);

    fetchPrices();
    readDataFile();
    setLists();


}

//Read and load the save file
void calculator::readDataFile(){

    jsonOb = saveFile->readDataFile();
    QJsonValue value = jsonOb.value(QString("Balance"));
    qWarning()<<jsonOb;
    QJsonObject balance = value.toObject();

    QJsonValue BTC = balance["BTC"];
    QJsonValue USDT = balance["USDT"];
    QJsonValue ETH = balance["ETH"];
    QJsonValue XRP = balance["XRP"];

    BTCbal = BTC.toString();
    ETHbal = ETH.toString();
    XRPbal = XRP.toString();
    USDTbal = USDT.toString();

     QJsonValue ledger = jsonOb.value(QString("ledger"));
     QJsonObject ledgerValues = ledger.toObject();

     foreach (QJsonValue index, ledgerValues) {
         ledgerList.append(index.toString());
     }



}

//fetch the current prices using the Binance API
void calculator::fetchPrices(){
    saveFile->DataFileInit();
    manager = new QNetworkAccessManager();
           QObject::connect(manager, &QNetworkAccessManager::finished,
               this, [=](QNetworkReply *reply) {
                   if (reply->error()) {
                       qDebug() << reply->errorString();
                       return;
                   }
                   QString answer = (QString)reply->readAll();
                   QJsonDocument crypto = QJsonDocument::fromJson(answer.toUtf8());
                   QJsonObject cryptoOb = crypto.object();

                   if(cryptoOb.value("symbol").toString() == "BTCUSDT"){
                       BTCprice = (cryptoOb.value("price").toString());
                       BTCval = QString::number( BTCbal.toFloat()*BTCprice.toFloat());
//                       qWarning() << BTCprice;
                       ui->BTCPrice->setText(BTCprice);
                       ui->BTCBalance->setText(BTCbal);
                       ui->BTCBalance2->setText(BTCbal);
                       ui->BTCValue->setText(BTCval);
                       ui->BTCValue2->setText(BTCval);
                   }else if (cryptoOb.value("symbol").toString() == "ETHUSDT") {
                       ETHprice = (cryptoOb.value("price").toString());
                       ETHval = QString::number( ETHbal.toFloat()*ETHprice.toFloat());
//                       qWarning() << ETHprice;
                       ui->ETHPrice->setText(ETHprice);
                       ui->ETHBalance->setText(ETHbal);
                       ui->ETHBalance2->setText(ETHbal);
                       ui->ETHValue->setText(ETHval);
                       ui->ETHValue2->setText(ETHval);
                   }else if (cryptoOb.value("symbol").toString() == "XRPUSDT") {
                       XRPprice = (cryptoOb.value("price").toString());
//                       qWarning()<< XRPprice;
                       XRPval = QString::number( XRPbal.toFloat()*XRPprice.toFloat());
//                       qWarning() << XRPprice;
                       ui->XRPPrice->setText(XRPprice);
                       ui->XRPBalance->setText(XRPbal);
                       ui->XRPBalance2->setText(XRPbal);
                       ui->XRPValue->setText(XRPval);
                       ui->XRPValue2->setText(XRPval);
                   }

                   ui->USDTBalance->setText(USDTbal);
                   ui->USDTBalance2->setText(USDTbal);

                   ui->totValue->setText(QString::number( BTCval.toFloat()+ETHval.toFloat()+USDTval.toFloat()+XRPval.toFloat()));
           });
           request.setUrl(QUrl("https://api.binance.com/api/v3/ticker/price?symbol=BTCUSDT"));
           manager->get(request);
           request.setUrl(QUrl("https://api.binance.com/api/v3/ticker/price?symbol=ETHUSDT"));
           manager->get(request);
           request.setUrl(QUrl("https://api.binance.com/api/v3/ticker/price?symbol=XRPUSDT"));
           manager->get(request);


}

//Calculate the bought amount
void calculator::calculateBuY(){
    float bought = (spent-((spent/100)*fee))/price;
    ui->boughtAm->setText(QString::number(bought));
    calculateMinSell();
}

//calculate the USDT recived
void calculator::calculateSell(){
    float recived = (sellAmount-((sellAmount/100)*sellFee))*sellPrice;
    ui->SellBaught->setText(QString::number(recived));
}

//Calculate the minimum price to sell to avoid loss
void calculator::calculateMinSell(){
    float minSell = price +(((spent/price)-(spent-((spent/100)*fee))/price)*price);
    ui->minSellLbl->setText(QString::number( minSell));
}

calculator::~calculator()
{
    delete ui;
    //save the data on json when closing
    saveFile->saveDataFile(BTCbal,ETHbal,XRPbal,USDTbal,ledgerList);
}


void calculator::on_buyPrice_textChanged(const QString &arg1)
{
    price = arg1.toFloat();
    calculateBuY();
}

void calculator::on_BuyFee_textChanged(const QString &arg1)
{
    fee = arg1.toFloat();
    calculateBuY();
}

void calculator::on_Spent_textChanged(const QString &arg1)
{
    spent = arg1.toFloat();
    calculateBuY();
}

void calculator::on_SellPrice_textChanged(const QString &arg1)
{
    sellPrice = arg1.toFloat();
    calculateSell();
}

void calculator::on_SellFee_textChanged(const QString &arg1)
{
    sellFee = arg1.toFloat();
    calculateSell();
}

void calculator::on_SellSpent_textChanged(const QString &arg1)
{
    sellAmount = arg1.toFloat();
    calculateSell();
}

//set the ledger list
void calculator::setLists(){
    ui->listWidget->clear();
    foreach (QString index, ledgerList) {
        qWarning()<< index;
        ui->listWidget->addItem(index);
    }

}

//Run calculations and update UI on add to ledger button click
void calculator::on_pushButton_8_clicked()
{
    if(ui->buyPrice->text() == "" || ui->BuyFee->text() =="" || ui->boughtAm->text()==""||ui->Spent->text()=="" ){
        QMessageBox msgWarning;
           msgWarning.setText("Fill out all the fields");
           msgWarning.setIcon(QMessageBox::Warning);
           msgWarning.setWindowTitle("Caution");
           msgWarning.exec();
    }else{
        USDTbal = QString::number( USDTbal.toFloat()- ui->Spent->text().toFloat());

        if(ui->comboBox_16->currentText()=="BTC"){
            BTCbal = QString::number( BTCbal.toFloat()+ ui->boughtAm->text().toFloat());
        }else if(ui->comboBox_16->currentText()=="XRP"){
            XRPbal = QString::number( XRPbal.toFloat()+ ui->boughtAm->text().toFloat());
        }else if(ui->comboBox_16->currentText()=="ETH"){
            ETHbal = QString::number( ETHbal.toFloat()+ ui->boughtAm->text().toFloat());
        }else if(ui->comboBox_16->currentText()=="USDT"){
            USDTbal = QString::number( USDTbal.toFloat()+ ui->boughtAm->text().toFloat());
        }

        ledgerList.append("Bought "+ui->boughtAm->text()+ui->comboBox_16->currentText()  + " at "
                       +ui->buyPrice->text()+"USDT spending "
                       +ui->Spent->text()+"USDT");
    }
//    qWarning()<<buyList;
    fetchPrices();
    setLists();
}


//Run calculations and update UI on add to ledger button click
void calculator::on_pushButton_7_clicked()
{
    if(ui->SellPrice->text() == "" || ui->SellFee->text() =="" || ui->SellBaught->text()==""||ui->SellSpent->text()=="" ){
        QMessageBox msgWarning;
           msgWarning.setText("Fill out all the fields");
           msgWarning.setIcon(QMessageBox::Warning);
           msgWarning.setWindowTitle("Caution");
           msgWarning.exec();
    }else{
        USDTbal = QString::number( USDTbal.toFloat()+ ui->SellBaught->text().toFloat());

        if(ui->comboBox_15->currentText()=="BTC"){
            BTCbal = QString::number( BTCbal.toFloat()-ui->SellSpent->text().toFloat());
        }else if(ui->comboBox_15->currentText()=="XRP"){
            XRPbal = QString::number( XRPbal.toFloat()- ui->SellSpent->text().toFloat());
        }else if(ui->comboBox_15->currentText()=="ETH"){
            ETHbal = QString::number( ETHbal.toFloat()- ui->SellSpent->text().toFloat());
        }else if(ui->comboBox_15->currentText()=="USDT"){
            USDTbal = QString::number( USDTbal.toFloat()- ui->SellSpent->text().toFloat());
        }
        ledgerList.append("Sold "+ui->SellSpent->text()+ui->comboBox_15->currentText()  + " at "
                       +ui->SellPrice->text()+"USDT buying "
                       +ui->SellBaught->text()+"USDT");
    }
//    qWarning()<<sellList;
    fetchPrices();
    setLists();

}
//clear the fields on clear button click
void calculator::on_pushButton_9_clicked()
{
    ui->buyPrice->setText("");
    ui->BuyFee->setText("");
    ui->Spent->setText("");
    ui->boughtAm->setText("");
}

//clear the fields on clear button click
void calculator::on_pushButton_10_clicked()
{
    ui->SellPrice->setText("");
    ui->SellFee->setText("");
    ui->SellSpent->setText("");
    ui->SellBaught->setText("");
}

//calculate values on balance change
void calculator::on_BTCBalance2_textChanged(const QString &arg1)
{
    BTCbal = arg1;
    fetchPrices();
}

//calculate values on balance change
void calculator::on_XRPBalance2_textChanged(const QString &arg1)
{
    XRPbal = arg1;
    fetchPrices();
}

//calculate values on balance change
void calculator::on_ETHBalance2_textChanged(const QString &arg1)
{
    ETHbal = arg1;
    fetchPrices();
}

//calculate values on balance change
void calculator::on_USDTBalance2_textChanged(const QString &arg1)
{
    USDTbal = arg1;
    fetchPrices();
}

//Navigate to tabs on button click
void calculator::on_btnCalc_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void calculator::on_btnLadger_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
