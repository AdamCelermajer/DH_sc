@echo off
setlocal
set "TASK_PYTHON=%USERPROFILE%\.local\share\comfy-mcp\venv\Scripts\python.exe"
if defined TEXTURE_LAB_MCP_HOME set "TASK_PYTHON=%TEXTURE_LAB_MCP_HOME%\venv\Scripts\python.exe"
if not exist "%TASK_PYTHON%" (
  echo Comfy MCP Python environment not found. Set TEXTURE_LAB_MCP_HOME to its installation folder.
  exit /b 1
)
set "PYTHONUTF8=1"
"%TASK_PYTHON%" "%~dp0lab.py" %*
exit /b %errorlevel%
