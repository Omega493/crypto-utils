# Crypto Utilities

A simple command-line utility for encrypting and decrypting files using the XChaCha20-Poly1305 Authenticated Encryption with Associated Data (AEAD) algorithm provided by `libsodium`.

## Prerequisites

You need a C++ compiler that supports C++20, the `libsodium` library and OpenSSL.

### On Debian/Ubuntu

You can run this command to install everything the project requires:
```bash
sudo apt update && sudo apt upgrade && sudo apt install build-essentials ninja-build libsodium-dev pkg-config
```

### On Windows

You can choose from two available presets defined in the `CMakePresets.json` file: using MinGW or MSVC as the compiler.

#### MSVC

1. **Install Visual Studio:**
  * Download and install Visual Studio from the [Visual Studio website](https://visualstudio.microsoft.com/downloads/).
  * During installation, select the "Desktop development with C++" workload. This will install the compiler and CMake.

2. **Install vcpkg:**
  * Choose a directory to install `vcpkg`, such as `C:\dev\vcpkg`. Avoid paths with spaces like `Program Files`.
  * Using Windows Terminal, clone and bootstrap `vcpkg`:
  ```powershell
  git clone https://github.com/microsoft/vcpkg.git; cd vcpkg; .\bootstrap-vcpkg.bat
  ```
  * Put the path to your `vcpkg` installation as a system environment variable and restart your terminal.
  
3. **Install `libsodium`:**
  * If you are compiling for 64-bit Windows, execute the following in a terminal:
    ```powershell
    vcpkg install libsodium:x64-windows
    ```
  * If you are compiling for 32-bit Windows, execute the following in a terminal:
    ```powershell
    vcpkg install libsodium:x86-windows
    ```

4. **Set Environment Variable:**
  In your system environment variables, create a new variable:
  * Variable name: `CMAKE_TOOLCHAIN_FILE`
  * Variable value: `[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake`
  Restart your terminal session for the new environment variable to take effect.

#### `g++`

1. **Install MSYS2 and `g++`:**
  * Download and install MSYS2 [from their website](https://www.msys2.org/).
  * It is recommended to create the following system variable:
    * Name: `MSYS64`
    * Value: `[path-to-you]\msys64` (ex. `C:\msys64`)
  * If you want to compile on 64-bit system,
      * Launch the UCRT64 terminal and execute `pacman -S mingw-w64-ucrt-x86_64-gcc`.
      * Paste the following path to your system variable: `%MSYS64%\ucrt64\bin`
  * If you want to compile on 32-bit system,
      * Launch the MINGW32 terminal and execute `pacman -S mingw-w64-i686-gcc`.
      * Paste the following path to your system variable: `%MSYS64%\mingw32\bin`

2. **Install `vcpkg`:**
  * Follow the steps from above.
  
3. **Install `libsodium`:**
  * If you are compiling for 64-bit Windows, execute the following in a terminal:
    ```powershell
    vcpkg install libsodium:x64-mingw-dynamic
    ```
  * If you are compiling for 32-bit Windows, execute the following in a terminal:
    ```powershell
    vcpkg install libsodium:x86-mingw-dynamic libsodium:x86-mingw-static
    ```

## Build Instructions 

1. **Clone the Repository:**
  Run the following command in a PowerShell, Windows Terminal or Bash session:
  ```bash
  git clone https://github.com/Omega493/crypto-utils.git
  ```

2. **Set Up Build Environment:**
  * Select your desired preset [from the list of available presets](#available-presets).
  * If it uses MSVC, open the cloned folder in Visual Studio, select the preset and press Control-Shift-B to build.
  * If it uses `g++`, or is Linux,
    * Configure CMake:
      Open a terminal session in the cloned folder and execute
      ```bash
      cmake --preset <preset_name>
      ```
      Replace `<preset_name>` with your desired preset, ex. `windows-mingw-x86-release`.
      
    * Build:
      Execute
      ```bash
      cmake --build --preset=<preset_name>
      ```
      where `<preset_name>` is the same from the step before.

3. **Locate the Executable:**
  The executable will be in the `binaryDir` specified in the preset:
  * Windows Example: `./build/windows/<preset_name>/encryptor.exe`
  * Linux Example: `./build/linux/<preset_name>/encryptor`

## Usage

The program is a single executable named `encryptor`. It uses flags to determine the operation (encrypt or decrypt) and prompts for a secret key.

1. Get a Secret Key
  * **For Encryption:** Generate a secure 256-bit (32-byte) key. **Do this once per secret and store the key in a safe place like a password manager.**
    ```bash
    openssl rand -hex 32
    ```
  * **For Decryption:** You will need the exact same key that was used to encrypt the file.

2. Run the Program
  * Synopsis: `encryptor <-e <input_file> | -d <input_file>> [-o <output_file>] [-h]`
  * Options:
    * `-e, --encrypt <input_file>`: Specifies the input file to be encrypted
    * `-d, --decrypt <input_file>`: Specifies the input file to be decrypted
    * `-o, --output <output_file>`: (Optional) Specifies the path for the output file (if not provided, the output will be `[base_name].enc` or `[base_name].dec`)
    * `-h, --help`: Show the help message

* Examples:
  * On Windows:
  ```powershell
  # To encrypt a file
  ./build/windows/x64-release/encryptor.exe -e "my_document.txt" -o "my_document.enc"

  # To decrypt a file
  ./build/windows/x64-release/encryptor.exe -d "my_document.enc" -o "my_document.txt"

  # To encrypt using the default output name (e.g., "secret.txt" -> "secret.enc")
  ./build/windows/x64-release/encryptor.exe -e "secret.txt"
  ```
  * On Ubuntu / Debian:
  ```bash
  # To encrypt a file
  ./build/linux/linux-debug/encryptor -e "my_document.txt" -o "my_document.enc"

  # To decrypt a file
  ./build/linux/linux-debug/encryptor -d "my_document.enc" -o "my_document.txt"

  # To encrypt using the default output name (e.g., "secret.txt" -> "secret.enc")
  ./build/linux/linux-debug/encryptor -e "secret.txt"
  ```

* After running the command, you will be prompted:
  ```bash
  Enter the secret key (hex):
  ```
  Just paste or type your key and press Enter.

* On Windows, you can even add the path to the executable to your system environment variables if you wish to call it from anywhere.
  * Copy the path to the build directory which holds the executable (ex. `E:\Programs\crypto-utils\build\windows\<preset_name>\`)
  * Or, you can place the executable to some other directory and copy the path to that directory. **Make sure the executable itself and the required `libsodium.dll` is present in your custom location. Initially they should be present in the build directory.**
  * In both the cases, simply add that location to your system's environment paths.
  * Examples:
    The examples remain almost the same:
    ```powershell
    # To encrypt a file
    encryptor -e "my_document.txt" -o "my_document.enc"

    # To decrypt a file
    encryptor -d "my_document.enc" -o "my_document.txt"

    # To encrypt using the default output name (e.g., "secret.txt" -> "secret.enc")
    encryptor -e "secret.txt"
    ```
    
# Available Presets

## Windows

### MSVC

1. 64-bit OS
  - `windows-msvc-x64-debug`
  - `windows-msvc-x64-release`
2. 32-bit OS
  - `windows-msvc-x86-debug`
  - `windows-msvc-x86-release`

### `g++`

1. 64-bit OS
  - `windows-mingw-x64-debug`
  - `windows-mingw-x64-release`
2. 32-bit OS
  - `windows-mingw-x86-debug`
  - `windows-mingw-x86-release`
  
## Linux

- `linux-debug`
- `linux-release`
