#include "bytecolorswidget.h"
#include "ui_bytecolorswidget.h"

ByteColorsWidget::ByteColorsWidget(QWidget *parent): QWidget(parent), ui(new Ui::ByteColorsWidget)
{
    ui->setupUi(this);

    QPalette p;

    p = ui->lbl00->palette();
    p.setColor(ui->lbl00->foregroundRole(), QColor(ByteColors::ZeroColor));
    ui->lbl00->setPalette(p);

    p = ui->lblFF->palette();
    p.setColor(ui->lblFF->foregroundRole(), QColor(ByteColors::FFColor));
    ui->lblFF->setPalette(p);

    p = ui->lblPunct->palette();
    p.setColor(ui->lblPunct->foregroundRole(), QColor(ByteColors::PunctColor));
    ui->lblPunct->setPalette(p);

    p = ui->lblDigits->palette();
    p.setColor(ui->lblDigits->foregroundRole(), QColor(ByteColors::DigitsColor));
    ui->lblDigits->setPalette(p);

    p = ui->lblAlpha->palette();
    p.setColor(ui->lblAlpha->foregroundRole(), QColor(ByteColors::AlphaColor));
    ui->lblAlpha->setPalette(p);

    p = ui->lblUncategorized->palette();
    p.setColor(ui->lblUncategorized->foregroundRole(), QColor(ByteColors::UncategorizedColor));
    ui->lblUncategorized->setPalette(p);
}

ByteColorsWidget::~ByteColorsWidget()
{
    delete ui;
}
