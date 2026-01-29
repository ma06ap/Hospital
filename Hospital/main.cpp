//#include "mainwindow.h"

//#include <QApplication>

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
//}


// start test main.cpp

#include <iostream>
#include <vector>
#include <string>

// اضافه کردن هدرهای پروژه
#include "DataManagement.h"
#include "Person.h"
#include "Patient.h"
#include "Personnel.h"
#include "Room.h"
#include "DoctorRoom.h"
#include "Pharmacy.h"
#include "InjectionRoom.h"
#include "Medicine.h"
#include "FK.h"

using namespace std;

// تابع کمکی برای چاپ خط جداکننده
void printHeader(string title) {
    cout << "\n========================================\n";
    cout << "   " << title << "\n";
    cout << "========================================\n";
}

int main() {
    printHeader("1. TEST: Data Creation & Validation");

    // --- تست ۱: اعتبارسنجی داده‌ها (Static Methods) ---
    if (Personnel::checkPersonnelCode("1234567")) {
        cout << "[PASS] Personnel Code Validator works." << endl;
    } else {
        cout << "[FAIL] Personnel Code Validator failed." << endl;
    }

    if (!Person::checkNationalCode("123")) { // باید غلط باشد
        cout << "[PASS] National Code Validator rejected invalid input." << endl;
    }

    // --- تست ۲: ایجاد پرسنل و ذخیره در DataManagement ---
    Personnel* doc1 = new Personnel("Dr. House", "0011223344", "1000001", Role::Doctor);
    Personnel* nurse1 = new Personnel("Nurse Joy", "0055667788", "2000001", Role::Nurse);
    Personnel* nurse2 = new Personnel("Nurse Ratched", "9988776655", "2000002", Role::Nurse);

    DataManagement<Personnel>::create(doc1);
    DataManagement<Personnel>::create(nurse1);
    DataManagement<Personnel>::create(nurse2);

    cout << "[INFO] Personnel created. Doctor ID: " << doc1->getID() << endl; // تست AutoIncrement

    printHeader("2. TEST: Polymorphism & Room Logic");

    // --- تست ۳: منطق اتاق‌ها و چندریختی (Abstract Class) ---
    DoctorRoom* docRoom = new DoctorRoom();
    Pharmacy* pharmacy = new Pharmacy();
    InjectionRoom* injectionRoom = new InjectionRoom();

    // سناریو الف: دکتر وارد اتاق دکتر شود (باید موفق شود)
    if (docRoom->enterStaff(doc1)) {
        cout << "[PASS] Doctor entered DoctorRoom." << endl;
    } else {
        cout << "[FAIL] Doctor could not enter DoctorRoom." << endl;
    }

    // سناریو ب: پرستار وارد اتاق دکتر شود (باید شکست بخورد)
    if (!docRoom->enterStaff(nurse1)) {
        cout << "[PASS] Nurse correctly blocked from DoctorRoom." << endl;
    } else {
        cout << "[FAIL] Security Breach! Nurse entered DoctorRoom." << endl;
    }

    // سناریو ج: دکتر وارد داروخانه شود (باید شکست بخورد)
    if (!pharmacy->enterStaff(doc1)) {
        cout << "[PASS] Doctor correctly blocked from Pharmacy." << endl;
    }

    // سناریو د: پرستار وارد داروخانه شود
    pharmacy->enterStaff(nurse1);
    cout << "[INFO] Nurse Joy entered Pharmacy." << endl;

    // سناریو ه: پرستار دوم وارد تزریقات شود
    injectionRoom->enterStaff(nurse2);
    cout << "[INFO] Nurse Ratched entered Injection Room." << endl;


    printHeader("3. TEST: Patient Flow & Automatic Treatment Logic");

    // --- تست ۴: ایجاد بیمار و بیماری ---
    Patient* p1 = new Patient("John Doe", "1234567890");
    Disease* flu = new Disease(DiseaseType::Infection, "Fever, Cough", "Started yesterday");
    p1->setDisease(flu);
    DataManagement<Patient>::create(p1);

    // ۱. ورود بیمار به اتاق دکتر
    docRoom->addPatient(p1);
    cout << "[STEP 1] Patient " << p1->getName() << " entered Doctor Room. Queue Size: "
         << docRoom->getActivePatients().size() << endl;

    // ۲. دکتر دارو تجویز می‌کند (یک قرص و یک آمپول)
    Medicine m1("Amoxicillin", MedicineUnit::Tablet, 10);
    Medicine m2("Penicillin", MedicineUnit::Ampoule, 1); // این باید نیاز به تزریق ایجاد کند

    p1->treatment->addMedicine(m1);
    p1->treatment->addMedicine(m2);

    cout << "[STEP 2] Doctor prescribed medicines." << endl;

    // تست لاجیک خودکار (آیا سیستم فهمید تزریق لازم است؟)
    if (p1->treatment->needsInjection) {
        cout << "[PASS] System AUTOMATICALLY detected injection need." << endl;
    } else {
        cout << "[FAIL] System failed to detect injection need." << endl;
    }

    // ۳. بیمار به داروخانه می‌رود
    docRoom->removePatient(p1);
    pharmacy->addPatient(p1);

    // شبیه‌سازی تحویل دارو
    p1->treatment->gotMedicine = true;
    cout << "[STEP 3] Patient got medicines from Pharmacy." << endl;

    // ۴. تصمیم‌گیری پرستار داروخانه
    if (p1->treatment->needsInjection) {
        cout << "[DECISION] Pharmacy Nurse sends patient to Injection Room." << endl;
        pharmacy->removePatient(p1);
        injectionRoom->addPatient(p1);
    }

    // ۵. انجام تزریقات
    // پرستار تزریق را انجام می‌دهد
    p1->treatment->needsInjection = false;
    cout << "[STEP 4] Injection done. Patient is ready to leave." << endl;
    injectionRoom->removePatient(p1);

    // ۶. ترخیص و درمان نهایی
    if (p1->treatment->gotMedicine && !p1->treatment->needsInjection) {
        p1->cure();
        cout << "[STEP 5] Patient CURED! Disease deleted." << endl;
    }

    if (p1->disease == nullptr && p1->isCured) {
        cout << "[PASS] Memory cleanup verified (Disease is null)." << endl;
    }


    printHeader("4. TEST: Advanced Templates & Lambda Search");

    // --- تست ۵: استفاده از FK Functor و Filter Lambda ---

    // ساخت چند بیمار دیگر برای تست جستجو
    Patient* p2 = new Patient("Jane Smith", "0987654321");
    p2->isCured = true; // این یکی قبلا خوب شده
    DataManagement<Patient>::create(p2);

    // استفاده از Lambda برای پیدا کردن بیماران درمان شده
    // (Lambda Concept)
    auto curedPatients = DataManagement<Patient>::filter([](Patient* p) {
        return p->isCured == true;
    });

    cout << "[SEARCH] Found " << curedPatients.size() << " cured patient(s)." << endl;
    cout << "         -> Name: " << curedPatients[0]->getName() << endl;

    // تست FK Functor
    // (Functor Concept)
    // فرض کنیم می‌خواهیم دکتری که ID=1 دارد را با استفاده از آبجکت FK پیدا کنیم
    FK<Personnel> doctorFinder(1); // ID 1 باید Dr. House باشد
    Personnel* foundDoc = doctorFinder(); // فراخوانی به صورت تابع ()

    if (foundDoc) {
        cout << "[FUNCTOR TEST] FK Functor found: " << foundDoc->getName() << endl;
    } else {
        cout << "[FAIL] FK Functor failed." << endl;
    }

    printHeader("TEST SUITE COMPLETED");
    return 0;
}