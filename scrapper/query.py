import sys
import json
import pickle


if len(sys.argv) != 4:
    print("\n\nPenggunaan:\n\tpython query_linux.py [index.pkl] [n] [query]..\n")
    sys.exit(1)

index_file = sys.argv[1]
n = int(sys.argv[2])
query = sys.argv[3].split(" ")


with open(index_file, 'rb') as f:
    index = pickle.load(f)


results = {}
for q in query:
    q = q.lower()  
    try:
        for doc in index[q]:
            if doc['url'] in results:
                results[doc['url']]['score'] += doc['score']
            else:
                results[doc['url']] = doc
    except KeyError:
        continue


list_data = list(results.values())


list_data_sorted = sorted(list_data, key=lambda x: x['score'], reverse=True)


for count, data in enumerate(list_data_sorted, start=1):
    print(json.dumps(data, ensure_ascii=False))
    if count == n:
        break