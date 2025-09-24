<div align="center">
  <a href="https://github.com/Jesus-DSP/TAMIU-OS-Simulator-Project">
    <img src="assets/images/banner.png" alt="OS Simulator Banner" width="100%">
  </a>
  <br/>
  <br/>
  <h1>OS Simulator Project</h1>
  <p>A simplified operating system simulator using C++, starting with a basic shell, authentication, and a simulated boot process.</p>
  <br/>

  [Report a Bug](https://github.com/Jesus-DSP/TAMIU-OS-Simulator-Project/issues/new?labels=bug&template=bug_report.md&title=%5BBUG%5D%20)
  ·
  [Request a Feature](https://github.com/Jesus-DSP/TAMIU-OS-Simulator-Project/issues/new?labels=enhancement&template=feature_request.md&title=%5BFEATURE%5D%20)
  ·
  [Ask a Question](https://github.com/Jesus-DSP/TAMIU-OS-Simulator-Project/discussions/new?category=general)
  <br/>
  <br/>
  
  ![version: 1.0.0](https://img.shields.io/badge/version-1.0.0-blue.svg?style=for-the-badge)
  [![Minimum C++ Standard: C++11](https://img.shields.io/badge/C%2B%2B-11-blue.svg?style=for-the-badge)](https://en.cppreference.com/w/cpp/compiler_support)
  [![Language: C++](https://img.shields.io/badge/language-C%2B%2B-00599C.svg?style=for-the-badge)](https://isocpp.org/)
  [![Total Commits](https://img.shields.io/github/commits-since/Jesus-DSP/TAMIU-OS-Simulator-Project/v1.0.0?style=for-the-badge)](https://github.com/Jesus-DSP/TAMIU-OS-Simulator-Project/commits/main)
</div>


### 🤔 Why create this basic OS?

This project aims to provide a hands-on learning experience in operating system concepts by building a simplified simulator from the ground up. It allows exploration of fundamental OS components such as user authentication, process management, and memory allocation in a controlled C++ environment.

The initial phase focuses on establishing a solid foundation for future development, ensuring a modular and extensible structure for more complex OS features to be added progressively.


## 📖 Table of Contents

<details>
<summary>Click to expand</summary>

- [📖 Table of Contents](#-table-of-contents)
- [⛓ Features](#-features)
- [🚀 Getting Started](#-getting-started)
    - [Requirements](#requirements)
    - [Installation](#installation)
        - [Download or Clone the Project](#download-or-clone-the-project)
        - [Step 1: Compile the Code](#step-1-compile-the-code)
    - [Config](#config)
        - [.gitignore](#gitignore)
    - [Step 2: Run the Program](#step-2-run-the-program)
- [🔨 Development](#-development)
    - [Tech Stack](#tech-stack)
- [☑️ TODOs](#%EF%B8%8F-todos)
- [✨ Future Features](#-future-features)
- [Changelog](#changelog)
- [Help & Questions](#help--questions)
- [Contributing](#contributing)
- [👤 Author](#-author)
- [🤝 Credits](#-credits)
- [💛 Support](#-support)
- [⚖️ Disclaimer](#%EF%B8%8F-disclaimer)
</details>


---

## ⛓ Features
- **Simulated Boot Process**: Displays a boot-up message to the user upon program start.
- **Basic Program Structure**: Organized C++ files (`main.cpp`, `auth.h`, `auth.cpp`).
- **Simple Authentication**: `authenticateUser()` function prompts for username and password, verifying against hardcoded credentials (`admin` / `password123`).


---

## 🚀 Getting Started

To get your copy of SimOS running locally, follow these simple steps.

### Requirements
- **C++ Compiler**: `g++` (or equivalent)
- **Git**: For cloning the repository.
- **make**: (Optional, but recommended for easy compilation).

### Installation

#### Download or Clone the Project

First, clone the repository to your local machine:

```shell
git clone https://github.com/Jesus-DSP/TAMIU-OS-Simulator-Project.git
```

Now cd into the project directory:

```shell
cd TAMIU-OS-Simulator-Project
```

#### Step 1: Compile the Code

This step takes your C++ source files from the `src` directory and uses the `g++` compiler to create a single executable program in the `bin` directory.

First, if the `bin` directory doesn't exist, create it:

```bash
mkdir -p bin
```

Now, run the compilation command:

```bash
g++ src/main.cpp src/auth.cpp -o bin/os_simulator
```

### Config

#### .gitignore

Ensure you have a `.gitignore` file in your root directory to exclude compiled binaries and other unnecessary files from your repository. A minimal `.gitignore` for this project should include:

```gitignore
# Ignore the binary/executable directory
bin/
```

### Step 2: Run the Program

After the command in Step 1 finishes without errors, your program is ready to run. To execute it, you just need to type the path to the new program file.

Run this command:

```bash
./bin/os_simulator
```

Your program will then start running in the terminal. 👍

---

## 🔨 Development

### Tech Stack

The project is built using:

- C++: The core programming language.
- g++: The GNU C++ Compiler for compilation.

---

## ☑️ TODOs

- [ ] Implement a more robust user management system (e.g., storing credentials in a file).
- [ ] Expand the main function to include a basic command-line shell after authentication.
- [ ] Add unit tests for the authentication function and other modules.

---

## ✨ Future Features

- Command-line Interface (CLI): A functional shell for user interaction.
- Process Management: Basic simulation of process creation, scheduling, and termination.
- Memory Management: Simple memory allocation and deallocation.
- File System: A basic simulated file system.
- Error Handling: More comprehensive error handling for user input and system operations.

---

## Changelog

Please see `CHANGELOG.md` for more information on what has changed recently.
(You will need to create this file manually as the project evolves.)

---

## Help & Questions

For any questions, issues, or discussions regarding this project:

- Start a new discussion in the Discussions Tab.
- Report a Bug.
- Request a Feature.

---

## Contributing

Contributions are welcome! Please follow these steps to contribute:

1. Fork the repository.
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`).
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`).
4. Push to the Branch (`git push origin feature/AmazingFeature`).
5. Open a Pull Request.

For major changes, please open an issue first to discuss what you would like to change.

Thank you!

---

## 👤 Author

SimOS is primarily developed by:

- 🦜 Jesus-DSP
  - GitHub: `@Jesus-DSP`
  - [Add other team members here with their GitHub links]

---

## 🤝 Credits

This project utilizes the standard C++ library and the g++ compiler. No external third-party libraries are used in this initial phase.

---

## 💛 Support

If this project helps you learn or is useful, please consider:

- ⭐️ Star this project on GitHub
- 🐙 Follow me on GitHub

Every bit of support is appreciated! 🙏

---

## ⚖️ Disclaimer

This SimOS project is a simplified educational simulator and is not intended to replicate a real-world operating system. It is designed for academic purposes to illustrate fundamental OS concepts. It does not provide the security, stability, or features of a commercial operating system. Use at your own risk.

<p align="right">^ <a href="#os-simulator-project">back to top</a> ^</p>