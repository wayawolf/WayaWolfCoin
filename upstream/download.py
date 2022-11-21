#!/usr/bin/env python3
# vim:ts=4:sw=4:ai:et:si:sts=4

import requests
import json
import os
import sys
import tarfile

base_os = os.environ.get("BASE_OS", "ubuntu20")
base_dir = os.path.realpath(os.path.join(sys.argv[0], ".."))
manifest_file = os.path.join(base_dir, "manifest.json")
archive_dir = os.path.join(base_dir, "archive")
makefile_dir = os.path.join(base_dir, "makefiles")
src_dir = os.path.join(base_dir, "src")

with open(manifest_file, "r") as f:
    manifest = json.load(f)

upstream = manifest.get("files", {})
deps = manifest.get(base_os, [])

os.makedirs(archive_dir, 0o755, exist_ok=True)
os.makedirs(src_dir, 0o755, exist_ok=True)

for dep in deps:
    details = upstream.get(dep, None)
    if not details:
        print("Can't find upstream for %s" % dep)
        sys.exit(1)

    url = details.get("url", None)
    file_ = details.get("filename", None)
    dir_ = details.get("dir", None)
    if not url or not file_ or not dir_:
        print("Invalid upstream for %s" % dep)
        sys.exit(1)

    outfile = os.path.join(archive_dir, file_)
    if os.path.exists(outfile):
        print("Archive file for %s already exists" % dep)
    else:
        print("Downloading dependency %s from %s" % (dep, url))
        
        with requests.get(url, stream=True) as r:
            r.raise_for_status()
            with open(outfile, "wb") as f:
                for chunk in r.iter_content(chunk_size=8192):
                    f.write(chunk)

    outdir = os.path.join(src_dir, dir_)
    if os.path.exists(outdir) and os.path.isdir(outdir):
        print("Upstream %s already expanded" % dep)
    else:
        print("Untarring %s" % dep)
        with tarfile.open(outfile) as tf:
            def is_within_directory(directory, target):
                
                abs_directory = os.path.abspath(directory)
                abs_target = os.path.abspath(target)
            
                prefix = os.path.commonprefix([abs_directory, abs_target])
                
                return prefix == abs_directory
            
            def safe_extract(tar, path=".", members=None, *, numeric_owner=False):
            
                for member in tar.getmembers():
                    member_path = os.path.join(path, member.name)
                    if not is_within_directory(path, member_path):
                        raise Exception("Attempted Path Traversal in Tar File")
            
                tar.extractall(path, members, numeric_owner=numeric_owner) 
                
            
            safe_extract(tf, path=src_dir)

    makefile_in = os.path.join(makefile_dir, "Makefile.%s" % dep)
    makefile_out = os.path.join(outdir, "Makefile.cryptocoin")
    print("Copying cryptocoin Makefile for %s" % dep)
    with open(makefile_in, "r") as fi:
        with open(makefile_out, "w") as fo:
            fo.write(fi.read())

    print("")
