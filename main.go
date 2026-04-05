package main

import (
	"encoding/csv"
	"fmt"
	"io"
	"log"
	"os"
	"reflect"
	"regexp"
	"strconv"
	"strings"
	"unsafe"

	"github.com/raphaelsty/gokapi"
)

type DocumentInfo struct {
	ID    string
	Title string
	Body  string
}

type Config struct {
	FilePath    string
	Limit       int
	IDColumn    int
	TitleColumn int
	BodyColumn  int
}

func Preprocess(text string) string {

	text = strings.ToLower(text)

	reg, _ := regexp.Compile("[^a-z0-9]+")
	text = reg.ReplaceAllString(text, " ")

	text = strings.TrimSpace(text)
	return text
}

func getUnexportedField(field reflect.Value) interface{} {
	return reflect.NewAt(field.Type(), unsafe.Pointer(field.UnsafeAddr())).Elem().Interface()
}

func main() {

	if len(os.Args) < 3 {
		fmt.Println("Usage: go run main.go \"your query here\" \"max_data\"")
		return
	}
	rawQuery := strings.Join(os.Args[1:], " ")
	maxData, err := strconv.Atoi(os.Args[len(os.Args)-1])
	if err != nil {
		log.Fatalf("Error: Gagal mengkonversi max_data ke integer: %v", err)
	}

	cfg := Config{
		FilePath:    "microsoft_issues.csv",
		Limit:       1000,
		IDColumn:    0,
		TitleColumn: 1,
		BodyColumn:  2,
	}

	file, err := os.Open(cfg.FilePath)
	if err != nil {
		log.Fatalf("Error: Gagal membuka file CSV: %v", err)
	}
	defer file.Close()

	reader := csv.NewReader(file)
	_, _ = reader.Read()

	indexData := make(map[string]string)
	docsStore := make(map[string]DocumentInfo)

	fmt.Printf("Memproses %d dokumen dari %s...\n", cfg.Limit, cfg.FilePath)

	count := 0
	for count < cfg.Limit {
		record, err := reader.Read()
		if err == io.EOF {
			break
		}
		if err != nil {
			log.Printf("Warning: Gagal membaca baris ke-%d: %v", count, err)
			continue
		}

		if len(record) <= cfg.BodyColumn {
			continue
		}

		id := record[cfg.IDColumn]
		title := record[cfg.TitleColumn]
		body := record[cfg.BodyColumn]

		docsStore[id] = DocumentInfo{ID: id, Title: title, Body: body}

		combinedText := title + " " + body
		indexData[id] = Preprocess(combinedText)

		count++
	}

	retriever := gokapi.BM25("index_storage")
	retriever.Reset()

	fmt.Println("Melakukan indexing BM25...")
	retriever.Add(indexData)

	processedQuery := Preprocess(rawQuery)
	fmt.Printf("\nQuery Asli: \"%s\"\n", rawQuery)
	fmt.Printf("Query Terproses: \"%s\"\n", processedQuery)

	answers := retriever.Query(processedQuery, maxData)

	fmt.Println("\n=== TOP 5 HASIL PENCARIAN BM25 ===")
	fmt.Println("--------------------------------------------------")
	if len(answers) == 0 {
		fmt.Println("Tidak ada hasil yang ditemukan.")
	}

	for i := range answers {
		item := answers[i]

		v := reflect.ValueOf(&item).Elem()
		idField := v.FieldByName("id")
		scoreField := v.FieldByName("score")

		id := getUnexportedField(idField).(string)
		score := getUnexportedField(scoreField).(float32)

		if doc, ok := docsStore[id]; ok {
			fmt.Printf("%d. [Skor: %.4f] %s\n", i+1, score, doc.Title)
			snippet := doc.Body

			snippet = strings.ReplaceAll(snippet, "\n", " ")
			if len(snippet) > 120 {
				snippet = snippet[:117] + "..."
			}
			fmt.Printf("   Ringkasan: %s\n", snippet)
			fmt.Println("--------------------------------------------------")
		}
	}
}
