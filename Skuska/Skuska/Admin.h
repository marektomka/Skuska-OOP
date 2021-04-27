#pragma once

#include <QWidget>
#include "ui_Admin.h"
#include "Login.h"

class LIEK {
protected:
	QString liek;
	float cena;
	int kusy;
public:
	LIEK() { liek = "", cena = 0, kusy = 0; }
	LIEK(QString liek1, float cena1, int kusy1) { liek = liek1, cena = cena1, kusy = kusy1; }
	void set_liek(QString liek1) { liek = liek1; }
	void set_cena(float cena1) { cena = cena1;  }
	void set_kusy(int kusy1) { kusy = kusy1; }
	QString get_liek() {  return liek; }
	float get_cena() {  return cena; }
	int get_kusy() { return kusy; }


};


class Admin : public QWidget
{
	Q_OBJECT

public:
	Admin(QWidget *parent = Q_NULLPTR);
	~Admin();
	int getZlava() { return zlava; }
	void setZlava(int zlav) { zlava = zlav; }
private:
	Ui::Admin ui;
	int zlava;
	void closeEvent(QCloseEvent* event);
	QVector<ADMIN> admin2;
	QVector<STAFF> staff2;
	QVector<USER> user2;
	QVector<LIEK> produkty;
	QVector<QString> objednavky;
	void loadUsers();
	void loadProdukty();
	void saveProdukty();
	void loadObjednavky();        // iba kvoli zmene username
	void saveObjednavky(QString oldUs, QString newUs);
	int typ=5;
	
	

private slots:
	void on_pushPotvrdit_clicked();
	void on_comboTyp_currentTextChanged();
	void on_checkAdd_clicked();
	void on_checkEdit_clicked();
	void on_comboName_currentTextChanged();
	void on_pushOdhlasenie_clicked();
	void on_checkEdit_2_clicked();
	void on_pushPotvrdit_2_clicked();
};
