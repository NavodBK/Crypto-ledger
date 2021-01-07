#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "savefile.h"

QT_BEGIN_NAMESPACE
namespace Ui { class calculator; }
QT_END_NAMESPACE

class calculator : public QMainWindow
{
    Q_OBJECT

public:
    calculator(QWidget *parent = nullptr);
    ~calculator();
    void calculateBuY();
    void calculateSell();
    void calculateMinSell();
    void fetchPrices();
    void readDataFile();
    void setLists();

private slots:
    void on_buyPrice_textChanged(const QString &arg1);

    void on_BuyFee_textChanged(const QString &arg1);

    void on_Spent_textChanged(const QString &arg1);

    void on_SellPrice_textChanged(const QString &arg1);

    void on_SellFee_textChanged(const QString &arg1);

    void on_SellSpent_textChanged(const QString &arg1);

    void on_pushButton_8_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_BTCBalance2_textChanged(const QString &arg1);

    void on_XRPBalance2_textChanged(const QString &arg1);

    void on_ETHBalance2_textChanged(const QString &arg1);

    void on_USDTBalance2_textChanged(const QString &arg1);

    void on_btnCalc_clicked();

    void on_btnLadger_clicked();

private:
    Ui::calculator *ui;
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    SaveFile *saveFile;
};
#endif // CALCULATOR_H
