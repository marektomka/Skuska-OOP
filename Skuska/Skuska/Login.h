#pragma once

#include <QtWidgets>
#include "ui_Login.h"

class USER {
protected:
    QString name;
    QString username;
    QString password;
    int typ;
    QString adress;
    float spend;

public:

    
    USER() { typ = NULL, username = "", password = "", name = "", adress = "", spend == NULL ; }
    USER(int typ1, QString usern, QString pass, QString nam, QString adress1, float spen) { typ = typ1, username = usern, password = pass, name = nam, adress = adress1, spend = spen; }

    void set_name(QString nam) { name = nam; }
    void set_username(QString usern) { username = usern;}
    void set_password(QString pass) { password = pass;  }
    void set_typ(int typ1) { typ = typ1;  }
    void set_adress(QString adress1) { adress = adress1; }
    void set_spend(float spen) { spend = spen; }

    QString get_name() { return name; }
    QString get_username() { return username; }
    QString get_password() { return password; }
    int get_typ() { return typ; }
    QString get_adress() { return adress; }
    float get_spend() { return spend; }
};

class ADMIN : public USER {
public:
    ADMIN() { typ = NULL, username = "", password = ""; }
    ADMIN(int typ1, QString usern, QString pass) { typ = typ1, username = usern, password = pass; }
};

class STAFF : public USER {
protected:
    QString pozicia;
public:
    STAFF() { typ = NULL, username = "", password = "", pozicia = ""; }
    STAFF(int typ1, QString usern, QString pass, QString poz) { typ = typ1, username = usern, password = pass, pozicia = poz; }
    QString set_pozicia(QString poz) { pozicia = poz; return pozicia; }
    QString get_pozicia() { return pozicia; }

};



class Login : public QMainWindow
{
    Q_OBJECT

public:
    Login(QWidget *parent = Q_NULLPTR);
    QVector<USER> users;
    QVector<ADMIN> admin;
    QVector<STAFF> staff; 
    QVector<QString> name;
    
    void loadData();
    int check();
    int getIndex() { return usIndex; }
    void setZlava(int zlav) { zlava = zlav; }
    int getZlava() { return zlava; }

private:
    Ui::LoginClass ui;
    int usIndex;
    void closeEvent(QCloseEvent* event);
    int zlava;

private slots:
    int on_comboTyp1_currentTextChanged();
    void on_pushReg_clicked();
    void on_pushLogin_clicked();
};
