"""Focused source-link and attribution checks shared by regen and cleancheck."""
import importlib
from common import write_json


def run(ctx):
    results = {}
    for module_name in ('global_lua_checks', 'ios_semantics_checks',
                        'xml_semantics_checks', 'resource_semantics_checks',
                        'entity_native_checks', 'package_cache_checks', 'character_template_checks',
                        'platform_graph_checks'):
        module = importlib.import_module(module_name)
        result = module.run(ctx)
        if isinstance(result,list):
            result={'passed':all(t['passed'] for t in result),'tests':result}
        results[module_name] = result
        write_json(ctx.out / 'data' / (module_name.replace('_', '-') + '.json'), result)
    result={'passed': all(r['passed'] for r in results.values()), 'suites': results}
    write_json(ctx.out/'data/global-link-verification.json',result)
    return result
