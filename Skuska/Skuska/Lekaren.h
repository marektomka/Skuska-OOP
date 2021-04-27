#pragma once

#include <QWidget>
#include "ui_Lekaren.h"
#include "Login.h"
#include "Admin.h"


class NAKUP {
private:
	QString nazov;
	float cena;
	int pocet;
public:
	NAKUP() { nazov = "", cena == NULL, pocet = NULL; }
	NAKUP(QString naz, float cen, int poc) { nazov = naz, cena = cen, pocet = poc; }
	QString get_liek() { return nazov; }
	float get_cena() { return cena; }
	int get_kusy() { return pocet; }

};
class Lekaren : public QWidget
{
	Q_OBJECT
private:
	Ui::Lekaren ui;
	int index;
	float cenan = 0;
	int zlava;
	QString LoginName;


public:
	Lekaren(int uindex,QString LogName,QWidget *parent = Q_NULLPTR);
	~Lekaren();
	QVector<LIEK> produkty;	
	QVector<LIEK> pomocneProd; // na zmenu kusov po nakupe
	QVector<NAKUP> nakup;    // celkova objednavka
	QVector<NAKUP> actnakup; // aktualne spraveny nakup
	QVector<USER> zakaznik;
	QVector<STAFF> zamestnanec;
	QVector<ADMIN> admin;
	QVector<QString> objednavky; // mena objednavok
	int iCheck;
	void set_userIndex(int usIndex) { index = usIndex; }
	void set_userName(QString nam) { LoginName = nam; }
	void setZlava(int zla) { zlava = zla; }
	int getIndex() { return index; }
	QString getUsername() { return LoginName; }
	int getZlava() { return zlava; }
	void loadProdukty();
	void saveProdukty();
	void loadUsers();
	void saveUsers();
	void saveObjednavky();
	void loadObjednavky();
	void indexCheck();
	void deleteData();

private slots:
	void on_pushOdhlasenie_clicked();
	void on_pushObjednat_clicked();
	void on_pushKosik_clicked();
	void on_pushOdstranit_clicked();
	void on_pushZobrazit_clicked();
	void on_pushObjednavka_clicked();
	void on_pushPotvrdit_clicked();
	void on_pushZmenit_clicked();
	void on_pushZmena_clicked();
	void on_pushFaktura_clicked();
};
