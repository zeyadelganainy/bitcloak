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

## 📜 License

MIT

---

## 👨‍💻 Author

**Zeyad Elganainy**

*BSc Computer Science | University of British Columbia*

[GitHub Profile](https://github.com/zeyadelganainy)

