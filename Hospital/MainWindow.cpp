#include "MainWindow.h"
#include "RoomWindows.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFormLayout>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // 1. ساخت اتاق‌ها (فقط یکبار در شروع برنامه)
    docRoom = new DoctorRoom();
    pharmacy = new Pharmacy();
    injectionRoom = new InjectionRoom();

    setupUi();     // چیدن دکمه‌ها
    refreshTables(); // پر کردن اولیه جدول‌ها
}

MainWindow::~MainWindow() {
    // پاکسازی حافظه اتاق‌ها هنگام بستن برنامه
    delete docRoom;
    delete pharmacy;
    delete injectionRoom;
}

void MainWindow::setupUi() {
    QWidget *centralWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // --- بخش ۱: جدول‌ها (Splitter یا کنار هم) ---
    QHBoxLayout *tablesLayout = new QHBoxLayout();

    // جدول بیماران
    patientTable = new QTableWidget();
    patientTable->setColumnCount(3);
    patientTable->setHorizontalHeaderLabels({"ID", "Patient Name", "Status"});
    patientTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // اتصال سیگنال دابل کلیک
    connect(patientTable, &QTableWidget::cellDoubleClicked, this, &MainWindow::onPatientDoubleClicked);

    // جدول پرسنل
    personnelTable = new QTableWidget();
    personnelTable->setColumnCount(3);
    personnelTable->setHorizontalHeaderLabels({"ID", "Personell Name", "Role"});
    personnelTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(personnelTable, &QTableWidget::cellDoubleClicked, this, &MainWindow::onPersonnelDoubleClicked);

    tablesLayout->addWidget(new QLabel("Patient list:"));
    tablesLayout->addWidget(patientTable);
    tablesLayout->addWidget(new QLabel("Personnel list:"));
    tablesLayout->addWidget(personnelTable);

    mainLayout->addLayout(tablesLayout, 2); // ضریب 2 برای اشغال فضای بیشتر

    // --- بخش ۲: فرم ثبت نام ---
    QGroupBox *registerBox = new QGroupBox("ADD");
    QVBoxLayout *formLayout = new QVBoxLayout(registerBox);

    // انتخاب نوع
    QHBoxLayout *typeLayout = new QHBoxLayout();
    radioPatient = new QRadioButton("Patient");
    radioPersonnel = new QRadioButton("Personnel");
    radioPatient->setChecked(true);
    typeLayout->addWidget(radioPatient);
    typeLayout->addWidget(radioPersonnel);

    connect(radioPatient, &QRadioButton::toggled, this, &MainWindow::onUserTypeChanged);

    // فیلدهای مشترک
    QLineEdit *nameInput = new QLineEdit();
    nameInput->setPlaceholderText("Full Name");
    this->nameInput = nameInput;

    QLineEdit *nationalCodeInput = new QLineEdit();
    nationalCodeInput->setPlaceholderText("National Code (10 digits)");
    this->nationalCodeInput = nationalCodeInput;

    formLayout->addLayout(typeLayout);
    formLayout->addWidget(nameInput);
    formLayout->addWidget(nationalCodeInput);

    // --- فیلدهای مخصوص بیمار ---
    patientFormGroup = new QGroupBox("Medical Info");
    QFormLayout *pLayout = new QFormLayout(patientFormGroup);

    diseaseTypeCombo = new QComboBox();
    diseaseTypeCombo->addItem("Cold", (int)DiseaseType::Cold);
    diseaseTypeCombo->addItem("Cancer", (int)DiseaseType::Cancer);
    diseaseTypeCombo->addItem("HeartIssue", (int)DiseaseType::HeartIssue);
    diseaseTypeCombo->addItem("Fracture", (int)DiseaseType::Fracture);
    diseaseTypeCombo->addItem("Infection", (int)DiseaseType::Infection);
    diseaseTypeCombo->addItem("Other...", (int)DiseaseType::Other);
    // ... سایر موارد

    symptomsInput = new QLineEdit();
    descriptionInput = new QTextEdit();
    descriptionInput->setMaximumHeight(50);

    pLayout->addRow("Disease Type:", diseaseTypeCombo);
    pLayout->addRow("symptoms:", symptomsInput);
    pLayout->addRow("Description:", descriptionInput);

    formLayout->addWidget(patientFormGroup);

    // --- فیلدهای مخصوص پرسنل ---
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
    personnelFormGroup->hide(); // چون پیش‌فرض بیمار است، این مخفی باشد

    // دکمه ثبت
    QPushButton *btnAdd = new QPushButton("ADD");
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    formLayout->addWidget(btnAdd);

    mainLayout->addWidget(registerBox, 1);
    setCentralWidget(centralWidget);
    setWindowTitle("Hospital");
    resize(800, 600);
}

// --- لاجیک تغییر فرم ---
void MainWindow::onUserTypeChanged() {
    if (radioPatient->isChecked()) {
        patientFormGroup->show();
        personnelFormGroup->hide();
    } else {
        patientFormGroup->hide();
        personnelFormGroup->show();
    }
}

// --- لاجیک دکمه ثبت (مهم) ---
void MainWindow::onAddClicked() {
    QString name = nameInput->text();
    QString natCode = nationalCodeInput->text();

    // 1. اعتبارسنجی عمومی
    if (!Person::checkName(name.toStdString()) || !Person::checkNationalCode(natCode.toStdString())) {
        QMessageBox::warning(this, "خطا", "نام خالی یا کد ملی نامعتبر است.");
        return;
    }

    if (radioPatient->isChecked()) {
        // --- ثبت بیمار ---
        Patient *p = new Patient(name.toStdString(), natCode.toStdString());

        // ساخت بیماری
        DiseaseType type = (DiseaseType)diseaseTypeCombo->currentData().toInt();
        Disease *d = new Disease(type, symptomsInput->text().toStdString(), descriptionInput->toPlainText().toStdString());
        p->setDisease(d);

        // ذخیره در دیتابیس کلی
        DataManagement<Patient>::create(p);

        // *** اصلاحیه مهم: اضافه کردن بیمار به صف اتاق دکتر ***
        // این خط باعث می‌شود بیمار در لیست انتظار دکتر دیده شود
        docRoom->addPatient(p);

        QMessageBox::information(this, "موفق", "بیمار ثبت شد و در صف انتظار دکتر قرار گرفت.");

    } else {
        // --- بخش پرسنل (بدون تغییر) ---
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
            QMessageBox::critical(this, "خطا", "این کد پرسنلی قبلاً ثبت شده است!");
            return;
        }

        if (!Personnel::checkPersonnelCode(pCode.toStdString())) {
            QMessageBox::warning(this, "خطا", "کد پرسنلی باید ۷ رقم باشد.");
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
            QMessageBox::information(this, "موفق", "پرسنل ثبت و وارد اتاق شد.");
        } else {
            delete staff;
            QMessageBox::critical(this, "خطا", "ظرفیت اتاق تکمیل است یا نقش نامناسب است.");
            return;
        }
    }

    refreshTables();
    // پاک کردن ورودی‌ها برای نفر بعدی
    nameInput->clear();
    nationalCodeInput->clear();
    symptomsInput->clear();
    descriptionInput->clear();
}
// --- لاجیک بروزرسانی جدول‌ها ---
void MainWindow::refreshTables() {
    // پر کردن جدول بیماران
    auto patients = DataManagement<Patient>::getAll();
    patientTable->setRowCount(0); // پاک کردن قبلی‌ها
    for (auto p : patients) {
        int row = patientTable->rowCount();
        patientTable->insertRow(row);

        // ذخیره پوینتر آبجکت در دیتای آیتم برای دسترسی بعدی
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(p->getID()));
        idItem->setData(Qt::UserRole, QVariant::fromValue((void*)p));

        patientTable->setItem(row, 0, idItem);
        patientTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(p->getName())));
        patientTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(p->getStatus())));
    }

    // پر کردن جدول پرسنل
    auto staffs = DataManagement<Personnel>::getAll();
    personnelTable->setRowCount(0);
    for (auto s : staffs) {
        int row = personnelTable->rowCount();
        personnelTable->insertRow(row);

        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(s->getID()));
        idItem->setData(Qt::UserRole, QVariant::fromValue((void*)s));

        personnelTable->setItem(row, 0, idItem);
        personnelTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(s->getName())));

        QString roleStr = (s->role == Role::Doctor) ? "دکتر" : "پرستار";
        personnelTable->setItem(row, 2, new QTableWidgetItem(roleStr));
    }
}

// --- لاجیک دابل کلیک بیمار (نمایش بیماری) ---
void MainWindow::onPatientDoubleClicked(int row, int col) {
    // بازیابی پوینتر بیمار از سطر کلیک شده
    Patient* p = (Patient*)patientTable->item(row, 0)->data(Qt::UserRole).value<void*>();

    if (p && !p->isCured && p->disease) {
        QDialog *detailDialog = new QDialog(this);
        detailDialog->setWindowTitle("جزئیات بیماری");
        QVBoxLayout *layout = new QVBoxLayout(detailDialog);

        layout->addWidget(new QLabel("نوع بیماری: " + QString::number((int)p->disease->type))); // می‌توان نام رشته‌ای گذاشت
        layout->addWidget(new QLabel("علائم: " + QString::fromStdString(p->disease->symptoms)));
        layout->addWidget(new QLabel("توضیحات بیمار: " + QString::fromStdString(p->disease->patientDescription)));

        QPushButton *okBtn = new QPushButton("بستن");
        connect(okBtn, &QPushButton::clicked, detailDialog, &QDialog::accept);
        layout->addWidget(okBtn);

        detailDialog->exec(); // نمایش به صورت مودال (پنجره کوچک)
    } else {
        QMessageBox::information(this, "پیام", "این بیمار درمان شده یا اطلاعات بیماری ندارد.");
    }
}

// --- لاجیک دابل کلیک پرسنل (رفتن به صفحه اتاق) ---
void MainWindow::onPersonnelDoubleClicked(int row, int col) {
    Personnel* s = (Personnel*)personnelTable->item(row, 0)->data(Qt::UserRole).value<void*>();

    if (!s) return;

    // تشخیص اینکه کدام پنجره باید باز شود
    // ما باید بدانیم این پرسنل در کدام اتاق است.
    // راه حل ساده: چک کردن لیست پرسنل هر اتاق

    // 1. آیا در اتاق دکتر است؟
    // (توجه: برای دسترسی به لیست پرسنل در کلاس Room، باید getter اضافه کنید یا friend class باشد.
    //  اما برای سادگی فرض میکنیم پرسنل نقش Doctor دارد)

    if (s->role == Role::Doctor) {
        DoctorWindow* docWin = new DoctorWindow(docRoom, pharmacy, this);
        this->hide(); // بستن (مخفی کردن) صفحه اصلی
        docWin->show();
        // با بسته شدن docWin، صفحه اصلی خودکار باز میشود (چون this را فرستادیم)
    }
    else if (s->role == Role::Nurse) {
        // باید بفهمیم پرستار داروخانه است یا تزریقات
        // چون در آبجکت Personnel فیلد اتاق نداریم،
        // می‌توانیم از طریق چک کردن لیست پرسنل اتاق‌ها بفهمیم (که getter میخواهد)
        // یا به صورت ساده: از کاربر بپرسیم یا بر اساس قراردادی عمل کنیم.

        // *راه حل استاندارد پروژه شما:*
        // بیایید فرض کنیم اگر اول لیست داروخانه بود، مال آنجاست.
        // اما یک راه ساده‌تر: یک دیالوگ کوچک باز شود و بپرسد "وارد کدام بخش میشوید؟"
        // یا بهتر: در کلاس Personnel یک فیلد int roomType اضافه می‌کردیم.

        // *راه حل موقت:* باز کردن پنجره بر اساس انتخاب کاربر (چون لاجیک سخت می‌شود)
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

// در فایل MainWindow.h زیر بخش protected:


// در فایل MainWindow.cpp
void MainWindow::closeEvent(QCloseEvent *event) {
    // بستن تمام پنجره‌های فرزند و خروج کامل از برنامه
    QApplication::quit();
    event->accept();
}
