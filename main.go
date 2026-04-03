package main

import (
	"fmt"

	"github.com/raphaelsty/gokapi"
)

func main() {
	data := make(map[string]string)
	data["document_0"] = "Paris is the capital of France"
	data["document_1"] = "Montreal is the capital of Canada"
	data["document_2"] = "Madrid is the capital of Spain"
	data["document_3"] = "Rome is the capital of Italy"

	retriver := gokapi.BM25("index")
	retriver.Add(data)

	answers := retriver.Query("Paris France Canada", 5)

	for _, item := range answers {
		fmt.Println(item)
	}
	retriver.Reset()
}
