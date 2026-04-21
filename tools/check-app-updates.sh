#!/bin/bash
# Check external apps for updates against their source GitHub repos
# Usage: ./tools/check-app-updates.sh [--stale-days 30]

STALE_DAYS="${2:-90}"
FIRMWARE_DIR="$(cd "$(dirname "$0")/.." && pwd)"
APPS_DIR="$FIRMWARE_DIR/applications/external"
TOKEN=$(grep github.com ~/.git-credentials 2>/dev/null | sed 's|.*://||;s|@.*||;s|.*:||')

if [ -z "$TOKEN" ]; then
    echo "ERROR: No GitHub token found in ~/.git-credentials"
    exit 1
fi

echo "Checking apps for updates (stale = older than ${STALE_DAYS} days)..."
echo ""

TOTAL=0
STALE=0
ERRORS=0
UPDATED=0

for app_dir in "$APPS_DIR"/*/; do
    fam="$app_dir/application.fam"
    [ -f "$fam" ] || continue
    
    # Extract fap_weburl
    url=$(grep -m1 'fap_weburl' "$fam" 2>/dev/null | grep -v '^\s*#' | sed 's/.*fap_weburl.*=.*"\(.*\)".*/\1/' | sed 's|\.git$||')
    [ -z "$url" ] && continue
    
    # Only GitHub repos
    echo "$url" | grep -q "github.com" || continue
    
    # Extract owner/repo
    owner_repo=$(echo "$url" | sed 's|https://github.com/||' | cut -d'/' -f1,2)
    [ -z "$owner_repo" ] && continue
    
    TOTAL=$((TOTAL + 1))
    
    # Get latest commit date from default branch
    result=$(curl -sf -H "Authorization: token $TOKEN" \
        -H "Accept: application/vnd.github.v3+json" \
        "https://api.github.com/repos/$owner_repo/commits?per_page=1" 2>/dev/null)
    
    if [ $? -ne 0 ] || [ -z "$result" ]; then
        ERRORS=$((ERRORS + 1))
        continue
    fi
    
    # Extract date
    last_commit=$(echo "$result" | python3 -c "import sys,json; d=json.load(sys.stdin); print(d[0]['commit']['committer']['date'][:10] if d else '')" 2>/dev/null)
    [ -z "$last_commit" ] && { ERRORS=$((ERRORS + 1)); continue; }
    
    # Compare with stale threshold
    last_epoch=$(date -d "$last_commit" +%s 2>/dev/null)
    threshold_epoch=$(date -d "-${STALE_DAYS} days" +%s)
    
    app_name=$(basename "$app_dir")
    
    if [ "$last_epoch" -gt "$threshold_epoch" ] 2>/dev/null; then
        UPDATED=$((UPDATED + 1))
        echo "UPDATE  $app_name  ($last_commit)  $owner_repo"
    fi
    
    # Rate limit: ~1 req/sec to stay under 5000/hr
    sleep 0.8
done

echo ""
echo "=== Summary ==="
echo "Checked: $TOTAL apps"
echo "Recently updated (< ${STALE_DAYS}d): $UPDATED"
echo "Errors/404s: $ERRORS"
