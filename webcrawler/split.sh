#!/usr/bin/env sh
set -o errexit
set -o nounset
set -o pipefail
set -o posix

DIRNAME="$(dirname "$0")"
md5sum -b "$DIRNAME"/texts.json > "$DIRNAME"/texts.json.md5
sha1sum -b "$DIRNAME"/texts.json > "$DIRNAME"/texts.json.sha1
mkdir "$TEMP"/texts.json
split -a 3 -b 64M -d "$DIRNAME"/texts.json "$TEMP"/texts.json/texts.json
rm -frd "$DIRNAME"/texts.json
mv "$TEMP"/texts.json "$DIRNAME"/texts.json
rm -frd "$TEMP"/texts.json
