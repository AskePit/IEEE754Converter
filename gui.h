#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QClipboard>

namespace Ui {
    class GUI;
}

class GUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit GUI(QWidget *parent = 0);
    ~GUI();

private slots:
    void on_DecLine32_editingFinished();
    void on_HexLine32_0_editingFinished();
    void on_HexLine32_1_editingFinished();
    void on_BinLine32_7_editingFinished();
    void on_BinLine32_6_editingFinished();
    void on_BinLine32_5_editingFinished();
    void on_BinLine32_4_editingFinished();
    void on_BinLine32_3_editingFinished();
    void on_BinLine32_2_editingFinished();
    void on_BinLine32_1_editingFinished();
    void on_BinLine32_0_editingFinished();
    void on_signBox32_clicked();
    void on_exp7Box32_clicked();
    void on_exp6Box32_clicked();
    void on_exp5Box32_clicked();
    void on_exp4Box32_clicked();
    void on_exp3Box32_clicked();
    void on_exp2Box32_clicked();
    void on_exp1Box32_clicked();
    void on_exp0Box32_clicked();
    void on_mantissa22Box32_clicked();
    void on_mantissa21Box32_clicked();
    void on_mantissa20Box32_clicked();
    void on_mantissa19Box32_clicked();
    void on_mantissa18Box32_clicked();
    void on_mantissa17Box32_clicked();
    void on_mantissa16Box32_clicked();
    void on_mantissa15Box32_clicked();
    void on_mantissa14Box32_clicked();
    void on_mantissa13Box32_clicked();
    void on_mantissa12Box32_clicked();
    void on_mantissa11Box32_clicked();
    void on_mantissa10Box32_clicked();
    void on_mantissa9Box32_clicked();
    void on_mantissa8Box32_clicked();
    void on_mantissa7Box32_clicked();
    void on_mantissa6Box32_clicked();
    void on_mantissa5Box32_clicked();
    void on_mantissa4Box32_clicked();
    void on_mantissa3Box32_clicked();
    void on_mantissa2Box32_clicked();
    void on_mantissa1Box32_clicked();
    void on_mantissa0Box32_clicked();
    void on_copyDecButton32_clicked();
    void on_copyBinButton32_clicked();
    void on_copyHexButton32_clicked();
    void on_clearButton32_clicked();
    void on_infButton32_clicked();
    void on_nanButton32_clicked();

private:
    Ui::GUI *ui;

    union {
        float e;
        uint32_t d;
    } value32;

    union {
        double e;
        uint64_t d;
    } value64;

    QClipboard* clipboard;

    void recalc32();
    //void recalc64();
};

#endif // GUI_H
