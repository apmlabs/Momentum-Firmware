#!/bin/bash
# Quick release: rebuild only spectrum_check.fap, repackage into existing firmware
# Usage: ./tools/quick-release.sh <new-tag> [base-tag]
# Example: ./tools/quick-release.sh kiisu-mntm-027 kiisu-mntm-026

set -e

NEW_TAG="${1:?Usage: $0 <new-tag> [base-tag]}"
BASE_TAG="${2:-$(git describe --tags --abbrev=0)}"
REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
DIST="$REPO_ROOT/dist/f7-C"
BASE_TGZ="$DIST/flipper-z-f7-update-${BASE_TAG}.tgz"
NEW_TGZ="$DIST/flipper-z-f7-update-${NEW_TAG}.tgz"
FAP="$REPO_ROOT/build/f7-firmware-C/.extapps/spectrum_check.fap"

if [ ! -f "$BASE_TGZ" ]; then
    echo "ERROR: Base firmware not found: $BASE_TGZ"
    echo "Available: $(ls $DIST/flipper-z-f7-update-*.tgz 2>/dev/null | xargs -I{} basename {})"
    exit 1
fi

# 1. Build just the app
echo "=== Building spectrum_check.fap ==="
cd "$REPO_ROOT"
FBT_NO_SYNC=1 ./fbt fap_spectrum_check 2>&1 | tail -3
echo "Built: $(ls -la $FAP | awk '{print $5, "bytes"}')"

# 2. Extract base firmware
WORK=$(mktemp -d)
trap "rm -rf $WORK" EXIT
echo "=== Extracting $BASE_TAG ==="
tar xzf "$BASE_TGZ" -C "$WORK"
UPDDIR="$WORK/f7-update-${BASE_TAG}"

# 3. Extract resources preserving original tar format
RESDIR=$(mktemp -d)
# The original tar uses absolute paths (leading /), extract with strip
tar xzf "$UPDDIR/resources.tar.gz" -C "$RESDIR" 2>/dev/null

# 4. Replace .fap
cp "$FAP" "$RESDIR/apps/Sub-GHz/spectrum_check.fap"
echo "=== Replaced spectrum_check.fap ==="

# 5. Update Manifest — fix MD5 hash and size for the replaced file
NEW_MD5=$(md5sum "$RESDIR/apps/Sub-GHz/spectrum_check.fap" | cut -d' ' -f1)
NEW_SIZE=$(stat -c%s "$RESDIR/apps/Sub-GHz/spectrum_check.fap")
sed -i "s|^F:[a-f0-9]*:[0-9]*:apps/Sub-GHz/spectrum_check.fap|F:${NEW_MD5}:${NEW_SIZE}:apps/Sub-GHz/spectrum_check.fap|" "$RESDIR/Manifest"
echo "=== Updated Manifest: F:${NEW_MD5}:${NEW_SIZE}:apps/Sub-GHz/spectrum_check.fap ==="

# 6. Repack resources preserving original format (absolute paths, no ./ prefix)
cd "$RESDIR"
# Original uses paths like /\nManifest\napps/... (root / then relative paths)
# We need to match this exactly
tar czf "$UPDDIR/resources.tar.gz" --transform='s|^\./|/|' --transform='s|^/\.$|/|' -C "$RESDIR" .
rm -rf "$RESDIR"

# 7. Update version in update.fuf
sed -i "s/^Info: .*/Info: ${NEW_TAG}/" "$UPDDIR/update.fuf"
echo "=== Updated version to $NEW_TAG ==="

# 8. Rename and repack
mv "$UPDDIR" "$WORK/f7-update-${NEW_TAG}"
cd "$WORK"
tar czf "$NEW_TGZ" "f7-update-${NEW_TAG}"
echo "=== Packaged: $(ls -la "$NEW_TGZ" | awk '{print $5, "bytes"}') ==="
echo ""
echo "Ready: $NEW_TGZ"
echo ""
echo "To release:"
echo "  git tag $NEW_TAG && git push origin kiisu-dev $NEW_TAG"
echo "  gh release create $NEW_TAG --repo apmlabs/Momentum-Firmware \\"
echo "    --title \"Title\" --notes \"Notes\" --target kiisu-dev $NEW_TGZ"
