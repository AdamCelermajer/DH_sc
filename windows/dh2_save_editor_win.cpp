// DH2 Save Editor for Windows: a double-click app. It serves the shared editor UI (the "web" folder next to the exe, the same
// files the Android build puts in the game APK) on 127.0.0.1 and shows it in a Microsoft Edge app window (no browser UI).
// Closing that window ends the program. File access goes through this program:
//   * a folder on this PC (for example a copy of the phone's files folder): list dh2_NNN.savegame, read, write;
//   * the phone over USB with adb: the saves in /sdcard/Android/data/<game package>/files are pulled into "phone\<serial>",
//     and written back with adb push after a backup.
// Every write keeps backups: the previous file becomes "<name>.bak" next to it (what the game itself does before it saves,
// Savegame::UpdateJobs backup job) and a dated copy goes to "editor-backups". A phone write is refused while the game runs.
//
// Build (llvm-mingw): see tools/save-editor/build_save_editor.py.
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <shellapi.h>
#include <shlobj.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
#include <thread>
#include <vector>

namespace {

const char* kPackage = "com.gameloft.android.GAND.GloftD2SS";
std::wstring g_appDir;     // folder of the exe
std::string g_token;       // random per run: every /api call must carry it (X-DH2-Token)
int g_port = 0;

// ---------------------------------------------------------------- strings
std::wstring W(const std::string& s) {
    if (s.empty()) return L"";
    int n = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), nullptr, 0);
    std::wstring w(n, 0);
    MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), &w[0], n);
    return w;
}
std::string U(const std::wstring& w) {
    if (w.empty()) return "";
    int n = WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), nullptr, 0, nullptr, nullptr);
    std::string s(n, 0);
    WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), &s[0], n, nullptr, nullptr);
    return s;
}
std::string Json(const std::string& s) {
    std::string o = "\"";
    for (unsigned char c : s) {
        if (c == '"') o += "\\\"";
        else if (c == '\\') o += "\\\\";
        else if (c < 0x20) { char b[8]; std::snprintf(b, sizeof b, "\\u%04x", c); o += b; }
        else o += (char)c;
    }
    return o + "\"";
}
std::string UrlDecode(const std::string& s) {
    std::string o;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '%' && i + 2 < s.size()) { o += (char)std::strtol(s.substr(i + 1, 2).c_str(), nullptr, 16); i += 2; }
        else if (s[i] == '+') o += ' ';
        else o += s[i];
    }
    return o;
}
std::string QueryParam(const std::string& query, const char* key) {
    std::string k = std::string(key) + "=";
    size_t p = 0;
    while (p < query.size()) {
        size_t e = query.find('&', p);
        if (e == std::string::npos) e = query.size();
        if (query.compare(p, k.size(), k) == 0) return UrlDecode(query.substr(p + k.size(), e - p - k.size()));
        p = e + 1;
    }
    return "";
}
const char kB64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
std::string B64(const std::vector<unsigned char>& d) {
    std::string o;
    size_t i = 0;
    for (; i + 2 < d.size(); i += 3) {
        unsigned v = (d[i] << 16) | (d[i + 1] << 8) | d[i + 2];
        o += kB64[v >> 18]; o += kB64[(v >> 12) & 63]; o += kB64[(v >> 6) & 63]; o += kB64[v & 63];
    }
    if (i + 1 == d.size()) { unsigned v = d[i] << 16; o += kB64[v >> 18]; o += kB64[(v >> 12) & 63]; o += "=="; }
    else if (i + 2 == d.size()) { unsigned v = (d[i] << 16) | (d[i + 1] << 8); o += kB64[v >> 18]; o += kB64[(v >> 12) & 63]; o += kB64[(v >> 6) & 63]; o += '='; }
    return o;
}
bool Unb64(const std::string& s, std::vector<unsigned char>& out) {
    int val = 0, bits = -8;
    for (unsigned char c : s) {
        if (c == '=' || c == '\r' || c == '\n') continue;
        const char* p = std::strchr(kB64, c);
        if (p == nullptr || c == 0) return false;
        val = (val << 6) | (int)(p - kB64);
        bits += 6;
        if (bits >= 0) { out.push_back((unsigned char)((val >> bits) & 0xff)); bits -= 8; }
    }
    return true;
}
std::string Stamp() {
    std::time_t t = std::time(nullptr);
    std::tm tm{};
    localtime_s(&tm, &t);
    char b[32];
    std::strftime(b, sizeof b, "%Y%m%d-%H%M%S", &tm);
    return b;
}

// ---------------------------------------------------------------- files
bool ReadFileAll(const std::wstring& path, std::vector<unsigned char>& out) {
    HANDLE h = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
    if (h == INVALID_HANDLE_VALUE) return false;
    LARGE_INTEGER sz;
    GetFileSizeEx(h, &sz);
    out.resize((size_t)sz.QuadPart);
    DWORD got = 0;
    bool ok = out.empty() || (ReadFile(h, out.data(), (DWORD)out.size(), &got, nullptr) && got == out.size());
    CloseHandle(h);
    return ok;
}
bool WriteFileAll(const std::wstring& path, const std::vector<unsigned char>& d) {
    HANDLE h = CreateFileW(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) return false;
    DWORD put = 0;
    bool ok = d.empty() || (WriteFile(h, d.data(), (DWORD)d.size(), &put, nullptr) && put == d.size());
    ok = FlushFileBuffers(h) && ok;
    CloseHandle(h);
    return ok;
}
std::wstring DirOf(const std::wstring& p) { size_t k = p.find_last_of(L"\\/"); return k == std::wstring::npos ? L"." : p.substr(0, k); }
std::wstring NameOf(const std::wstring& p) { size_t k = p.find_last_of(L"\\/"); return k == std::wstring::npos ? p : p.substr(k + 1); }
bool IsDir(const std::wstring& p) { DWORD a = GetFileAttributesW(p.c_str()); return a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY); }
bool Exists(const std::wstring& p) { return GetFileAttributesW(p.c_str()) != INVALID_FILE_ATTRIBUTES; }
void MakeDirs(const std::wstring& p) { SHCreateDirectoryExW(nullptr, p.c_str(), nullptr); }

// A player save of the game: "dh2_NNN.savegame" (PlayerSavegame::SG_GetFilename; SG_GetSavegameList drops .bak, settings
// and level files).
bool IsPlayerSave(const std::wstring& n) {
    return n.size() == 16 && n.compare(0, 4, L"dh2_") == 0 && iswdigit(n[4]) && iswdigit(n[5]) && iswdigit(n[6]) && n.compare(7, 9, L".savegame") == 0;
}

// The checkpoint copies of the same hero: dh2_NNN_single.checkpoint and dh2_NNN_multi.checkpoint
// (PlayerSavegame::SG_GetFilename with checkpoint = true). The editor writes the same GEAR into them.
bool IsCheckpoint(const std::wstring& n) {
    return (n.size() == 25 || n.size() == 24) && n.compare(0, 4, L"dh2_") == 0 && iswdigit(n[4]) && iswdigit(n[5]) && iswdigit(n[6]) &&
           (n.compare(7, 18, L"_single.checkpoint") == 0 || n.compare(7, 17, L"_multi.checkpoint") == 0);
}
bool IsEditable(const std::wstring& n) { return IsPlayerSave(n) || IsCheckpoint(n); }
std::vector<std::wstring> CheckpointNames(const std::wstring& save) {   // save = dh2_NNN.savegame
    const std::wstring stem = save.substr(0, 7);
    return {stem + L"_single.checkpoint", stem + L"_multi.checkpoint"};
}

// Folder write with backups: <dir>\editor-backups\<name>.<stamp>, then <name>.bak (the game's own backup name), then the
// new bytes go to a temporary file in editor-backups and replace the save in one rename.
std::string WriteWithBackups(const std::wstring& path, const std::vector<unsigned char>& bytes, std::string& backupOut) {
    const std::wstring dir = DirOf(path), name = NameOf(path);
    const std::wstring bdir = dir + L"\\editor-backups";
    MakeDirs(bdir);
    if (Exists(path)) {
        const std::wstring dated = bdir + L"\\" + name + L"." + W(Stamp());
        if (!CopyFileW(path.c_str(), dated.c_str(), FALSE)) return "could not make the backup copy";
        if (!CopyFileW(path.c_str(), (path + L".bak").c_str(), FALSE)) return "could not write " + U(name) + ".bak";
        backupOut = U(dated);
    }
    const std::wstring tmp = bdir + L"\\" + name + L".new";
    if (!WriteFileAll(tmp, bytes)) return "could not write the new file";
    if (!MoveFileExW(tmp.c_str(), path.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) return "could not replace the save";
    return "";
}

// ---------------------------------------------------------------- processes (adb)
std::wstring g_adb;
std::wstring FindAdb() {
    std::vector<std::wstring> c;
    c.push_back(g_appDir + L"\\adb\\adb.exe");
    c.push_back(g_appDir + L"\\adb.exe");
    wchar_t buf[MAX_PATH * 2];
    for (const wchar_t* var : {L"ANDROID_HOME", L"ANDROID_SDK_ROOT"}) {
        DWORD n = GetEnvironmentVariableW(var, buf, MAX_PATH * 2);
        if (n > 0 && n < MAX_PATH * 2) c.push_back(std::wstring(buf) + L"\\platform-tools\\adb.exe");
    }
    DWORD n = GetEnvironmentVariableW(L"LOCALAPPDATA", buf, MAX_PATH * 2);
    if (n > 0 && n < MAX_PATH * 2) c.push_back(std::wstring(buf) + L"\\Android\\Sdk\\platform-tools\\adb.exe");
    for (auto& p : c) if (Exists(p)) return p;
    if (SearchPathW(nullptr, L"adb.exe", nullptr, MAX_PATH * 2, buf, nullptr) > 0) return buf;
    return L"";
}
// Runs adb with arguments (UTF-8), returns the exit code and stdout+stderr.
int RunAdb(const std::string& args, std::string& out) {
    out.clear();
    if (g_adb.empty()) g_adb = FindAdb();
    if (g_adb.empty()) { out = "adb.exe not found (install Android platform-tools or put adb.exe in the app folder)"; return -1; }
    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    HANDLE rd, wr;
    CreatePipe(&rd, &wr, &sa, 0);
    SetHandleInformation(rd, HANDLE_FLAG_INHERIT, 0);
    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    si.hStdOutput = wr; si.hStdError = wr; si.hStdInput = nullptr;
    PROCESS_INFORMATION pi{};
    std::wstring cmd = L"\"" + g_adb + L"\" " + W(args);
    if (!CreateProcessW(nullptr, &cmd[0], nullptr, nullptr, TRUE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
        CloseHandle(rd); CloseHandle(wr);
        out = "could not start adb";
        return -1;
    }
    CloseHandle(wr);
    char buf[4096];
    DWORD got;
    while (ReadFile(rd, buf, sizeof buf, &got, nullptr) && got > 0) out.append(buf, got);
    CloseHandle(rd);
    WaitForSingleObject(pi.hProcess, 120000);
    DWORD code = 1;
    GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hProcess); CloseHandle(pi.hThread);
    return (int)code;
}
std::string Trim(std::string s) { while (!s.empty() && (s.back() == '\r' || s.back() == '\n' || s.back() == ' ')) s.pop_back(); return s; }
bool SafeSerial(const std::string& s) { if (s.empty()) return false; for (char c : s) if (!(isalnum((unsigned char)c) || c == '-' || c == '.' || c == ':' || c == '_')) return false; return true; }
std::string PhoneDir() { return std::string("/sdcard/Android/data/") + kPackage + "/files/"; }
std::wstring LocalPhoneDir(const std::string& serial) {
    std::string s = serial; for (char& c : s) if (c == ':') c = '_';
    return g_appDir + L"\\phone\\" + W(s);
}

// ---------------------------------------------------------------- API
struct Reply { int code = 200; std::string type = "application/json"; std::string body; };
Reply Err(const std::string& m) { Reply r; r.body = "{\"error\":" + Json(m) + "}"; return r; }

std::string FileListJson(const std::vector<std::wstring>& paths, const std::string& where) {
    std::string o = "{\"files\":[";
    for (size_t i = 0; i < paths.size(); ++i) {
        std::string cps;
        for (const auto& cp : CheckpointNames(NameOf(paths[i]))) {
            if (!Exists(DirOf(paths[i]) + L"\\" + cp)) continue;
            cps += std::string(cps.empty() ? "" : ",") + Json(where.empty() ? U(DirOf(paths[i]) + L"\\" + cp) : where + U(cp));
        }
        WIN32_FILE_ATTRIBUTE_DATA a{};
        GetFileAttributesExW(paths[i].c_str(), GetFileExInfoStandard, &a);
        SYSTEMTIME st; FILETIME lt;
        FileTimeToLocalFileTime(&a.ftLastWriteTime, &lt);
        FileTimeToSystemTime(&lt, &st);
        char info[96];
        std::snprintf(info, sizeof info, "%lu bytes, %04d-%02d-%02d %02d:%02d", a.nFileSizeLow, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
        o += std::string(i ? "," : "") + "{\"name\":" + Json(U(NameOf(paths[i]))) + ",\"path\":" + Json(where.empty() ? U(paths[i]) : where + U(NameOf(paths[i]))) +
             ",\"where\":" + Json(where.empty() ? U(DirOf(paths[i])) : where.substr(4)) + ",\"info\":" + Json(info) + ",\"checkpoints\":[" + cps + "]}";
    }
    return o + "]}";
}
std::vector<std::wstring> SavesIn(const std::wstring& dir) {
    std::vector<std::wstring> r;
    WIN32_FIND_DATAW fd;
    HANDLE h = FindFirstFileW((dir + L"\\dh2_*.savegame").c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) return r;
    do { if (IsPlayerSave(fd.cFileName)) r.push_back(dir + L"\\" + fd.cFileName); } while (FindNextFileW(h, &fd));
    FindClose(h);
    return r;
}

Reply ApiLocations() {
    Reply r;
    r.body = "{\"locations\":[{\"id\":\"folder\",\"label\":\"Open folder\",\"needsPath\":true,\"browse\":true,\"placeholder\":\"folder with dh2_000.savegame\"},{\"id\":\"adb\",\"label\":\"Phone over USB (adb)\",\"needsPath\":true,\"placeholder\":\"phone serial - only when several are connected\"}]}";
    return r;
}

// adb: first device in state "device" (or the one named by arg).
std::string AdbSerial(const std::string& want, std::string& err) {
    std::string out;
    if (RunAdb("devices", out) != 0) { err = Trim(out); return ""; }
    std::vector<std::string> found;
    size_t p = 0;
    while (p < out.size()) {
        size_t e = out.find('\n', p); if (e == std::string::npos) e = out.size();
        std::string line = Trim(out.substr(p, e - p)); p = e + 1;
        size_t t = line.find('\t');
        if (t != std::string::npos && line.substr(t + 1) == "device") found.push_back(line.substr(0, t));
    }
    if (!want.empty()) { for (auto& s : found) if (s == want) return s; err = "phone " + want + " not connected"; return ""; }
    if (found.empty()) { err = "no phone found by adb (connect it with USB debugging on and accept the prompt on the phone)"; return ""; }
    if (found.size() > 1) { err = "more than one phone/emulator is connected; type its serial in the box: "; for (auto& s : found) err += s + " "; return ""; }
    return found[0];
}

Reply ApiList(const std::string& loc, const std::string& arg) {
    if (loc == "folder") {
        std::wstring dir = W(arg);
        while (!dir.empty() && (dir.back() == L'\\' || dir.back() == L'/')) dir.pop_back();
        if (dir.empty() || !IsDir(dir)) return Err("folder not found: " + arg);
        Reply r; r.body = FileListJson(SavesIn(dir), ""); return r;
    }
    if (loc == "adb") {
        std::string err;
        const std::string serial = AdbSerial(arg, err);
        if (serial.empty()) return Err(err);
        if (!SafeSerial(serial)) return Err("unexpected device name");
        std::string out;
        RunAdb("-s " + serial + " shell ls " + PhoneDir(), out);
        const std::wstring local = LocalPhoneDir(serial);
        MakeDirs(local);
        std::vector<std::wstring> got;
        size_t p = 0;
        while (p < out.size()) {
            size_t e = out.find('\n', p); if (e == std::string::npos) e = out.size();
            std::string n = Trim(out.substr(p, e - p)); p = e + 1;
            if (!IsEditable(W(n))) continue;
            std::string o2;
            const std::wstring dst = local + L"\\" + W(n);
            if (RunAdb("-s " + serial + " pull \"" + PhoneDir() + n + "\" \"" + U(dst) + "\"", o2) != 0) return Err("adb pull failed: " + Trim(o2));
            if (IsPlayerSave(W(n))) got.push_back(dst);
        }
        if (got.empty()) return Err("no dh2_NNN.savegame on the phone in " + PhoneDir() + " (play until the game has saved once)");
        Reply r; r.body = FileListJson(got, "adb:" + serial + ":"); return r;
    }
    return Err("unknown location");
}

// path is a local path, or "adb:<serial>:<name>" (read from the copy pulled by ApiList).
bool ParseAdbPath(const std::string& path, std::string& serial, std::string& name) {
    if (path.compare(0, 4, "adb:") != 0) return false;
    size_t c = path.rfind(':');
    serial = path.substr(4, c - 4); name = path.substr(c + 1);
    return SafeSerial(serial) && IsEditable(W(name));
}

Reply ApiRead(const std::string& path) {
    std::wstring local;
    std::string serial, name;
    if (ParseAdbPath(path, serial, name)) local = LocalPhoneDir(serial) + L"\\" + W(name);
    else { local = W(path); if (!IsEditable(NameOf(local))) return Err("not a dh2_NNN.savegame file"); }
    std::vector<unsigned char> d;
    if (!ReadFileAll(local, d)) return Err("cannot read " + U(local));
    Reply r; r.body = "{\"data\":\"" + B64(d) + "\"}"; return r;
}

Reply ApiWrite(const std::string& path, const std::string& body) {
    std::vector<unsigned char> bytes;
    if (!Unb64(body, bytes) || bytes.size() < 8) return Err("bad data");
    std::string serial, name, backup;
    if (ParseAdbPath(path, serial, name)) {
        std::string out;
        // The game must not run: it would overwrite the file when it saves next (pidof prints the pid when it runs).
        RunAdb("-s " + serial + " shell pidof " + kPackage, out);
        if (!Trim(out).empty()) return Err("the game is running on the phone - close it first (swipe it away from recent apps), then save again");
        const std::wstring local = LocalPhoneDir(serial);
        const std::wstring bdir = local + L"\\editor-backups";
        MakeDirs(bdir);
        // backup 1: the phone's current file, pulled fresh into editor-backups
        const std::wstring dated = bdir + L"\\" + W(name) + L"." + W(Stamp());
        if (RunAdb("-s " + serial + " pull \"" + PhoneDir() + name + "\" \"" + U(dated) + "\"", out) != 0) return Err("backup pull failed, nothing written: " + Trim(out));
        // backup 2: on the phone, <name>.bak (the game's own backup name; the game reads it if the save is damaged)
        RunAdb("-s " + serial + " shell cp \"" + PhoneDir() + name + "\" \"" + PhoneDir() + name + ".bak\"", out);
        const std::wstring tmp = bdir + L"\\" + W(name) + L".new";
        if (!WriteFileAll(tmp, bytes)) return Err("could not write the new file");
        if (RunAdb("-s " + serial + " push \"" + U(tmp) + "\" \"" + PhoneDir() + name + "\"", out) != 0) return Err("adb push failed: " + Trim(out) + " (backup: " + U(dated) + ")");
        // verify
        const std::wstring check = bdir + L"\\" + W(name) + L".check";
        std::vector<unsigned char> back;
        if (RunAdb("-s " + serial + " pull \"" + PhoneDir() + name + "\" \"" + U(check) + "\"", out) != 0 || !ReadFileAll(check, back) || back != bytes)
            return Err("the file on the phone does not match after the push (backup: " + U(dated) + ")");
        DeleteFileW(check.c_str());
        WriteFileAll(local + L"\\" + W(name), bytes);
        Reply r; r.body = "{\"ok\":true,\"note\":" + Json("written to the phone (" + serial + ")") + ",\"backup\":" + Json(U(dated)) + "}"; return r;
    }
    const std::wstring local = W(path);
    if (!IsEditable(NameOf(local))) return Err("not a dh2_NNN.savegame file");
    const std::string e = WriteWithBackups(local, bytes, backup);
    if (!e.empty()) return Err(e);
    Reply r; r.body = "{\"ok\":true,\"note\":\"written\",\"backup\":" + Json(backup) + "}"; return r;
}

Reply ApiBrowse() {
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    BROWSEINFOW bi{};
    bi.lpszTitle = L"Folder with dh2_000.savegame (for example a copy of the game's files folder)";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    PIDLIST_ABSOLUTE id = SHBrowseForFolderW(&bi);
    std::string dir;
    if (id) { wchar_t p[MAX_PATH]; if (SHGetPathFromIDListW(id, p)) dir = U(p); CoTaskMemFree(id); }
    CoUninitialize();
    Reply r; r.body = "{\"dir\":" + Json(dir) + "}"; return r;
}

// ---------------------------------------------------------------- HTTP
Reply Static(std::string path) {
    if (path == "/" || path.empty()) path = "/index.html";
    if (path.find("..") != std::string::npos || path.find('\\') != std::string::npos) return Err("bad path");
    std::vector<unsigned char> d;
    std::wstring f = g_appDir + L"\\web" + W(path);
    for (auto& c : f) if (c == L'/') c = L'\\';
    if (!ReadFileAll(f, d)) { Reply r; r.code = 404; r.type = "text/plain"; r.body = "not found"; return r; }
    Reply r;
    r.body.assign(d.begin(), d.end());
    const auto ends = [&](const char* e) { return path.size() >= std::strlen(e) && path.compare(path.size() - std::strlen(e), std::strlen(e), e) == 0; };
    r.type = ends(".html") ? "text/html; charset=utf-8" : ends(".js") ? "application/javascript; charset=utf-8" : ends(".css") ? "text/css" : "application/octet-stream";
    return r;
}

void Handle(SOCKET s) {
    std::string req;
    char buf[65536];
    size_t headerEnd = std::string::npos;
    while (headerEnd == std::string::npos) {
        int n = recv(s, buf, sizeof buf, 0);
        if (n <= 0) { closesocket(s); return; }
        req.append(buf, n);
        headerEnd = req.find("\r\n\r\n");
        if (req.size() > (1u << 20) && headerEnd == std::string::npos) { closesocket(s); return; }
    }
    std::string head = req.substr(0, headerEnd);
    std::string body = req.substr(headerEnd + 4);
    auto header = [&](const char* name) -> std::string {
        std::string h = head; std::string k = std::string("\r\n") + name + ":";
        for (auto& c : h) c = (char)tolower((unsigned char)c);
        std::string kl = k; for (auto& c : kl) c = (char)tolower((unsigned char)c);
        size_t p = h.find(kl);
        if (p == std::string::npos) return "";
        size_t e = head.find("\r\n", p + 2);
        std::string v = head.substr(p + kl.size(), e - p - kl.size());
        while (!v.empty() && v[0] == ' ') v.erase(0, 1);
        return v;
    };
    const size_t len = (size_t)std::strtoull(header("Content-Length").c_str(), nullptr, 10);
    while (body.size() < len) {
        int n = recv(s, buf, sizeof buf, 0);
        if (n <= 0) break;
        body.append(buf, n);
    }
    const size_t sp1 = head.find(' '), sp2 = head.find(' ', sp1 + 1);
    const std::string method = head.substr(0, sp1);
    std::string target = head.substr(sp1 + 1, sp2 - sp1 - 1);
    std::string query;
    const size_t qm = target.find('?');
    if (qm != std::string::npos) { query = target.substr(qm + 1); target = target.substr(0, qm); }

    Reply r;
    char hostExpect[64];
    std::snprintf(hostExpect, sizeof hostExpect, "127.0.0.1:%d", g_port);
    if (header("Host") != hostExpect) { r = Err("bad host"); r.code = 403; }
    else if (target.compare(0, 5, "/api/") == 0) {
        if (header("X-DH2-Token") != g_token) { r = Err("bad token"); r.code = 403; }
        else if (target == "/api/locations") r = ApiLocations();
        else if (target == "/api/list") r = ApiList(QueryParam(query, "loc"), QueryParam(query, "arg"));
        else if (target == "/api/read") r = ApiRead(QueryParam(query, "path"));
        else if (target == "/api/write" && method == "POST") r = ApiWrite(QueryParam(query, "path"), body);
        else if (target == "/api/browse") r = ApiBrowse();
        else r = Err("unknown call");
    } else r = Static(target);

    char hdr[256];
    std::snprintf(hdr, sizeof hdr, "HTTP/1.1 %d %s\r\nContent-Type: %s\r\nContent-Length: %zu\r\nCache-Control: no-store\r\nConnection: close\r\n\r\n",
                  r.code, r.code == 200 ? "OK" : "Error", r.type.c_str(), r.body.size());
    std::string out = std::string(hdr) + r.body;
    size_t sent = 0;
    while (sent < out.size()) { int n = send(s, out.data() + sent, (int)(out.size() - sent), 0); if (n <= 0) break; sent += n; }
    shutdown(s, SD_SEND);
    closesocket(s);
}

std::wstring FindEdge() {
    wchar_t buf[MAX_PATH * 2];
    std::vector<std::wstring> c;
    for (const wchar_t* var : {L"ProgramFiles(x86)", L"ProgramFiles", L"LOCALAPPDATA"}) {
        DWORD n = GetEnvironmentVariableW(var, buf, MAX_PATH * 2);
        if (n > 0 && n < MAX_PATH * 2) c.push_back(std::wstring(buf) + L"\\Microsoft\\Edge\\Application\\msedge.exe");
    }
    for (auto& p : c) if (Exists(p)) return p;
    return L"";
}

}  // namespace

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR cmdLine, int) {
    wchar_t exe[MAX_PATH * 2];
    GetModuleFileNameW(nullptr, exe, MAX_PATH * 2);
    g_appDir = DirOf(exe);
    if (!Exists(g_appDir + L"\\web\\index.html")) {
        MessageBoxW(nullptr, L"The \"web\" folder is missing next to the program. Keep the whole save-editor folder together.", L"DH2 Save Editor", MB_ICONERROR);
        return 1;
    }
    // per-run token
    LARGE_INTEGER pc; QueryPerformanceCounter(&pc);
    unsigned long long seed = pc.QuadPart ^ ((unsigned long long)GetCurrentProcessId() << 32) ^ (unsigned long long)std::time(nullptr);
    for (int i = 0; i < 24; ++i) { seed = seed * 6364136223846793005ull + 1442695040888963407ull; g_token += "abcdefghijklmnopqrstuvwxyz0123456789"[(seed >> 33) % 36]; }

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    SOCKET ls = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in a{};
    a.sin_family = AF_INET;
    a.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    a.sin_port = 0;
    // a fixed port can be asked for on the command line (--port N), used by the automated checks
    int fixed = 0;
    if (cmdLine && std::wcsstr(cmdLine, L"--port ")) fixed = _wtoi(std::wcsstr(cmdLine, L"--port ") + 7);
    a.sin_port = htons((u_short)fixed);
    if (bind(ls, (sockaddr*)&a, sizeof a) != 0 || listen(ls, 16) != 0) {
        MessageBoxW(nullptr, L"Could not open a local port.", L"DH2 Save Editor", MB_ICONERROR);
        return 1;
    }
    int alen = sizeof a;
    getsockname(ls, (sockaddr*)&a, &alen);
    g_port = ntohs(a.sin_port);
    std::thread([ls]() {
        for (;;) {
            SOCKET s = accept(ls, nullptr, nullptr);
            if (s == INVALID_SOCKET) return;
            std::thread(Handle, s).detach();
        }
    }).detach();

    wchar_t url[160];
    std::swprintf(url, 160, L"http://127.0.0.1:%d/index.html#t=%hs", g_port, g_token.c_str());
    const bool noBrowser = cmdLine && std::wcsstr(cmdLine, L"--no-browser");
    if (cmdLine && std::wcsstr(cmdLine, L"--print-url")) {
        // automated checks: write the URL next to the exe
        std::vector<unsigned char> u; std::string s = U(url); u.assign(s.begin(), s.end());
        WriteFileAll(g_appDir + L"\\last-url.txt", u);
    }
    const std::wstring edge = noBrowser ? L"" : FindEdge();
    if (!edge.empty()) {
        const std::wstring profile = g_appDir + L"\\data\\edge-profile";
        MakeDirs(profile);
        std::wstring cmd = L"\"" + edge + L"\" --app=" + url + L" --user-data-dir=\"" + profile + L"\" --no-first-run --no-default-browser-check --window-size=1100,900";
        STARTUPINFOW si{}; si.cb = sizeof si;
        PROCESS_INFORMATION pi{};
        if (CreateProcessW(nullptr, &cmd[0], nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
            WaitForSingleObject(pi.hProcess, INFINITE);   // the app window was closed
            CloseHandle(pi.hProcess); CloseHandle(pi.hThread);
            return 0;
        }
    }
    if (!noBrowser) ShellExecuteW(nullptr, L"open", url, nullptr, nullptr, SW_SHOWNORMAL);
    MessageBoxW(nullptr, L"The DH2 Save Editor is open in your web browser.\nPress OK to quit the editor.", L"DH2 Save Editor", MB_OK);
    return 0;
}
