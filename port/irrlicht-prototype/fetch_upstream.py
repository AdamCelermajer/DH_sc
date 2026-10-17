#!/usr/bin/env python3
"""Fetch the official Irrlicht 1.8.5 SVN tag source subset and pin its bytes.

The SourceForge project release download endpoint is not required: the same
official source is read by immutable tag path from the project's SVN browser.
The script only mirrors include/, source/Irrlicht/, and upstream release notes.
"""
from __future__ import annotations

from concurrent.futures import ThreadPoolExecutor, as_completed
from html.parser import HTMLParser
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import sys
import time
from urllib.parse import unquote, urljoin, urlparse
from urllib.request import Request, urlopen


ROOT = Path(__file__).resolve().parent
UPSTREAM = ROOT / "upstream" / "irrlicht-1.8.5"
BASE = "https://svn.code.sf.net/p/irrlicht/code/tags/release-1.8.5/"
MAX_WORKERS = 8
USER_AGENT = "DH_sc-Irrlicht-prototype/1.0 (official source mirror; tag audit)"


class Listing(HTMLParser):
    def __init__(self) -> None:
        super().__init__()
        self.hrefs: list[str] = []
        self.in_a = False
        self.current = ""

    def handle_starttag(self, tag: str, attrs: list[tuple[str, str | None]]) -> None:
        if tag == "a":
            self.in_a = True
            self.current = dict(attrs).get("href") or ""

    def handle_endtag(self, tag: str) -> None:
        if tag == "a":
            self.in_a = False
            if self.current:
                self.hrefs.append(self.current)
            self.current = ""


def get(url: str) -> tuple[bytes, str]:
    if urlparse(url).hostname != "svn.code.sf.net":
        raise ValueError(f"Refusing non-upstream host: {url}")
    last: Exception | None = None
    for attempt in range(4):
        try:
            request = Request(url, headers={"User-Agent": USER_AGENT})
            with urlopen(request, timeout=60) as response:
                payload = response.read()
                content_type = response.headers.get("Content-Type", "")
                if response.status != 200:
                    raise IOError(f"HTTP {response.status} for {url}")
                return payload, content_type
        except Exception as error:  # retry transient source-browser failures
            last = error
            if attempt == 3:
                break
            time.sleep(0.4 * (attempt + 1))
    assert last is not None
    raise last


def list_dir(relative: str) -> list[str]:
    url = urljoin(BASE, relative)
    payload, content_type = get(url)
    if "text/html" not in content_type:
        raise ValueError(f"Expected SVN directory listing at {url}; got {content_type}")
    listing = Listing()
    listing.feed(payload.decode("utf-8", "strict"))
    result: list[str] = []
    for href in listing.hrefs:
        if href in ("../", "./") or href.startswith(("?", "#")):
            continue
        resolved = urlparse(urljoin(url, href))
        if resolved.hostname != "svn.code.sf.net":
            continue
        child = unquote(resolved.path.split("/code/tags/release-1.8.5/", 1)[-1])
        prefix = relative.rstrip("/") + "/"
        if not child.startswith(prefix):
            raise ValueError(f"Listing escaped pinned tag path: {href}")
        result.append(child)
    return result


def collect() -> list[str]:
    files: list[str] = []
    todo = ["include/", "source/Irrlicht/"]
    while todo:
        directory = todo.pop()
        for child in list_dir(directory):
            if child.endswith("/"):
                todo.append(child)
            else:
                files.append(child)
    files.extend(["readme.txt", "changes.txt", "source/source.txt"])
    return sorted(set(files))


def fetch_file(relative: str) -> tuple[str, bytes]:
    path = PurePosixPath(relative)
    if path.is_absolute() or ".." in path.parts:
        raise ValueError(f"Unsafe tag-relative path: {relative}")
    data, content_type = get(urljoin(BASE, relative))
    if "text/html" in content_type or data[:64].lstrip().lower().startswith(b"<!doctype html"):
        raise ValueError(f"Unexpected HTML where upstream source expected: {relative}")
    return relative, data


def main() -> int:
    files = collect()
    print(f"Official tag file count: {len(files)}")
    UPSTREAM.mkdir(parents=True, exist_ok=True)
    records: list[dict[str, object]] = []
    with ThreadPoolExecutor(max_workers=MAX_WORKERS) as pool:
        futures = [pool.submit(fetch_file, relative) for relative in files]
        for future in as_completed(futures):
            relative, payload = future.result()
            target = UPSTREAM.joinpath(*PurePosixPath(relative).parts)
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(payload)
            records.append({
                "path": relative,
                "bytes": len(payload),
                "sha256": hashlib.sha256(payload).hexdigest(),
            })
    records.sort(key=lambda row: str(row["path"]))
    canonical = json.dumps(records, sort_keys=True, separators=(",", ":")).encode()
    manifest = {
        "schema": "dh2.irrlicht-upstream-tree.v1",
        "upstream": "Irrlicht Engine official SourceForge SVN repository",
        "tag_url": BASE.rstrip("/"),
        "version": "1.8.5",
        "description": "Exact bytes fetched from the immutable release-1.8.5 tag; the full SDK media/examples are omitted.",
        "file_count": len(records),
        "tree_manifest_sha256": hashlib.sha256(canonical).hexdigest(),
        "files": records,
    }
    manifest_path = ROOT / "upstream-source-manifest.json"
    manifest_path.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    print(f"Fetched {len(records)} files; tag-tree manifest SHA-256 {manifest['tree_manifest_sha256']}")
    print(f"Source tree: {UPSTREAM}")
    print(f"Manifest: {manifest_path}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as error:
        print(f"fetch failed: {error}", file=sys.stderr)
        raise SystemExit(1)
