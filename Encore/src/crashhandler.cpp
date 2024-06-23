#include "crashhandler.h"

#ifndef WIN32
void registerCrashHandler() {}
#else

#ifndef GIT_COMMIT_HASH
#define GIT_COMMIT_HASH
#endif

#ifndef ENCORE_VERSION
#define ENCORE_VERSION
#endif

#define NOMINMAX

#include "client/crashpad_client.h"
#include "client/crash_report_database.h"
#include "client/settings.h"

using namespace crashpad;

std::wstring GetExeDirPath()
{
	HMODULE hModule = GetModuleHandleW(NULL);
	WCHAR path[MAX_PATH];
	DWORD retVal = GetModuleFileNameW(hModule, path, MAX_PATH);
	if (retVal == 0) return NULL;

	wchar_t * lastBackslash = wcsrchr(path, '\\');
	if (lastBackslash == NULL) return NULL;
	*lastBackslash = 0;

	return path;
}

std::wstring GetCrashpadHandlerPath(std::wstring exeDirPath)
{
	return exeDirPath + L"\\crashpad_handler.exe";
}

std::wstring GetAttachmentPath(std::wstring exeDirPath)
{
	return exeDirPath + L"\\attachment.txt";
}

std::wstring GetReportsPath(std::wstring exeDirPath)
{
	return exeDirPath + L"\\crashpad";
}

std::wstring GetMetricsPath(std::wstring exeDirPath)
{
	return exeDirPath + L"\\crashpad";
}

CrashpadClient *initializeCrashpad(char *dbName, char *appName, char *appVersion)
{
	// Path to directory containing myCrashpadCrasher.exe
	std::wstring exeDirPath = GetExeDirPath();

	// Path to crashpad_handler.exe
	std::wstring handlerPath = GetCrashpadHandlerPath(exeDirPath);
	if (INVALID_FILE_ATTRIBUTES == GetFileAttributesW(handlerPath.c_str()))
	{
		return NULL;
	}

	// Ensure that handler is shipped with your application
	base::FilePath handler(handlerPath);

	// Crashpad minidump files will be saved in .\crashpad
	std::wstring reportsPath = GetReportsPath(exeDirPath);
	base::FilePath reportsDir(reportsPath);

	// Crashpad metrics metadata will be saved in .\crashpad
	std::wstring metricsPath = GetMetricsPath(exeDirPath);
	base::FilePath metricsDir(metricsPath);

	// Configure url with your BugSplat database
	std::string url;
	url = "https://";
	url += dbName;
	url += ".bugsplat.com/post/bp/crash/crashpad.php";

	// Metadata that will be posted to BugSplat
	std::map<std::string, std::string> annotations;
	annotations["format"] = "minidump";                 // Required: Crashpad setting to save crash as a minidump
	annotations["product"].assign(appName);             // Required: BugSplat appName
	annotations["version"].assign(appVersion);      	// Required: BugSplat appVersion
	// annotations["key"] = "Sample key";                  // Optional: BugSplat key field
	// annotations["user"] = "fred@bugsplat.com";          // Optional: BugSplat user email
	// annotations["list_annotations"] = "Sample comment";	// Optional: BugSplat crash description

	// Disable crashpad rate limiting 
	std::vector<std::string> arguments;
	arguments.push_back("--no-rate-limit");

	// Initialize crashpad database
	std::unique_ptr<CrashReportDatabase> database = crashpad::CrashReportDatabase::Initialize(reportsDir);
	if (database == NULL) return NULL;

	// Enable automated crash uploads
	Settings *settings = database->GetSettings();
	if (settings == NULL) return NULL;
	settings->SetUploadsEnabled(true);

	// Files to upload with the crash report - default bundle size limit is 2MB
	std::vector<base::FilePath> attachments;
	std::wstring attachmentPath = GetAttachmentPath(exeDirPath);
	base::FilePath attachment(attachmentPath);
	attachments.push_back(attachment);

	// Start crash handler
	CrashpadClient *client = new CrashpadClient();
	bool status = client->StartHandler(handler, reportsDir, metricsDir, url, annotations, arguments, true, true, attachments);
	if (status == false) return NULL;

	// Wait for handler to initialize
	status = client->WaitForHandlerStart(INFINITE);
	if (status == false) return NULL;

	return client;
}


void registerCrashHandler()
{
    char* dbName = (char*)"raylib_encore_ermil_experiment";
    char* appName = (char*)"Encore";

	#if defined(DEBUG)
	char* appVersion = (char*)GIT_COMMIT_HASH;
	#else
    char* appVersion = (char*)ENCORE_VERSION;
	#endif

    initializeCrashpad(dbName, appName, appVersion);
}

#endif