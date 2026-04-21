#!/bin/bash
# Quick release: rebuild only spectrum_check.fap, repackage into existing firmware
# Usage: ./tools/quick-release.sh <new-tag> [base-tag]
# Example: ./tools/quick-release.sh kiisu-mntm-029 kiisu-mntm-028

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
    echo "Available:"; ls "$DIST"/flipper-z-f7-update-*.tgz 2>/dev/null | xargs -I{} basename {}
    exit 1
fi

# 1. Build just the app
echo "=== Building spectrum_check.fap ==="
cd "$REPO_ROOT"
FBT_NO_SYNC=1 ./fbt fap_spectrum_check 2>&1 | tail -3
echo "Built: $(stat -c%s "$FAP") bytes"

# 2. Extract base firmware
WORK=$(mktemp -d)
trap "rm -rf $WORK" EXIT
echo "=== Extracting $BASE_TAG ==="
tar xzf "$BASE_TGZ" -C "$WORK"
UPDDIR="$WORK/f7-update-${BASE_TAG}"

# 3. Extract resources, replace fap, update manifest, repack
# Using Python to match the exact tar format the build system uses (USTAR, arcname="", sanitized)
echo "=== Replacing spectrum_check.fap and repacking ==="
python3 -c "
import tarfile, gzip, io, hashlib, os, sys

res_gz = '$UPDDIR/resources.tar.gz'
fap_src = '$FAP'
fap_path = 'apps/Sub-GHz/spectrum_check.fap'

# Read new fap
with open(fap_src, 'rb') as f:
    fap_data = f.read()
fap_md5 = hashlib.md5(fap_data).hexdigest()
fap_size = len(fap_data)

# Open original tar
with gzip.open(res_gz, 'rb') as gz:
    raw = gz.read()

inp = tarfile.open(fileobj=io.BytesIO(raw), mode='r:')

# Build new tar in memory
out_buf = io.BytesIO()
out = tarfile.open(fileobj=out_buf, mode='w:', format=tarfile.USTAR_FORMAT)

manifest_updated = False
for member in inp:
    if member.name == fap_path:
        # Replace the fap
        member.size = fap_size
        member.mtime = 0
        member.uid = member.gid = 0
        member.uname = member.gname = 'furippa'
        member.mode = 0o644
        out.addfile(member, io.BytesIO(fap_data))
        print(f'  Replaced: {fap_path} ({fap_size} bytes, md5={fap_md5})')
    elif member.name == 'Manifest':
        # Update manifest
        data = inp.extractfile(member).read().decode()
        old_line = None
        for line in data.split('\n'):
            if fap_path in line:
                old_line = line
                break
        if old_line:
            new_line = f'F:{fap_md5}:{fap_size}:{fap_path}'
            data = data.replace(old_line, new_line)
            print(f'  Manifest: {old_line} -> {new_line}')
        raw_manifest = data.encode()
        member.size = len(raw_manifest)
        member.mtime = 0
        member.uid = member.gid = 0
        member.uname = member.gname = 'furippa'
        member.mode = 0o644
        out.addfile(member, io.BytesIO(raw_manifest))
    else:
        # Copy as-is
        if member.isfile():
            out.addfile(member, inp.extractfile(member))
        else:
            out.addfile(member)

out.close()
inp.close()

# Gzip with mtime=0 to match build system
out_buf.seek(0)
compressed = gzip.compress(out_buf.read(), compresslevel=9, mtime=0)
with open(res_gz, 'wb') as f:
    f.write(compressed)
print(f'  resources.tar.gz: {len(compressed)} bytes')
"

# 4. Update version in update.fuf
sed -i "s/^Info: .*/Info: ${NEW_TAG}/" "$UPDDIR/update.fuf"
echo "=== Updated version to $NEW_TAG ==="

# 5. Rename and repack
mv "$UPDDIR" "$WORK/f7-update-${NEW_TAG}"
cd "$WORK"
tar czf "$NEW_TGZ" "f7-update-${NEW_TAG}"
echo "=== Packaged: $(stat -c%s "$NEW_TGZ") bytes ==="
echo ""
echo "Ready: $NEW_TGZ"
echo ""
echo "To release:"
echo "  git tag $NEW_TAG && git push origin kiisu-dev $NEW_TAG"
echo "  gh release create $NEW_TAG --repo apmlabs/Momentum-Firmware \\"
echo "    --title \"Title\" --notes \"Notes\" --target kiisu-dev $NEW_TGZ"
