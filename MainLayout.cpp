#include "MainLayout.h"





MainLayout::MainLayout(QWidget *parent):QVBoxLayout(parent)
{
    setMargin(0); // 控件与窗体的边距
     setSpacing(0); // 控件之间的间距
//    pHeaderLayout= new HeaderLayout(parent);
//  // pBodyLayout= new BodyLayout(parent);
//    pFooterLayout= new FooterLayout(parent);
//  addLayout(pHeaderLayout);
// // addLayout(pBodyLayout);
//   addLayout(pFooterLayout);
//  // 设置 最优化显示，使用户无法改变对话框大小，最优化就是控件按其 sizeHint 的大小显示
//      setSizeConstraint(QLayout::SetFixedSize);
}

MainLayout::~MainLayout()
{



}
