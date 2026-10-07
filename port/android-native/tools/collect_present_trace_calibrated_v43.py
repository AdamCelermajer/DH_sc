"""Root-only bounded calibration; default dry-run and all V42 guardrails remain."""
from pathlib import Path
import collect_present_trace_v42 as collector
if __name__=='__main__':
 collector.CONFIG=Path(__file__).resolve().parents[1]/'reports/present-attribution-v42/capture-calibrated-v43.textproto'
 raise SystemExit(collector.main())
