#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QRadioButton>
#include <QComboBox>
#include <QTextEdit>
#include <QGroupBox>
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>

// اضافه کردن کلاس‌های لاجیک پروژه
#include "DataManagement.h"
#include "DoctorRoom.h"
#include "Pharmacy.h"
#include "InjectionRoom.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *event) override;

private slots:
    // اسلات‌ها (توابع واکنش به کلیک)
    void onAddClicked();            // دکمه ثبت
    void onUserTypeChanged();       // تغییر رادیو باتن (بیمار/پرسنل)
    void refreshTables();           // بروزرسانی جدول‌ها
    void onPatientDoubleClicked(int row, int col);   // کلیک روی بیمار
    void onPersonnelDoubleClicked(int row, int col); // کلیک روی پرسنل

private:
    // --- عناصر UI ---
    QTableWidget *patientTable;
    QTableWidget *personnelTable;

    // فرم‌های ورودی
    QLineEdit *nameInput;
    QLineEdit *nationalCodeInput;

    // انتخاب نوع کاربر
    QRadioButton *radioPatient;
    QRadioButton *radioPersonnel;

    // پنل‌های مخصوص (که مخفی/ظاهر می‌شوند)
    QGroupBox *patientFormGroup;
    QGroupBox *personnelFormGroup;

    // ورودی‌های بیمار
    QComboBox *diseaseTypeCombo;
    QLineEdit *symptomsInput;
    QTextEdit *descriptionInput;

    // ورودی‌های پرسنل
    QLineEdit *personnelCodeInput;
    QComboBox *roomSelectCombo;

    // --- مدیریت اتاق‌ها (State) ---
    // این‌ها باید در طول اجرای برنامه زنده باشند
    DoctorRoom *docRoom;
    Pharmacy *pharmacy;
    InjectionRoom *injectionRoom;

    void setupUi(); // تابع کمکی برای چیدمان
};

#endif // MAINWINDOW_H
