#include "Admin.h"



Admin::Admin(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}
Admin::~Admin()
{
	delete this;
}

void Admin::closeEvent(QCloseEvent* event)
{
    // dialog pri zatvoreni Admina

    if (QMessageBox::Yes == QMessageBox::question(this, "Close Confirmation", "Are you sure you want to exit?", QMessageBox::Yes | QMessageBox::No))
    {
        event->accept();
    }
    else {
        event->ignore();
    }
}


void Admin::loadUsers()
{
    // nacitanie dat o users zo suboru //

    QString fileName = "Users.txt";
    if (fileName.isEmpty()) { return; }

    QFile file(fileName);
    QTextStream stream(&file);
    QString line;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text )) 
    {
        do {
            line = stream.readLine();
            if (!line.isNull()) {

                QStringList data = line.split(",");
                ui.comboName->addItem(data.at(1));

                if (line.split(",").at(0) == "0")
                {                   
                    admin2.push_back(ADMIN(data.at(0).toInt(), data.at(1), data.at(2)));
                }
                else if (line.split(",").at(0) == "1")
                {
                    staff2.push_back(STAFF(data.at(0).toInt(), data.at(1), data.at(2), data.at(3)));
                }

                else if (line.split(",").at(0) == "2")
                {
                    user2.push_back(USER(data.at(0).toInt(), data.at(1), data.at(2), data.at(3), data.at(4),data.at(5).toFloat()));
                }


            }
        } while (!line.isNull());
        file.close();
    }
}

void Admin::loadProdukty()
{
    // nacitanie liekov //

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
                    }
                    
            }
        } while (!line.isNull());
        file.close();
    }

    for (size_t i = 0; i < produkty.size(); i++)
    {
        ui.listLieky->addItem(produkty[i].get_liek());
        ui.listCena->addItem(QString("%1").arg(produkty[i].get_cena()));
        ui.listKusy->addItem(QString("%1").arg(produkty[i].get_kusy()));
    }
}

void Admin::saveProdukty()
{
    // Save produktov po edite //

    QString fileName = "Lieky.txt";                                                               
    if (fileName.isEmpty()) { return; }
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << getZlava() << "," <<  "\n";
        for (size_t i = 0; i < produkty.size(); i++)
        {   
            
            out << produkty[i].get_liek() << "," << produkty[i].get_cena() << "," << produkty[i].get_kusy() << "\n";

        }
    }
    else
    {
        return;
    }

    file.close();
}

void Admin::loadObjednavky()
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

void Admin::saveObjednavky(QString oldUs, QString newUs)
{
    // ulozenie zmenu username aj do suboru s objednavkami // 

    QString oldUs2 = oldUs;
    QString newUs2 = newUs;
    oldUs2.append(",ready");
    newUs2.append(",ready");

    for (size_t i = 0; i < objednavky.size(); i++)
    {
        if (oldUs == objednavky[i]) { 
            objednavky[i] = newUs;
        }
        if (oldUs2 == objednavky[i]) {
            objednavky[i] = newUs2;
        }
    }

        QString fileName1 = "Objednavky.txt";
        if (fileName1.isEmpty()) { return; }

        QFile file(fileName1);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text ))
        {
            QTextStream out(&file);
            for (size_t i = 0; i < objednavky.size(); i++) {
                out << objednavky[i] << "\n";
            }
        }
        else
        {
            return;
        }

        file.close();
}

void Admin::on_comboTyp_currentTextChanged()
{    
    // uprava vzhladu groupBoxu na pridanie users // 

    if (ui.comboTyp->currentText() == "Admin") 
    {
        ui.lineName->hide();
        ui.label_6->hide();
        ui.lineAdress->hide();
        ui.label_7->hide();
        ui.label_8->hide();
        ui.comboStaff->hide();
    }
    if (ui.comboTyp->currentText() == "Zamestnanec") 
    {
        ui.lineName->hide();
        ui.label_6->hide();
        ui.lineAdress->hide();
        ui.label_7->hide();
        ui.label_8->show();
        ui.comboStaff->show();
    }
    if (ui.comboTyp->currentText() == "Zakaznik") 
    {
        ui.lineName->show();
        ui.label_6->show();
        ui.lineAdress->show();
        ui.label_7->show();
        ui.label_8->hide();
        ui.comboStaff->hide();
    }

}

void Admin::on_checkAdd_clicked()
{
    // uprava vzhladu Admin layoutu //

    ui.groupBoxAdd->setEnabled(TRUE);
    ui.groupBoxEdit->setDisabled(TRUE);
    ui.checkEdit->setChecked(FALSE);
    ui.checkEdit_2->setChecked(FALSE);
    ui.groupBox_5->setDisabled(TRUE);
    ui.groupBox_6->setDisabled(TRUE);
    ui.groupBox_7->setDisabled(TRUE);
    ui.groupBox_10->setDisabled(TRUE);
    ui.comboName->clear();
    

}

void Admin::on_checkEdit_clicked()
{
    // uprava vzhladu Admin layoutu //

    ui.groupBoxEdit->setEnabled(TRUE);
    ui.groupBoxAdd->setDisabled(TRUE);
    ui.checkAdd->setChecked(FALSE);
    ui.checkEdit_2->setChecked(FALSE);
    ui.groupBox_5->setDisabled(TRUE);
    ui.groupBox_6->setDisabled(TRUE);
    ui.groupBox_7->setDisabled(TRUE);
    ui.groupBox_10->setDisabled(TRUE);
    ui.comboName->clear();
    loadUsers();
}

void Admin::on_comboName_currentTextChanged()
{
    // uprava vzhladu Edit layoutu - ak je checked checkEdit // 

    QString username = ui.comboName->currentText();
    if (ui.checkEdit->isChecked()) {
        QString fileName = "Users.txt";
        if (fileName.isEmpty()) { return; }

        QFile file(fileName);
        QTextStream stream(&file);
        QString line;

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) 
        {
            do {                                                                // cyklus na zistenie typu usera 
                line = stream.readLine();
                if (!line.isNull())
                {

                    if (line.split(",").at(1) == username)
                    {
                        typ = line.split(",").at(0).toInt();
                    }
                }
            } while (line.split(",").at(1) != username);
            file.close();
        }
        qDebug() << typ;
        if (typ == 0) 
        {
            ui.label_12->hide();
            ui.label_13->hide();
            ui.label_14->hide();
            ui.lineName_2->hide();
            ui.lineAdress_2->hide();
            ui.lineStaff_2->hide();
        }
        if (typ == 1) 
        {
            ui.label_12->hide();
            ui.label_13->hide();
            ui.lineName_2->hide();
            ui.lineAdress_2->hide();
            ui.label_14->show();
            ui.lineStaff_2->show();
        }
        if (typ == 2) 
        {
            ui.label_12->show();
            ui.label_13->show();
            ui.lineName_2->show();
            ui.lineAdress_2->show();
            ui.lineStaff_2->hide();
            ui.label_14->hide();
        }
    }
}

void Admin::on_pushOdhlasenie_clicked()
{
    Login* log = new Login();
    this->hide();
    log->show();
}

void Admin::on_checkEdit_2_clicked()
{
    ui.groupBox_2->setEnabled(TRUE);
    ui.groupBox_3->setEnabled(TRUE);
    ui.groupBoxAdd->setDisabled(TRUE);
    ui.groupBoxEdit->setDisabled(TRUE);
    ui.groupBox_5->setEnabled(TRUE);
    ui.groupBox_6->setEnabled(TRUE);
    ui.groupBox_7->setEnabled(TRUE);
    ui.groupBox_10->setEnabled(TRUE);
    ui.checkAdd->setChecked(FALSE);
    ui.checkEdit->setChecked(FALSE);
    loadProdukty();
}

void Admin::on_pushPotvrdit_2_clicked()
{
    // potvrdzovacie tlacitko pri edite produktov adminom //

    if (ui.checkBoxAdd->isChecked())                                                                          // ADD PRODUKT //
    {
        ui.checkBoxDelete->setChecked(FALSE);
        ui.checkBoxEdit->setChecked(FALSE);

        if (ui.lineLiek->text() != "" && ui.lineCena->text() != "" && ui.lineKusy->text() != "") 
        {
            produkty.push_back(LIEK(ui.lineLiek->text(), ui.lineCena->text().toFloat(), ui.lineKusy->text().toInt()));
            int pocet = produkty.size();

            QString fileName = "Lieky.txt";
            if (fileName.isEmpty()) { return; }

            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
            {
                QTextStream out(&file);
                out << produkty[pocet-1].get_liek() << "," << produkty[pocet-1].get_cena() << "," << produkty[pocet-1].get_kusy() << "\n";
            }
            else
            {
                return;
            }

            file.close();
            QMessageBox conf;
            conf.setWindowTitle("Lieky");
            conf.setText("Zapisali ste novy liek!");
            conf.exec();
        }
        else
        {
            QMessageBox conf;
            conf.setWindowTitle("Lieky");
            conf.setText("Nespravne zadany novy liek!");
            conf.exec();
        }
    }

    if (ui.checkBoxDelete->isChecked()) {                                                                       // DELETE PRODUKT //

        if (ui.listLieky->currentRow() > -1 && ui.listLieky->currentRow() < produkty.size()) {
            int index = ui.listLieky->currentRow();
            qDebug() << index << "\n";
            qDebug() << produkty[index].get_liek() << "," << produkty[index].get_cena() << "," << produkty[index].get_kusy() << "\n";
            produkty.remove(index);
            produkty.shrink_to_fit();
            saveProdukty();
            QMessageBox conf;
            conf.setWindowTitle("Lieky");
            conf.setText("Odstranili ste vybrany liek!");
            conf.exec();
        }
        else {
            QMessageBox conf;
            conf.setWindowTitle("Lieky");
            conf.setText("Nevybrali ste ziadny liek na odstranenie!");
            conf.exec();
        }
    }

    if (ui.checkBoxEdit->isChecked()) 
    {
        if (ui.lineLiek->text() != "") 
        {
            if (ui.listLieky->currentRow() > -1 && ui.listLieky->currentRow() < produkty.size()) {
                if (ui.lineCena->text() == "" && ui.lineKusy->text() == "")
                {
                    int index = ui.listLieky->currentRow();
                    QString newLiek = ui.lineLiek->text();
                    produkty[index].set_liek(newLiek);
                    saveProdukty();
                    QMessageBox conf;
                    conf.setWindowTitle("Lieky");
                    conf.setText("Uspesne ste zmenili nazov produktu!");
                    conf.exec();

                }
                else
                {
                    QMessageBox conf;
                    conf.setWindowTitle("Lieky");
                    conf.setText("Nie je mozne editovat viacej udajov naraz!");
                    conf.exec();

                }
            }
            else {
                QMessageBox conf;
                conf.setWindowTitle("Lieky");
                conf.setText("Nevybrali ste ziadny liek na edit!");
                conf.exec();
            }

        }

        if (ui.lineCena->text() != "")
        {
            if (ui.listCena->currentRow() > -1 && ui.listCena->currentRow() < produkty.size()) {
                if (ui.lineLiek->text() == "" && ui.lineKusy->text() == "")
                {
                    int index = ui.listCena->currentRow();
                    float newCena = ui.lineCena->text().toFloat();
                    produkty[index].set_cena(newCena);
                    saveProdukty();
                    QMessageBox conf;
                    conf.setWindowTitle("Lieky");
                    conf.setText("Uspesne ste zmenili cenu produktu!");
                    conf.exec();

                }
                else
                {
                    QMessageBox conf;
                    conf.setWindowTitle("Lieky");
                    conf.setText("Nie je mozne editovat viacej udajov naraz!");
                    conf.exec();

                }
            }
            else {
                QMessageBox conf;
                conf.setWindowTitle("Lieky");
                conf.setText("Nevybrali ste ziadny liek na edit!");
                conf.exec();
            }

        }

        if (ui.lineKusy->text() != "")
        {
            if (ui.listKusy->currentRow() > -1 && ui.listKusy->currentRow() < produkty.size()) {
                if (ui.lineLiek->text() == "" && ui.lineCena->text() == "")
                {
                    int index = ui.listKusy->currentRow();
                    int newKusy = ui.lineKusy->text().toInt();
                    produkty[index].set_kusy(newKusy);
                    saveProdukty();
                    QMessageBox conf;
                    conf.setWindowTitle("Lieky");
                    conf.setText("Uspesne ste zmenili pocet kusov produktu!");
                    conf.exec();

                }
                else
                {
                    QMessageBox conf;
                    conf.setWindowTitle("Lieky");
                    conf.setText("Nie je mozne editovat viacej udajov naraz!");
                    conf.exec();

                }
            }
            else {
                QMessageBox conf;
                conf.setWindowTitle("Lieky");
                conf.setText("Nevybrali ste ziadny liek na edit!");
                conf.exec();
            }
        }    
        
    }

    if (ui.checkEdit_2->isChecked()) {
        if (ui.spinBoxZlava->value() != 0)
        {
            zlava = ui.spinBoxZlava->value();
            saveProdukty();
            QMessageBox conf;
            conf.setWindowTitle("Zlava");
            conf.setText("Uspesne ste zmenili vysku zlavy!");
            conf.exec();
        }
        
    }


    Admin* reset = new Admin();
    this->hide();
    reset->show();

}


void Admin::on_pushPotvrdit_clicked()
{
    // potvrdenie bud pridania noveho user alebo upravu existujuceho user // 

    // pridanie noveho user // 

    int done = 0;
    if (ui.checkAdd->isChecked()) {

        int count = 0;
        int index = ui.comboTyp->currentIndex();
        QString uname = ui.lineUsername->text();
        QString sifra;
        if (ui.lineUsername->text() != "" && ui.linePassword->text() == ui.lineRePassword->text()) {
            if (index == 0)
            {
                count = admin2.size();
                sifra = QCryptographicHash::hash(ui.linePassword->text().toStdString().c_str(), QCryptographicHash::Sha3_224).toHex();
                admin2.push_back(ADMIN(index, ui.lineUsername->text(), sifra));

                QString fileName = "Users.txt";
                if (fileName.isEmpty()) { return; }

                QFile file(fileName);
                if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
                {
                    QTextStream out(&file);
                    out << index << "," << admin2[count].get_username() << "," << admin2[count].get_password() << "\n";

                }
                else
                {
                    return;
                }

                file.close();
                QMessageBox conf;
                conf.setWindowTitle("Sign Up");
                conf.setText("The operation completed successfully!");
                conf.exec();


            }

            if (index == 1)
            {
                count = staff2.size();
                sifra = QCryptographicHash::hash(ui.linePassword->text().toStdString().c_str(), QCryptographicHash::Sha3_224).toHex();
                staff2.push_back(STAFF(index, ui.lineUsername->text(), sifra, ui.comboStaff->currentText()));

                QString fileName = "Users.txt";
                if (fileName.isEmpty()) { return; }

                QFile file(fileName);
                if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
                {
                    QTextStream out(&file);
                    out << index << "," << staff2[count].get_username() << "," << staff2[count].get_password() << "," << staff2[count].get_pozicia() << "\n";
                }
                else
                {
                    return;
                }

                file.close();
                QMessageBox conf;
                conf.setWindowTitle("Sign Up");
                conf.setText("The operation completed successfully!");
                conf.exec();


            }

            if (index == 2)
            {
                count = user2.size();
                sifra = QCryptographicHash::hash(ui.linePassword->text().toStdString().c_str(), QCryptographicHash::Sha3_224).toHex();
                user2.push_back(USER(index, ui.lineUsername->text(), sifra, ui.lineName->text(), ui.lineAdress->text(),0));

                QString fileName = "Users.txt";
                if (fileName.isEmpty()) { return; }

                QFile file(fileName);
                if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
                {
                    QTextStream out(&file);
                    for (size_t i = 0; i < user2.size(); i++)
                    {
                        out << index << "," << user2[count].get_username() << "," << user2[count].get_password() << "," << user2[count].get_name() << "," << user2[count].get_adress() << "," << user2[count].get_spend() <<  "\n";
                    }
                }
                else
                {
                    return;
                }

                file.close();
                QMessageBox conf;
                conf.setWindowTitle("Sign Up");
                conf.setText("The operation completed successfully!");
                conf.exec();


            }

        }
        else
        {
            QMessageBox conf;
            conf.setWindowTitle("Sign Up");
            conf.setText("The operation failed. Try again.");
            conf.exec();
        }

    }
    
    // edit uz existujuceho user // 

   if (ui.checkEdit->isChecked()) {

       QString uname = ui.comboName->currentText();
       QString newUname = ui.lineUsername_2->text();
       QString newPassword = ui.linePassword_2->text();
       QString newRepassword = ui.lineRePassword_2->text();
       QString newName = ui.lineName_2->text();
       QString newAdress = ui.lineAdress_2->text();
       QString staff = ui.lineStaff_2->text();
       int done = 0, j = 0;

       if (typ == 0) {                                                                                              // EDIT ADMINOV //
           j = 0,
           done = 0;
           
           if (newUname != "") {                                                                                    // zmena username 
               if (newPassword == ""){

                    for (size_t i = 0; i < admin2.size(); i++) {                                                    // kontrola, ci take username uz neexistuje
                        if (newUname == admin2[i].get_username()) {

                            j++;                   
                            QMessageBox conf;
                            conf.setWindowTitle("User edit");
                            conf.setText("Takyto user uz existuje!");
                            conf.exec();
                        }
                    }
                    if (j == 0)
                    {
                        for (size_t i = 0; i < admin2.size(); i++)                                                  // cyklus na zistenie, ktoreho admina username menime
                        {
                            if (uname == admin2[i].get_username()) {
                                admin2[i].set_username(newUname);
                                done++;
                            }

                        }
                    }
               }
               else
               {
                   QMessageBox conf;
                   conf.setWindowTitle("User edit");
                   conf.setText("Nie je mozne menit viacej udajov naraz!");
                   conf.exec();
               }
           }                                
           if (newPassword != "") {                                                   // zmena password 
               if (newPassword == newRepassword) {
                   if (newUname == "") {
                       QString newUpass = QCryptographicHash::hash(newPassword.toStdString().c_str(), QCryptographicHash::Sha3_224).toHex();

                       for (size_t i = 0; i < admin2.size(); i++)
                       {
                           if (uname == admin2[i].get_username()) {
                               admin2[i].set_password(newUpass);
                               done++;
                           }
                       }
                   }
                   else
                   {
                       QMessageBox conf;
                       conf.setWindowTitle("User edit");
                       conf.setText("Nie je mozne menit viacej udajov naraz!");
                       conf.exec();
                   }
               }
               else
               {
                   QMessageBox conf;
                   conf.setWindowTitle("User edit");
                   conf.setText("Vase hesla sa nezhoduju!");
                   conf.exec();
               }
           }
           
           QString fileName = "Users.txt";                                                                          // zapisanie zmien do txt file 
           if (fileName.isEmpty()) { return; }
           QFile file(fileName);
           if (file.open(QIODevice::WriteOnly | QIODevice::Text))
           {
               QTextStream out(&file);
               for (size_t i = 0; i < admin2.size(); i++)
               {
                   out << "0" << "," << admin2[i].get_username() << "," << admin2[i].get_password() << "\n";
               }
               for (size_t i = 0; i < staff2.size(); i++)
               {
                   out << "1" << "," << staff2[i].get_username() << "," << staff2[i].get_password() << "," << staff2[i].get_pozicia() << "\n";
               }
               for (size_t i = 0; i < user2.size(); i++)
               {
                   out << "2" << "," << user2[i].get_username() << "," << user2[i].get_password() << "," << user2[i].get_name() << "," << user2[i].get_adress() << "\n";
               }

           }
           else
           {
               return;
           }

           file.close();
           if (done > 0) {
               QMessageBox conf;
               conf.setWindowTitle("User edit");
               conf.setText("The operation completed successfully!");
               conf.exec();
           }

           done = 0;
       }

       if (typ == 1)                                                                            // EDIT STAFF //
       {
           done = 0;
           j = 0;
           if (newUname != "") 
           {                                                                                    // zmena username 
               if (newPassword == "" && staff == "") 
               {
                   for (size_t i = 0; i < staff2.size(); i++) {                                 // kontrola, ci take username uz neexistuje
                       if (newUname == staff2[i].get_username()) {

                           j++;                   
                           QMessageBox conf;
                           conf.setWindowTitle("User edit");
                           conf.setText("Takyto user uz existuje!");
                           conf.exec();
                       }
                   }
                   if (j == 0)
                   {
                       for (size_t i = 0; i < staff2.size(); i++)                               // cyklus na zistenie, ktoreho zamestnanca username menime
                       {
                           if (uname == staff2[i].get_username()) {
                               staff2[i].set_username(newUname);
                               done++;
                           }
                       }
                   }
               }
               else {
                    QMessageBox conf;
                    conf.setWindowTitle("User edit");
                    conf.setText("Nie je mozne menit viacej udajov naraz!");
                    conf.exec();
               }
           }

           if (newPassword != "" ) {                                                            // zmena password 
               if (newPassword == newRepassword) {
                   if (newUname == "" && staff == "") {
                       QString newUpass = QCryptographicHash::hash(newPassword.toStdString().c_str(), QCryptographicHash::Sha3_224).toHex();

                       for (size_t i = 0; i < staff2.size(); i++)
                       {
                           if (uname == staff2[i].get_username()) {
                               staff2[i].set_password(newUpass);
                               done++;
                           }
                       }
                   }
                   else
                   {
                       QMessageBox conf;
                       conf.setWindowTitle("User edit");
                       conf.setText("Nie je mozne menit viacej udajov naraz!");
                       conf.exec();
                   }
               }
               else
               {
                    QMessageBox conf;
                    conf.setWindowTitle("User edit");
                    conf.setText("Vase hesla sa nezhoduju!");
                    conf.exec();
               }
               
           }
           if (staff != "") {                                                                             // zmena pozicie zamestnanca 
               if (staff == "Farmaceut" || staff == "Laborant" || staff == "Sanitar") 
               {
                   if (newUname == "" && newPassword == "") {
                       for (size_t i = 0; i < staff2.size(); i++)
                       {
                           if (uname == staff2[i].get_username()) {
                               staff2[i].set_pozicia(staff);
                               done++;
                           }
                       }
                   }
                   else
                   {
                       QMessageBox conf;
                       conf.setWindowTitle("User edit");
                       conf.setText("Nie je mozne menit viacej udajov naraz!");
                       conf.exec();
                   }
               }
               else
               {
                   QMessageBox conf;
                   conf.setWindowTitle("User edit");
                   conf.setText("Takato pozicia neexistuje!");
                   conf.exec();
               }

           }
       }
       
       if (typ == 2) 
       {
           if (newUname != "")                                                                  // zmena username pre zakaznika
           {
               if (newPassword == "" && newName == "" && newAdress == "")
               {
                   for (size_t i = 0; i < user2.size(); i++) {                                 // kontrola, ci take username uz neexistuje
                       if (newUname == user2[i].get_username()) {

                           j++;
                           QMessageBox conf;
                           conf.setWindowTitle("User edit");
                           conf.setText("Takyto user uz existuje!");
                           conf.exec();
                       }
                   }
                   if (j == 0)
                   {
                       for (size_t i = 0; i < staff2.size(); i++)                               // cyklus na zistenie, ktoreho zakaznika username menime
                       {
                           if (uname == user2[i].get_username()) {
                              
                               QString povod = user2[i].get_username();
                               QString povodne = user2[i].get_username();                               
                               povodne.append(".txt");
                               QString path = "C:/Users/tomka/source/repos/Skuska/Skuska/Objednavky/";
                               path.append(povodne);

                               user2[i].set_username(newUname);                                                 // nastavenie noveho username
                               done++;
                               
                               QString nov = user2[i].get_username();
                               QString nove = user2[i].get_username();   
                               nove.append(".txt");
                               
                               QString path2 = "C:/Users/tomka/source/repos/Skuska/Skuska/Objednavky/";
                               path2.append(nove);

                               QFile::rename(path.toStdString().c_str(), path2.toStdString().c_str());        // zmena nazvu objednavky
                               loadObjednavky();
                               saveObjednavky(povod,nov);
                           }
                       }
                   }
               }
               else {
                   QMessageBox conf;
                   conf.setWindowTitle("User edit");
                   conf.setText("Nie je mozne menit viacej udajov naraz!");
                   conf.exec();
               }

           }
           if (newPassword != "")                                                                   // zmena hesla pre zakaznika
           {
               if (newPassword == newRepassword) 
               {
                   if (newName == "" && newAdress == "" && newUname == "") 
                   {
                       QString newUpass = QCryptographicHash::hash(newPassword.toStdString().c_str(), QCryptographicHash::Sha3_224).toHex();

                       for (size_t i = 0; i < user2.size(); i++)
                       {
                           if (uname == user2[i].get_username()) {
                               user2[i].set_password(newUpass);
                               done++;
                           }
                       }
                   }
                   else
                   {
                       QMessageBox conf;
                       conf.setWindowTitle("User edit");
                       conf.setText("Nie je mozne menit viacej udajov naraz!");
                       conf.exec();
                   }
               }
               else
               {
                   QMessageBox conf;
                   conf.setWindowTitle("User edit");
                   conf.setText("Vase hesla sa nezhoduju!");
                   conf.exec();
               }
           }

           if (newName != "") 
           {                                                             // zmena mena pre zakaznika
               if (newPassword == "" && newUname == "" && newAdress == "") 
               {
                   for (size_t i = 0; i < staff2.size(); i++)                               // cyklus na zistenie, ktoreho zakaznika username menime
                   {
                       if (uname == user2[i].get_username()) 
                       {
                           qDebug() << user2[i].get_name();
                           user2[i].set_name(newUname);
                           qDebug() << user2[i].get_name();
                           done++;
                       }
                   }
               }
               else 
               {
                    QMessageBox conf;
                    conf.setWindowTitle("User edit");
                    conf.setText("Nie je mozne menit viacej udajov naraz!");
                    conf.exec();
               }
           }

           if (newAdress != "") 
           {
               if (newPassword == "" && newUname == "" && newName == "") 
               {
                   for (size_t i = 0; i < staff2.size(); i++)                               // cyklus na zistenie, ktoreho zakaznika username menime
                   {
                       if (uname == user2[i].get_username())
                       {
                           qDebug() << user2[i].get_adress();
                           user2[i].set_adress(newAdress);
                           qDebug() << user2[i].get_adress();
                           done++;
                       }
                   }
               }
               else 
               {
                   QMessageBox conf;
                   conf.setWindowTitle("User edit");
                   conf.setText("Nie je mozne menit viacej udajov naraz!");
                   conf.exec();
               }
           }
       }

       if (ui.checkDelete->isChecked()) {                                                // DELETE USER // 
           QString user = ui.comboName->currentText();
           if (typ == 0) {
               for (size_t i = 0; i < admin2.size(); i++)                               // cyklus na zistenie, ktoreho admina odstranujeme
               {
                   if (user == admin2[i].get_username()) {
                       admin2.remove(i);
                       admin2.shrink_to_fit();
                       done++;
                   }
               }
           }
           if (typ == 1) {
               for (size_t i = 0; i < staff2.size(); i++)                               // cyklus na zistenie, ktoreho zamestnanca odstranujeme
               {
                   if (user == staff2[i].get_username()) {
                       staff2.remove(i);
                       staff2.shrink_to_fit();
                       done++;
                   }
               }
           }
           if (typ == 2) {
               for (size_t i = 0; i < user2.size(); i++)                               // cyklus na zistenie, ktoreho zakaznika odstranujeme
               {
                   if (user == user2[i].get_username()) {
                       user2.remove(i);
                       user2.shrink_to_fit();
                       done++;
                   }
               }
           }
       }
       QString fileName = "Users.txt";                                                                          // zapisanie zmien do txt file 
       if (fileName.isEmpty()) { return; }
       QFile file(fileName);
       if (file.open(QIODevice::WriteOnly | QIODevice::Text))
       {
           QTextStream out(&file);
           for (size_t i = 0; i < admin2.size(); i++)
           {
               out << "0" << "," << admin2[i].get_username() << "," << admin2[i].get_password() << "\n";
           }
           for (size_t i = 0; i < staff2.size(); i++)
           {
               out << "1" << "," << staff2[i].get_username() << "," << staff2[i].get_password() << "," << staff2[i].get_pozicia() << "\n";
           }
           for (size_t i = 0; i < user2.size(); i++)
           {
               out << "2" << "," << user2[i].get_username() << "," << user2[i].get_password() << "," << user2[i].get_name() << "," << user2[i].get_adress() << "," << user2[i].get_spend() << "\n";
           }

       }
       else
       {
           return;
       }

       file.close();
       if (done > 0) {

           QMessageBox conf;
           conf.setWindowTitle("User edit");
           conf.setText("The operation completed successfully!");
           conf.exec();

       }
   }           


    done = 0; 
    Admin* reset = new Admin();
    this->hide();
    reset->show();
}

