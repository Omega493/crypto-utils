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

These instructions are based on the CMakePresets.json file, which is configured for the Visual Studio C++ (MSVC) compiler.

1. **Install Visual Studio:**
  * Download and install "Build Tools for Visual Studio" or "Visual Studio Community" from the [Visual Studio website](https://visualstudio.microsoft.com/downloads/).
  * During installation, select the "Desktop development with C++" workload. This will install the `cl.exe` compiler and CMake.

2. **Install vcpkg:**
  * Choose a directory to install `vcpkg`, such as `C:\dev\vcpkg`. Avoid paths with spaces like `Program Files`.
  * Using Windows Terminal, clone and bootstrap `vcpkg`:
  ```powershell
  git clone https://github.com/microsoft/vcpkg.git
  cd vcpkg
  .\bootstrap-vcpkg.bat
  ```
3. **Install `libsodium`:**
  Install `libsodium` using the `x64-windows` triplet:
  ```powershell
  .\vcpkg.exe install libsodium:x64-windows
  ```

4. **Set Environment Variable:**
  The CMake Presets requires an environment variable `CMAKE_TOOLCHAIN_FILE` to be set. You must set this variable to point to your `vcpkg.cmake` script.
  In your system environment variables, create a new variable:
  * Variable name: `CMAKE_TOOLCHAIN_FILE`
  * Variable value: `[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake`
  Restart your terminal session for the new environment variable to take effect.

## Build Instructions 

1. **Clone the Repository:**
  Run the following command in a PowerShell or Windows Terminal session:
  ```powershell
  git clone https://github.com/Omega493/crypto-utils.git
  ```

2. **Set Up Build Environment:**
  All subsequent commands must be run from a `Developer Command Prompt for VS 2022`. This ensures that the C++ compiler (`cl.exe`), CMake, and Ninja are all available in your terminal's PATH. You can find this in your Start Menu (e.g., "x64 Native Tools Command Prompt for VS 2022").

3. **Configure with CMake:**
  In the developer command prompt, head to the your `crypto-utils` directory. Run CMake using a preset to generate the build files. This will automatically select the generator (Ninja) and create the correct build directory as defined in `CMakePresets.json`.
  Choose one of the following presets to configure:
  * **On Windows:**
    * To Configure for Debug:
      ```powershell
      cmake --preset x64-debug
      ```
    * To Configure for Release:
      ```powershell
      cmake --preset x64-release
      ```
    
  * **On Ubuntu / Debian:**
    * To Configure for Debug:
      ```bash
      cmake --preset linux-debug
      ```
    * To Configure for Release:
      ```bash
      cmake --preset linux-release
      ```

4. **Build the Project**
  Compile the project using the build preset that matches the configuration you just set up.
  * **On Windows:**
    * If you configured for Debug:
      ```powershell
      cmake --build --preset x64-debug
      ```
    * If you configured for Release:
      ```powershell
      cmake --build --preset x64-release
      ```
   
  * **On Ubuntu / Debian:**
    * If you configured for Debug:
      ```bash
      cmake --build --preset linux-debug
      ```
    * To Configure for Release:
      ```bash
      cmake --build --preset linux-release
      ```

5. **Locate the Executable:**
  The executable will be in the `binaryDir` specified in the preset:
  * Windows Example: `build/windows/x64-release/encryptor.exe`
  * Linux Example: `build/linux/linux-debug/encryptor`

## Usage

The program is a single executable named `encryptor`. It uses flags to determine the operation (encrypt or decrypt) and prompts for a secret key.

### 1\. Get a Secret Key

  * **For Encryption:** Generate a secure 256-bit (32-byte) key. **Do this once per secret and store the key in a safe place like a password manager.**
    ```bash
    openssl rand -hex 32
    ```
    This will output a long hexadecimal string. This is your secret key. Keep it safe!

  * **For Decryption:** You will need the exact same key that was used to encrypt the file.

### 2\. Run the Program

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
  Paste or type your key and press **Enter**. The input will be hidden for security. The program will then produce the output file.

* On Windows, you can even add the path to the executable to your system environment variables if you wish to call it from anywhere.
  * Copy the path to the build directory which holds the executable (ex. `E:\Programs\crypto-utils\build\windows\x64-release\`)
  * Or, you can place the executable to some other directory and copy the path to that directory. **Make sure the executable itself and `libsodium.dll` is present in your custom location. Initially they should be present in the build directory.**
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