#ifndef MAINLAYOUT_H
#define MAINLAYOUT_H
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include <QGridLayout>

class  MainLayout :public QVBoxLayout
{

public:
  explicit  MainLayout(QWidget *parent = nullptr);
    ~MainLayout();

};

#endif // MAINLAYOUT_H
