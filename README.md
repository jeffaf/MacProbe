# MacProbe

**MacProbe** is a lightweight, C-based reconnaissance tool designed for macOS  security auditing. It performs rapid local environment checks to identify sandboxes, virtual machines, and the system's security posture.

## Features

*   **Environment Detection**:
    *   Identifies Hardware Model (e.g., `MacBookPro18,3`).
    *   Detects Virtual Machines (VMware, Parallels, UTM) using `sysctl` and CPU features.
*   **Security Posture**:
    *   Checks **SIP** (System Integrity Protection) status.
    *   Checks **Gatekeeper** status.
*   **Privacy (TCC) Enumeration**:
    *   Checks access to sensitive user data protected by TCC (Transparency, Consent, and Control).
    *   Probes for: Full Disk Access, Safari History, Messages/SMS, and Mail.
*   **Entitlement Scanner**:
    *   Scans `/Applications` for apps with risky entitlement combinations:
        *   `com.apple.security.cs.disable-library-validation`
        *   `com.apple.security.cs.allow-unsigned-executable-memory`
    *   Identifies potential targets for **DYLD_INSERT_LIBRARIES** injection.
*   **Injection Test**:
    *   Interactive test to verify if a vulnerable app allows code injection.
    *   Injects a benign dylib that prints a success message to stdout and syslog.

## Build & Install

MacProbe is written in C and requires no external dependencies other than the standard macOS SDK.

```bash
# Clone the repository
git clone https://github.com/jeffaf/MacProbe.git
cd MacProbe

# Compile
make
```

## Usage

Simply run the binary from your terminal:

```bash
./macprobe
```

**Example Output:**
```text
MacProbe - MacOS Security Check Tool
====================================

Model: MacBookAir10,1
OS Version: 15.6.1
Environment: Physical Hardware (Likely)

[+] Security Posture:
  [+] SIP: Enabled
  [+] Gatekeeper: Enabled
  [?] Checking TCC (Privacy) Permissions for current context:
    [-] No sensitive TCC data accessible (Clean/Sandboxed)

[+] Entitlement Scanner (DYLD Injection Candidates):
    Scanning /Applications for risky entitlements...
    [!] VULNERABLE: /Applications/VulnerableApp.app
        -> Has 'disable-library-validation' AND 'allow-unsigned-executable-memory'
        -> Potential DYLD_INSERT_LIBRARIES injection target!
        [?] Test injection on this app? (y/n): y
        [*] Attempting injection...
        [*] Running in background: DYLD_INSERT_LIBRARIES=.../inject.dylib ... &
        [*] Check stdout/syslog for 'Hello from dylib!'
    [=] Scan complete.
```

## Disclaimer

This tool is for **educational and authorized security testing purposes only**. Use responsibly.
