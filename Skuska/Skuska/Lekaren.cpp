#include "Lekaren.h"
#include <stdio.h>
#include<iostream>
#include<ctime>

int r = 0;

Lekaren::Lekaren(int uindex, QString LogName, QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    set_userIndex(uindex);
    set_userName(LogName);

    // nastavenie uvodneho layoutu zamestnanec/zakaznik //

    this->setWindowTitle(getUsername());

    loadUsers();
    indexCheck();
    loadObjednavky();

    if (getIndex() == 1)
    {
        ui.pushFaktura->setDisabled(TRUE);
        ui.pushPotvrdit->setDisabled(TRUE);
        ui.groupBox->setHidden(TRUE);
        ui.groupBoxKusy->setHidden(TRUE);
        ui.label_2->setHidden(TRUE);
        ui.doubleSpinBoxCena->setHidden(TRUE);
        ui.groupBox_2->setHidden(TRUE);

        for (size_t i = 0; i < objednavky.size(); i++)
        {
            ui.listLieky->addItem(objednavky[i]);
        }
    }
    if (getIndex() == 2)
    {   

        for (size_t i = 0; i < objednavky.size(); i++)                                       // kontrola, ci je objednavka pripravena na vyzdvihnutie
        {
            if (getUsername().append(",ready") == objednavky[i]){
                r = 1;
            }
        }
        if (r == 1) {
            ui.pushObjednat->setDisabled(TRUE);
            ui.pushKosik->setDisabled(TRUE);
            ui.pushOdstranit->setDisabled(TRUE);
            ui.pushObjednavka->setEnabled(TRUE);
            ui.groupBoxZamestnanec->setHidden(TRUE);

        }
        else {
                                                                                              // ak existuje nevybavena objednavka na taketo meno 
            nakup.clear();
            QString path = "C:/Users/tomka/source/repos/Skuska/Skuska/Objednavky/";
            QString login = getUsername();
            login.append(".txt");
            path.append(login);
            if (QFile::exists(path))
            {
                QFile file(path);
                QTextStream stream(&file);
                QString line;

                if (file.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    do {
                        line = stream.readLine();
                        if (!line.isNull()) {

                            QStringList data = line.split(",");
                            nakup.push_back(NAKUP(data.at(0), data.at(1).toFloat(), data.at(2).toInt()));
                        }
                    } while (!line.isNull());
                    file.close();
                }
            }

        ui.pushObjednavka->setDisabled(TRUE);
        ui.groupBox->setVisible(TRUE);
        ui.groupBox_2->setVisible(TRUE);
        ui.groupBoxKusy->setVisible(TRUE);
        ui.label_2->setVisible(TRUE);
        ui.doubleSpinBoxCena->setVisible(TRUE);
        ui.groupBoxZamestnanec->setHidden(TRUE);  
        }            

        loadProdukty();
    }

    
}

Lekaren::~Lekaren()
{
	delete this;
}

void Lekaren::loadProdukty()
{
    // nacitanie ponuky liekov //

    for (size_t i = 0; i < produkty.size(); i++)                                                 // zobrazenie ponuky do listWidgetu
    {
        ui.listLieky->takeItem(0);
    }

    pomocneProd.clear();
    produkty.clear();

    QString fileName = "Lieky.txt";
    if (fileName.isEmpty()) { return; }

    QFile file(fileName);
    QTextStream stream(&file);
    QString line;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        do {
            line = stream.readLine();
            if (!line.isNull()) {

                QStringList data = line.split(",");
                if (data.at(1) == "")
                {
                    setZlava(data.at(0).toInt());
                }
                else {
                    produkty.push_back(LIEK(data.at(0), data.at(1).toFloat(), data.at(2).toInt()));
                    pomocneProd.push_back(LIEK(data.at(0), data.at(1).toFloat(), data.at(2).toInt()));   // aby som mohol menit pocet kusov na sklade v txt file
                }
            }
        } while (!line.isNull());
        file.close();
    }
                                                                                                                        
    for (size_t i = 0; i < produkty.size(); i++)                                                       // zobrazenie ponuky do listWidgetu
    {
        ui.listLieky->addItem(produkty[i].get_liek());
    }                                                             
    if (zakaznik[iCheck].get_spend() > 100) {                                                       // zmena cien pre premioveho zakaznika
        int discount =floor(zakaznik[iCheck].get_spend() / 100) * getZlava() ;
        
        if (discount > 20) {
            discount = 20;
        }
        for (size_t i = 0; i < produkty.size(); i++)
        {
            float actual = produkty[i].get_cena();
            produkty[i].set_cena(actual - (actual / 100 * discount));
        }
        
    }
}

void Lekaren::loadUsers()
{
    // nacitanie dat o users zo suboru kvoli zmene minutych penazi,mena,adresy alebo zmeny pozicie //

    QString fileName = "Users.txt";
    if (fileName.isEmpty()) { return; }

    QFile file(fileName);
    QTextStream stream(&file);
    QString line;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        do {
            line = stream.readLine();
            if (!line.isNull()) {

                QStringList data = line.split(",");

                if (line.split(",").at(0) == "0")
                {
                    admin.push_back(ADMIN(data.at(0).toInt(), data.at(1), data.at(2)));
                }
                else if (line.split(",").at(0) == "1")
                {
                    zamestnanec.push_back(STAFF(data.at(0).toInt(), data.at(1), data.at(2), data.at(3)));
                }

                else if (line.split(",").at(0) == "2")
                {
                    zakaznik.push_back(USER(data.at(0).toInt(), data.at(1), data.at(2), data.at(3), data.at(4), data.at(5).toFloat()));
                }


            }
        } while (!line.isNull());
        file.close();
    }
}

void Lekaren::on_pushObjednat_clicked()
{
    // vlozenie vybraneho produktu do kosika //

    cenan = 0;
    int drugIndex;
    int drugCount;
    drugIndex = ui.listLieky->currentRow();
    drugCount = ui.spinBoxKusy->value();
    if (drugCount != 0) {
        if (drugCount <= produkty[drugIndex].get_kusy()) {
            actnakup.push_back(NAKUP(produkty[drugIndex].get_liek(), produkty[drugIndex].get_cena(), drugCount));
            int kusy = pomocneProd[drugIndex].get_kusy();
            pomocneProd[drugIndex].set_kusy(kusy-drugCount);                                // zníženie poètu kusov na sklade
            if (pomocneProd[drugIndex].get_kusy() == 0) {
                pomocneProd.remove(drugIndex);
                pomocneProd.shrink_to_fit();
            }
        }
        else {
            QMessageBox log;
            log.setWindowTitle("Shop");
            log.setText("Nedostatok kusov na sklade!");
            log.exec();
        }
    }
    else {
        QMessageBox log;
        log.setWindowTitle("Shop");
        log.setText("Zly pocet kusov!");
        log.exec();
    }
    for (size_t i = 0; i < actnakup.size(); i++)
    {
        cenan += actnakup[i].get_cena() * actnakup[i].get_kusy();
        ui.doubleSpinBoxCena->setValue(cenan);
    }
    ui.spinBoxKusy->setValue(0);
    
    saveProdukty();
    loadProdukty();
}

void Lekaren::on_pushKosik_clicked()
{
    // zobrazenie aktualneho nakupu v tablewidgete // 

    for (size_t i = 0; i < ui.tableWidgetObjednavka->rowCount(); i++)
    {
        ui.tableWidgetObjednavka->removeRow(i);
    } 


    ui.tableWidgetObjednavka->setRowCount(actnakup.size());
    ui.tableWidgetObjednavka->setColumnCount(3);
    for (size_t i = 0; i < actnakup.size(); i++)
    {
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setText(actnakup[i].get_liek());
        ui.tableWidgetObjednavka->setItem(i,0,item);

        QTableWidgetItem* item1 = new QTableWidgetItem;
        item1->setText(QString::number(actnakup[i].get_cena()));
        ui.tableWidgetObjednavka->setItem(i, 1, item1);
        
        QTableWidgetItem* item2 = new QTableWidgetItem;
        item2->setText(QString::number(actnakup[i].get_kusy()));
        ui.tableWidgetObjednavka->setItem(i, 2, item2);
    }
}

void Lekaren::indexCheck()
{
    // zistovanie indexu zakaznika alebo zamestnanca //
    // kvoli zlave a potom zmena spend,meno,adresa alebo pozicie // 

    if (getIndex() == 1) {
        for (size_t i = 0; i < zamestnanec.size(); i++)
        {
            if (zamestnanec[i].get_username() == getUsername())
            iCheck = i;
        }
    }

    if (getIndex() == 2) {
        for (size_t i = 0; i < zakaznik.size(); i++)
        {
            if (zakaznik[i].get_username() == getUsername())
            iCheck = i;
        }
    }
}

void Lekaren::saveUsers()
{
    // ulozenie zmien do txt file Users // 

    QString fileName = "Users.txt";                                                                        
    if (fileName.isEmpty()) { return; }
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        for (size_t i = 0; i < admin.size(); i++)
        {
            out << "0" << "," << admin[i].get_username() << "," << admin[i].get_password() << "\n";
        }
        for (size_t i = 0; i < zamestnanec.size(); i++)
        {
            out << "1" << "," << zamestnanec[i].get_username() << "," << zamestnanec[i].get_password() << "," << zamestnanec[i].get_pozicia() << "\n";
        }
        for (size_t i = 0; i < zakaznik.size(); i++)
        {
            out << "2" << "," << zakaznik[i].get_username() << "," << zakaznik[i].get_password() << "," << zakaznik[i].get_name() << "," << zakaznik[i].get_adress() << "," << zakaznik[i].get_spend() << "\n";
        }

    }
    else
    {
        return;
    }

    file.close();
}

void Lekaren::on_pushOdstranit_clicked()
{
    // odstranenie produktu z aktualneho nakupu // 

    int out = ui.tableWidgetObjednavka->currentRow();
    ui.tableWidgetObjednavka->removeRow(out);
    int povKus; 
    int kusy = actnakup[out].get_kusy();
    cenan-= actnakup[out].get_cena() * actnakup[out].get_kusy();
    ui.doubleSpinBoxCena->setValue(cenan);
    
    QString liek = actnakup[out].get_liek();                                // vrátenie poètu kusov do pôvodného stavu
    for (size_t i = 0; i < pomocneProd.size(); i++)
        if (liek == pomocneProd[i].get_liek())
        {
            povKus = pomocneProd[i].get_kusy();
            pomocneProd[i].set_kusy(povKus + kusy);
        }    
    
    actnakup.remove(out);
    actnakup.shrink_to_fit();

    saveProdukty();
    loadProdukty();
}

void Lekaren::saveObjednavky()
{
    // ulozenie nazvu objednavky do txt file // 

    if (nakup.size() != 0) {
        QString fileName1 = "Objednavky.txt";
        if (fileName1.isEmpty()) { return; }

        QFile file(fileName1);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        {
            QTextStream out(&file);
            out << getUsername() << "\n";
        }
        else
        {
            return;
        }

        file.close();
    }
}

void Lekaren::loadObjednavky()
{
    // nacitanie "nazvov" objednavok //

    QString fileName = "Objednavky.txt";
    if (fileName.isEmpty()) { return; }

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    while (!in.atEnd()) { 
        QString line = in.readLine();
        objednavky.push_back(line);  // pushovanie dat do vektora z file.txt
    }
    file.close();
    
}

void Lekaren::saveProdukty()
{
    // Save produktov - zmena kusov na sklade po nakupe //

    QString fileName = "Lieky.txt";
    if (fileName.isEmpty()) { return; }
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << getZlava() << "," << "\n";
        for (size_t i = 0; i < pomocneProd.size(); i++)
        {
            out << pomocneProd[i].get_liek() << "," << pomocneProd[i].get_cena() << "," << pomocneProd[i].get_kusy() << "\n";

        }
    }
    else
    {
        return;
    }

    file.close();
}

void Lekaren::deleteData()
{
    // odstranenie potvrdenej objednavky == prevzatie objednavky zakaznikom //

    QString name = getUsername();
    name.append(".txt");
    QString path = "C:/Users/tomka/source/repos/Skuska/Skuska/Objednavky/";
    path.append(name);
    qDebug() << path << "\n";
    remove(path.toStdString().c_str());

    for (size_t i = 0; i < objednavky.size(); i++)
    {
        if (getUsername().append(",ready") == objednavky[i]) {
            objednavky.remove(i);
            objednavky.shrink_to_fit();
        }
    }

    QString fileName1 = "Objednavky.txt";                               // prepisanie suboru s menami objednavok
    //remove(fileName1.toStdString().c_str());
    if (fileName1.isEmpty()) { return; }
    for (size_t i = 0; i < objednavky.size(); i++)
    {
    }
    QFile file(fileName1);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text ))
    {
        QTextStream out(&file);
        for (size_t i = 0; i < objednavky.size(); i++)
        {
            out << objednavky[i] << "\n";
        }
        
    }
    else
    {
        return;
    }

    file.close();
    
}

void Lekaren::on_pushZobrazit_clicked()
{
    // zobrazenie vybranej objednavky v listLieky do TableWidgetu// 

   nakup.clear();

   QString currentOrder;
   currentOrder = objednavky[ui.listLieky->currentRow()];
   QString path = "C:/Users/tomka/source/repos/Skuska/Skuska/Objednavky/";

   currentOrder.append(".txt");
   path.append(currentOrder);
   if (QFile::exists(path))
   {
       QFile file(path);
       QTextStream stream(&file);
       QString line;
       
       if (file.open(QIODevice::ReadOnly | QIODevice::Text))
       {
           do {
               line = stream.readLine();
               if (!line.isNull()) {

                   QStringList data = line.split(",");
                   nakup.push_back(NAKUP(data.at(0), data.at(1).toFloat(), data.at(2).toInt()));
               }
           } while (!line.isNull());
           file.close();
       }
   }
   ui.tableWidgetObjednavka->setRowCount(nakup.size());
   ui.tableWidgetObjednavka->setColumnCount(3);

   for (size_t i = 0; i < nakup.size(); i++)
   {
       QTableWidgetItem* item = new QTableWidgetItem;
       item->setText(nakup[i].get_liek());
       ui.tableWidgetObjednavka->setItem(i, 0, item);

       QTableWidgetItem* item1 = new QTableWidgetItem;
       item1->setText(QString::number(nakup[i].get_cena()));
       ui.tableWidgetObjednavka->setItem(i, 1, item1);

       QTableWidgetItem* item2 = new QTableWidgetItem;
       item2->setText(QString::number(nakup[i].get_kusy()));
       ui.tableWidgetObjednavka->setItem(i, 2, item2);
   }

   ui.pushFaktura->setEnabled(TRUE);
}

void Lekaren::on_pushObjednavka_clicked()
{
    // zobrazenie objednavky ak este nie je pripravena na vyzdvihnutie, inak oznam že je pripravená a zároven imaginárne vyzdvihnutie // 

    if (r == 1) {
        QMessageBox conf;
        conf.setWindowTitle("Order");
        conf.setText("Vasa objednavka je pripravena k vyzdvihnutiu!");
        conf.exec();
        deleteData();
        saveObjednavky();
        ui.pushObjednat->setEnabled(TRUE);
        ui.pushKosik->setEnabled(TRUE);
        ui.pushOdstranit->setEnabled(TRUE);  
        r = 0;
    }

    ui.tableWidgetObjednavka->setRowCount(nakup.size());
    ui.tableWidgetObjednavka->setColumnCount(3);

    for (size_t i = 0; i < nakup.size(); i++)
    {
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setText(nakup[i].get_liek());
        ui.tableWidgetObjednavka->setItem(i, 0, item);

        QTableWidgetItem* item1 = new QTableWidgetItem;
        item1->setText(QString::number(nakup[i].get_cena()));
        ui.tableWidgetObjednavka->setItem(i, 1, item1);

        QTableWidgetItem* item2 = new QTableWidgetItem;
        item2->setText(QString::number(nakup[i].get_kusy()));
        ui.tableWidgetObjednavka->setItem(i, 2, item2);
    }
}

void Lekaren::on_pushPotvrdit_clicked()
{
    // zmena stavu objednavky na pripravenu //

    int ready = ui.listLieky->currentRow();
    objednavky[ready].append(",ready");
  
     QString fileName1 = "Objednavky.txt";                                      // ulozenie nazvu ready objednavky do txt file
     if (fileName1.isEmpty()) { return; }

     QFile file(fileName1);
     if (file.open(QIODevice::WriteOnly | QIODevice::Text))
     {
         QTextStream out(&file);
         for (size_t i = 0; i < objednavky.size(); i++)
         {
            out << objednavky[i] << "\n";
         }
         
         QMessageBox conf;
         conf.setWindowTitle("Order");
         conf.setText("Zmenili ste stav objednavky na - pripravene k vyzdvihnutiu!");
         conf.exec();
     }
     else
     {
         return;
     }

     file.close();
     ui.listLieky->takeItem(ready);
     for(size_t i = 0; i < nakup.size(); i++)
     {
         ui.tableWidgetObjednavka->removeRow(0);
     }

}

void Lekaren::on_pushZmenit_clicked()
{
    // zmena pozicie zamestnanca //

    QString newpoz = ui.comboStaff->currentText();

    zamestnanec[iCheck].set_pozicia(newpoz);
    saveUsers();

    QMessageBox conf;
    conf.setWindowTitle("Edit");
    conf.setText("Uspesne ste zmenili vasu pracovnu poziciu.");
    conf.exec();
}

void Lekaren::on_pushZmena_clicked()
{
    // zmena mena a adresy zakaznika // 

    QString newName = ui.lineName->text();
    QString newAdress = ui.lineAdress->text();
    if (newName != "" && newAdress != "") {
        zakaznik[iCheck].set_name(newName);
        zakaznik[iCheck].set_adress(newAdress);
        QMessageBox conf;
        conf.setWindowTitle("Edit");
        conf.setText("Uspesne ste zmenili vase údaje.");
        conf.exec();
    }
    else if (newName != "" && newAdress == "") {
        zakaznik[iCheck].set_name(newName);
        QMessageBox conf;
        conf.setWindowTitle("Edit");
        conf.setText("Uspesne ste zmenili vase udaje.");
        conf.exec();
    }
    else if (newName == "" && newAdress != "") {
        zakaznik[iCheck].set_adress(newAdress);
        QMessageBox conf;
        conf.setWindowTitle("Edit");
        conf.setText("Uspesne ste zmenili vase udaje.");
        conf.exec();
    }
    else {
        QMessageBox conf;
        conf.setWindowTitle("Edit");
        conf.setText("Nezadali ste ziadne nove udaje!");
        conf.exec();
    }
    saveUsers();
}

void Lekaren::on_pushFaktura_clicked()
{
    srand(time(NULL));
    int number = (rand() % 10000) + 1000;
    float total = 0;
    for (size_t i = 0; i < nakup.size(); i++)
    {
        total += nakup[i].get_cena() * nakup[i].get_kusy();
    }

    QString filename = "Objednavka_";
    QString path = "C:/Users/tomka/source/repos/Skuska/Skuska/Objednavky/";

    filename.append(QString::number(number)).append(".txt");

    time_t timetoday;
    time(&timetoday);
    

    path.append(filename);
    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << "Lekaren Tomka" << "\n";
        out << "Datum a cas: " << asctime(localtime(&timetoday));
        out << "Cislo objednavky: " << "\n" << number << "\n";
        out << "________________________________________" << "\n";

        for (size_t i = 0; i < nakup.size(); i++)
        {
            out << nakup[i].get_kusy() << "ks" << " .... " << nakup[i].get_liek() << ", " << nakup[i].get_cena() << " EUR"  << "\n";
        }

        out << "________________________________________" << "\n";
        out << "Celkovo: " << total << " EUR \n";

        QMessageBox conf;
        conf.setWindowTitle("Order");
        conf.setText("Uspesne ste vystavili fakturu pre tuto objednavku!");
        conf.exec();
    }
    else
    {
        return;
    }

    file.close();
    ui.pushPotvrdit->setEnabled(TRUE);
}

void Lekaren::on_pushOdhlasenie_clicked()
{       
    int newOrder = 0;
    nakup.append(actnakup);
    float aspend = zakaznik[iCheck].get_spend();
    zakaznik[iCheck].set_spend(aspend + cenan);
    for (size_t i = 0; i < objednavky.size(); i++)              // kontrola, èi už existuje objednavka na taketo meno
    {
        if (getUsername() == objednavky[i]) {
            newOrder = 1;
        }
    }
    if (getIndex() == 2) {

        if (actnakup.size() != 0)
        {

            // ulozenie objednavky do Username.txt file // 

            QString filename = getUsername();
            QString path = "C:/Users/tomka/source/repos/Skuska/Skuska/Objednavky/";

            filename.append(".txt");
            path.append(filename);
            QFile file(path);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream out(&file);
                for (size_t i = 0; i < nakup.size(); i++)
                {
                    out << nakup[i].get_liek() << "," << nakup[i].get_cena() << "," << nakup[i].get_kusy() << "\n";
                }
            }
            else
            {
                return;
            }

            file.close();

            // ak neexistuje este, tak ulozenie nazvu objednavky do txt file // 
            if (newOrder == 0)
            {
                saveObjednavky();
            }
            
        }
    }

    saveUsers();

    Login* log = new Login();
    this->hide();
    log->show();
}
