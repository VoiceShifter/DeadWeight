#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_NoRecoil.h"
# include <iostream>
# include <Windows.h>
# include <thread>
# include <chrono>


class NoRecoil : public QMainWindow
{
    Q_OBJECT

public:
    NoRecoil(QWidget *parent = nullptr);
    ~NoRecoil();
	
	
	
	

private:
    Ui::NoRecoilClass ui;

public slots:
	unsigned int ChangeSpeedButtonClicked();
};

void CheckerFunction(unsigned int, signed int);


