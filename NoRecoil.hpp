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
	void DetectSpeedChange(double);
	
	void ChangeSpeed(double);
	

private:
    Ui::NoRecoilClass ui;

public slots:
	unsigned int ChangeSpeedButtonClicked();
};

void CheckerFunction(double);
void MoveMouse(double);

