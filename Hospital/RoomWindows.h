#ifndef ROOMWINDOWS_H
#define ROOMWINDOWS_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QCloseEvent>

#include "DoctorRoom.h"
#include "Pharmacy.h"
#include "InjectionRoom.h"
#include "Medicine.h"

// ==========================================
// 1. Doctor Window
// ==========================================
class DoctorWindow : public QDialog {
    Q_OBJECT

public:
    // نیاز به دسترسی به داروخانه داریم تا بیمار را بفرستیم آنجا
    DoctorWindow(DoctorRoom* dRoom, Pharmacy* pharm, QWidget* parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override; // برای باز کردن مجدد صفحه اصلی

private slots:
    void onPatientClicked(int row, int col);
    void onAddMedicine();
    void onFinalizePrescription();
    void onUpdateQueue(); // دکمه آوردن بیمار جدید

private:
    DoctorRoom* room;
    Pharmacy* pharmacyRef;
    Patient* currentSelectedPatient;

    QTableWidget* patientTable;
    QTableWidget* medicineTable;

    QComboBox* medTypeCombo;
    QLineEdit* medNameInput;
    QSpinBox* medQtyInput;

    void setupUi();
    void refreshPatientTable();
    void refreshMedicineTable();
};

// ==========================================
// 2. Pharmacy Window
// ==========================================
class PharmacyWindow : public QDialog {
    Q_OBJECT

public:
    // نیاز به دسترسی به تزریقات داریم
    PharmacyWindow(Pharmacy* pRoom, InjectionRoom* injRoom, QWidget* parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onPatientDoubleClicked(int row, int col);

private:
    Pharmacy* room;
    InjectionRoom* injectionRef;
    QTableWidget* queueTable;

    void setupUi();
    void refreshTable();
    void showMedicineDialog(Patient* p);
};

// ==========================================
// 3. Injection Window
// ==========================================
class InjectionWindow : public QDialog {
    Q_OBJECT

public:
    InjectionWindow(InjectionRoom* iRoom, QWidget* parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onPatientDoubleClicked(int row, int col);
    void onUpdateQueue();

private:
    InjectionRoom* room;
    QTableWidget* queueTable;

    void setupUi();
    void refreshTable();
    void showInjectionDialog(Patient* p);
};

#endif // ROOMWINDOWS_H
