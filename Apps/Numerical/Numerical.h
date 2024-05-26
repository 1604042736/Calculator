#pragma once

#include <QWidget>

#include "ui_Numerical.h"

class Numerical : public QWidget
{
public:
    Numerical(QWidget *parent = nullptr);

    Ui_Numerical *ui;

public slots:
    void calculate();
};