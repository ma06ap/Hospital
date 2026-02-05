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

class DoctorWindow : public QDialog {
    Q_OBJECT

public:
    DoctorWindow(DoctorRoom* dRoom, Pharmacy* pharm, QWidget* parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onPatientClicked(int row, int col);
    void onAddMedicine();
    void onFinalizePrescription();
    void onUpdateQueue();

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


class PharmacyWindow : public QDialog {
    Q_OBJECT

public:

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
