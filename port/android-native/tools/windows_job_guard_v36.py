"""Fail-closed Windows committed-memory job for owned emulator descendants.

Create suspended, assign to a fresh hard-limited job, verify limits, then resume.
Never supports breakaway. Closing the last job handle kills remaining members.
"""
import ctypes as C
from ctypes import wintypes as W
import os,subprocess,uuid,msvcrt

if os.name!='nt':raise RuntimeError('Windows job guard requires Windows')
k=C.WinDLL('kernel32',use_last_error=True)
SIZE=C.c_size_t;ULONG_PTR=C.c_size_t
class BASIC(C.Structure):
 _fields_=[('PerProcessUserTimeLimit',C.c_int64),('PerJobUserTimeLimit',C.c_int64),('LimitFlags',W.DWORD),('MinimumWorkingSetSize',SIZE),('MaximumWorkingSetSize',SIZE),('ActiveProcessLimit',W.DWORD),('Affinity',ULONG_PTR),('PriorityClass',W.DWORD),('SchedulingClass',W.DWORD)]
class IO(C.Structure):
 _fields_=[(n,C.c_uint64) for n in ['ReadOperationCount','WriteOperationCount','OtherOperationCount','ReadTransferCount','WriteTransferCount','OtherTransferCount']]
class EXTENDED(C.Structure):
 _fields_=[('BasicLimitInformation',BASIC),('IoInfo',IO),('ProcessMemoryLimit',SIZE),('JobMemoryLimit',SIZE),('PeakProcessMemoryUsed',SIZE),('PeakJobMemoryUsed',SIZE)]
class STARTUP(C.Structure):
 _fields_=[('cb',W.DWORD),('lpReserved',W.LPWSTR),('lpDesktop',W.LPWSTR),('lpTitle',W.LPWSTR),('dwX',W.DWORD),('dwY',W.DWORD),('dwXSize',W.DWORD),('dwYSize',W.DWORD),('dwXCountChars',W.DWORD),('dwYCountChars',W.DWORD),('dwFillAttribute',W.DWORD),('dwFlags',W.DWORD),('wShowWindow',W.WORD),('cbReserved2',W.WORD),('lpReserved2',C.POINTER(W.BYTE)),('hStdInput',W.HANDLE),('hStdOutput',W.HANDLE),('hStdError',W.HANDLE)]
class PROCESS(C.Structure):
 _fields_=[('hProcess',W.HANDLE),('hThread',W.HANDLE),('dwProcessId',W.DWORD),('dwThreadId',W.DWORD)]
def api(name,args,ret=W.BOOL):
 f=getattr(k,name);f.argtypes=args;f.restype=ret;return f
CreateJob=api('CreateJobObjectW',[C.c_void_p,W.LPCWSTR],W.HANDLE)
SetJob=api('SetInformationJobObject',[W.HANDLE,C.c_int,C.c_void_p,W.DWORD])
QueryJob=api('QueryInformationJobObject',[W.HANDLE,C.c_int,C.c_void_p,W.DWORD,C.POINTER(W.DWORD)])
Assign=api('AssignProcessToJobObject',[W.HANDLE,W.HANDLE])
TerminateJob=api('TerminateJobObject',[W.HANDLE,W.UINT])
Close=api('CloseHandle',[W.HANDLE])
CreateProcess=api('CreateProcessW',[W.LPCWSTR,W.LPWSTR,C.c_void_p,C.c_void_p,W.BOOL,W.DWORD,C.c_void_p,W.LPCWSTR,C.POINTER(STARTUP),C.POINTER(PROCESS)])
Resume=api('ResumeThread',[W.HANDLE],W.DWORD)
TerminateProcess=api('TerminateProcess',[W.HANDLE,W.UINT])
GetTimes=api('GetProcessTimes',[W.HANDLE,C.POINTER(W.FILETIME),C.POINTER(W.FILETIME),C.POINTER(W.FILETIME),C.POINTER(W.FILETIME)])
OpenProcess=api('OpenProcess',[W.DWORD,W.BOOL,W.DWORD],W.HANDLE)
Wait=api('WaitForSingleObject',[W.HANDLE,W.DWORD],W.DWORD)
GetExit=api('GetExitCodeProcess',[W.HANDLE,C.POINTER(W.DWORD)])
def checked(value,label):
 if not value:raise C.WinError(C.get_last_error(),label)
 return value
def creation_time(pid=None,handle=None):
 owned=handle is None
 if owned:handle=checked(OpenProcess(0x1000,False,pid),'OpenProcess identity')
 try:
  values=[W.FILETIME() for _ in range(4)]
  checked(GetTimes(handle,*[C.byref(x) for x in values]),'GetProcessTimes')
  return (values[0].dwHighDateTime<<32)|values[0].dwLowDateTime
 finally:
  if owned:Close(handle)
class MemoryJob:
 def __init__(self,limit_bytes,active_process_limit=32):
  if not 16*1024**2<=limit_bytes<=15*1024**3:raise ValueError('Hard cap must be16MiB..15GiB')
  self.name='Local\\DH2-Emulator-'+str(uuid.uuid4());self.handle=None;self.process=None;self.resumed=False
  C.set_last_error(0);handle=CreateJob(None,self.name);checked(handle,'CreateJobObject')
  if C.get_last_error()==183:Close(handle);raise RuntimeError('Job identity collision')
  self.handle=handle
  try:
   info=EXTENDED();info.BasicLimitInformation.LimitFlags=0x200|0x100|0x2000|0x8
   info.BasicLimitInformation.ActiveProcessLimit=active_process_limit
   info.ProcessMemoryLimit=limit_bytes;info.JobMemoryLimit=limit_bytes
   checked(SetJob(handle,9,C.byref(info),C.sizeof(info)),'Set hard job limits')
   actual=self.limits()
   if actual['job_memory_limit']!=limit_bytes or actual['process_memory_limit']!=limit_bytes or actual['flags']!=info.BasicLimitInformation.LimitFlags:raise RuntimeError('OS hard limit readback differs')
  except BaseException:self.close();raise
 def limits(self):
  info=EXTENDED();checked(QueryJob(self.handle,9,C.byref(info),C.sizeof(info),None),'Read job limits')
  return {'flags':info.BasicLimitInformation.LimitFlags,'job_memory_limit':info.JobMemoryLimit,'process_memory_limit':info.ProcessMemoryLimit,'peak_job_bytes':info.PeakJobMemoryUsed,'active_process_limit':info.BasicLimitInformation.ActiveProcessLimit}
 def create_suspended(self,executable,args,cwd=None,stdout_path=None,stderr_path=None):
  if self.process:raise RuntimeError('One root process per owned job')
  startup=STARTUP();startup.cb=C.sizeof(startup) # GUI remains visible; CREATE_NO_WINDOW hides console only.
  info=PROCESS();command=C.create_unicode_buffer(subprocess.list2cmdline([str(executable),*map(str,args)]));fds=[]
  try:
   if stdout_path and stderr_path:
    for path in ('NUL',stdout_path,stderr_path):
     fd=os.open(str(path),os.O_RDWR|os.O_CREAT|os.O_APPEND|os.O_BINARY);fds.append(fd)
     os.set_handle_inheritable(msvcrt.get_osfhandle(fd),True)
    startup.dwFlags|=0x100;startup.hStdInput,startup.hStdOutput,startup.hStdError=[msvcrt.get_osfhandle(fd) for fd in fds]
   checked(CreateProcess(str(executable),command,None,None,bool(fds),0x4|0x08000000,None,str(cwd) if cwd else None,C.byref(startup),C.byref(info)),'Create suspended owned process')
  finally:
   for fd in fds:os.close(fd)
  try:checked(Assign(self.handle,info.hProcess),'Assign suspended process to bounded job')
  except BaseException:TerminateProcess(info.hProcess,125);Close(info.hThread);Close(info.hProcess);raise
  self.process=info;return {'pid':info.dwProcessId,'create_time_filetime':creation_time(handle=info.hProcess)}
 def resume(self):
  if not self.process or self.resumed:raise RuntimeError('Invalid resume state')
  if Resume(self.process.hThread)==0xffffffff:raise C.WinError(C.get_last_error(),'Resume owned process')
  self.resumed=True;Close(self.process.hThread);self.process.hThread=None
 def members(self):
  class IDS(C.Structure):_fields_=[('assigned',W.DWORD),('count',W.DWORD),('ids',ULONG_PTR*4096)]
  info=IDS();checked(QueryJob(self.handle,3,C.byref(info),C.sizeof(info),None),'Query owned job membership')
  if info.assigned>4096 or info.count>4096:raise RuntimeError('Owned process budget exceeded')
  return list(info.ids[:info.count])
 def terminate(self,code=125):
  if self.handle:checked(TerminateJob(self.handle,code),'Terminate owned job only')
 def close(self):
  if self.process:
   if self.process.hThread:Close(self.process.hThread)
   if self.process.hProcess:Close(self.process.hProcess)
   self.process=None
  if self.handle:Close(self.handle);self.handle=None
 def __enter__(self):return self
 def __exit__(self,*_):self.close()
