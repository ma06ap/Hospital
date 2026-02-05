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

    void onAddClicked();
    void onUserTypeChanged();
    void refreshTables();
    void onPatientDoubleClicked(int row, int col);
    void onPersonnelDoubleClicked(int row, int col);

private:

    QTableWidget *patientTable;
    QTableWidget *personnelTable;


    QLineEdit *nameInput;
    QLineEdit *nationalCodeInput;


    QRadioButton *radioPatient;
    QRadioButton *radioPersonnel;


    QGroupBox *patientFormGroup;
    QGroupBox *personnelFormGroup;


    QComboBox *diseaseTypeCombo;
    QLineEdit *symptomsInput;
    QTextEdit *descriptionInput;


    QLineEdit *personnelCodeInput;
    QComboBox *roomSelectCombo;


    DoctorRoom *docRoom;
    Pharmacy *pharmacy;
    InjectionRoom *injectionRoom;

    void setupUi();
};

#endif // MAINWINDOW_H
