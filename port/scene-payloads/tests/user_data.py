#!/usr/bin/env python3
"""Check bounded SNode +0x48 UserProperties views and real swamp records."""
import argparse
import ctypes as c
import struct
from pathlib import Path

from audit_cache import Bres, Node, Scene, Visual, VISIT, bind

OK, ARGUMENT, RANGE, STRING = 0, 1, 2, 3


def opened(dll, data):
    storage = c.create_string_buffer(bytes(data))
    bres = Bres()
    assert dll.dh2_bres_open(c.byref(bres), storage, len(data)) == OK
    return storage, bres


def read_user_data(dll, node):
    text = c.c_void_p()
    byte_count = c.c_size_t()
    error = dll.dh2_scene_user_data_string(
        c.byref(node), c.byref(text), c.byref(byte_count))
    return error, text.value, byte_count.value


def synthetic_node(data, user_data_offset):
    storage = c.create_string_buffer(data)
    node = Node()
    node.image.bytes = c.cast(storage, c.c_void_p)
    node.image.size = len(data)
    node.user_data_offset = user_data_offset
    return storage, node


def malformed_cases(dll):
    checks = 0
    empty_storage, node = synthetic_node(bytes(16), 0)
    error, text, byte_count = read_user_data(dll, node)
    assert (error, text, byte_count) == (OK, None, 0)
    checks += 1

    # The +0x48 pointer itself must leave room for its first 32-bit string offset.
    for invalid_record in (13, 16, 0xffffffff):
        _storage, bad_node = synthetic_node(bytes(16), invalid_record)
        error, text, byte_count = read_user_data(dll, bad_node)
        assert (error, text, byte_count) == (RANGE, None, 0)
        checks += 1

    short_data = bytearray(16)
    struct.pack_into('<I', short_data, 4, 0)
    short_data[8] = 0
    _storage, node = synthetic_node(bytes(short_data), 4)
    error, text, byte_count = read_user_data(dll, node)
    assert (error, text, byte_count) == (OK, None, 0)
    checks += 1

    invalid_text = bytearray(16)
    struct.pack_into('<I', invalid_text, 4, 0xffffffff)
    _storage, node = synthetic_node(bytes(invalid_text), 4)
    error, text, byte_count = read_user_data(dll, node)
    assert (error, text, byte_count) == (STRING, None, 0)
    checks += 1

    unterminated = bytearray(16)
    struct.pack_into('<I', unterminated, 4, 8)
    unterminated[8:16] = b'abcdefgh'
    _storage, node = synthetic_node(bytes(unterminated), 4)
    error, text, byte_count = read_user_data(dll, node)
    assert (error, text, byte_count) == (STRING, None, 0)
    checks += 1

    valid = bytearray(16)
    struct.pack_into('<I', valid, 4, 8)
    valid[8:12] = b'abc\0'
    _storage, node = synthetic_node(bytes(valid), 4)
    node.extension_offset = 0xdecafbad
    error, text, byte_count = read_user_data(dll, node)
    assert error == OK and byte_count == 4 and c.string_at(text, byte_count) == b'abc\0'
    assert node.extension_offset == 0xdecafbad
    checks += 2

    text = c.c_void_p(1234)
    byte_count = c.c_size_t(999)
    assert dll.dh2_scene_user_data_string(
        None, c.byref(text), c.byref(byte_count)) == ARGUMENT
    assert text.value is None and byte_count.value == 0
    checks += 1

    return checks


def cache_cases(dll, cache_path):
    raw = cache_path.read_bytes()
    storage, bres = opened(dll, raw)
    scene = Scene()
    assert dll.dh2_scene_open(c.byref(scene), c.byref(bres)) == OK

    expected = {
        0xD8C54: b'floortypes = %22hole%22\0',
        0xD8DB4: b'floortypes = %22water%22\0',
        0xDA038: b'floortypes = %22wood%22\0',
        0x168A1C: b'floortypes = %22door%22\r\n\0',
    }
    found = {}
    no_user_data_seen = False

    @VISIT
    def visit(node_ptr, _world, _depth, _user):
        nonlocal no_user_data_seen
        node = node_ptr.contents
        record = node.record
        raw_user_data = struct.unpack_from('<I', raw, record + 0x48)[0]
        raw_opaque = struct.unpack_from('<I', raw, record + 0x4c)[0]
        if node.user_data_offset != raw_user_data or node.extension_offset != raw_opaque:
            found['layout_error'] = True
        if raw_user_data == 0:
            error, text, byte_count = read_user_data(dll, node)
            if (error, text, byte_count) != (OK, None, 0):
                found['absent_error'] = True
            no_user_data_seen = True
        if record in expected:
            error, text, byte_count = read_user_data(dll, node)
            if error != OK or not text:
                found[record] = None
            else:
                found[record] = c.string_at(text, byte_count)
        return True

    visits = 0
    for index in range(scene.visuals):
        visual = Visual()
        assert dll.dh2_scene_visual(c.byref(scene), index, c.byref(visual)) == OK
        assert dll.dh2_scene_walk_visual(c.byref(visual), visit, None, 100000) == OK
        visits += 1
    assert visits and not found.get('layout_error') and not found.get('absent_error')
    assert no_user_data_seen
    for record, text in expected.items():
        assert found.get(record) == text, (hex(record), found.get(record))
    return len(expected) + 4


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--cache', type=Path, required=True)
    parser.add_argument('--library', type=Path, required=True)
    args = parser.parse_args()
    dll = bind(args.library)
    checks = malformed_cases(dll) + cache_cases(dll, args.cache)
    print({'checks': checks, 'passed': True,
           'cache': str(args.cache.resolve()),
           'library': str(args.library.resolve())})


if __name__ == '__main__':
    main()
