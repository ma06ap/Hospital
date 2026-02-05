# 🏥 Hospital Simulator

> **A smart, lightweight hospital management system built with modern C++ and Qt.** > *Efficiency meets Object-Oriented Design.*

![C++](https://img.shields.io/badge/C++-17-blue.svg) ![Qt](https://img.shields.io/badge/Qt-6.x-green.svg) ![License](https://img.shields.io/badge/License-MIT-orange.svg)

---

## 🚀 Overview

This project simulates a fully functional hospital workflow. It manages the lifecycle of patients from admission to discharge, enforcing real-world constraints through strict Object-Oriented programming. It features a custom **Generic Data Layer** and a **Dynamic UI** that adapts to hospital events in real-time.

---

## ✨ Key Features

- **🧠 Smart Treatment Logic:** The system analyzes prescriptions automatically. If a doctor prescribes an *Ampoule* or *Serum*, the patient is auto-routed to the **Injection Room**. No manual triage needed.
- **🛡️ Role-Based Access Control:** Security via Polymorphism. Doctors 🩺 and Nurses 👩‍⚕️ can only access rooms relevant to their specific roles.
- **🧬 Generic Data Engine:** Powered by custom **C++ Templates** and **Functors** to handle data storage, ID generation (`AutoIncrement`), and complex relationships (`Smart FK`) efficiently.
- **⚡ Reactive UI:** Built with **Qt**, the interface updates dynamically. Forms and dashboards reshape themselves based on user type (Patient vs. Personnel).
- **📊 Real-time Queueing:** Utilizes `STL` containers (`std::deque`, `std::queue`) to manage patient waiting lists and active treatment slots seamlessly.

---

## 🔄 The Workflow

1.  **Registration:** Patient enters data. System validates ID and assigns a unique medical record. 📝
2.  **Consultation:** Patient enters the **Doctor's Office**. Doctor prescribes meds. 🩺
3.  **Pharmacy:** Patient moves to the Pharmacy. Meds are dispensed. 💊
4.  **Decision Node:**
    * *Tablets/Syrups?* → Patient is **Discharged** (Cured). ✅
    * *Injectables?* → Auto-routed to **Injection Room**. 💉
5.  **Treatment:** Injection administered. Patient is **Discharged** (Cured). ✅

---

## 🛠️ Tech Stack

* **Language:** C++ (Strong usage of Templates, Polymorphism, Abstract Classes)
* **Framework:** Qt (Widgets)
* **Architecture:** Layered Architecture (Data / Logic / UI)
* **Concepts:** OOP, Functors, Lambdas, STL Algorithms.

---

## 💻 How to Run

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/ma06ap/Hospital.git](https://github.com/ma06ap/Hospital.git)
    ```
2.  **Open in Qt Creator:**
    * Open `CMakeLists.txt` or the `.pro` file.
3.  **Build & Run:**
    * Select your Kit (Desktop Qt 6.x) and hit **Run**.

---

<p align="center">
  Made with ❤️ and clean C++ code.
</p>
