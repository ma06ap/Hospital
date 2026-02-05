#include "MainWindow.h"
#include "RoomWindows.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFormLayout>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    docRoom = new DoctorRoom();
    pharmacy = new Pharmacy();
    injectionRoom = new InjectionRoom();

    setupUi();
    refreshTables();
}

MainWindow::~MainWindow() {

    delete docRoom;
    delete pharmacy;
    delete injectionRoom;
}

void MainWindow::setupUi() {
    QWidget *centralWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);


    QHBoxLayout *tablesLayout = new QHBoxLayout();


    patientTable = new QTableWidget();
    patientTable->setColumnCount(3);
    patientTable->setHorizontalHeaderLabels({"ID", "Patient Name", "Status"});
    patientTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(patientTable, &QTableWidget::cellDoubleClicked, this, &MainWindow::onPatientDoubleClicked);


    personnelTable = new QTableWidget();
    personnelTable->setColumnCount(3);
    personnelTable->setHorizontalHeaderLabels({"ID", "Personell Name", "Role"});
    personnelTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(personnelTable, &QTableWidget::cellDoubleClicked, this, &MainWindow::onPersonnelDoubleClicked);

    tablesLayout->addWidget(new QLabel("Patient list:"));
    tablesLayout->addWidget(patientTable);
    tablesLayout->addWidget(new QLabel("Personnel list:"));
    tablesLayout->addWidget(personnelTable);

    mainLayout->addLayout(tablesLayout, 2);


    QGroupBox *registerBox = new QGroupBox("ADD");
    QVBoxLayout *formLayout = new QVBoxLayout(registerBox);


    QHBoxLayout *typeLayout = new QHBoxLayout();
    radioPatient = new QRadioButton("Patient");
    radioPersonnel = new QRadioButton("Personnel");
    radioPatient->setChecked(true);
    typeLayout->addWidget(radioPatient);
    typeLayout->addWidget(radioPersonnel);

    connect(radioPatient, &QRadioButton::toggled, this, &MainWindow::onUserTypeChanged);


    QLineEdit *nameInput = new QLineEdit();
    nameInput->setPlaceholderText("Full Name");
    this->nameInput = nameInput;

    QLineEdit *nationalCodeInput = new QLineEdit();
    nationalCodeInput->setPlaceholderText("National Code (10 digits)");
    this->nationalCodeInput = nationalCodeInput;

    formLayout->addLayout(typeLayout);
    formLayout->addWidget(nameInput);
    formLayout->addWidget(nationalCodeInput);


    patientFormGroup = new QGroupBox("Medical Info");
    QFormLayout *pLayout = new QFormLayout(patientFormGroup);

    diseaseTypeCombo = new QComboBox();
    diseaseTypeCombo->addItem("Cold", (int)DiseaseType::Cold);
    diseaseTypeCombo->addItem("Cancer", (int)DiseaseType::Cancer);
    diseaseTypeCombo->addItem("HeartIssue", (int)DiseaseType::HeartIssue);
    diseaseTypeCombo->addItem("Fracture", (int)DiseaseType::Fracture);
    diseaseTypeCombo->addItem("Infection", (int)DiseaseType::Infection);
    diseaseTypeCombo->addItem("Other...", (int)DiseaseType::Other);


    symptomsInput = new QLineEdit();
    descriptionInput = new QTextEdit();
    descriptionInput->setMaximumHeight(50);

    pLayout->addRow("Disease Type:", diseaseTypeCombo);
    pLayout->addRow("symptoms:", symptomsInput);
    pLayout->addRow("Description:", descriptionInput);

    formLayout->addWidget(patientFormGroup);


    personnelFormGroup = new QGroupBox("Job Info");
    QFormLayout *staffLayout = new QFormLayout(personnelFormGroup);

    personnelCodeInput = new QLineEdit();
    personnelCodeInput->setPlaceholderText("Personnel Code (7-digits)");

    roomSelectCombo = new QComboBox();
    roomSelectCombo->addItem("Doctor", 1);
    roomSelectCombo->addItem("Pharmecy", 2);
    roomSelectCombo->addItem("Injection", 3);

    staffLayout->addRow("Personnel Code:", personnelCodeInput);
    staffLayout->addRow("Room:", roomSelectCombo);

    formLayout->addWidget(personnelFormGroup);
    personnelFormGroup->hide();


    QPushButton *btnAdd = new QPushButton("ADD");
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    formLayout->addWidget(btnAdd);

    mainLayout->addWidget(registerBox, 1);
    setCentralWidget(centralWidget);
    setWindowTitle("Hospital");
    resize(800, 600);
}

void MainWindow::onUserTypeChanged() {
    if (radioPatient->isChecked()) {
        patientFormGroup->show();
        personnelFormGroup->hide();
    } else {
        patientFormGroup->hide();
        personnelFormGroup->show();
    }
}


void MainWindow::onAddClicked() {
    QString name = nameInput->text();
    QString natCode = nationalCodeInput->text();


    if (!Person::checkName(name.toStdString()) || !Person::checkNationalCode(natCode.toStdString())) {
        QMessageBox::warning(this, "Error" , "Name is empty or Nationality code is wrong");
        return;
    }

    if (radioPatient->isChecked()) {

        Patient *p = new Patient(name.toStdString(), natCode.toStdString());


        DiseaseType type = (DiseaseType)diseaseTypeCombo->currentData().toInt();
        Disease *d = new Disease(type, symptomsInput->text().toStdString(), descriptionInput->toPlainText().toStdString());
        p->setDisease(d);


        DataManagement<Patient>::create(p);

        docRoom->addPatient(p);

        QMessageBox::information(this,"Successful" , "Patient added");

    } else {

        QString pCode = personnelCodeInput->text();
        auto allStaff = DataManagement<Personnel>::getAll();
        bool exists = false;
        for (auto staff : allStaff) {
            if (staff->personnelCode == pCode.toStdString()) {
                exists = true;
                break;
            }
        }
        if (exists) {
            QMessageBox::critical(this, "Error" , "This number was before" );
            return;
        }

        if (!Personnel::checkPersonnelCode(pCode.toStdString())) {
            QMessageBox::warning(this, "Error" , "Must be 7 digits");
            return;
        }

        int roomType = roomSelectCombo->currentData().toInt();
        Role role = (roomType == 1) ? Role::Doctor : Role::Nurse;

        Personnel *staff = new Personnel(name.toStdString(), natCode.toStdString(), pCode.toStdString(), role);

        bool entered = false;
        if (roomType == 1) entered = docRoom->enterStaff(staff);
        else if (roomType == 2) entered = pharmacy->enterStaff(staff);
        else if (roomType == 3) entered = injectionRoom->enterStaff(staff);

        if (entered) {
            DataManagement<Personnel>::create(staff);
            QMessageBox::information(this, "Successful"  , "Personnel Entered");
        } else {
            delete staff;
            QMessageBox::critical(this, "Error", "Room is full or the wrong role");
            return;
        }
    }

    refreshTables();

    nameInput->clear();
    nationalCodeInput->clear();
    symptomsInput->clear();
    descriptionInput->clear();
}

void MainWindow::refreshTables() {

    auto patients = DataManagement<Patient>::getAll();
    patientTable->setRowCount(0);
    for (auto p : patients) {
        int row = patientTable->rowCount();
        patientTable->insertRow(row);


        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(p->getID()));
        idItem->setData(Qt::UserRole, QVariant::fromValue((void*)p));

        patientTable->setItem(row, 0, idItem);
        patientTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(p->getName())));
        patientTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(p->getStatus())));
    }


    auto staffs = DataManagement<Personnel>::getAll();
    personnelTable->setRowCount(0);
    for (auto s : staffs) {
        int row = personnelTable->rowCount();
        personnelTable->insertRow(row);

        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(s->getID()));
        idItem->setData(Qt::UserRole, QVariant::fromValue((void*)s));

        personnelTable->setItem(row, 0, idItem);
        personnelTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(s->getName())));

        QString roleStr = (s->role == Role::Doctor) ? "Doctor" : "Nurse";
        personnelTable->setItem(row, 2, new QTableWidgetItem(roleStr));
    }
}


void MainWindow::onPatientDoubleClicked(int row, int col) {

    Patient* p = (Patient*)patientTable->item(row, 0)->data(Qt::UserRole).value<void*>();

    if (p && !p->isCured && p->disease) {
        QDialog *detailDialog = new QDialog(this);
        detailDialog->setWindowTitle("Detail");
        QVBoxLayout *layout = new QVBoxLayout(detailDialog);

        layout->addWidget(new QLabel("Disease: " + QString::number((int)p->disease->type)));
        layout->addWidget(new QLabel("symptoms: " + QString::fromStdString(p->disease->symptoms)));
        layout->addWidget(new QLabel("Description: " + QString::fromStdString(p->disease->patientDescription)));

        QPushButton *okBtn = new QPushButton("بستن");
        connect(okBtn, &QPushButton::clicked, detailDialog, &QDialog::accept);
        layout->addWidget(okBtn);

        detailDialog->exec();
    } else {
        QMessageBox::information(this, "Message" , "This person has no medicine or cured");
    }
}

void MainWindow::onPersonnelDoubleClicked(int row, int col) {
    Personnel* s = (Personnel*)personnelTable->item(row, 0)->data(Qt::UserRole).value<void*>();

    if (!s) return;



    if (s->role == Role::Doctor) {
        DoctorWindow* docWin = new DoctorWindow(docRoom, pharmacy, this);
        this->hide();
        docWin->show();

    }
    else if (s->role == Role::Nurse) {
        QMessageBox msgBox;
        msgBox.setText("Select Room to Enter:");
        QPushButton *pBtn = msgBox.addButton("Pharmacy", QMessageBox::ActionRole);
        QPushButton *iBtn = msgBox.addButton("Injection Room", QMessageBox::ActionRole);
        msgBox.exec();

        if (msgBox.clickedButton() == pBtn) {
            PharmacyWindow* pWin = new PharmacyWindow(pharmacy, injectionRoom, this);
            this->hide();
            pWin->show();
        } else if (msgBox.clickedButton() == iBtn) {
            InjectionWindow* iWin = new InjectionWindow(injectionRoom, this);
            this->hide();
            iWin->show();
        }
    }
}


void MainWindow::closeEvent(QCloseEvent *event) {

    QApplication::quit();
    event->accept();
}
