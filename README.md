
# BitCloak

BitCloak is a high-performance, systems-level steganography tool for securely embedding and extracting messages or files inside **PNG** images using **Least Significant Bit (LSB)** encoding, with optional **AES-256** or **XOR** encryption.

---

## 🚀 Features

- ✅ Embed and extract **text** or **files** in 24-bit **RGB PNG images**
- ✅ Full **AES-256 encryption** support with passphrase
- ✅ Optional lightweight **XOR cipher** (`--use-xor`)
- ✅ Automatically restores original filename on extraction
- ✅ Support for binary files (`.zip`, `.pdf`, etc.)
- ✅ CLI-first design, portable C implementation
- ✅ RGB format using [lodepng](https://github.com/lvandeve/lodepng)

---

## 📦 Installation

### 1. Clone & Build

```bash
git clone https://github.com/yourusername/bitcloak.git
cd bitcloak
make
```

### 2. Requirements

- C compiler (GCC/MinGW or Clang)
- `make`
- PNG images (24-bit RGB)

---

## 🔧 Usage

### ➕ Embed Message

```bash
bitcloak embed -i input.png -m "Secret Message" -o stego.png
```

### ➕ Embed File (AES-256)

```bash
bitcloak embedfile -i input.png -f secret.pdf -o stego.png --key hunter2
```

### ➕ Embed File (XOR)

```bash
bitcloak embedfile -i input.png -f notes.txt -o stego.png --key password --use-xor
```

### 🔍 Extract Message

```bash
bitcloak extract -i stego.png -o output.txt
```

### 🔍 Extract File (restores original name unless `-o` given)

```bash
bitcloak extractfile -i stego.png -o - --key hunter2
```

---

## 📊 PNG Capacity Guide

BitCloak stores data in **LSBs of RGB pixels**.

| Image Resolution | Pixels     | Max Payload (Bytes) | Approx File Size |
|------------------|------------|----------------------|------------------|
| 256 × 256        | 65,536     | ~24.5 KB             | Small files/messages
| 512 × 512        | 262,144    | ~98.5 KB             | Short documents
| 1024 × 1024      | 1,048,576  | ~393 KB              | PDFs, code, etc.
| 1920 × 1080      | 2,073,600  | ~777 KB              | Medium-sized files
| 3840 × 2160      | 8,294,400  | ~3 MB                | Archive files, binaries

📌 *Max payload = floor((W × H × 3) / 8) - header bytes*

Header includes:
- 1 byte: filename length
- N bytes: filename
- 8 bytes: file size
- Payload bytes

---

## 🛡️ Encryption Notes

- **AES-256-CBC**: High security, passphrase-based key
- **XOR cipher**: Lightweight alternative, for educational/demo use
- Uses **PKCS#7 padding**
- Key is directly derived from the passphrase (can be upgraded to PBKDF2)

---

## 🧪 Coming Soon

- Performance benchmarks (OpenMP parallel encoding)
- GUI application
- Multi-payload steganography
- Unit tests and automated test suite

---

## 📁 Folder Structure

```
bitcloak/
├── include/        # Header files
│   ├── aes.h
│   ├── crypto.h
│   ├── embed.h
│   ├── extract.h
│   ├── image_io.h
│   └── utils.h
├── src/            # C source files
│   ├── aes.c
│   ├── crypto.c
│   ├── embed.c
│   ├── extract.c
│   ├── image_io.c
│   ├── lodepng.c
│   └── utils.c
├── assets/         # Cover PNGs and test files
├── Makefile
└── README.md
```

---

## 🧠 Author

Created by Zeyad Elganainy, for portfolio and systems-level research purposes.

---

## 📜 License

MIT License.
