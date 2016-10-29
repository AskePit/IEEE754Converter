#include "gui.h"
#include "ui_gui.h"
#include <math.h>

GUI::GUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GUI)
{
    ui->setupUi(this);
    value32.d = 0;
    value64.d = 0;
    clipboard = QApplication::clipboard();
}

GUI::~GUI()
{
    delete ui;
}

QString getHex4String(uint val)
{
    QString hex4String = QString::number(val, 16);
    if(hex4String.size() < 4) {
        QString zeros;
        for(int i = 0; i<4-hex4String.size(); ++i)
            zeros += "0";
        hex4String = zeros + hex4String;
    }

    return hex4String.toUpper();
}

QString getBin4String(uint val)
{
    QString bin4String = QString::number(val, 2);
    if(bin4String.size() < 4) {
        QString zeros;
        for(int i = 0; i<4-bin4String.size(); ++i)
            zeros += "0";
        bin4String = zeros + bin4String;
    }

    return bin4String;
}

float get23Mantissa(uint mantissa)
{
    if(mantissa == 0)
        return 0.0;

    int n = 1;
    if(mantissa & 0x800000)
        --n;

    while(!(mantissa&1)) {
        mantissa >>= 1;
        ++n;
    }

    return mantissa/(pow(2, 23-n));
}

void GUI::recalc32()
{
    uint32_t hex = value32.d;
    uint lowWord = hex & 0x0000FFFF;
    uint highWord = hex >> 16;

    ui->HexLine32_0->setText(getHex4String(highWord));
    ui->HexLine32_1->setText(getHex4String(lowWord));

    uint bin7 =  hex & 0x0000000F;
    uint bin6 = (hex & 0x000000F0) >> 4;
    uint bin5 = (hex & 0x00000F00) >> 8;
    uint bin4 = (hex & 0x0000F000) >> 12;
    uint bin3 = (hex & 0x000F0000) >> 16;
    uint bin2 = (hex & 0x00F00000) >> 20;
    uint bin1 = (hex & 0x0F000000) >> 24;
    uint bin0 = (hex & 0xF0000000) >> 28;

    ui->BinLine32_0->setText(getBin4String(bin0));
    ui->BinLine32_1->setText(getBin4String(bin1));
    ui->BinLine32_2->setText(getBin4String(bin2));
    ui->BinLine32_3->setText(getBin4String(bin3));
    ui->BinLine32_4->setText(getBin4String(bin4));
    ui->BinLine32_5->setText(getBin4String(bin5));
    ui->BinLine32_6->setText(getBin4String(bin6));
    ui->BinLine32_7->setText(getBin4String(bin7));

    uint32_t mask = 0x80000000;
    uint sign = hex & mask;
    int exponent = (hex >> 23) & 0x000000FF;
    uint mantissa = hex & 0x007FFFFF;
    if(exponent) {
        mantissa |= 0x800000;
        ui->hiddenMantissaBox32->setChecked(true);
        ui->normalizedLabel->setText("normalized");
    } else {
        ui->hiddenMantissaBox32->setChecked(false);
        ui->normalizedLabel->setText("denormalized");
    }

    ui->DecLine32->setText(QString::number(value32.e, 'g', 7));
    QString decText = ui->DecLine32->text();
    if(!decText.contains('.')) {
        if(decText != "inf" && decText != "-inf" && decText != "nan") {
            ui->DecLine32->setText(decText + ".0");
        }
    }

    if(ui->DecLine32->text() == "0.0" && sign) {
        ui->DecLine32->setText("-" + ui->DecLine32->text());
    }

    ui->signBox32->setChecked(sign); mask >>= 1;

    ui->exp7Box32->setChecked(hex & mask); mask >>= 1;
    ui->exp6Box32->setChecked(hex & mask); mask >>= 1;
    ui->exp5Box32->setChecked(hex & mask); mask >>= 1;
    ui->exp4Box32->setChecked(hex & mask); mask >>= 1;
    ui->exp3Box32->setChecked(hex & mask); mask >>= 1;
    ui->exp2Box32->setChecked(hex & mask); mask >>= 1;
    ui->exp1Box32->setChecked(hex & mask); mask >>= 1;
    ui->exp0Box32->setChecked(hex & mask); mask >>= 1;

    ui->mantissa22Box32->setChecked(hex & mask); mask >>= 1;
    ui->mantissa21Box32->setChecked(hex & mask); mask >>= 1;
    ui->mantissa20Box32->setChecked(hex & mask); mask >>= 1;
    ui->mantissa19Box32->setChecked(hex & mask); mask >>= 1;
    ui->mantissa18Box32->setChecked(hex & mask); mask >>= 1;
    ui->mantissa17Box32->setChecked(hex & mask); mask >>= 1;
    ui->mantissa16Box32->setChecked(hex & mask); mask >>= 1;
    ui->mantissa15Box32->setChecked(hex & mask); mask >>= 1;
    ui->mantissa14Box32->setChecked(hex & mask); mask >>= 1;
    ui->mantissa13Box32->setChecked(hex & mask); mask >>= 1;
    ui->mantissa12Box32->setChecked(hex & mask); mask >>= 1;
    ui->mantissa11Box32->setChecked(hex & mask); mask >>= 1;
    ui->mantissa10Box32->setChecked(hex & mask); mask >>= 1;
    ui->mantissa9Box32->setChecked (hex & mask); mask >>= 1;
    ui->mantissa8Box32->setChecked (hex & mask); mask >>= 1;
    ui->mantissa7Box32->setChecked (hex & mask); mask >>= 1;
    ui->mantissa6Box32->setChecked (hex & mask); mask >>= 1;
    ui->mantissa5Box32->setChecked (hex & mask); mask >>= 1;
    ui->mantissa4Box32->setChecked (hex & mask); mask >>= 1;
    ui->mantissa3Box32->setChecked (hex & mask); mask >>= 1;
    ui->mantissa2Box32->setChecked (hex & mask); mask >>= 1;
    ui->mantissa1Box32->setChecked (hex & mask); mask >>= 1;
    ui->mantissa0Box32->setChecked (hex & mask);

    if(sign)
        ui->signLabel32->setText("-");
    else
        ui->signLabel32->setText("+");

    ui->expPowLabel32->setText(QString::number(exponent-127));
    ui->mantissaLabel32->setText(QString::number(get23Mantissa(mantissa), 'g', 7));
}

void GUI::on_DecLine32_editingFinished()
{
    value32.e = ui->DecLine32->text().toFloat();
    recalc32();
}

void GUI::on_HexLine32_0_editingFinished()
{
    uint high = ui->HexLine32_0->text().toInt(NULL, 16);
    value32.d = (high<<16) + (value32.d & 0x0000FFFF);
    recalc32();
}

void GUI::on_HexLine32_1_editingFinished()
{
    uint low = ui->HexLine32_1->text().toInt(NULL, 16);
    value32.d = (value32.d & 0xFFFF0000) + low;
    recalc32();
}

void GUI::on_BinLine32_7_editingFinished()
{
    uint binPart = ui->BinLine32_7->text().toInt(NULL, 2);
    value32.d = (value32.d & 0xFFFFFFF0) + binPart;
    recalc32();
}

void GUI::on_BinLine32_6_editingFinished()
{
    uint binPart = ui->BinLine32_6->text().toInt(NULL, 2);
    value32.d = (value32.d & 0xFFFFFF0F) + (binPart<<4);
    recalc32();
}

void GUI::on_BinLine32_5_editingFinished()
{
    uint binPart = ui->BinLine32_5->text().toInt(NULL, 2);
    value32.d = (value32.d & 0xFFFFF0FF) + (binPart<<8);
    recalc32();
}

void GUI::on_BinLine32_4_editingFinished()
{
    uint binPart = ui->BinLine32_4->text().toInt(NULL, 2);
    value32.d = (value32.d & 0xFFFF0FFF) + (binPart<<12);
    recalc32();
}

void GUI::on_BinLine32_3_editingFinished()
{
    uint binPart = ui->BinLine32_3->text().toInt(NULL, 2);
    value32.d = (value32.d & 0xFFF0FFFF) + (binPart<<16);
    recalc32();
}

void GUI::on_BinLine32_2_editingFinished()
{
    uint binPart = ui->BinLine32_2->text().toInt(NULL, 2);
    value32.d = (value32.d & 0xFF0FFFFF) + (binPart<<20);
    recalc32();
}

void GUI::on_BinLine32_1_editingFinished()
{
    uint binPart = ui->BinLine32_1->text().toInt(NULL, 2);
    value32.d = (value32.d & 0xF0FFFFFF) + (binPart<<24);
    recalc32();
}

void GUI::on_BinLine32_0_editingFinished()
{
    uint binPart = ui->BinLine32_0->text().toInt(NULL, 2);
    value32.d = (value32.d & 0x0FFFFFFF) + (binPart<<28);
    recalc32();
}

void GUI::on_signBox32_clicked()
{
    value32.d ^= 1<<31;
    recalc32();
}

void GUI::on_exp7Box32_clicked()
{
    value32.d ^= 1<<30;
    recalc32();
}

void GUI::on_exp6Box32_clicked()
{
    value32.d ^= 1<<29;
    recalc32();
}

void GUI::on_exp5Box32_clicked()
{
    value32.d ^= 1<<28;
    recalc32();
}

void GUI::on_exp4Box32_clicked()
{
    value32.d ^= 1<<27;
    recalc32();
}

void GUI::on_exp3Box32_clicked()
{
    value32.d ^= 1<<26;
    recalc32();
}

void GUI::on_exp2Box32_clicked()
{
    value32.d ^= 1<<25;
    recalc32();
}

void GUI::on_exp1Box32_clicked()
{
    value32.d ^= 1<<24;
    recalc32();
}

void GUI::on_exp0Box32_clicked()
{
    value32.d ^= 1<<23;
    recalc32();
}

void GUI::on_mantissa22Box32_clicked()
{
    value32.d ^= 1<<22;
    recalc32();
}

void GUI::on_mantissa21Box32_clicked()
{
    value32.d ^= 1<<21;
    recalc32();
}

void GUI::on_mantissa20Box32_clicked()
{
    value32.d ^= 1<<20;
    recalc32();
}

void GUI::on_mantissa19Box32_clicked()
{
    value32.d ^= 1<<19;
    recalc32();
}

void GUI::on_mantissa18Box32_clicked()
{
    value32.d ^= 1<<18;
    recalc32();
}

void GUI::on_mantissa17Box32_clicked()
{
    value32.d ^= 1<<17;
    recalc32();
}

void GUI::on_mantissa16Box32_clicked()
{
    value32.d ^= 1<<16;
    recalc32();
}

void GUI::on_mantissa15Box32_clicked()
{
    value32.d ^= 1<<15;
    recalc32();
}

void GUI::on_mantissa14Box32_clicked()
{
    value32.d ^= 1<<14;
    recalc32();
}

void GUI::on_mantissa13Box32_clicked()
{
    value32.d ^= 1<<13;
    recalc32();
}

void GUI::on_mantissa12Box32_clicked()
{
    value32.d ^= 1<<12;
    recalc32();
}

void GUI::on_mantissa11Box32_clicked()
{
    value32.d ^= 1<<11;
    recalc32();
}

void GUI::on_mantissa10Box32_clicked()
{
    value32.d ^= 1<<10;
    recalc32();
}

void GUI::on_mantissa9Box32_clicked()
{
    value32.d ^= 1<<9;
    recalc32();
}

void GUI::on_mantissa8Box32_clicked()
{
    value32.d ^= 1<<8;
    recalc32();
}

void GUI::on_mantissa7Box32_clicked()
{
    value32.d ^= 1<<7;
    recalc32();
}

void GUI::on_mantissa6Box32_clicked()
{
    value32.d ^= 1<<6;
    recalc32();
}

void GUI::on_mantissa5Box32_clicked()
{
    value32.d ^= 1<<5;
    recalc32();
}

void GUI::on_mantissa4Box32_clicked()
{
    value32.d ^= 1<<4;
    recalc32();
}

void GUI::on_mantissa3Box32_clicked()
{
    value32.d ^= 1<<3;
    recalc32();
}

void GUI::on_mantissa2Box32_clicked()
{
    value32.d ^= 1<<2;
    recalc32();
}

void GUI::on_mantissa1Box32_clicked()
{
    value32.d ^= 1<<1;
    recalc32();
}

void GUI::on_mantissa0Box32_clicked()
{
    value32.d ^= 1<<0;
    recalc32();
}

void GUI::on_copyDecButton32_clicked()
{
    clipboard->setText(ui->DecLine32->text());
}

void GUI::on_copyBinButton32_clicked()
{
#define QUAD(x) ui->BinLine32_##x->text()
    QString binary = QUAD(0) + QUAD(1) + QUAD(2) + QUAD(3) + QUAD(4) + QUAD(5) + QUAD(6) + QUAD(7);
#undef QUAD
    clipboard->setText(binary);
}

void GUI::on_copyHexButton32_clicked()
{
    clipboard->setText(ui->HexLine32_0->text() + ui->HexLine32_1->text());
}

void GUI::on_clearButton32_clicked()
{
    value32.e = 0.0f;
    recalc32();
}

void GUI::on_infButton32_clicked()
{
    value32.d = 0x7F800000;
    recalc32();
}

void GUI::on_nanButton32_clicked()
{
    value32.d = 0x7FC00000;
    recalc32();
}
