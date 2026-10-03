"""Use the existing Android TLS-canary fixture for the project-built world DSO."""
import hashlib,json,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import character_ai_state_differential as audit
from visual_timeline_differential import TimelineCpu
if __name__=='__main__':
 audit.Cpu=TimelineCpu
 audit.main()
 report=Path(sys.argv[sys.argv.index('--report')+1]);value=json.loads(report.read_text())
 value['project_runner_sha256']=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
 value['android_tls_canary']='Existing TimelineCpu fixture installs native TLS. Actual stack-protector instructions execute; this is not a device thread runtime proof.'
 report.write_text(json.dumps(value,indent=2)+'\n')
