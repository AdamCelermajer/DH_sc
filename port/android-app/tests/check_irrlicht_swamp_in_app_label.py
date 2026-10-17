#!/usr/bin/env python3
"""Host-only check for the in-app Irrlicht label metadata path."""
from __future__ import annotations

import importlib.util
from pathlib import Path
import re


REPO = Path(__file__).resolve().parents[3]
APP = REPO / 'port/android-app'


def load_module(name: str, path: Path):
    spec = importlib.util.spec_from_file_location(name, path)
    if spec is None or spec.loader is None:
        raise RuntimeError(f'could not load source module: {path}')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def main() -> int:
    builder = load_module('dh2_android_build', APP / 'build.py')
    runtime = load_module(
        'dh2_irrlicht_swamp_in_app_runtime',
        APP / 'tests/irrlicht_swamp_in_app_runtime.py')
    manifest_data = builder.validate_irrlicht_swamp_in_app_manifest(
        APP / 'AndroidManifest.irrlicht-swamp-in-app.xml')
    java = (APP / 'src/local/dh2/sourceviewer/MainActivity.java').read_text(
        encoding='utf-8')

    expected_title = 'SWAMP module 0 · Irrlicht source view'
    if manifest_data['diagnostic_title'] != expected_title:
        raise AssertionError(f'unexpected manifest diagnostics label: {manifest_data}')
    if runtime.IRRLICHT_BUTTON_LABEL != expected_title:
        raise AssertionError('runtime UI assertion differs from manifest diagnostics label')
    expected_description = (
        'Open SWAMP module 0 in the local Irrlicht NativeActivity source view.')
    if manifest_data['diagnostic_description'] != expected_description:
        raise AssertionError('manifest diagnostics content description changed unexpectedly')

    metadata_reader = re.search(
        r'getPackageManager\(\)\s*\.\s*getApplicationInfo\(\s*'
        r'getPackageName\(\)\s*,\s*'
        r'android\.content\.pm\.PackageManager\.GET_META_DATA\s*\)', java)
    if not metadata_reader:
        raise AssertionError(
            'MainActivity must request application metadata from PackageManager with GET_META_DATA')
    if 'String value = metadata.getString(name);' not in java:
        raise AssertionError('MainActivity no longer reads the requested diagnostics metadata key')
    if 'return value == null || value.trim().isEmpty() ? fallback : value;' not in java:
        raise AssertionError('MainActivity metadata lookup must retain its fallback label')
    if 'irrlicht.setText(diagnosticMetadata(' not in java:
        raise AssertionError('Diagnostics button does not use the manifest metadata label')
    if 'click_text(IRRLICHT_BUTTON_LABEL, exact=True)' not in (
            APP / 'tests/irrlicht_swamp_in_app_runtime.py').read_text(encoding='utf-8'):
        raise AssertionError('runtime harness must click the exact SWAMP diagnostics label')

    print('SWAMP manifest label, PackageManager metadata lookup, fallback, and exact UI assertion passed')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
