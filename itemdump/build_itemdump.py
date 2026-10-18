#!/usr/bin/env python3
"""Build dh2_itemdump.exe against a configured, built Track B host-runner build directory, without changing dh_noam.

usage: py -3 -I itemdump/build_itemdump.py <runner build dir> <out exe>

The runner build dir is `cmake -S <dh_noam>/source_code/port/decomp-reconstruction -B <dir> -G Ninja -DDH2_HOST_RUNNER=ON ...`
with `ninja dh2_texconv` done (see build.py). dh2_texconv is the runner's own tool that links the whole game like the
runner does; this script takes its compile and link lines from build.ninja and swaps tool_texconv.cpp for
itemdump/tool_itemdump.cpp.
"""
import os, re, shlex, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))


def block(text, header_prefix):
    i = text.index(header_prefix)
    end = text.index("\n\n", i)
    lines = text[i:end].split("\n")
    head = lines[0]
    vars_ = {}
    for l in lines[1:]:
        m = re.match(r"\s+(\w+) = (.*)", l)
        if m:
            vars_[m.group(1)] = m.group(2)
    return head, vars_


def main():
    bdir, out = os.path.abspath(sys.argv[1]), os.path.abspath(sys.argv[2])
    nin = open(os.path.join(bdir, "build.ninja"), encoding="utf-8").read()
    _, cv = block(nin, "build host-runner/CMakeFiles/dh2_texconv.dir/tool_texconv.cpp.obj:")
    head, lv = block(nin, "build host-runner/dh2_texconv.exe:")
    m = re.search(r"CMAKE_CXX_COMPILER:\w+=(.*)", open(os.path.join(bdir, "CMakeCache.txt"), encoding="utf-8").read())
    cxx = m.group(1).strip()
    obj = os.path.join(os.path.dirname(out), "tool_itemdump.cpp.obj")
    os.makedirs(os.path.dirname(out), exist_ok=True)
    split = lambda s: shlex.split(s.replace("\\", "/"), posix=True)
    cmd = [cxx] + split(cv["DEFINES"]) + split(cv["INCLUDES"]) + split(cv["FLAGS"]) + ["-c", os.path.join(HERE, "tool_itemdump.cpp"), "-o", obj]
    print("> compile tool_itemdump.cpp", flush=True)
    subprocess.check_call(cmd, cwd=bdir)
    # inputs of the texconv link: everything after the rule name up to "||", minus tool_texconv.cpp.obj
    inputs = head.split(":", 1)[1].split("||")[0].split("|")[0].split()[1:]   # explicit inputs only
    inputs = [x.replace("$:", ":") for x in inputs if not x.endswith("tool_texconv.cpp.obj")]
    cmd = [cxx] + split(lv["FLAGS"]) + split(lv["LINK_FLAGS"]) + [obj] + inputs + ["-o", out] + split(lv["LINK_LIBRARIES"])
    print("> link", out, flush=True)
    subprocess.check_call(cmd, cwd=bdir)


if __name__ == "__main__":
    main()
