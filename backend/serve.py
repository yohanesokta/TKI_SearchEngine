"""
Aplikasi Search Engine Sederhana Berbasis TF-IDF
------------------------------------------------
Nama      : Yohanes Oktanio
GitHub    : yohanes okta
Mata Kuliah : Temu Kembali Informasi
Tahun     : 2026

Deskripsi:
Program ini merupakan web service sederhana menggunakan Flask
yang berfungsi untuk melakukan pencarian dokumen berdasarkan
query pengguna menggunakan metode TF-IDF.

Data index disimpan dalam file pickle (books-tfidf.raw)
yang berisi struktur inverted index dengan skor TF-IDF.

Menghapus/Mengubah ini = Melukai ((😡
"""


from flask import Flask,request
import json
import pickle
from flask_cors import CORS

app = Flask(__name__)
CORS(app)
def query(nQuery,Query):
   
    argv = [False,"./books-tfidf.raw",nQuery,Query]
    if len(argv) != 4 :
        print ("\n\nPenggunaan\n\tquery.py [index] [n] [query]..\n")
        exit(1)

    query = argv[3].split(" ")
    n = int(argv[2])

    with open(argv[1], 'rb') as indexdb:
        indexFile = pickle.load(indexdb)

    list_doc = {}
    for q in query:
        try :
            for doc in indexFile[q]:
                if doc['url'] in list_doc :
                    list_doc[doc['url']]['score'] += doc['score']
                else :
                    list_doc[doc['url']] = doc
        except :
            continue


    list_data=[]
    for data in list_doc :
        list_data.append(list_doc[data])


    count=1
    y = []
    for data in sorted(list_data, key=lambda k: k['score'], reverse=True):
        y.append(data)
        if (count == n) :
            break
        count+=1
    return y

@app.route("/search")
def hello_world():
    nQuery = request.args.get('n', default='1', type=str)
    Query = request.args.get('query', default='', type=str)
    return query(nQuery,Query)

if __name__ == "__main__":
    app.run(debug=True, port=5000)

