# ELF Parser
## What problem are you trying to solve?
I was looking for ELF based malware on https://malwr.com and couldn't help but notice how little analysis gets done on them. More surprising, to me, is that even well known malware like Kaiten gets flagged by very few AV (https://malwr.com/analysis/NThiZTU0MWUwZGI2NDAzYWI5YWU2ZjkzNTJmYTNjZTY/). ELF Parser attempts to move ELF malware analysis forward by quickly providing basic information and static analysis of the binary. The end goal of ELF Parser is to indicate to the analyst if it thinks the binary is malicious / dangerous and if so why.

## Website
www.elfparser.com

## How do I compile it?
ELF Parser can be compiled on Windows, OS X, or Linux (demangling and unit tests don't work on Windows). Windows uses the VS 2010 project in the base directory for compilation whereas Linux/OS X uses CMake. Compiling on Linux goes like this:

```
cd ~/elfparser
mkdir build
cd build/
cmake ..
make
```
Obviously, you will need to resolve any dependencies. Specifically, Boost is required and Qt is required for the GUI build.

## Compile Targets
ELF Parser has a number of compilation targets that can be configured by CMakeLists.txt. The targets are:
* Unit tests
* CLI build
* GUI build
* Visual Studios build

## CLI Usage
The user can pass in a single file (-f) or a directory (-d) of files:
```
./elfparser-cli --help
options:
  --help                 A list of command line options
  --version              Display version information
  -f [ --file ] arg      The ELF file to examine
  -d [ --directory ] arg The directory to look through.
  -r [ --reasons ]       Print the scoring reasons
  -c [ --capabilities ]  Print the files observed capabilities
  -p [ --print ]         Print the ELF files various parsed structures.
```
## Example Output
```
/elfparser-cli -f /bin/ls -c -r
/bin/ls - Score: 6
---- Scoring Reasons ----
4 -> Process manipulation functions
2 -> Environment variable manipulation
---- Detected Capabilities ----
    File Functions
        fclose() found
    Process Manipulation
        raise() found
    Environment Variables
        getenv() found

```
```
./elfparser-cli -f ~/Desktop/malwr.BillGates -r -c
/home/charlie/Desktop/malwr.BillGates - Score: 241
---- Scoring Reasons ----
8 -> Network functions
24 -> Process manipulation functions
8 -> Information gathering
8 -> Environment variable manipulation
3 -> Syslog manipulation functions
20 -> Shell commands
50 -> Packed
60 -> Hard coded IPv4 addresses
10 -> Anti debug techniques
50 -> Dropper functionaltiy
---- Detected Capabilities ----
    File Functions
        fclose() found
        feof() found
        fopen() found
        funlockfile() found
        unlink() found
    Network Functions
        accept() found
        bind() found
        connect() found
        inet_addr() found
        listen() found
        recv() found
        sendto() found
        socket() found
    Process Manipulation
        clone() found
        daemon() found
        execve() found
        fork() found
        kill() found
        raise() found
    Random Functions
        rand() found
        random_r() found
        srand() found
        srandom_r() found
    Information Gathering
        Examines /proc/cpuinfo
        Examines /proc/meminfo
        Examines /proc/stat
        access() found
        fstat() found
        getpagesize() found
        phys_pages_info() found
        uname() found
    Environment Variables
        clearenv() found
        getenv() found
        setenv() found
        unsetenv() found
    System Log
        closelog() found
        openlog() found
        vsyslog() found
    Shell
        chmod 0755 %s
        system() found
    Packed
        UPX copyright string found
        UPX signature found
    IP Addresses
        1.0.0.0
        1.0.0.1
        10.0.0.0
        10.255.255.255
        127.0.0.0
        127.0.0.1
        127.255.255.255
        172.16.0.0
        172.31.255.255
        192.168.0.0
        192.168.255.255
        254.255.255.254
        255.0.0.0
        8.8.4.4
        8.8.8.8
    Anti-Debug
        Fake dynamic symbol table in sections
    Dropper
        Embedded ELF binary found at file offset 0xb7730 (751408)
```

## Shortcomings
This tool has a number of shortcomings. From the standpoint of a programmer I'm unhappy with all of the hardcoded values (ie, detection of BillGates, examination of symbols). If anyone has a better design I'm all ears!

## Report Bugs
Please report any issues, bugs, or feature requests on Github or contact me at admin@elfparser.com. Feel free to submit pull requests or email patches.

## Source License
GPLv3. See the LICENSE file.