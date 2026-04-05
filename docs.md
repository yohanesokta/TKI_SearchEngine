# Dokumentasi Proyek Search Engine BM25

Proyek ini mengimplementasikan mesin pencari berbasis algoritma **BM25 (Best Matching 25)** menggunakan bahasa pemrograman Go. Sistem ini dirancang untuk melakukan perangkingan dokumen berdasarkan relevansi terhadap query pengguna dari dataset GitHub Microsoft Issues.

## 1. Tahapan Proses Sistem

Sistem ini mengikuti alur kerja *Information Retrieval* standar yang terdiri dari lima tahap utama:

### A. Tahap 0: Penanganan Argumen (Input Query)
Sistem mengambil input pencarian langsung dari baris perintah saat aplikasi dijalankan.
- **Implementasi**: Menggunakan `os.Args` untuk menangkap string yang dimasukkan pengguna.
- **Contoh**: `go run main.go "pwa offline"` akan menangkap `"pwa offline"` sebagai query pencarian.

### B. Tahap 1: Konfigurasi Dataset
Dataset didefinisikan melalui struct `Config`. Ini memungkinkan sistem untuk beradaptasi dengan file CSV lain tanpa mengubah logika inti.
- **Parameter**: Nama file, limit dokumen (untuk efisiensi), dan indeks kolom (ID, Title, Body).

### C. Tahap 2: Penyiapan Data & Preprocessing
Data dibaca secara *streaming* untuk menjaga efisiensi memori.
- **Fungsi `Preprocess`**: Melakukan transformasi teks untuk meningkatkan akurasi:
  1. **Case Folding**: Mengubah semua teks menjadi huruf kecil (*lowercase*).
  2. **Filtering**: Menggunakan Regular Expression `[^a-z0-9]+` untuk membuang simbol dan tanda baca.
  3. **Normalisasi**: Menghapus spasi berlebih.
- **Penyimpanan**: Data asli disimpan dalam `docsStore` untuk tampilan hasil, sedangkan data terproses disimpan dalam `indexData` untuk mesin pencari.

### D. Tahap 3: Penggunaan BM25 (Indexing)
Menggunakan library `gokapi` untuk perhitungan skor BM25.
- **Proses**: Sistem membangun *Inversed Index* dari data terproses.
- **Reset**: Fungsi `retriever.Reset()` dipanggil setiap kali dijalankan untuk memastikan index bersih dari data demo sebelumnya.

### E. Tahap 4: Keluaran Hasil (Output)
Menampilkan 5 dokumen paling relevan.
- **Skor BM25**: Menunjukkan tingkat relevansi (semakin tinggi semakin baik).
- **Metadata**: Menampilkan Judul asli dan ringkasan (snippet) isi dokumen agar mudah dibaca pengguna.

---

## 2. Dokumentasi Fungsi Utama

### `func Preprocess(text string) string`
Fungsi ini adalah inti dari tahap prapemrosesan. Tanpa fungsi ini, kata seperti "Apple." dan "apple" akan dianggap berbeda, yang akan menurunkan kualitas hasil pencarian.

### `func getUnexportedField(field reflect.Value) interface{}`
Karena library `gokapi` menyembunyikan field `id` dan `score` (tidak diekspor), fungsi ini menggunakan teknik **Refleksi** dan **Unsafe Pointer** untuk membaca alamat memori field tersebut. Ini memungkinkan kita menampilkan metadata yang tepat dari `docsStore`.

### `func main()`
Fungsi utama yang mengoordinasikan seluruh alur kerja, mulai dari membaca argumen baris perintah hingga menampilkan hasil akhir di terminal.

---

## 3. Panduan Modifikasi Dataset

Kode ini dirancang agar fleksibel. Jika Anda ingin menggunakan dataset khusus lainnya, Anda hanya perlu mengubah blok `Config` di dalam `main.go`:

```go
cfg := Config{
    FilePath:    "nama_file_anda.csv",
    Limit:       5000,          // Ubah batas dokumen
    IDColumn:    0,             // Indeks kolom ID di CSV baru
    TitleColumn: 1,             // Indeks kolom Judul di CSV baru
    BodyColumn:  2,             // Indeks kolom Isi di CSV baru
}
```

Pastikan dataset baru dalam format CSV dengan pemisah koma.
