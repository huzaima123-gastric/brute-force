## 👨‍💻 Author

**Huzaima Asim**

---

# 🔐 Linux Password Cracker (Ethical Hacking Project)

A **Linux-based password cracking tool** developed in **C++** for **educational and ethical hacking purposes only**.
This project demonstrates how Linux stores passwords securely using **salts and hashes** and how **brute-force attacks** work at a low level.

> ⚠️ **Disclaimer:**
> This project is strictly for learning and must only be used in a **local Linux VM** on accounts you own.
> Unauthorized use is illegal and unethical.

---

## 📌 Project Overview

This program:

* Reads password hashes from `/etc/shadow`
* Extracts the **hashing algorithm ID**, **salt**, and **stored hash**
* Performs a **recursive brute-force attack** using lowercase characters (`a–z`)
* Compares generated hashes using the Linux `crypt()` function
* Measures and reports **performance and execution time**

---

## 🛠️ Technologies Used

* **Language:** C++
* **Platform:** Linux (Ubuntu recommended)
* **Libraries:**

  * `libcrypt` (for `crypt()` hashing)
  * `<chrono>` for performance measurement
* **Tools:**

  * GNU Compiler Collection (`g++`)
  * Makefile for automated compilation

---

## 📂 Project Structure

```
Linux-Password-Cracker/
│
├── cracker.cpp        # Main C++ source code
├── Makefile           # Compilation automation
├── README.md          # Project documentation
└── report.pdf         # Detailed project report (optional)
```

---

## ⚙️ Prerequisites

Before running the project, ensure you have:

* A **Linux VM** (Ubuntu 20.04 / 22.04 recommended)
* Root access (`sudo`)
* Required packages installed:

```bash
sudo apt update
sudo apt install build-essential g++ libcrypt-dev
```

---

## 🚀 Installation & Compilation

Clone the repository:

```bash
git clone https://github.com/your-username/linux-password-cracker.git
cd linux-password-cracker
```

Compile the project using Makefile:

```bash
make
```

This will generate an executable named `cracker`.

---

## 👤 Test User Setup (Safe Testing)

Create a test user for ethical testing:

```bash
sudo adduser linuxuser
sudo passwd linuxuser
```

> Use a **simple lowercase password** like `abc` or `abcd` for testing.

Verify the hash exists:

```bash
sudo grep '^linuxuser:' /etc/shadow
```

---

## ▶️ Running the Program

Run the cracker **with sudo** (required to read `/etc/shadow`):

```bash
sudo ./cracker linuxuser
```

The program will:

* Try passwords starting from length 1
* Incrementally increase length
* Stop when the password is found or the max length is reached

---

## 🧠 How It Works

### 🔹 Hash Extraction

* Reads `/etc/shadow`
* Parses entries in the format:

  ```
  username:$id$salt$hash:...
  ```
* Builds the correct salt prefix for hashing

### 🔹 Brute-Force Algorithm

* Uses recursion to generate all combinations (`a`–`z`)
* Hashes each candidate using `crypt()`
* Compares computed hash with stored hash

### 🔹 Performance Tracking

* Measures time per password length
* Outputs total execution time

---

## 📊 Sample Results

| Password Length | Time Taken | Status |
| --------------- | ---------- | ------ |
| 3               | ~0.9 s     | Found  |
| 4               | ~23 s      | Found  |
| 5               | ~2 hours   | Found  |
| 6               | ~5.2 hrs   | Found  |

> Demonstrates the **exponential cost of brute-force attacks**

---

## 🔍 Key Learning Outcomes

* Understanding Linux password storage (`/etc/shadow`)
* Real-world hashing and salting mechanisms
* Recursive brute-force logic
* Importance of strong passwords
* Ethical boundaries in offensive security

---

## ⚠️ Ethical & Legal Warning

This project is intended **only for academic learning**.

❌ Do NOT use on:

* Production systems
* University servers
* Accounts you do not own

Misuse may violate cybersecurity laws such as:

* Computer Fraud and Abuse Act (CFAA)
* Local cybercrime regulations

---


## ⭐ Future Improvements

* Add multithreading (OpenMP / pthreads)
* Support larger character sets
* Implement dictionary attacks
* Add GPU acceleration
* Improve performance analytics

---

## 📜 License

This project is released for **educational use only**.
No warranty. Use responsibly.



Just tell me 😎
