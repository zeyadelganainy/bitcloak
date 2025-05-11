# 🕵️‍♂️ Bitcloak

**Bitcloak** is a high-performance steganography tool that embeds and extracts hidden messages inside image files using **Least Significant Bit (LSB)** techniques — accelerated with **parallel computing** via OpenMP (and optionally CUDA/MPI). Designed for speed, modularity, and clean systems-level coding.

---

## 🚀 Features

- 🔐 Hide plaintext messages inside BMP/PNG images
- ⚡ Accelerated embedding/extraction using **OpenMP**
- 🔍 Extract messages with byte-level precision
- 🧪 Includes test suite and benchmarking tools
- 🖥️ Lightweight **CLI** interface (GUI or API optional later)

---

## 📁 Folder Structure

```

bitcloak/
├── src/            # Core embedding & extraction logic
├── include/        # Header files
├── tests/          # Unit tests
├── assets/         # Sample images & messages
├── Makefile        # Build script
└── README.md

```

---

## 🧠 How It Works

Bitcloak hides binary message data inside the **least significant bits** of an image’s pixel data. To improve performance, OpenMP parallelizes the encoding/decoding process across CPU threads.

**Conceptual Flow:**

```

\[Message] + \[Cover Image]
└──► Embed Bits (LSB) ──► \[Stego Image]

\[Stego Image]
└──► Extract Bits (LSB) ──► \[Original Message]

````

---

## ⚙️ Build Instructions

### 🔧 Requirements
- GCC or Clang with OpenMP support
- Make (for building)
- C99 or later

### 🛠️ Build
```bash
make
````

### ▶️ Run

```bash
./bitcloak embed -i cover.bmp -m secret.txt -o stego.bmp
./bitcloak extract -i stego.bmp -o revealed.txt
```

---

## 📊 Performance

> Parallel performance benchmarks will go here.
> Use OpenMP speedup graphs vs serial baseline.

---

## 📌 Roadmap

* [x] Single-threaded baseline
* [x] OpenMP accelerated encoding
* [ ] CUDA backend
* [ ] MPI multi-node support
* [ ] GUI or Web frontend
* [ ] AES encryption for payload

---
## 📦 File Embedding Limits

Bitcloak uses **Least Significant Bit (LSB)** steganography, storing **1 bit of hidden data in each byte of image data**.

### 🔢 How Much Can I Hide?
Every 1 **byte of secret data** requires **8 bytes of image data**.

Formula:
```
Max file size = (image width × image height × 3) ÷ 8
              = total RGB bytes ÷ 8
```

### 📈 Examples:
| Image Size    | RGB Bytes       | Max Hidden Data |
|---------------|------------------|------------------|
| 256×256       | 196,608 bytes     | ~24.5 KB         |
| 512×512       | 786,432 bytes     | ~98.3 KB         |
| 1024×1024     | 3,145,728 bytes   | ~392 KB          |
| 2048×2048     | 12,582,912 bytes  | ~1.57 MB         |

### 🧰 Tips:
- Try compressing large files before embedding (e.g., `.zip` or `.7z`).
- For bigger payloads, use a higher resolution BMP image.
- You can generate test BMPs using Paint, Photoshop, or via script.

> Future versions may support file splitting across multiple images.
---

## 📜 License

MIT

---

## 👨‍💻 Author

**Zeyad Elganainy**

*BSc Computer Science | University of British Columbia*

[GitHub Profile](https://github.com/zeyadelganainy)

