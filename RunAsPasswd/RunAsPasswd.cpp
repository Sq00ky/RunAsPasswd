#include <Windows.h>
#include <iostream>

using namespace std;
int main(int argc, char* argv[])
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    string username, password, domain, args, command;
    DWORD netonly;
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-u" || arg == "--username") {
            if (i + 1 < argc) {
                username = argv[++i];
            }
            else {
                cout << "-u || --username option requires one argument." << std::endl;
                exit(1);
            }
        }
        else if (arg == "-p" || arg == "--password") {
            if (i + 1 < argc) {
                password = argv[++i];
            }
            else {
                cout << "-p || --password option requires one argument." << std::endl;
                exit(1);
            }
        }
        else if (arg == "-d" || arg == "--domain") {
            if (i + 1 < argc) {
                domain = argv[++i];
            }
            else {
                cout << "-d || --domain option requires one argument." << std::endl;
                exit(1);
            }

        }
        else if (arg == "-c" || arg == "--command") {
            if (i + 1 < argc) {
                command = argv[++i];
            }
            else {
                cout << "-c || --command option requires one argument." << std::endl;
                exit(1);
            }
        }
        else if (arg == "-a" || arg == "--args") {
            if (i + 1 < argc) {
                args = argv[++i];
            }
        }
        else if (arg == "-n" || arg == "--netonly") {
            netonly = 0x00000002;
            cout << "[!] Running with the NETLOGON flag may result in session creation with invalid credentials.\n";
        }
    }

        if (username.empty() && password.empty() && domain.empty() && args.empty() && command.empty()) {
            cout << "Usage: RunAsPasswd.exe -u da-nanaisu -p P@ssw0rd123! -d CONTOSO -c command\n\nOptions:\n  -u USERNAME, --username USERNAME\n                        the username to use for the process\n  -p PASSWORD, --password PASSWORD\n                        the password to use for the process\n  -d DOMAIN, --domain DOMAIN\n                        the domain to use for the process\n  -c COMMAND, --command COMMAND\n                        the command to execute with the specified credentials\n  -a ARGS, --args ARGS\n                        the arguments to pass to the command\n  -n, --netonly         run with the NETLOGON flag (may result in process creation with invalid credentials)\n\nThis program is a runas.exe clone, but with the added ability to provide the password as a command line argument. It uses the CreateProcessWithLogonW function to create a new process with the given credentials.\n";
            exit(1);
        }
        if (username.empty()) {
            cerr << "-u || --username option is required." << endl;
            return 1;
        }
        if (password.empty()) {
            cerr << "-p || --password option is required." << endl;
            return 1;
        }
        if (domain.empty()) {
            cerr << "-d || --domain option is required." << endl;
            return 1;
        }
        if (command.empty()) {
            cerr << "-c || --command option is required." << endl;
        }
        wchar_t w_username[1024], w_password[1024], w_domain[1024], w_command[1024], w_args[1024];
        mbstowcs(w_password, password.data(), sizeof(password));
        mbstowcs(w_username, username.data(), sizeof(username));
        mbstowcs(w_domain, domain.data(), sizeof(domain));
        mbstowcs(w_command, command.data(), sizeof(command));

        if (args.empty()) {
            if(CreateProcessWithLogonW(w_username, w_domain, w_password, netonly, w_command, NULL, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)){
                cout << "[+] Command executed successfully!\n";
            }
            else {
                cout << "[-] Failed to execute command. Error: " << GetLastError();
            }
        }
        else {
            string combinedArgs;
            combinedArgs = command + " " + args;
            mbstowcs(w_args, combinedArgs.data(), sizeof(args));
            if (CreateProcessWithLogonW(w_username, w_domain, w_password, netonly, w_command, w_args, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
                cout << "[+] Command executed successfully!\n";
            }
            else {
                cout << "[-] Failed to execute command. Error: " << GetLastError();
            }
        }
    }
