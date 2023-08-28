#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_NoRecoil.h"

class NoRecoil : public QMainWindow
{
    Q_OBJECT

public:
    NoRecoil(QWidget *parent = nullptr);
    ~NoRecoil();

private:
    Ui::NoRecoilClass ui;
};
