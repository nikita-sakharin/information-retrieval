#!/usr/bin/env sh
set -o errexit
set -o nounset
set -o pipefail
set -o posix

readonly DIRNAME="$(dirname "$0")"
cat "$DIRNAME"/texts.json/texts.json* > "$TEMP"/texts.json
rm -frd "$DIRNAME"/texts.json
mv "$TEMP"/texts.json "$DIRNAME"/texts.json
rm -frd "$TEMP"/texts.json
md5sum -c --quiet --strict --warn "$DIRNAME"/texts.json.md5
sha1sum -c --quiet --strict --warn "$DIRNAME"/texts.json.sha1
