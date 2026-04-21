#!/usr/bin/env python3
"""Check external apps for upstream updates on GitHub.
Usage: python3 tools/check-app-updates.py [--days 30] [--all]
  --days N   Show repos updated in last N days (default: 30)
  --all      Show all repos with dates, not just recent
"""
import json, urllib.request, sys, os
from concurrent.futures import ThreadPoolExecutor, as_completed
from datetime import datetime, timedelta

days = 30
show_all = False
for i, arg in enumerate(sys.argv[1:], 1):
    if arg == "--days" and i < len(sys.argv) - 1: days = int(sys.argv[i + 1])
    if arg == "--all": show_all = True

firmware_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..")
apps_dir = os.path.join(firmware_dir, "applications", "external")

# Get token
token = None
cred_file = os.path.expanduser("~/.git-credentials")
if os.path.exists(cred_file):
    with open(cred_file) as f:
        for line in f:
            if "github.com" in line:
                token = line.strip().split("@")[0].split(":")[-1]
                break
if not token:
    print("ERROR: No GitHub token in ~/.git-credentials"); sys.exit(1)

# Build app→repo mapping
apps = []
for entry in sorted(os.listdir(apps_dir)):
    fam = os.path.join(apps_dir, entry, "application.fam")
    if not os.path.isfile(fam): continue
    with open(fam) as f:
        for line in f:
            if "fap_weburl" in line and not line.strip().startswith("#"):
                url = line.split('"')[1] if '"' in line else ""
                url = url.rstrip("/").removesuffix(".git")
                if "github.com" in url:
                    parts = url.replace("https://github.com/", "").split("/")
                    if len(parts) >= 2:
                        apps.append((entry, f"{parts[0]}/{parts[1]}"))
                break

# Dedupe repos
repo_apps = {}
for name, repo in apps:
    repo_apps.setdefault(repo, []).append(name)

def check_repo(repo):
    try:
        req = urllib.request.Request(
            f"https://api.github.com/repos/{repo}/commits?per_page=1",
            headers={"Authorization": f"token {token}", "Accept": "application/vnd.github.v3+json"})
        with urllib.request.urlopen(req, timeout=10) as resp:
            data = json.loads(resp.read())
            if data:
                return repo, data[0]["commit"]["committer"]["date"][:10], None
    except Exception as e:
        return repo, None, str(e)[:60]
    return repo, None, "empty"

cutoff = datetime.now() - timedelta(days=days)
print(f"Checking {len(repo_apps)} unique repos ({days}-day window)...\n")

results, errors = [], 0
with ThreadPoolExecutor(max_workers=10) as pool:
    futures = {pool.submit(check_repo, r): r for r in repo_apps}
    done = 0
    for future in as_completed(futures):
        done += 1
        if done % 100 == 0: print(f"  ...{done}/{len(repo_apps)}", file=sys.stderr)
        repo, date_str, err = future.result()
        if err: errors += 1; continue
        if date_str:
            dt = datetime.strptime(date_str, "%Y-%m-%d")
            if show_all or dt > cutoff:
                for app_name in repo_apps[repo]:
                    results.append((date_str, app_name, repo))

results.sort(reverse=True)
for date, name, repo in results:
    print(f"{date}  {name:40s}  {repo}")

recent = len([r for r in results if datetime.strptime(r[0], "%Y-%m-%d") > cutoff])
print(f"\nTotal: {len(results)} apps shown, {recent} updated in last {days}d")
print(f"Checked: {len(repo_apps)} repos, Errors: {errors}")
