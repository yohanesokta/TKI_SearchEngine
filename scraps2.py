import requests
import os
import csv
import time
from concurrent.futures import ThreadPoolExecutor, as_completed
from threading import Lock

GITHUB_TOKEN = os.getenv("GITHUB_TOKEN")
URL = "https://api.github.com/graphql"

HEADERS = {
    "Authorization": f"Bearer {GITHUB_TOKEN}"
}

lock = Lock()

REPO_QUERY = """
query($cursor: String) {
  organization(login: "microsoft") {
    repositories(
      first: 100,
      after: $cursor,
      privacy: PUBLIC,
      orderBy: {field: STARGAZERS, direction: DESC}
    ) {
      nodes {
        name
        stargazerCount
      }
      pageInfo {
        hasNextPage
        endCursor
      }
    }
  }
}
"""

ISSUE_QUERY = """
query($repo: String!, $cursor: String) {
  repository(owner: "microsoft", name: $repo) {
    issues(first: 100, after: $cursor, states: [OPEN, CLOSED]) {
      nodes {
        id
        title
        body
        url
      }
      pageInfo {
        hasNextPage
        endCursor
      }
    }
  }
}
"""

def graphql(query, variables=None):
    res = requests.post(
        URL,
        json={"query": query, "variables": variables},
        headers=HEADERS
    )

    if res.status_code != 200:
        raise Exception(f"Query failed: {res.status_code} {res.text}")

    return res.json()

def get_top_repos(limit=100):
    repos = []
    cursor = None

    while len(repos) < limit:
        data = graphql(REPO_QUERY, {"cursor": cursor})

        nodes = data["data"]["organization"]["repositories"]["nodes"]
        page = data["data"]["organization"]["repositories"]["pageInfo"]

        for repo in nodes:
            repos.append(repo["name"])

            print(f"[INFO] Repo: {repo['name']} ⭐ {repo['stargazerCount']}")

            if len(repos) >= limit:
                break

        if not page["hasNextPage"]:
            break

        cursor = page["endCursor"]
        time.sleep(0.5)

    return repos






def fetch_issues(repo_name, writer):
    cursor = None
    count = 0

    try:
        while True:
            data = graphql(ISSUE_QUERY, {
                "repo": repo_name,
                "cursor": cursor
            })

            repo_data = data["data"]["repository"]
            if not repo_data:
                break

            issues = repo_data["issues"]["nodes"]
            page = repo_data["issues"]["pageInfo"]

            rows = []
            for issue in issues:
                rows.append([
                    issue["id"],
                    issue["title"],
                    (issue["body"] or "").replace("\n", " "),
                    issue["url"]
                ])

            with lock:
                writer.writerows(rows)

            count += len(rows)

            if not page["hasNextPage"]:
                break

            cursor = page["endCursor"]

        print(f"[DONE] {repo_name}: {count} issues")

    except Exception as e:
        print(f"[ERROR] {repo_name}: {e}")






def main():
    print("[START] Fetch TOP repos...")
    repos = get_top_repos(100)

    print(f"[INFO] Total repos: {len(repos)}")

    with open("microsoft_top100_issues.csv", "w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(["id", "title", "body", "url"])

        print("[START] Fetch issues with threads...")

        with ThreadPoolExecutor(max_workers=10) as executor:
            futures = [
                executor.submit(fetch_issues, repo, writer)
                for repo in repos
            ]

            for future in as_completed(futures):
                pass

    print("[DONE] Saved to microsoft_top100_issues.csv")


if __name__ == "__main__":
    main()