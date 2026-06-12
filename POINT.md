# Materi Presentasi: Hybrid Search Engine C++

Dokumen ini berisi poin-poin utama untuk pembuatan slide PowerPoint proyek Hybrid Search Engine.

---

## 1. Penyiapan & Instalasi Proyek
**Slide: Persiapan Lingkungan Pengembangan**
- **Persyaratan Sistem:**
    - C++ Compiler (GCC/Clang) dengan dukungan C++17.
    - CMake 3.10 ke atas.
    - Library Eksternal (Header-only): `httplib.h` (HTTP) dan `json.hpp` (JSON processing).
- **Langkah Instalasi:**
    1. Clone/Download source code.
    2. Struktur folder modular: `include/`, `src/`, `templates/`.
    3. Proses build menggunakan CMake:
       ```bash
       mkdir build && cd build
       cmake ..
       make
       ```
    4. Menjalankan server: `./hybrid_server`

---

## 2. Deskripsi Dataset
**Slide: PubMed 200k Abstracts Dataset**
- **Asal Dataset:** [Kaggle - 200,000 Abstracts for Sequential Sentence Classification](https://www.kaggle.com/datasets/anshulmehtakaggl/200000-abstracts-for-seq-sentence-classification)
- **Karakteristik:**
    - Dataset terdiri dari ~200.000 kalimat dari abstrak medis PubMed.
    - Setiap baris memiliki label (BACKGROUND, METHODS, RESULTS, CONCLUSIONS, OBJECTIVE).
- **Contoh Baris Dataset:**
    ```text
    ###24491034
    BACKGROUND      The emergence of HIV as a chronic condition means...
    METHODS         This study is designed as a randomised controlled trial...
    RESULTS         Both groups showed improvement in symptoms...
    CONCLUSIONS     Results of the Positive Outlook study will provide...
    ```

---

## 3. Metodologi: Hybrid Search
**Slide: Arsitektur Sistem & Algoritma**
- **Metode 1: BM25 (Best Matching 25)**
    - Berbasis frekuensi kata (TF) dan invers frekuensi dokumen (IDF).
    - Efektif untuk pencarian kata kunci eksak.
- **Metode 2: Vector Search (TF-IDF + Cosine Similarity)**
    - Merepresentasikan dokumen sebagai vektor dalam ruang multidimensi.
    - Mengukur kedekatan semantik melalui sudut antar vektor.
- **Fusion: Reciprocal Rank Fusion (RRF)**
    - Menggabungkan peringkat dari kedua metode untuk menghasilkan urutan hasil yang lebih relevan dan stabil.

**Capture Code: Proses Hybrid Fusion (RRF)**
```cpp
// Cuplikan logik penggabungan peringkat di src/hybrid_searcher.cpp
for (size_t i = 0; i < bm25_results.size(); ++i) {
    rrf_scores[bm25_results[i].first] += 1.0 / (60.0 + i + 1);
}
for (size_t i = 0; i < vector_results.size(); ++i) {
    rrf_scores[vector_results[i].first] += 1.0 / (60.0 + i + 1);
}
```

---

## 4. Tampilan Antarmuka (UI)
**Slide: Antarmuka Web Modern & Responsif**
- **Fitur UI:**
    - Search bar interaktif dengan auto-focus.
    - Pengaturan parameter `Top-K` secara dinamis.
    - Kartu hasil pencarian dengan highlight ID Dokumen dan Skor Kesamaan.
    - Desain bersih menggunakan font "Inter" dan palet warna profesional.

**Panduan Capture UI (Silakan ambil screenshot pada browser):**
1. **Tampilan Awal:** Capture halaman [http://localhost:8080](http://localhost:8080) sebelum melakukan pencarian (Empty State).
2. **Pencarian 1 (Kata Kunci: "HIV self-management"):**
   - Menampilkan hasil terkait manajemen mandiri pasien HIV.
3. **Pencarian 2 (Kata Kunci: "Prednisolone treatment"):**
   - Menampilkan hasil terkait efektivitas pengobatan steroid.
4. **Pencarian 3 (Kata Kunci: "Randomized controlled trial"):**
   - Menampilkan hasil yang berfokus pada metodologi penelitian klinis.

---

## 5. Fitur Tambahan & Kesimpulan
**Slide: Keunggulan Proyek**
- **Web-Ready:** Server C++ langsung melayani file HTML/CSS/JS tanpa server tambahan (Nginx/Apache).
- **Real-time Scoring:** Nilai kesamaan (similarity) dihitung secara instan dan ditampilkan dalam persentase.
- **Scalability:** Struktur modular memudahkan penambahan algoritma pencarian baru (misal: BERT embeddings).
- **User Experience:** Dilengkapi animasi loading dan penanganan error yang user-friendly.
