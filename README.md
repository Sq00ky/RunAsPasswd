```
______             ___      ______                         _ 
| ___ \           / _ \     | ___ \                       | |
| |_/ /   _ _ __ / /_\ \___ | |_/ /_ _ ___ _____      ____| |
|    / | | | '_ \|  _  / __||  __/ _` / __/ __\ \ /\ / / _` |
| |\ \ |_| | | | | | | \__ \| | | (_| \__ \__ \\ V  V / (_| |
\_| \_\__,_|_| |_\_| |_/___/\_|  \__,_|___/___/ \_/\_/ \__,_|                                                          
```
I'll preface this by saying I'm sure there's plenty of other already existing projects out there that have already written this and have probably done so much better than I have. I built this to expand my personal portfolio and for my own education. If you use it, cool :D I appreciate it. 

### Overview

This is a simple "RunAs.exe" clone that has the ability to specify a ``-p || --password`` flag. There has been several instances where I have not been in a fully interactive command prompt and have needed to use the ``runas.exe`` command. Mimikatz's sekurlsa::pth command is great, but obfuscating Mimikatz is not! Sometimes I just need a simple solution. Why bring a sledge hammer when all you need is a normal hammer? So, that's why I built RunAsPasswd. One day I hope to implement a ``-h | --hash`` mechanism for passing a NTLM hash, though I need to do a bit more research on how to actually implement that first.

### Technical Details
This clone utilizies the ``CreateProcessWithLogonW`` Windows API and supports ``LOGON_NETCREDENTIALS_ONLY`` and the default (which I believe is ``LOGON_WITH_PROFILE``). You can control which is used by passing with ``-n || --netonly`` flag. If the flag is provided, it with run the program with ``LOGON_NETCREDENTIALS_ONLY``, else, the default.

### Help & Changes
The program is fairly similar to the ``runas.exe`` command on Windows, just with a few minor changes. Flags are now passed in a unix stlye vs the windows /arg style, so /user: is now -u or --username
```
C:\Users\nanaisu\Downloads>.\RunAsPasswd.exe
Usage: RunAsPasswd.exe -u da-nanaisu -p P@ssw0rd123! -d CONTOSO -c command

Options:
  -u USERNAME, --username USERNAME
                        the username to use for the process
  -p PASSWORD, --password PASSWORD
                        the password to use for the process
  -d DOMAIN, --domain DOMAIN
                        the domain to use for the process
  -c COMMAND, --command COMMAND
                        the command to execute with the specified credentials
  -a ARGS, --args ARGS
                        the arguments to pass to the command
  -n, --netonly         run with the NETLOGON flag (may result in process creation with invalid credentials)

This program is a runas.exe clone, but with the added ability to provide the password as a command line argument. It uses the CreateProcessWithLogonW function to create a new process with the given credentials.
```

### Usage
Usage is fairly simple, specify the username, password, domain name, command and optionally argument and netonly flag. Sample usage looks like so:
```
C:\Users\nanaisu\Downloads>.\RunAsPasswd.exe -u administrator -p P@ssw0rd123! -d CONTOSO -c shell.exe
[+] Command executed successfully!

C:\Users\hubert\Downloads>.\RunAsPasswd.exe -u administrator -p P@ssw0rd123! -d CONTOSO -c shell.exe -n
[!] Running with the NETLOGON flag may result in session creation with invalid credentials.
[+] Command executed successfully!
```

<img src="https://raw.githubusercontent.com/Sq00ky/RunAsPasswd/main/Image/cli.png">

And the results in Metasploit...

<img src="https://raw.githubusercontent.com/Sq00ky/RunAsPasswd/main/Image/msf.png">

I'll point out a quick note here that I thought was interesting - There are two sessions returned, one with the netlogon flag running as Hubert, and the other as Administrator. This is due to the behavior of the netlogon flag. From what I understand, the only real trace that can be found that netlogon worked correctly is the Kerberos tickets stored in memory. Interestingly enough, no process tokens are created for the Administrator user.

<img src="https://raw.githubusercontent.com/Sq00ky/RunAsPasswd/main/Image/klist.png">

Anyways - I'm no Windows expert, but I thought that was an interesting thing to note. At first, I was going to have it default to ``LOGON_NETCREDENTIALS_ONLY``, but it turns out that may be confusing for some individuals, so I left it as is.

### Disclaimers
I'm not responsible for any damage this program may cause. Use at your own risk. This program is meant for educational purposes only.

### Resources
- https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-createprocesswithlogonw
- https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes
