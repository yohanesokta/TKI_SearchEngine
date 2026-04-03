# TKI Search Engine (BM25 Implementation)

Proyek ini adalah implementasi mesin pencari sederhana menggunakan algoritma **BM25 (Best Matching 25)** di bahasa pemrograman Go. Mesin pencari ini memanfaatkan library `gokapi` untuk melakukan pengindeksan dokumen dan perankingan berdasarkan relevansi kueri.

##  Fitur
- **BM25 Ranking:** Algoritma perankingan dokumen yang lebih akurat dibandingkan TF-IDF standar.
- **Persistent Indexing:** Menyimpan indeks pencarian ke dalam direktori lokal (`index/`).
- **Simple API:** Mudah digunakan untuk menambahkan dokumen dan melakukan kueri.

---

##  Instalasi

Pastikan Anda telah menginstal [Go (Golang)](https://go.dev/doc/install) versi 1.20 ke atas.

1. **Clone repository** (atau masuk ke direktori proyek):
   ```bash
   cd TKI_SearchEngine
   ```

2. **Unduh dependensi:**
   Gunakan perintah `go mod tidy` untuk mengunduh library `gokapi` dan modul pendukung lainnya:
   ```bash
   go mod tidy
   ```

---

## 💻 Cara Penggunaan

Untuk menjalankan contoh pencarian yang ada di `main.go`, jalankan perintah berikut:

```bash
go run main.go
```

### Contoh Kode (`main.go`)
```go
package main

import (
	"fmt"
	"github.com/raphaelsty/gokapi"
)

func main() {
	// 1. Inisialisasi data dokumen
	data := make(map[string]string)
	data["doc_0"] = "Paris is the capital of France"
	data["doc_1"] = "Montreal is the capital of Canada"
	
	// 2. Inisialisasi BM25 dengan folder penyimpanan "index"
	retriver := gokapi.BM25("index")
	
	// 3. Menambahkan dokumen ke indeks
	retriver.Add(data)

	// 4. Melakukan pencarian (Top 5 hasil)
	answers := retriver.Query("Paris France", 5)

	for _, item := range answers {
		fmt.Println(item)
	}

	// 5. Reset indeks (opsional)
	retriver.Reset()
}
```

---

##  Penjelasan Algoritma BM25

**BM25 (Best Matching 25)** adalah fungsi peringkat (ranking function) yang digunakan oleh mesin pencari untuk memperkirakan relevansi dokumen terhadap kueri pencarian yang diberikan. BM25 dianggap sebagai standar industri untuk pencarian berbasis teks (Information Retrieval).

### Komponen Utama BM25:

1.  **Term Frequency (TF) dengan Saturation:**
    Berbeda dengan TF biasa yang terus meningkat linear, pada BM25, pengaruh kemunculan kata yang sama berulang kali dalam satu dokumen akan "jenuh" (saturated). Artinya, jika kata "Paris" muncul 100 kali, skornya tidak akan 100x lipat lebih tinggi dari dokumen yang mengandung kata "Paris" 10 kali. Ini mencegah spam kata kunci.

2.  **Inverse Document Frequency (IDF):**
    Memberikan bobot lebih tinggi pada kata-kata yang "langka" di seluruh koleksi dokumen. Kata umum seperti "is", "the", "of" akan memiliki skor IDF yang sangat rendah, sedangkan kata spesifik seperti "Montreal" atau "Paris" akan memiliki skor tinggi.

3.  **Document Length Normalization:**
    BM25 memberikan penalti pada dokumen yang terlalu panjang. Jika sebuah kata ditemukan di dokumen pendek, itu dianggap lebih relevan daripada kata yang sama ditemukan di dokumen yang sangat panjang (di mana kata tersebut mungkin hanya muncul secara kebetulan).

### Rumus Sederhana:
Skor untuk sebuah dokumen $D$ terhadap kueri $Q$ dihitung sebagai:
$$Score(D, Q) = \sum_{q \in Q} IDF(q) \cdot \frac{f(q, D) \cdot (k_1 + 1)}{f(q, D) + k_1 \cdot (1 - b + b \cdot \frac{|D|}{avgdl})}$$

*   $f(q, D)$: Frekuensi kata $q$ dalam dokumen $D$.
*   $|D|$: Panjang dokumen.
*   $avgdl$: Rata-rata panjang semua dokumen dalam koleksi.
*   $k_1$ & $b$: Parameter konstanta (biasanya $k_1=1.2$ dan $b=0.75$).

---

## 📂 Struktur Proyek
- `main.go`: Logika utama aplikasi.
- `index/`: Folder otomatis yang dibuat untuk menyimpan database indeks.
- `go.mod`: Definisi modul dan dependensi.
