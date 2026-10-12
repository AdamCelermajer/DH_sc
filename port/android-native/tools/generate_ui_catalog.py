"""Generate native exact-resource lookup from immutable staged-asset receipts."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
ASSETS = ROOT/'port/android-native/app/src/main/assets'
CPP = ROOT/'port/android-native/app/src/main/cpp'


def main():
    output = ROOT/'port/android-native/reports/original-ui-catalog.json'
    assert not output.exists(), 'Preserve the existing catalog receipt'
    resources = {}
    reports = ('original-ui-assets-stage.json','original-ui-text-assets-stage.json','original-ui-texture-assets-stage.json')
    inputs = {}
    for name in reports:
        path = ROOT/'port/android-native/reports'/name
        inputs[path.relative_to(ROOT).as_posix()] = hashlib.sha256(path.read_bytes()).hexdigest()
        report = json.loads(path.read_text())
        assert report['validation'] == 'PASS' and report['unmodified_original_bytes']
        for key, row in report['resources'].items():
            assert key == row['uri'].casefold() and key.isascii()
            assert hashlib.sha256((ASSETS/row['asset']).read_bytes()).hexdigest() == row['sha256']
            assert (ASSETS/row['asset']).stat().st_size == row['bytes']
            if key in resources:
                assert resources[key] == row, ('Conflicting canonical resource',key)
            resources[key] = row
    assert len(resources) == 385
    lines = ['// Generated from verified original asset receipts; preserve physical case and hashes.']
    for key,row in sorted(resources.items()):
        lines.append('{'+','.join(json.dumps(row[field]) for field in ('uri','asset','sha256'))+','+str(row['bytes'])+'u},')
        # Lookup key is stored separately from the physical original URI.
        lines[-1] = '{'+json.dumps(key)+','+lines[-1][1:]
    raw = ('\n'.join(lines)+'\n').encode()
    target = CPP/'original_ui_asset_catalog.inc'
    target.write_bytes(raw)
    receipt = dict(validation='PASS',resources=len(resources),stage_report_sha256=inputs,
                   catalog_sha256=hashlib.sha256(raw).hexdigest(),runtime_casefold_service=True,
                   original_archive_registration_parity=False,packaged_apk=False)
    output.write_text(json.dumps(receipt,indent=2)+'\n')
    print(json.dumps(dict(validation='PASS',resources=len(resources))))


if __name__ == '__main__':
    main()
