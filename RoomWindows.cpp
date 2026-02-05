#include "RoomWindows.h"
#include <QGroupBox>
#include <QLineEdit>
#include <QListWidget>

DoctorWindow::DoctorWindow(DoctorRoom* dRoom, Pharmacy* pharm, QWidget* parent)
    : QDialog(parent), room(dRoom), pharmacyRef(pharm), currentSelectedPatient(nullptr) {
    setupUi();
    refreshPatientTable();
    setWindowTitle("Doctor's Office");
    resize(900, 600);
}

void DoctorWindow::closeEvent(QCloseEvent *event) {
    if (parentWidget()) {
        parentWidget()->show();
    }
    event->accept();
}

void DoctorWindow::setupUi() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);


    mainLayout->addWidget(new QLabel("Active Patients (In Room):"));
    patientTable = new QTableWidget();
    patientTable->setColumnCount(3);
    patientTable->setHorizontalHeaderLabels({"ID", "Name", "Symptoms"});
    patientTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    patientTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(patientTable, &QTableWidget::cellClicked, this, &DoctorWindow::onPatientClicked);
    mainLayout->addWidget(patientTable);


    mainLayout->addWidget(new QLabel("Prescribed Medicines:"));
    medicineTable = new QTableWidget();
    medicineTable->setColumnCount(3);
    medicineTable->setHorizontalHeaderLabels({"Medicine Name", "Type", "Qty"});
    medicineTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mainLayout->addWidget(medicineTable);


    QGroupBox* formGroup = new QGroupBox("Add Prescription");
    QHBoxLayout* formLayout = new QHBoxLayout(formGroup);

    medNameInput = new QLineEdit();
    medNameInput->setPlaceholderText("Medicine Name");

    medTypeCombo = new QComboBox();
    medTypeCombo->addItem("Tablet", (int)MedicineUnit::Tablet);
    medTypeCombo->addItem("Syrup", (int)MedicineUnit::Syrup);
    medTypeCombo->addItem("Ampoule (Injection)", (int)MedicineUnit::Ampoule);
    medTypeCombo->addItem("Serum (Injection)", (int)MedicineUnit::Serum);

    medQtyInput = new QSpinBox();
    medQtyInput->setRange(1, 100);

    QPushButton* btnAdd = new QPushButton("Add Medicine");
    connect(btnAdd, &QPushButton::clicked, this, &DoctorWindow::onAddMedicine);

    formLayout->addWidget(new QLabel("Name:"));
    formLayout->addWidget(medNameInput);
    formLayout->addWidget(new QLabel("Type:"));
    formLayout->addWidget(medTypeCombo);
    formLayout->addWidget(new QLabel("Qty:"));
    formLayout->addWidget(medQtyInput);
    formLayout->addWidget(btnAdd);

    mainLayout->addWidget(formGroup);

    // --- Buttons ---
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* btnFinalize = new QPushButton("Finalize & Send to Pharmacy");
    connect(btnFinalize, &QPushButton::clicked, this, &DoctorWindow::onFinalizePrescription);

    QPushButton* btnUpdate = new QPushButton("Call Next Patient (Update Queue)");
    connect(btnUpdate, &QPushButton::clicked, this, &DoctorWindow::onUpdateQueue);

    QPushButton* btnClose = new QPushButton("Close Room");
    connect(btnClose, &QPushButton::clicked, this, &QDialog::close);

    btnLayout->addWidget(btnUpdate);
    btnLayout->addWidget(btnFinalize);
    btnLayout->addWidget(btnClose);
    mainLayout->addLayout(btnLayout);
}

void DoctorWindow::refreshPatientTable() {
    patientTable->setRowCount(0);

    auto patients = room->getActivePatients();

    for (auto p : patients) {
        int row = patientTable->rowCount();
        patientTable->insertRow(row);

        QTableWidgetItem* item = new QTableWidgetItem(QString::number(p->getID()));
        item->setData(Qt::UserRole, QVariant::fromValue((void*)p));

        patientTable->setItem(row, 0, item);
        patientTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(p->getName())));

        QString sym = (p->disease) ? QString::fromStdString(p->disease->symptoms) : "No Info";
        patientTable->setItem(row, 2, new QTableWidgetItem(sym));
    }
}

void DoctorWindow::onPatientClicked(int row, int col) {
    currentSelectedPatient = (Patient*)patientTable->item(row, 0)->data(Qt::UserRole).value<void*>();
    refreshMedicineTable();
}

void DoctorWindow::onAddMedicine() {
    if (!currentSelectedPatient) {
        QMessageBox::warning(this, "Warning", "Please select a patient first.");
        return;
    }

    QString name = medNameInput->text();
    if (name.isEmpty()) return;

    MedicineUnit unit = (MedicineUnit)medTypeCombo->currentData().toInt();
    int qty = medQtyInput->value();

    Medicine med(name.toStdString(), unit, qty);
    currentSelectedPatient->treatment->addMedicine(med);

    refreshMedicineTable();
    medNameInput->clear();
}

void DoctorWindow::refreshMedicineTable() {
    medicineTable->setRowCount(0);
    if (!currentSelectedPatient || !currentSelectedPatient->treatment) return;

    for (const auto& med : currentSelectedPatient->treatment->medicines) {
        int row = medicineTable->rowCount();
        medicineTable->insertRow(row);

        medicineTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(med.name)));

        QString typeStr;
        switch(med.unit) {
        case MedicineUnit::Tablet: typeStr = "Tablet"; break;
        case MedicineUnit::Syrup: typeStr = "Syrup"; break;
        case MedicineUnit::Ampoule: typeStr = "Ampoule"; break;
        case MedicineUnit::Serum: typeStr = "Serum"; break;
        }
        medicineTable->setItem(row, 1, new QTableWidgetItem(typeStr));
        medicineTable->setItem(row, 2, new QTableWidgetItem(QString::number(med.quantity)));
    }
}

void DoctorWindow::onFinalizePrescription() {
    if (!currentSelectedPatient) return;


    room->removePatient(currentSelectedPatient);


    pharmacyRef->addPatient(currentSelectedPatient);

    QMessageBox::information(this, "Success", "Patient sent to Pharmacy.");

    currentSelectedPatient = nullptr;
    medicineTable->setRowCount(0);
    refreshPatientTable();
}

void DoctorWindow::onUpdateQueue() {
    refreshPatientTable();
}




PharmacyWindow::PharmacyWindow(Pharmacy* pRoom, InjectionRoom* injRoom, QWidget* parent)
    : QDialog(parent), room(pRoom), injectionRef(injRoom) {
    setupUi();
    refreshTable();
    setWindowTitle("Pharmacy Queue");
    resize(800, 500);
}

void PharmacyWindow::closeEvent(QCloseEvent *event) {
    if (parentWidget()) parentWidget()->show();
    event->accept();
}

void PharmacyWindow::setupUi() {
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Patients Waiting for Medicine (Double Click to Serve):"));

    queueTable = new QTableWidget();
    queueTable->setColumnCount(2);
    queueTable->setHorizontalHeaderLabels({"ID", "Patient Name"});
    queueTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    queueTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(queueTable, &QTableWidget::cellDoubleClicked, this, &PharmacyWindow::onPatientDoubleClicked);

    layout->addWidget(queueTable);

    QPushButton* closeBtn = new QPushButton("Close Pharmacy");
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::close);
    layout->addWidget(closeBtn);
}

void PharmacyWindow::refreshTable() {
    queueTable->setRowCount(0);

    auto patients = room->getActivePatients();

    for (auto p : patients) {
        int row = queueTable->rowCount();
        queueTable->insertRow(row);

        QTableWidgetItem* item = new QTableWidgetItem(QString::number(p->getID()));
        item->setData(Qt::UserRole, QVariant::fromValue((void*)p));

        queueTable->setItem(row, 0, item);
        queueTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(p->getName())));
    }
}

void PharmacyWindow::onPatientDoubleClicked(int row, int col) {
    Patient* p = (Patient*)queueTable->item(row, 0)->data(Qt::UserRole).value<void*>();
    if (p) showMedicineDialog(p);
}

void PharmacyWindow::showMedicineDialog(Patient* p) {
    QDialog dlg(this);
    dlg.setWindowTitle("Deliver Medicines");
    QVBoxLayout* lay = new QVBoxLayout(&dlg);

    QTableWidget* medTable = new QTableWidget();
    medTable->setColumnCount(2);
    medTable->setHorizontalHeaderLabels({"Medicine", "Unit"});

    for (const auto& m : p->treatment->medicines) {
        int r = medTable->rowCount();
        medTable->insertRow(r);
        medTable->setItem(r, 0, new QTableWidgetItem(QString::fromStdString(m.name)));

        QString uStr = (m.unit == MedicineUnit::Ampoule || m.unit == MedicineUnit::Serum) ? "Injection" : "Oral";
        medTable->setItem(r, 1, new QTableWidgetItem(uStr));
    }
    lay->addWidget(medTable);

    QPushButton* btnConfirm = new QPushButton("Confirm Delivery");
    connect(btnConfirm, &QPushButton::clicked, &dlg, &QDialog::accept);
    lay->addWidget(btnConfirm);

    if (dlg.exec() == QDialog::Accepted) {

        p->treatment->gotMedicine = true;
        room->removePatient(p);

        if (p->treatment->needsInjection) {
            injectionRef->addPatient(p);
            QMessageBox::information(this, "Info", "Patient needs injection. Sent to Injection Room.");
        } else {
            p->cure();
            QMessageBox::information(this, "Info", "Treatment Completed. Patient Cured.");
        }
        refreshTable();
    }
}


InjectionWindow::InjectionWindow(InjectionRoom* iRoom, QWidget* parent)
    : QDialog(parent), room(iRoom) {
    setupUi();
    refreshTable();
    setWindowTitle("Injection Room");
    resize(600, 400);
}

void InjectionWindow::closeEvent(QCloseEvent *event) {
    if (parentWidget()) parentWidget()->show();
    event->accept();
}

void InjectionWindow::setupUi() {
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Patients Waiting for Injection:"));

    queueTable = new QTableWidget();
    queueTable->setColumnCount(2);
    queueTable->setHorizontalHeaderLabels({"ID", "Name"});
    queueTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(queueTable, &QTableWidget::cellDoubleClicked, this, &InjectionWindow::onPatientDoubleClicked);

    layout->addWidget(queueTable);

    QHBoxLayout* btnLay = new QHBoxLayout();
    QPushButton* btnUpdate = new QPushButton("Next Patient (Refresh)");
    connect(btnUpdate, &QPushButton::clicked, this, &InjectionWindow::onUpdateQueue);

    QPushButton* btnClose = new QPushButton("Close Room");
    connect(btnClose, &QPushButton::clicked, this, &QDialog::close);

    btnLay->addWidget(btnUpdate);
    btnLay->addWidget(btnClose);
    layout->addLayout(btnLay);
}

void InjectionWindow::refreshTable() {
    queueTable->setRowCount(0);
    auto patients = room->getActivePatients();

    for (auto p : patients) {
        int row = queueTable->rowCount();
        queueTable->insertRow(row);

        QTableWidgetItem* item = new QTableWidgetItem(QString::number(p->getID()));
        item->setData(Qt::UserRole, QVariant::fromValue((void*)p));

        queueTable->setItem(row, 0, item);
        queueTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(p->getName())));
    }
}

void InjectionWindow::onUpdateQueue() {
    refreshTable();
}

void InjectionWindow::onPatientDoubleClicked(int row, int col) {
    Patient* p = (Patient*)queueTable->item(row, 0)->data(Qt::UserRole).value<void*>();
    if (p) showInjectionDialog(p);
}

void InjectionWindow::showInjectionDialog(Patient* p) {
    QDialog dlg(this);
    dlg.setWindowTitle("Injection Details");
    QVBoxLayout* lay = new QVBoxLayout(&dlg);

    lay->addWidget(new QLabel("Injectables for " + QString::fromStdString(p->getName()) + ":"));
    QListWidget* list = new QListWidget();


    for (const auto& m : p->treatment->medicines) {
        if (m.unit == MedicineUnit::Ampoule || m.unit == MedicineUnit::Serum) {
            list->addItem(QString::fromStdString(m.name) + " (Qty: " + QString::number(m.quantity) + ")");
        }
    }
    lay->addWidget(list);

    QPushButton* btnDone = new QPushButton("Confirm Injection & Finish");
    connect(btnDone, &QPushButton::clicked, &dlg, &QDialog::accept);
    lay->addWidget(btnDone);

    if (dlg.exec() == QDialog::Accepted) {
        p->treatment->needsInjection = false;
        room->removePatient(p);

        p->cure();
        QMessageBox::information(this, "Done", "Injection done. Patient Cured and Discharged.");

        refreshTable();
    }
}
