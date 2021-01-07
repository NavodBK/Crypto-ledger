#include "savefile.h"


//QString saveFilePath = "C:/Users/navodk/Desktop/data.json";
QString saveFilePath = "data.json";

SaveFile::SaveFile()
{

}

//Initiate the save file if not exist
void SaveFile::DataFileInit(){
    QDir dir;
    QFile savingFile(saveFilePath);
    if (!savingFile.exists(saveFilePath)){

        QJsonObject Jroot;

        QJsonObject jBalance;
        jBalance.insert("BTC","0");
        jBalance.insert("XRP","0");
        jBalance.insert("ETH","0");
        jBalance.insert("USDT","0");

        QJsonObject jBuyList;


        Jroot.insert("ledger",jBuyList);

        Jroot.insert("Balance",jBalance);



        if (!savingFile.open(QIODevice::WriteOnly)) {
                qWarning("Couldn't open save file.");

            }
        savingFile.write(QJsonDocument(Jroot).toJson());
    }
}

//Read and return the save file data
QJsonObject SaveFile::readDataFile(){
    QFile saveFile(saveFilePath);
    if(!saveFile.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "We could not find the data file";
    }
    QString values = saveFile.readAll();
    QJsonDocument d = QJsonDocument::fromJson(values.toUtf8());
    QJsonObject jsonOb = d.object();
    return jsonOb;
}

//save the data to the file on exit
void SaveFile::saveDataFile(QString BTCbal,QString ETHbal,QString XRPbal,QString USDTbal,QList<QString>ledgerList){

    QJsonObject Jroot;

    QJsonObject jBalance;
    jBalance.insert("BTC",BTCbal);
    jBalance.insert("XRP",XRPbal);
    jBalance.insert("ETH",ETHbal);
    jBalance.insert("USDT",USDTbal);

    QJsonObject jBuyList;
    int count=0;
    foreach (QString val, ledgerList) {
        jBuyList.insert(QString::number(count),val);
        count++;
    }

    Jroot.insert("ledger",jBuyList);

    Jroot.insert("Balance",jBalance);


    QFile savingFile(saveFilePath);
    if (!savingFile.open(QIODevice::WriteOnly)) {
            qWarning("Couldn't open save file.");

        }

    savingFile.write(QJsonDocument(Jroot).toJson());
}


