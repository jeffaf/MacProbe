# MacProbe 🕵️‍♂️💻

**MacProbe** is a lightweight, C-based reconnaissance tool designed for macOS red teaming and security auditing. It performs rapid local environment checks to identify sandboxes, virtual machines, and the system's security posture.

## 🚀 Features

*   **Environment Detection**:
    *   Identifies Hardware Model (e.g., `MacBookPro18,3`).
    *   Detects Virtual Machines (VMware, Parallels, UTM) using `sysctl` and CPU features.
*   **Security Posture**:
    *   Checks **SIP** (System Integrity Protection) status.
    *   Checks **Gatekeeper** status.
*   **Privacy (TCC) Enumeration**:
    *   Checks access to sensitive user data protected by TCC (Transparency, Consent, and Control).
    *   Probes for: Full Disk Access, Safari History, Messages/SMS, and Mail.

## 🛠️ Build & Install

MacProbe is written in C and requires no external dependencies other than the standard macOS SDK.

```bash
# Clone the repository
git clone https://github.com/jeffaf/MacProbe.git
cd MacProbe

# Compile
make
```

## 📖 Usage

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
```

## ⚠️ Disclaimer

This tool is for **educational and authorized security testing purposes only**. Use responsibly.
