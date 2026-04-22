#!/bin/bash
# Quick release: rebuild FAP apps, repackage into existing firmware
# Usage: ./tools/quick-release.sh <new-tag> [base-tag]
# Builds and packages all .fap files found in build output that differ from base.

set -e

NEW_TAG="${1:?Usage: $0 <new-tag> [base-tag]}"
BASE_TAG="${2:-$(git describe --tags --abbrev=0)}"
REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
DIST="$REPO_ROOT/dist/f7-C"
BASE_TGZ="$DIST/flipper-z-f7-update-${BASE_TAG}.tgz"
NEW_TGZ="$DIST/flipper-z-f7-update-${NEW_TAG}.tgz"
BUILD_APPS="$REPO_ROOT/build/f7-firmware-C/.extapps"

if [ ! -f "$BASE_TGZ" ]; then
    echo "ERROR: Base firmware not found: $BASE_TGZ"
    echo "Available:"; ls "$DIST"/flipper-z-f7-update-*.tgz 2>/dev/null | xargs -I{} basename {}
    exit 1
fi

# 1. Build apps
echo "=== Building apps ==="
cd "$REPO_ROOT"
FBT_NO_SYNC=1 ./fbt fap_spectrum_check fap_dooya_remote 2>&1 | tail -5

# 2. Extract base firmware
WORK=$(mktemp -d)
trap "rm -rf $WORK" EXIT
echo "=== Extracting $BASE_TAG ==="
tar xzf "$BASE_TGZ" -C "$WORK"
UPDDIR="$WORK/f7-update-${BASE_TAG}"

# 3. Replace/add faps in resources.tar.gz, update manifest
echo "=== Updating apps in resources ==="
python3 -c "
import tarfile, gzip, io, hashlib, os, glob

res_gz = '$UPDDIR/resources.tar.gz'
build_dir = '$BUILD_APPS'

# Find all built .fap files and map to their tar path (apps/Category/name.fap)
# We need to know the category. Read application.fam for fap_category.
faps = {}
for fap_file in glob.glob(os.path.join(build_dir, '*.fap')):
    name = os.path.basename(fap_file)
    # Determine category from the tar contents (match by filename)
    faps[name] = {'src': fap_file}

# Read original tar
with gzip.open(res_gz, 'rb') as gz:
    raw = gz.read()
inp = tarfile.open(fileobj=io.BytesIO(raw), mode='r:')

# First pass: find existing fap paths and manifest
existing_paths = {}
for member in inp:
    if member.name.endswith('.fap'):
        fname = os.path.basename(member.name)
        existing_paths[fname] = member.name

# Map our faps to tar paths
fap_updates = {}  # tar_path -> local_file
for name, info in faps.items():
    if name in existing_paths:
        fap_updates[existing_paths[name]] = info['src']
    else:
        # New app — put in Sub-GHz category (default for our apps)
        fap_updates['apps/Sub-GHz/' + name] = info['src']

# Read fap data
fap_data = {}
for tar_path, src in fap_updates.items():
    with open(src, 'rb') as f:
        data = f.read()
    fap_data[tar_path] = {
        'data': data,
        'md5': hashlib.md5(data).hexdigest(),
        'size': len(data),
    }

# Second pass: rebuild tar
inp.close()
inp = tarfile.open(fileobj=io.BytesIO(raw), mode='r:')
out_buf = io.BytesIO()
out = tarfile.open(fileobj=out_buf, mode='w:', format=tarfile.USTAR_FORMAT)

manifest_data = None
added_new = set()

for member in inp:
    if member.name in fap_data:
        # Replace existing fap
        fd = fap_data[member.name]
        member.size = fd['size']
        member.mtime = 0
        member.uid = member.gid = 0
        member.uname = member.gname = 'furippa'
        member.mode = 0o644
        out.addfile(member, io.BytesIO(fd['data']))
        added_new.add(member.name)
        print(f'  Replaced: {member.name} ({fd[\"size\"]}B)')
    elif member.name == 'Manifest':
        manifest_data = inp.extractfile(member).read().decode()
        # Will process after adding new files
    else:
        if member.isfile():
            out.addfile(member, inp.extractfile(member))
        else:
            out.addfile(member)

# Add new faps not in original tar
for tar_path, fd in fap_data.items():
    if tar_path not in added_new:
        info = tarfile.TarInfo(name=tar_path)
        info.size = fd['size']
        info.mtime = 0
        info.uid = info.gid = 0
        info.uname = info.gname = 'furippa'
        info.mode = 0o644
        out.addfile(info, io.BytesIO(fd['data']))
        print(f'  Added: {tar_path} ({fd[\"size\"]}B)')

# Update manifest
if manifest_data:
    lines = manifest_data.rstrip('\n').split('\n')
    # Update existing entries, add new ones
    for tar_path, fd in fap_data.items():
        new_line = f'F:{fd[\"md5\"]}:{fd[\"size\"]}:{tar_path}'
        found = False
        for i, line in enumerate(lines):
            if tar_path in line:
                print(f'  Manifest: {line} -> {new_line}')
                lines[i] = new_line
                found = True
                break
        if not found:
            lines.append(new_line)
            print(f'  Manifest: +{new_line}')
    manifest_raw = ('\n'.join(lines) + '\n').encode()
    info = tarfile.TarInfo(name='Manifest')
    info.size = len(manifest_raw)
    info.mtime = 0
    info.uid = info.gid = 0
    info.uname = info.gname = 'furippa'
    info.mode = 0o644
    out.addfile(info, io.BytesIO(manifest_raw))

out.close()
inp.close()

# Gzip with mtime=0
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
