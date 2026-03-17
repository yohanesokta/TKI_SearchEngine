import requests
from bs4 import BeautifulSoup
import json
import time
from concurrent.futures import ThreadPoolExecutor, as_completed

headers = {
    "User-Agent": "Mozilla/5.0"
}

BASE_URL = "https://www.tecmint.com"


def get_links(max_articles=200):
    links = []
    page = 1

    while len(links) < max_articles:
        url = f"{BASE_URL}/page/{page}/"
        print(f"Ambil halaman: {url}")

        res = requests.get(url, headers=headers)
        soup = BeautifulSoup(res.text, "html.parser")

        articles = soup.find_all("h2", class_="entry-title")

        for art in articles:
            a = art.find("a")
            if a:
                links.append(a["href"])

        if not articles:
            break

        page += 1

    return links[:max_articles]


def scrape(url):
    try:
        res = requests.get(url, headers=headers, timeout=10)
        soup = BeautifulSoup(res.text, "html.parser")

        title_tag = soup.find("h1")
        title = title_tag.get_text(strip=True) if title_tag else ""

        p = soup.find("p")
        description = p.get_text(strip=True) if p else ""

        og_img = soup.find("meta", property="og:image")
        image = og_img["content"] if og_img and og_img.has_attr("content") else ""

        return {
            "title": title,
            "description": description,
            "image-url": image,
            "url": url
        }

    except Exception as e:
        print(f"Error scraping {url}: {e}")
        return None


def main():
    max_articles = 10000
    links = get_links(max_articles)
    print(f"Total link: {len(links)}")

    data = []

    
    with ThreadPoolExecutor(max_workers=20) as executor:
        future_to_url = {executor.submit(scrape, url): url for url in links}

        for i, future in enumerate(as_completed(future_to_url), start=1):
            url = future_to_url[future]
            try:
                item = future.result()
                if item:
                    data.append(item)
                    print(f"[{i}/{len(links)}] Scraped: {url}")
            except Exception as e:
                print(f"Error: {url} -> {e}")

    with open("linux_dataset.json", "w", encoding="utf-8") as f:
        json.dump(data, f, indent=4, ensure_ascii=False)

    print(f"✅ DONE! Total artikel: {len(data)} → linux_dataset.json")


if __name__ == "__main__":
    start = time.time()
    main()
    print(f"⏱ Total waktu: {time.time() - start:.2f} detik")