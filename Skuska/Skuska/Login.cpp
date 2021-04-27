#include "Login.h"
#include "Registracia.h"
#include "Admin.h"
#include "Lekaren.h"

int zlavicka;
void Login::loadData()
{
    // nacitanie dat o users zo suboru //

    QString fileName = "Users.txt";
    if (fileName.isEmpty()) { return;}

    QFile file(fileName);
    QTextStream stream(&file);
    QString line;
    
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        do {
            line = stream.readLine();
            if (!line.isNull()) {

                QStringList data = line.split(",");
                ui.comboMeno->addItem(data.at(1));

                if (data.at(0) == "0")
                {                    
                    admin.push_back(ADMIN(data.at(0).toInt(), data.at(1), data.at(2)));            
                }
                else if (line.split(",").at(0) == "1")
                {
                    staff.push_back(STAFF(data.at(0).toInt(), data.at(1), data.at(2), data.at(3)));
                }
                
                else if (line.split(",").at(0) == "2")
                {
                    users.push_back(USER(data.at(0).toInt(), data.at(1), data.at(2), data.at(3), data.at(4),data.at(5).toFloat()));
                }
                
                
            }
        } while (!line.isNull());
        file.close();
    }

}

int Login::check()
{
    // funkcia pre cely cyklus pri prihlasovani // 

    int index, index2;
    index = ui.comboMeno->currentIndex();
    index2 = ui.comboTyp1->currentIndex();
    QString pass, pass1;
    pass1 = QCryptographicHash::hash(ui.linePassword->text().toStdString().c_str(), QCryptographicHash::Sha3_224).toHex();

    if (index2 == 0) {
        pass = admin[index].get_password();
    }
    if (index2 == 1) {
        pass = staff[index].get_password();
    }
    if (index2 == 2) {
        pass = users[index].get_password();
    }

    if (ui.linePassword->text() == "" || pass1 != pass) {
        return 1;
    }
    else {
        return 0;
    }
}


Login::Login(QWidget *parent)
    : QMainWindow(parent)
{
    
    ui.setupUi(this);
    loadData();
    ui.comboMeno->setDisabled(TRUE);
   
}

int Login::on_comboTyp1_currentTextChanged()
{
    // zmena moznosti pri logine - po vybere typu sa zobrazia iba vyhuvujuce mena //

    ui.comboMeno->setEnabled(TRUE);
    ui.comboMeno->clear();
    QString typ;
    typ = ui.comboTyp1->currentText();
    if (typ == "Admin") {
        usIndex = 0;
        for (size_t i = 0; i < admin.size(); i++)
        {           
            ui.comboMeno->addItem(admin[i].get_username());
        }
        
    }
    if (typ == "Zamestnanec") {
        usIndex = 1;
        for (size_t i = 0; i < staff.size(); i++)
        {           
            ui.comboMeno->addItem(staff[i].get_username());
        }        
    }
    if (typ == "Zakaznik") {
         usIndex = 2;
        for (size_t i = 0; i < users.size(); i++)
        {
            ui.comboMeno->addItem(users[i].get_username());
        }       
    }
    return usIndex;
}

void Login::on_pushReg_clicked()
{
    // prepnutie na Registracia.ui

    Registracia* druhe = new Registracia();
    this->hide();
    druhe->isModal();
    druhe->show();
}

void Login::on_pushLogin_clicked()
{   
    int checkValue = 5;
    checkValue = check();
        
    if (checkValue == 1) {
        QMessageBox log;
        log.setWindowTitle("Login");
        log.setText("Wrong username or password. Try again.");
        log.exec();
    }
    else if (checkValue == 0){

        if (getIndex() == 0) {
            Admin* udaje = new Admin();
            this->hide();
            udaje->show();
        }
        if (getIndex() == 1 || getIndex() == 2) {
            qDebug() << getIndex() << "\n";
            Lekaren* obchod = new Lekaren(getIndex(),ui.comboMeno->currentText());
            this->hide();
            obchod->show();                                     
        }

    }
    
}

void Login::closeEvent(QCloseEvent* event)
{
    // dialog po zatvoreni Login.ui

    if (QMessageBox::Yes == QMessageBox::question(this, "Close Confirmation", "Are you sure you want to exit?", QMessageBox::Yes | QMessageBox::No))
    {
        event->accept();
    }
    else {
        event->ignore();
    }
}
