"""ctypes bindings to the C++ component. All views borrow the Input's bytes."""
import ctypes as c
from pathlib import Path

U = c.c_uint32
I = c.c_int32
P = c.c_void_p

class Bres(c.Structure):
    _fields_ = [('bytes', P), ('size', c.c_size_t)] + [(x, U) for x in
                ['fixup_count', 'fixup_offset', 'root_offset', 'tail_offset', 'bulk_size', 'block_count', 'tail_size']]

class Mesh(c.Structure):
    _fields_ = [('image', Bres), ('id', P), ('name', P)] + [(x, U) for x in
                ['vertices', 'stride', 'attributes', 'primitives', 'stream', 'buffers']] + [
                ('minimum', c.c_float*3), ('maximum', c.c_float*3)]

class Type1Geometry(c.Structure):
    _fields_ = [('embedded_mesh', Mesh), ('opaque_header', U*5)]

class Attribute(c.Structure):
    _fields_ = [('data', P)] + [(x, U) for x in ['type', 'components', 'stride', 'vertices']]

class Primitive(c.Structure):
    _fields_ = [('material', P), ('indices', P)] + [(x, U) for x in
                ['collada_type', 'engine_type', 'declared_count', 'index_count', 'index_width', 'minimum_index', 'maximum_index']] + [('attributes', I*18)]

class Vector(c.Structure):
    _fields_ = [('data', P), ('count', U), ('type', U), ('components', U)]

class Animation(c.Structure):
    _fields_ = [('image', Bres)] + [(x, U) for x in ['record', 'data', 'data_size', 'entries']] + [('segment_start', I), ('segment_end', I)]

def bind(path):
    dll = c.CDLL(str(Path(path).resolve()))
    specs = {
        'dh2_bres_open': (U, [c.POINTER(Bres), P, c.c_size_t]),
        'dh2_bres_library_count': (U, [c.POINTER(Bres), U]),
        'dh2_mesh_open': (U, [c.POINTER(Mesh), c.POINTER(Bres), I]),
        'dh2_type1_geometry_open': (U, [c.POINTER(Type1Geometry), c.POINTER(Bres), I]),
        'dh2_mesh_attribute': (U, [c.POINTER(Mesh), I, c.POINTER(Attribute)]),
        'dh2_mesh_primitive': (U, [c.POINTER(Mesh), I, c.POINTER(Primitive)]),
        'dh2_attribute_read': (c.c_bool, [c.POINTER(Attribute), U, c.POINTER(c.c_float)]),
        'dh2_index_read': (c.c_bool, [c.POINTER(Primitive), U, c.POINTER(U)]),
        'dh2_animation_segments': (U, [c.POINTER(Bres)]),
        'dh2_animation_open': (U, [c.POINTER(Animation), c.POINTER(Bres), I, I]),
        'dh2_animation_vector': (c.c_bool, [c.POINTER(Animation), I, c.c_bool, c.POINTER(Vector)]),
        'dh2_vector_read': (c.c_bool, [c.POINTER(Vector), U, c.POINTER(c.c_float)]),
        'dh2_animation_channel': (P, [c.POINTER(Animation), I]),
        'dh2_animation_type': (U, [c.POINTER(Animation), I]),
        'dh2_animation_time_type': (U, [c.POINTER(Animation), I]),
        'dh2_animation_interpolation': (U, [c.POINTER(Animation), I]),
        'dh2_animation_key_time': (I, [c.POINTER(Animation), I, I]),
        'dh2_animation_start': (I, [c.POINTER(Animation), I]),
        'dh2_animation_end': (I, [c.POINTER(Animation), I]),
        'dh2_animation_length': (I, [c.POINTER(Animation), I]),
        'dh2_animation_find_raw_index': (c.c_bool, [c.POINTER(Vector), I, c.POINTER(I)]),
        'dh2_animation_find_index': (c.c_bool, [c.POINTER(Animation), I, I, c.POINTER(I)]),
        'dh2_animation_find': (c.c_bool, [c.POINTER(Animation), I, I, c.POINTER(I), c.POINTER(c.c_float)]),
    }
    for name in ['target', 'default', 'offsets', 'scales']:
        specs['dh2_animation_'+name] = (P, [c.POINTER(Animation)])
    for name in ['channels', 'samplers', 'animator', 'scale_type']:
        specs['dh2_animation_'+name] = (U, [c.POINTER(Animation)])
    specs['dh2_animation_has_default'] = (c.c_bool, [c.POINTER(Animation)])
    for name, (result, args) in specs.items():
        f = getattr(dll, name); f.restype = result; f.argtypes = args
    return dll

class Input:
    def __init__(self, dll, data):
        self.dll = dll
        self.bytes = c.create_string_buffer(data)
        self.view = Bres()
        error = dll.dh2_bres_open(c.byref(self.view), self.bytes, len(data))
        if error:
            raise ValueError(f'BRES rejected with code {error}')

    def mesh(self, index):
        m = Mesh(); error = self.dll.dh2_mesh_open(c.byref(m), c.byref(self.view), index)
        if error:
            raise ValueError(f'Geometry {index} rejected with code {error}')
        return m

    def type1_geometry(self, index):
        value = Type1Geometry()
        error = self.dll.dh2_type1_geometry_open(c.byref(value), c.byref(self.view), index)
        if error:
            raise ValueError(f'Type-1 geometry {index} rejected with code {error}')
        return value

    def animation(self, index, segment):
        a = Animation(); error = self.dll.dh2_animation_open(c.byref(a), c.byref(self.view), index, segment)
        if error:
            raise ValueError(f'Animation {index}, segment {segment} rejected with code {error}')
        return a

def string(p):
    return c.string_at(p).decode('utf-8', errors='replace') if p else None

def numeric(v):
    """Raw vector values; deliberately preserves serialized shader values."""
    widths = [1, 1, 2, 2, 4, 4, 4]
    formats = ['b', 'B', 'h', 'H', 'i', 'I', 'f']
    import struct
    raw = c.string_at(v.data, v.count*v.components*widths[v.type])
    return list(struct.unpack('<'+formats[v.type]*(v.count*v.components), raw))
