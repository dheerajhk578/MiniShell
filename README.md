# 🐚 MiniShell – Interactive Unix-like Shell in C

MiniShell is a custom-built Unix-like command-line interpreter developed in **C** using **Linux system programming** concepts. It supports execution of built-in and external commands, piping, I/O redirection, background processes, and signal handling, closely mimicking basic shell behavior.

---

## 📌 Features

* ✅ Execute **built-in commands** (cd, pwd, echo, exit, etc.)
* ✅ Execute **external Linux commands**
* ✅ **Piping (`|`)** between multiple commands
* ✅ **Input & Output Redirection** (`<`, `>`, `>>`)
* ✅ **Foreground & Background execution** (`&`)
* ✅ **Signal Handling** (Ctrl+C, Ctrl+Z)
* ✅ **Job Control** – fg, bg, jobs
* ✅ Custom prompt display

---

## 🛠️ Technologies Used

* **C Programming**
* **Linux System Programming**
* **POSIX System Calls** – `fork()`, `execvp()`, `wait()`, `pipe()`, `dup2()`
* **File I/O**
* **Signal Handling**
* **Makefile**

---

## 📁 Project Structure

```
MiniShell/
│
├── main.c
├── commands.c
├── scan_input.c
├── signals.c
├── header.h
├── external_commands.txt
├── documentation.txt
├── Makefile
└── interactive_unix_like_shell/
```

---

## ⚙️ How It Works

1. Displays a custom prompt.
2. Accepts user input.
3. Tokenizes and parses the command.
4. Checks for built-in or external commands.
5. Handles piping, redirection, and background execution.
6. Creates child processes using `fork()` and executes commands using `execvp()`.
7. Manages signals and job control.

---

## ▶️ How to Compile and Run

```bash
gcc *.c -o minishell
./minishell
```

Or using Makefile:

```bash
make
./minishell
```

---

## 🧪 Sample Usage

```bash
Minishell: $ ls -l | grep .c
Minishell: $ cat file.txt > output.txt
Minishell: $ sleep 10 &
Minishell: $ jobs
Minishell: $ fg 1
```

---

## 🚧 Key Challenges & Learnings

* Parsing and tokenizing user input efficiently.
* Implementing piping using `pipe()` and `dup2()`.
* Handling multiple child processes.
* Managing foreground/background jobs.
* Implementing signal handling for `SIGINT`, `SIGTSTP`, and `SIGCHLD`.

---

## 🎯 Learning Outcomes

* Deep understanding of **process management**
* Hands-on experience with **Linux system calls**
* Improved skills in **memory management and debugging**
* Strong foundation in **shell internals**

---

## 👨‍💻 Author

**Dheeraj H K**
Embedded Systems Trainee | C Programmer | Linux Enthusiast
📍 Karnataka, India

---
* Customize for **embedded profile**
* Or make a **shorter version** for quick GitHub display 😊
