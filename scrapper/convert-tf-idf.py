import re
import sys
import json
import pickle
import math


if len(sys.argv) != 3:
    print("\nUsage: python tfidf_linux.py [data.json] [output.pkl]\n")
    sys.exit(1)

input_data = sys.argv[1]
output_data = sys.argv[2]


with open(input_data, encoding="utf-8") as f:
    content = json.load(f)


with open("stopword.txt", encoding="utf-8") as f:
    sw = [x.strip().lower() for x in f.readlines() if x.strip()]


def clean_str(text):
    text = (text.encode('ascii', 'ignore')).decode("utf-8")
    text = re.sub("&.*?;", "", text)
    text = re.sub(">", "", text)
    text = re.sub("[\]\|\[\@\,\$\%\*\&\\\(\)\":]", "", text)
    text = re.sub("-", " ", text)
    text = re.sub("\.+", "", text)
    text = re.sub("^\s+", "", text)
    text = text.lower()
    return text

df_data = {}
tf_data = {}
idf_data = {}


for data in content:
    tf = {}

    clean_title = clean_str(data['title'])
    list_word = clean_title.split(" ")

    for word in list_word:
        if word in sw or not word:
            continue

        
        if word in tf:
            tf[word] += 1
        else:
            tf[word] = 1

        
        if word in df_data:
            df_data[word] += 1
        else:
            df_data[word] = 1

    tf_data[data['url']] = tf


for word in df_data:
    idf_data[word] = 1 + math.log10(len(tf_data)/df_data[word])


tf_idf = {}

for word in df_data:
    list_doc = []

    for data in content:
        tf_value = tf_data[data['url']].get(word, 0)
        weight = tf_value * idf_data[word]

        doc = {
            'url': data['url'],
            'title': data['title'],
            'image': data['image-url'],
            'description': data['description'],
            'score': weight
        }

        if weight != 0:
            list_doc.append(doc)

    tf_idf[word] = list_doc


with open(output_data, 'wb') as file:
    pickle.dump(tf_idf, file)

print(f"✅ TF-IDF selesai, data disimpan di {output_data}")