# Crypto Utilities

A simple command-line utility for encrypting and decrypting files using the ChaCha20-Poly1305 Authenticated Encryption with Associated Data (AEAD) algorithm provided by `libsodium`.

## Prerequisites

You need a C++ compiler that supports C++20 and the `libsodium` library.

### On Debian/Ubuntu

You can run this command to install everything the project requires:
```bash
sudo apt update && sudo apt upgrade && sudo apt install build-essentials ninja-build libsodium-dev pkg-config
```

### On Windows

These instructions use the MSYS2 environment to provide the `g++` compiler and other build tools.

1.  **Install MSYS2**:
    * Download and install MSYS2 from the [official website](https://www.msys2.org/).
    * After installation, open the **MSYS2 UCRT64** terminal.
    * Update the package database and core packages by running:
        ```bash
        pacman -Syu
        ```
        You may need to close the terminal and run it again to complete the update. Then run it one more time.

2.  **Install Build Tools**:
    * In the **MSYS2 UCRT64** terminal, install the MinGW `g++` toolchain, `git`, `cmake`, and `ninja` with a single command:
      ```bash
      pacman -S --needed mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja git
      ```
    * Add the path to your MinGW bin folder to the system environment variable (ex. `C:\msys64\ucrt64\bin`).

3.  **Install vcpkg and libsodium**:
    * Choose a directory to install `vcpkg`, such as `C:\dev\vcpkg`. Avoid paths with spaces like `Program Files`.
    * From a standard PowerShell or Command Prompt, clone and bootstrap `vcpkg`:
      ```powershell
      git clone https://github.com/microsoft/vcpkg.git
      cd vcpkg
      .\bootstrap-vcpkg.bat
      ```
    * Install `libsodium` using the MinGW triplet. This builds the library for use with the `g++` compiler you installed.
      ```powershell
      .\vcpkg.exe install libsodium:x64-mingw-dynamic
      ```

## Build Instructions 

1.  **Configure the project:** Navigate to the project directory and run CMake.
    * On Debian/Ubuntu/Windows:
      ```bash
      cmake -B build -G Ninja
      ```
    * If `libsodium` is in a custom location:
      ```bash
      cmake -B build -G Ninja -D CMAKE_PREFIX_PATH="/path/to/your/install/dir"
      ```

2.  **Compile the program:**
    Run the following code to compile the program:
    ```bash
    cmake --build build
    ```
    The final `encryptor` and `decryptor` executables will be located in the `build` directory.

## Usage

The program prompts for a secret key to perform encryption or decryption.

### 1\. Get a Secret Key

  * **For Encryption:** Generate a secure 256-bit (32-byte) key. **Do this once per secret and store the key in a safe place like a password manager.**
    ```bash
    openssl rand -hex 32
    ```
    This will output a long hexadecimal string. This is your secret key.

  * **For Decryption:** You will need the exact same key that was used to encrypt the file.

### 2\. Run the Program

Execute the compiled program, providing the input file and desired output file as arguments. The program will then prompt you to enter the secret key.

  * **On Windows:**
      * To encrypt:
        ```powershell
        ./build/encryptor.exe <input_file> <output_file>
        ```
      * To decrypt:
        ```powershell
        ./build/decryptor.exe <input_file> <output_file>
        ```
  * **Everywhere else:**
      * To encrypt:
        ```bash
        ./build/encryptor <input_file> <output_file>
        ```
      * To decrypt:
        ```bash
        ./build/decryptor <input_file> <output_file>
        ```

After running the command, you will be prompted:

```bash
Enter the secret key (hex):
```

Paste or type your key and press **Enter**. The input will be hidden for security. The program will then produce the output file.