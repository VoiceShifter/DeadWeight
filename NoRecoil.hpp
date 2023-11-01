#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_NoRecoil.h"
# include <iostream>
# include <Windows.h>
# include <thread>
# include <chrono>

namespace MyKeys 
{
      enum Keys
      {
            LMBUTTON = 1,
            RMBUTTON,           
            MBUTTON = 4,
            MXBUTTON1,
            MXBUTTON2,
            CAPS = 18
      };
}




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
      //signed int SwithForKey();
};

void CheckerFunction(unsigned int, signed int);


