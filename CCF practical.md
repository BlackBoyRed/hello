Of course. This practical covers three classic password cracking tools: **Hashcat**, **John the Ripper**, and **RainbowCrack**. Here is a detailed explanation of each part of your practical, breaking down what each command does and why.

### ## Overall Concept: Password Cracking

First, remember that passwords are not stored as plain text. They are stored as **hashes**—the output of a one-way mathematical function.

- `password` -> `md5sum` -> `5f4dcc3b5aa765d61d8327deb882cf99`
    

You cannot reverse this process directly. Password cracking is the process of guessing a password, hashing it, and comparing it to the target hash. If they match, you've found the password. Your practical demonstrates three different ways to do this.

---

### ## 1. Hashcat: The Dictionary Attack 📖

Hashcat is a very fast, modern password cracker that can use your GPU to accelerate the guessing process. This section uses a **dictionary attack**, which means it takes a list of common passwords (a "wordlist") and tries every single one.

#### ### Creating the Hash File

Bash

```
echo -n 'password' | md5sum | awk '{print $1}' > hash.txt
echo -n 'password123' | md5sum | awk '{print $1}' >> hash.txt
...
```

Let's break down this chain of commands:

1. `echo -n 'password'`: This prints the string 'password'. The `-n` is **very important**; it tells `echo` not to add a newline character at the end. Without it, you would be hashing `'password\n'`, which creates a completely different hash.
    
2. `|`: This is the **pipe**. It sends the output of the `echo` command (`password`) as the input to the next command.
    
3. `md5sum`: This command reads the input it receives from the pipe and calculates its MD5 hash. The output looks something like this: `5f4dcc3b5aa765d61d8327deb882cf99 -`.
    
4. `| awk '{print $1}'`: The output from `md5sum` includes the hash and a dash. Hashcat only wants the hash itself. This `awk` command is a simple way to say "print only the first column ($1) of the text you receive." This isolates the hash string.
    
5. `>` and `>>`:
    
    - `> hash.txt`: The first command uses `>` to **create** a new file called `hash.txt` and put the first hash in it. If `hash.txt` already existed, it would be overwritten.
        
    - `>> hash.txt`: The subsequent commands use `>>` to **append** the new hashes to the end of `hash.txt` without deleting its existing content.
        

**Goal of this step:** You have created a file named `hash.txt` that contains one MD5 hash per line, ready for Hashcat to crack.

#### ### Cracking with Hashcat

Bash

```
sudo hashcat -m 0 -a 0 -o cracked.txt hash.txt /usr/share/wordlists/rockyou.txt
```

This command tells Hashcat to start the attack.

- `sudo`: Hashcat may need administrator privileges to access hardware like the GPU.
    
- `hashcat`: The name of the program.
    
- `-m 0`: This is the **mode** flag, specifying the hash type. `-m 0` tells Hashcat you are cracking **MD5** hashes. Hashcat supports hundreds of hash types (e.g., `-m 1000` is for NTLM).
    
- `-a 0`: This is the **attack mode**. `-a 0` specifies a straight **Dictionary attack**.
    
- `-o cracked.txt`: This specifies the **output** file. Any passwords Hashcat successfully cracks will be saved here.
    
- `hash.txt`: This is the first argument—the input file containing the list of hashes you want to crack.
    
- `/usr/share/wordlists/rockyou.txt`: This is the second argument—the dictionary file. `rockyou.txt` is a famous wordlist containing over 14 million real-world passwords from a past data breach. Hashcat will hash every word in this file and compare it to the hashes in `hash.txt`.
    

**Result:** As seen in your screenshot, Hashcat successfully cracked `password`, `password123`, and `ilovedog` because those passwords were present in the `rockyou.txt` wordlist.

---

### ## 2. John the Ripper: Cracking a Protected File 🔐

John the Ripper (JTR) is another very popular password cracker. It's known for its versatility and ability to automatically detect hash types. This example shows its strength in cracking passwords on files, not just raw hashes.

#### ### The Workflow Explained

1. **Create a Sample File and ZIP it:**
    
    Bash
    
    ```
    echo "Hello World" > sample.txt
    zip -e encrypted.zip sample.txt
    ```
    
    - You create a simple text file.
        
    - `zip -e` tells the `zip` utility to **encrypt** the archive. It will prompt you to enter a password. This password is what you will be cracking.
        
2. **Extract the Hash with `zip2john`:**
    
    Bash
    
    ```
    zip2john encrypted.zip > encrypted.txt
    ```
    
    - Password crackers don't work directly on ZIP files. They need the password's hash.
        
    - `zip2john` is a utility that comes with John the Ripper. It reads the metadata inside the `encrypted.zip` file and extracts the password's hash representation into a format that John can understand.
        
    - You save this hash output into a file called `encrypted.txt`.
        
3. **Crack the Hash with `john`:**
    
    Bash
    
    ```
    john --wordlist=/usr/share/wordlists/rockyou.txt encrypted.txt
    ```
    
    - `john`: The name of the program.
        
    - `--wordlist=...`: This tells John to perform a dictionary attack using the specified wordlist (`rockyou.txt`).
        
    - `encrypted.txt`: This is the input file containing the hash you extracted with `zip2john`.
        

**Result:** John will try every password from `rockyou.txt`. If the password you used to protect the ZIP file is in that list, John will find it and display it on the screen, as shown in your screenshot.

---

### ## 3. RainbowCrack: The Rainbow Table Attack 🌈

This method is different. Instead of actively guessing and hashing passwords during the attack, it uses a massive, pre-computed lookup table called a **rainbow table**. This trades a huge amount of disk space and initial computation time for extremely fast cracking times later.

#### ### The Workflow Explained

1. **Generate the Rainbow Table (`rtgen`):**
    
    Bash
    
    ```
    sudo rtgen md5 loweralpha 1 3 0 1000 1000 0
    ```
    
    This is the most complex command. It tells `rtgen` to build the tables.
    
    - `md5`: The hash algorithm to use.
        
    - `loweralpha`: The character set to generate passwords from (a-z).
        
    - `1`: The minimum password length.
        
    - `3`: The maximum password length.
        
    - `0`: The table index.
        
    - `1000`: The "chain length" for the table (a technical parameter for how the table is built).
        
    - `1000`: The number of chains to generate.
        
    - 0: The part index.
        
        In simple terms: You are telling it to pre-compute the hashes for every possible password from 1 to 3 characters long using only lowercase letters (a, b, c, aa, ab, ac... zzz).
        
2. **Sort the Table (`rtsort`):**
    
    Bash
    
    ```
    sudo rtsort .
    ```
    
    - This command sorts the generated rainbow table files (`.rt` files) in the current directory. This is a required step that organizes the tables for fast lookups.
        
3. **Crack the Hash (`rcrack`):**
    
    Bash
    
    ```
    echo -n 'dog' | md5sum | awk '{print $1}'
    # Output: 06d80eb0c50b49a509b49f2424e8c805
    sudo rcrack -h 06d80eb0c50b49a509b49f2424e8c805
    ```
    
    - First, you generate the MD5 hash for 'dog' to have a target to crack.
        
    - `rcrack`: This is the RainbowCrack cracking tool.
        
    - `-h HASH_VALUE`: The `-h` flag tells `rcrack` that you are providing a single hash to crack directly on the command line.
        
    - Instead of guessing, `rcrack` looks up this hash in the rainbow tables you generated and sorted. Since 'dog' is a 3-character lowercase password, it exists in your pre-computed tables.
        

**Result:** The tool finds the hash in the table and instantly returns the original plaintext password, 'dog', as seen in your screenshot. This is much faster than a dictionary or brute-force attack, but only if the password's hash is in your pre-computed table.

# now can you give me top 20 file headers like FF D8 ,FF D9 for jpg
Of course. These file headers are called **"magic numbers."** They are the first few bytes of a file used to identify the format, so programs know how to handle them.

Here are 20 of the most common file headers.

---

### ## Common File Headers (Magic Numbers)

| File Type                  | Common Extension(s)               | Hex Signature (Header)                                      | ASCII Representation |
| -------------------------- | --------------------------------- | ----------------------------------------------------------- | -------------------- |
| **JPEG Image**             | `.jpg`, `.jpeg`                   | `FF D8 FF E0` (Start) and `FF D9` (End)                     | ÿØÿà / ÿÙ            |
| **PNG Image**              | `.png`                            | `89 50 4E 47 0D 0A 1A 0A`                                   | .PNG....             |
| **GIF Image**              | `.gif`                            | `47 49 46 38 39 61` or `...37 61`                           | GIF89a / GIF87a      |
| **Bitmap Image**           | `.bmp`                            | `42 4D`                                                     | BM                   |
| **TIFF Image**             | `.tif`, `.tiff`                   | `49 49 2A 00` (Little-endian) or `4D 4D 00 2A` (Big-endian) | II*. / MM.*          |
| **PDF Document**           | `.pdf`                            | `25 50 44 46`                                               | %PDF                 |
| **ZIP Archive**            | `.zip`, `.docx`, `.xlsx`, `.pptx` | `50 4B 03 04`                                               | PK..                 |
| **RAR Archive**            | `.rar`                            | `52 61 72 21 1A 07 00`                                      | Rar!...              |
| **GZIP Archive**           | `.gz`                             | `1F 8B`                                                     | ..                   |
| **7-Zip Archive**          | `.7z`                             | `37 7A BC AF 27 1C`                                         | 7z¼¯'.               |
| **Windows Executable**     | `.exe`, `.dll`                    | `4D 5A`                                                     | MZ                   |
| **ELF Executable (Linux)** | `.elf`, `.o`, `.so`               | `7F 45 4C 46`                                               | .ELF                 |
| **Java Class File**        | `.class`                          | `CA FE BA BE`                                               | ïþº¾                 |
| **WAV Audio**              | `.wav`                            | `52 49 46 46` ... `57 41 56 45`                             | RIFF...WAVE          |
| **MP3 Audio**              | `.mp3`                            | `49 44 33` (ID3v2 tag) or `FF FB`                           | ID3 / ÿû             |
| **AVI Video**              | `.avi`                            | `52 49 46 46` ... `41 56 49 20`                             | RIFF...AVI           |
| **MP4 Video**              | `.mp4`                            | `... 66 74 79 70` (at offset 4)                             | ...ftyp              |
| **ISO Disk Image**         | `.iso`                            | `43 44 30 30 31`                                            | CD001                |
| **SQLite Database**        | `.sqlite`, `.db`                  | `53 51 4C 69 74 65 20 66 6F 72 6D 61 74 20 33 00`           | SQLite format 3.     |
| **Rich Text Format**       | `.rtf`                            | `7B 5C 72 74 66 31`                                         | {\rtf1               |


You got it. Here's a consolidated list of all 40 file types.

First, it's important to know that most file formats have a defined **header** (magic number) but **not a standard footer**. Files typically rely on the file system's size information to determine where they end.

A footer is a specific sequence of bytes that formally terminates a file's data structure. It's common in formats that need to ensure data integrity or are streamed, like images and complex archives. For all other file types, there isn't a specific "end" signature to look for.

---

### ## Consolidated List of 40 File Signatures

Here is the complete list with the footer information included where applicable.

|#|File Type|Extension(s)|Hex Signature (Header)|Hex Signature (Footer)|
|---|---|---|---|---|
|1|**JPEG Image**|`.jpg`, `.jpeg`|`FF D8 FF E0`|`FF D9`|
|2|**PNG Image**|`.png`|`89 50 4E 47 0D 0A 1A 0A`|`49 45 4E 44 AE 42 60 82`|
|3|**GIF Image**|`.gif`|`47 49 46 38 39 61` or `...37 61`|`00 3B`|
|4|**Bitmap Image**|`.bmp`|`42 4D`|N/A|
|5|**TIFF Image**|`.tif`, `.tiff`|`49 49 2A 00` or `4D 4D 00 2A`|N/A|
|6|**PDF Document**|`.pdf`|`25 50 44 46`|N/A (Often ends with `%%EOF`)|
|7|**ZIP Archive**|`.zip`, `.docx`, etc.|`50 4B 03 04`|`50 4B 05 06`|
|8|**RAR Archive**|`.rar`|`52 61 72 21 1A 07 00`|N/A|
|9|**GZIP Archive**|`.gz`|`1F 8B`|N/A|
|10|**7-Zip Archive**|`.7z`|`37 7A BC AF 27 1C`|N/A|
|11|**Windows Executable**|`.exe`, `.dll`|`4D 5A`|N/A|
|12|**ELF Executable**|`.elf`, `.so`|`7F 45 4C 46`|N/A|
|13|**Java Class File**|`.class`|`CA FE BA BE`|N/A|
|14|**WAV Audio**|`.wav`|`52 49 46 46` ... `57 41 56 45`|N/A|
|15|**MP3 Audio**|`.mp3`|`49 44 33` or `FF FB`|N/A (May have `TAG` at end)|
|16|**AVI Video**|`.avi`|`52 49 46 46` ... `41 56 49 20`|N/A|
|17|**MP4 Video**|`.mp4`|`... 66 74 79 70` (at offset 4)|N/A|
|18|**ISO Disk Image**|`.iso`|`43 44 30 30 31`|N/A|
|19|**SQLite Database**|`.sqlite`, `.db`|`53 51 4C 69 74 65 ... 33 00`|N/A|
|20|**Rich Text Format**|`.rtf`|`7B 5C 72 74 66 31`|N/A (Ends with `}`)|
|21|**Old MS Office**|`.doc`, `.xls`|`D0 CF 11 E0 A1 B1 1A E1`|N/A|
|22|**TAR Archive**|`.tar`|`75 73 74 61 72` (at offset 257)|N/A|
|23|**Microsoft Cabinet**|`.cab`|`4D 53 43 46`|N/A|
|24|**PostScript Doc**|`.ps`, `.eps`|`25 21 50 53 2D 41 64 6F 62 65`|N/A|
|25|**Email Message**|`.eml`|`52 65 63 65 69 76 65 64 3A`|N/A|
|26|**Outlook Data File**|`.pst`|`21 42 44 4E`|N/A|
|27|**Photoshop Doc**|`.psd`|`38 42 50 53`|N/A|
|28|**MPEG Video**|`.mpg`, `.mpeg`|`00 00 01 B3`|N/A|
|29|**Flash Video**|`.flv`|`46 4C 56`|N/A|
|30|**MIDI Audio**|`.mid`, `.midi`|`4D 54 68 64`|N/A|
|31|**Windows Media**|`.wmv`, `.wma`|`30 26 B2 75 8E 66 CF 11`|N/A|
|32|**Java Archive**|`.jar`|`50 4B 03 04`|`50 4B 05 06` (Same as ZIP)|
|33|**Dalvik Executable**|`.dex`|`64 65 78 0A 30 33 35 00`|N/A|
|34|**WebAssembly**|`.wasm`|`00 61 73 6D`|N/A|
|35|**TrueType Font**|`.ttf`|`00 01 00 00 00`|N/A|
|36|**OpenType Font**|`.otf`|`4F 54 54 4F`|N/A|
|37|**Apple Disk Image**|`.dmg`|`78 01 73 0D 62 62 60`|N/A|
|38|**CR2 RAW Image**|`.cr2`|`49 49 2A 00 ... 43 52`|N/A|
|39|**Plain Text**|`.txt`|N/A|N/A|
|40|**XML/HTML**|`.xml`, `.html`|`3C 3F 78 6D 6C` or `3C 21 44 4F`|N/A|


# Custody
Of course. Below are the Chain of Custody forms for one key piece of evidence from each of the five scenarios you provided.

A **Chain of Custody (CoC)** form is a critical document that tracks the chronological lifecycle of evidence. According to NIST guidelines, it must detail who collected, handled, transferred, and stored each piece of evidence to ensure its integrity is legally defensible in court.

---

### ## Case Scenario 1 — Online Banking Fraud

Here is the completed Chain of Custody form for one of the server hard disks.

CHAIN OF CUSTODY RECORD

(NIST SP 800-86 Compliant)

|||
|---|---|
|**Case Number:**|`CCIC-2025-09-001`|
|**Case Title:**|Cosmos Bank Server Breach|
|**Lead Investigator:**|Hency Sharma|

---

EVIDENCE DETAILS

| | |

| :--- | :--- |

| Evidence ID: | CCIC-HDD-001 |

| Item Description: | Seagate Exos 8TB SAS Hard Drive, removed from core banking server (Slot 1). |

| Serial Number: | SN: ZY7X8W9V |

| Location Collected: | Cosmos Bank Data Center, Pune - Server Rack #7, Bay A. |

| Date/Time Collected: | 08-Sept-2025, 11:30 AM IST |

| Collected By: | Hency Sharma |

---

INTEGRITY VERIFICATION

| | |

| :--- | :--- |

| Hashing Algorithm: | SHA-256 |

| Hash Value (at collection): | a4b1c8f21a7d3e5c6b8f0a1d3c5b7e9a2d4f6c8b1a3e5d7f9c1b3a5e7d9f2c4b |

---

CUSTODY LOG

| Date/Time | Released By (Name & Signature) | Received By (Name & Signature) | Purpose of Transfer / Remarks |

| :--- | :--- | :--- | :--- |

| 08-Sept-2025, 12:00 PM | Hency Sharma | Rohan Verma (Forensic Tech) | Transport to Cyber Forensics Lab, Pune. |

| 08-Sept-2025, 02:15 PM | Rohan Verma | Priya Singh (Lab Custodian) | Logged into lab evidence locker #101. |

| 09-Sept-2025, 09:00 AM | Priya Singh | Aman Gill (Forensic Analyst) | For disk imaging and malware analysis. |

| 10-Sept-2025, 05:30 PM | Aman Gill | Priya Singh | Returned to evidence locker #101 post-analysis. |

---

FINAL DISPOSITION

| | |

| :--- | :--- |

| Date: | 11-Sept-2025 |

| Action Taken: | Evidence analysis report submitted. Item sealed and held in secure storage pending legal proceedings. |

| Authorized By: | Hency Sharma |

---

### ## Case Scenario 2 — Corporate Espionage via Hacking

Here is the completed Chain of Custody form for the external hard disk.

CHAIN OF CUSTODY RECORD

(NIST SP 800-86 Compliant)

|||
|---|---|
|**Case Number:**|`TWS-2025-08-002`|
|**Case Title:**|Data Breach at TechWave Solutions|
|**Lead Investigator:**|Hency Sharma|

---

EVIDENCE DETAILS

| | |

| :--- | :--- |

| Evidence ID: | TWS-EHD-001 |

| Item Description: | Western Digital My Passport 2TB External Hard Disk, black. |

| Serial Number: | SN: WX12Y345Z678 |

| Location Collected: | TechWave Solutions Pvt. Ltd., Mumbai - Suspect's workstation desk. |

| Date/Time Collected: | 15-Aug-2025, 04:00 PM IST |

| Collected By: | Hency Sharma |

---

INTEGRITY VERIFICATION

| | |

| :--- | :--- |

| Hashing Algorithm: | SHA-256 |

| Hash Value (at collection): | f3e5d7c9b1a3f5e7d9c1b3a5e7d9f2c4ba4b1c8f21a7d3e5c6b8f0a1d3c5b7e9 |

---

CUSTODY LOG

| Date/Time | Released By (Name & Signature) | Received By (Name & Signature) | Purpose of Transfer / Remarks |

| :--- | :--- | :--- | :--- |

| 15-Aug-2025, 04:30 PM | Hency Sharma | Rohan Verma (Forensic Tech) | Transport to Cyber Forensics Lab, Mumbai. |

| 15-Aug-2025, 07:00 PM | Rohan Verma | Priya Singh (Lab Custodian) | Logged into lab evidence locker #205. |

| 16-Aug-2025, 10:00 AM | Priya Singh | Aman Gill (Forensic Analyst) | For forensic imaging and data extraction. |

| 18-Aug-2025, 01:00 PM | Aman Gill | Priya Singh | Returned to evidence locker #205 post-analysis. |

---

FINAL DISPOSITION

| | |

| :--- | :--- |

| Date: | 19-Aug-2025 |

| Action Taken: | Forensic report submitted to legal team. Evidence stored for prosecution. |

| Authorized By: | Hency Sharma |

---

### ## Case Scenario 3 — E-commerce Phishing Scam

Here is the completed Chain of Custody form for the pen drive with source code.

CHAIN OF CUSTODY RECORD

(NIST SP 800-86 Compliant)

|||
|---|---|
|**Case Number:**|`FEH-2025-07-003`|
|**Case Title:**|Fraudulent Shopping Portal Investigation|
|**Lead Investigator:**|Hency Sharma|

---

EVIDENCE DETAILS

| | |

| :--- | :--- |

| Evidence ID: | FEH-PD-001 |

| Item Description: | SanDisk Ultra 64GB USB 3.0 Pen Drive, red/black. Suspected to contain website source code. |

| Serial Number: | SN: 4C531001551211109352 |

| Location Collected: | Fake E-commerce Hub, Bengaluru - Office premises. |

| Date/Time Collected: | 21-July-2025, 01:45 PM IST |

| Collected By: | Hency Sharma |

---

INTEGRITY VERIFICATION

| | |

| :--- | :--- |

| Hashing Algorithm: | SHA-256 |

| Hash Value (at collection): | c6b8f0a1d3c5b7e9a2d4f6c8b1a3e5d7f9c1b3a5e7d9f2c4ba4b1c8f21a7d3e5 |

---

CUSTODY LOG

| Date/Time | Released By (Name & Signature) | Received By (Name & Signature) | Purpose of Transfer / Remarks |

| :--- | :--- | :--- | :--- |

| 21-July-2025, 02:15 PM | Hency Sharma | Rohan Verma (Forensic Tech) | Transport to Cyber Forensics Lab, Bengaluru. |

| 21-July-2025, 03:30 PM | Rohan Verma | Priya Singh (Lab Custodian) | Logged into lab evidence locker #310. |

| 22-July-2025, 11:00 AM | Priya Singh | Aman Gill (Forensic Analyst) | For forensic analysis and source code review. |

| 23-July-2025, 04:00 PM | Aman Gill | Priya Singh | Returned to evidence locker #310 post-analysis. |

---

FINAL DISPOSITION

| | |

| :--- | :--- |

| Date: | 24-July-2025 |

| Action Taken: | Findings handed to Enforcement Directorate. Evidence stored securely. |

| Authorized By: | Hency Sharma |

---

### ## Case Scenario 4 — Ransomware Attack on Hospital

Here is the completed Chain of Custody form for a hard disk from an affected server.

CHAIN OF CUSTODY RECORD

(NIST SP 800-86 Compliant)

|||
|---|---|
|**Case Number:**|`MSH-2025-08-004`|
|**Case Title:**|MedSecure Hospital Data Encryption Attack|
|**Lead Investigator:**|Hency Sharma|

---

EVIDENCE DETAILS

| | |

| :--- | :--- |

| Evidence ID: | MSH-SRV-HDD-001 |

| Item Description: | Dell PowerEdge Server HDD 2TB, from main patient records server. |

| Serial Number: | SN: 9WM2PXYZ |

| Location Collected: | MedSecure Multispecialty Hospital, Delhi - Server Room. |

| Date/Time Collected: | 02-Aug-2025, 10:00 AM IST |

| Collected By: | Hency Sharma |

---

INTEGRITY VERIFICATION

| | |

| :--- | :--- |

| Hashing Algorithm: | SHA-256 |

| Hash Value (at collection): | 1a3e5d7f9c1b3a5e7d9f2c4ba4b1c8f21a7d3e5c6b8f0a1d3c5b7e9a2d4f6c8b |

---

CUSTODY LOG

| Date/Time | Released By (Name & Signature) | Received By (Name & Signature) | Purpose of Transfer / Remarks |

| :--- | :--- | :--- | :--- |

| 02-Aug-2025, 10:30 AM | Hency Sharma | Rohan Verma (Forensic Tech) | Transport to National Cyber Crime Unit Lab, Delhi. |

| 02-Aug-2025, 12:45 PM | Rohan Verma | Priya Singh (Lab Custodian) | Logged into lab evidence locker #415. |

| 03-Aug-2025, 09:30 AM | Priya Singh | Aman Gill (Forensic Analyst) | For ransomware analysis and data recovery attempt. |

| 05-Aug-2025, 06:00 PM | Aman Gill | Priya Singh | Returned to evidence locker #415 post-analysis. |

---

FINAL DISPOSITION

| | |

| :--- | :--- |

| Date: | 06-Aug-2025 |

| Action Taken: | Findings submitted to CERT-In. Evidence stored for case prosecution. |

| Authorized By: | Hency Sharma |

---

### ## Case Scenario 5 — Cryptocurrency Investment Fraud

Here is the completed Chain of Custody form for a laptop used by the organizers.

CHAIN OF CUSTODY RECORD

(NIST SP 800-86 Compliant)

|||
|---|---|
|**Case Number:**|`CXP-2025-09-005`|
|**Case Title:**|CoinX Crypto Scam|
|**Lead Investigator:**|Hency Sharma|

---

EVIDENCE DETAILS

| | |

| :--- | :--- |

| Evidence ID: | CXP-LAP-001 |

| Item Description: | Apple MacBook Pro 16-inch, Space Grey. Used by primary scam organizer. |

| Serial Number: | SN: C02F1234PQ05 |

| Location Collected: | CoinX Pvt. Ltd. Office, Hyderabad - Director's Cabin. |

| Date/Time Collected: | 18-Sept-2025, 03:20 PM IST |

| Collected By: | Hency Sharma |

---

INTEGRITY VERIFICATION

| | |

| :--- | :--- |

| Hashing Algorithm: | SHA-256 |

| Hash Value (at collection): | 5e7d9f2c4ba4b1c8f21a7d3e5c6b8f0a1d3c5b7e9a2d4f6c8b1a3e5d7f9c1b3a |

---

CUSTODY LOG

| Date/Time | Released By (Name & Signature) | Received By (Name & Signature) | Purpose of Transfer / Remarks |

| :--- | :--- | :--- | :--- |

| 18-Sept-2025, 04:00 PM | Hency Sharma | Rohan Verma (Forensic Tech) | Transport to Cyber Forensics Unit, Hyderabad. |

| 18-Sept-2025, 05:15 PM | Rohan Verma | Priya Singh (Lab Custodian) | Logged into lab evidence locker #520. |

| 19-Sept-2025, 10:15 AM | Priya Singh | Aman Gill (Forensic Analyst) | For blockchain analysis and transaction tracing. |

| 22-Sept-2025, 02:00 PM | Aman Gill | Priya Singh | Returned to evidence locker #520 post-analysis. |

---

FINAL DISPOSITION

| | |

| :--- | :--- |

| Date: | 23-Sept-2025 |

| Action Taken: | Findings shared with Interpol. Evidence stored for extradition and legal proceedings. |

| Authorized By: | Hency Sharma |






Of course. Here is the complete and proper evidence log for all five scenarios.

In a real investigation, **each item listed below would have its own separate, multi-page Chain of Custody form** detailing its collection, handling, and analysis. This initial log serves as the master list for all collected evidence.

---

### ## Case Scenario 1 — Online Banking Fraud

Case Title: Cosmos Bank Server Breach

Case Number: CCIC-2025-09-001

Lead Investigator: Hency Sharma

**Evidence Log:**

- **`CCIC-HDD-001`**: Hard Disk 1 (from core banking server)
    
- **`CCIC-HDD-002`**: Hard Disk 2 (from core banking server)
    
- **`CCIC-HDD-003`**: Hard Disk 3 (from core banking server)
    
- **`CCIC-RTR-001`**: Router (connected to SWIFT terminal)
    
- **`CCIC-LAP-001`**: Laptop 1 (used by network admin)
    
- **`CCIC-LAP-002`**: Laptop 2 (used by network admin)
    
- **`CCIC-PD-001`**: Pen Drive (64GB, suspected malware)
    
- **`CCIC-MOB-001`**: Mobile Phone 1 (recovered from employee)
    
- **`CCIC-MOB-002`**: Mobile Phone 2 (recovered from employee)
    

---

### ## Case Scenario 2 — Corporate Espionage via Hacking

Case Title: Data Breach at TechWave Solutions

Case Number: TWS-2025-08-002

Lead Investigator: Hency Sharma

**Evidence Log:**

- **`TWS-LAP-001`**: Company Laptop 1 (used by suspect)
    
- **`TWS-LAP-002`**: Company Laptop 2 (used by suspect)
    
- **`TWS-EHD-001`**: External Hard Disk (containing stolen data)
    
- **`TWS-RTR-001`**: Wi-Fi Router
    
- **`TWS-PD-001`**: Pen Drive (64GB, encrypted files)
    
- **`TWS-MOB-001`**: Mobile Phone 1 (used for communication)
    
- **`TWS-MOB-002`**: Mobile Phone 2 (used for communication)
    

---

### ## Case Scenario 3 — E-commerce Phishing Scam

Case Title: Fraudulent Shopping Portal Investigation

Case Number: FEH-2025-07-003

Lead Investigator: Hency Sharma

**Evidence Log:**

- **`FEH-LAP-001`**: Laptop 1 (used for website hosting)
    
- **`FEH-LAP-002`**: Laptop 2 (used for website hosting)
    
- **`FEH-HDD-001`**: Hard Disk 1 (payment gateway logs)
    
- **`FEH-HDD-002`**: Hard Disk 2 (payment gateway logs)
    
- **`FEH-HDD-003`**: Hard Disk 3 (payment gateway logs)
    
- **`FEH-RTR-001`**: Router (connected to fraudulent servers)
    
- **`FEH-PD-001`**: Pen Drive (64GB, website source code)
    
- **`FEH-MOB-001`**: Mobile Phone 1 (OTP interception)
    
- **`FEH-MOB-002`**: Mobile Phone 2 (OTP interception)
    

---

### ## Case Scenario 4 — Ransomware Attack on Hospital

Case Title: MedSecure Hospital Data Encryption Attack

Case Number: MSH-2025-08-004

Lead Investigator: Hency Sharma

**Evidence Log:**

- **`MSH-HDD-001`**: Hard Disk 1 (from affected server)
    
- **`MSH-HDD-002`**: Hard Disk 2 (from affected server)
    
- **`MSH-HDD-003`**: Hard Disk 3 (from affected server)
    
- **`MSH-LAP-001`**: Laptop 1 (hospital IT staff)
    
- **`MSH-LAP-002`**: Laptop 2 (hospital IT staff)
    
- **`MSH-RTR-001`**: Router
    
- **`MSH-PD-001`**: Pen Drive (64GB, ransom notes)
    
- **`MSH-MOB-001`**: Mobile Phone 1 (suspected employee)
    
- **`MSH-MOB-002`**: Mobile Phone 2 (suspected employee)
    

---

### ## Case Scenario 5 — Cryptocurrency Investment Fraud

Case Title: CoinX Crypto Scam

Case Number: CXP-2025-09-005

Lead Investigator: Hency Sharma

**Evidence Log:**

- **`CXP-LAP-001`**: Laptop 1 (used by scam organizers)
    
- **`CXP-LAP-002`**: Laptop 2 (used by scam organizers)
    
- **`CXP-HDD-001`**: Hard Disk 1 (transaction data)
    
- **`CXP-HDD-002`**: Hard Disk 2 (transaction data)
    
- **`CXP-HDD-003`**: Hard Disk 3 (transaction data)
    
- **`CXP-RTR-001`**: Router (connected to CoinX servers)
    
- **`CXP-PD-001`**: Pen Drive (64GB, fake KYC documents)
    
- **`CXP-MOB-001`**: Mobile Phone 1 (investor communication)
    
- **`CXP-MOB-002`**: Mobile Phone 2 (investor communication)




#  all custody 
Of course. Here is the complete and proper documentation for all five scenarios.

For each case, I will provide:

1. A **fully detailed Chain of Custody form** for one primary piece of evidence as a complete example.
    
2. A **log of all remaining evidence items** from that case with their unique IDs.
    

In a real investigation, each item in the log would also have its own detailed form filled out just like the example provided.

---

### ## Case Scenario 1 — Online Banking Fraud

Case Title: Cosmos Bank Server Breach

Case Number: CCIC-2025-09-001

Lead Investigator: Hency Sharma

#### ### Detailed Chain of Custody Form (Sample)

CHAIN OF CUSTODY RECORD

(NIST SP 800-86 Compliant)

|||
|---|---|
|**Evidence ID:**|`CCIC-HDD-001`|
|**Item Description:**|Seagate Exos 8TB SAS Hard Drive, removed from core banking server (Slot 1).|
|**Serial Number:**|`SN: ZY7X8W9V`|
|**Location Collected:**|Cosmos Bank Data Center, Pune - Server Rack #7, Bay A.|
|**Date/Time Collected:**|08-Sept-2025, 11:30 AM IST|
|**Collected By:**|Hency Sharma|

---

INTEGRITY VERIFICATION

| | |

| :--- | :--- |

| Hashing Algorithm: | SHA-256 |

| Hash Value (at collection): | a4b1c8f21a7d3e5c6b8f0a1d3c5b7e9a2d4f6c8b1a3e5d7f9c1b3a5e7d9f2c4b |

---

CUSTODY LOG

| Date/Time | Released By (Name & Signature) | Received By (Name & Signature) | Purpose of Transfer / Remarks |

| :--- | :--- | :--- | :--- |

| 08-Sept-2025, 12:00 PM | Hency Sharma | Rohan Verma (Forensic Tech) | Transport to Cyber Forensics Lab, Pune. |

| 08-Sept-2025, 02:15 PM | Rohan Verma | Priya Singh (Lab Custodian) | Logged into lab evidence locker #101. |

| 09-Sept-2025, 09:00 AM | Priya Singh | Aman Gill (Forensic Analyst) | For disk imaging and malware analysis. |

| 10-Sept-2025, 05:30 PM | Aman Gill | Priya Singh | Returned to evidence locker #101 post-analysis. |

---

FINAL DISPOSITION

| | |

| :--- | :--- |

| Date: | 11-Sept-2025 |

| Action Taken: | Evidence analysis report submitted. Item sealed and held in secure storage pending legal proceedings. |

| Authorized By: | Hency Sharma |

#### ### Remaining Evidence Items Log

- **`CCIC-HDD-002`**: Hard Disk 2 (from core banking server)
    
- **`CCIC-HDD-003`**: Hard Disk 3 (from core banking server)
    
- **`CCIC-RTR-001`**: Router (connected to SWIFT terminal)
    
- **`CCIC-LAP-001`**: Laptop 1 (used by network admin)
    
- **`CCIC-LAP-002`**: Laptop 2 (used by network admin)
    
- **`CCIC-PD-001`**: Pen Drive (64GB, suspected malware)
    
- **`CCIC-MOB-001`**: Mobile Phone 1 (recovered from employee)
    
- **`CCIC-MOB-002`**: Mobile Phone 2 (recovered from employee)
    

---

### ## Case Scenario 2 — Corporate Espionage via Hacking

Case Title: Data Breach at TechWave Solutions

Case Number: TWS-2025-08-002

Lead Investigator: Hency Sharma

#### ### Detailed Chain of Custody Form (Sample)

CHAIN OF CUSTODY RECORD

(NIST SP 800-86 Compliant)

|||
|---|---|
|**Evidence ID:**|`TWS-LAP-001`|
|**Item Description:**|Dell Latitude 7400 Laptop, used by suspect.|
|**Serial Number:**|`SN: 8G9H0J-1K2L`|
|**Location Collected:**|TechWave Solutions Pvt. Ltd., Mumbai - Suspect's workstation desk.|
|**Date/Time Collected:**|15-Aug-2025, 04:00 PM IST|
|**Collected By:**|Hency Sharma|

---

INTEGRITY VERIFICATION

| | |

| :--- | :--- |

| Hashing Algorithm: | SHA-256 |

| Hash Value (at collection): | e9a2d4f6c8b1a3e5d7f9c1b3a5e7d9f2c4ba4b1c8f21a7d3e5c6b8f0a1d3c5b7 |

---

CUSTODY LOG

| Date/Time | Released By (Name & Signature) | Received By (Name & Signature) | Purpose of Transfer / Remarks |

| :--- | :--- | :--- | :--- |

| 15-Aug-2025, 04:30 PM | Hency Sharma | Rohan Verma (Forensic Tech) | Transport to Cyber Forensics Lab, Mumbai. |

| 15-Aug-2025, 07:00 PM | Rohan Verma | Priya Singh (Lab Custodian) | Logged into lab evidence locker #205. |

| 16-Aug-2025, 10:00 AM | Priya Singh | Aman Gill (Forensic Analyst) | For forensic imaging and data exfiltration analysis. |

| 18-Aug-2025, 01:00 PM | Aman Gill | Priya Singh | Returned to evidence locker #205 post-analysis. |

---

FINAL DISPOSITION

| | |

| :--- | :--- |

| Date: | 19-Aug-2025 |

| Action Taken: | Forensic report submitted to legal team. Evidence stored for prosecution. |

| Authorized By: | Hency Sharma |

#### ### Remaining Evidence Items Log

- **`TWS-LAP-002`**: Company Laptop 2 (used by suspect)
    
- **`TWS-EHD-001`**: External Hard Disk (containing stolen data)
    
- **`TWS-RTR-001`**: Wi-Fi Router
    
- **`TWS-PD-001`**: Pen Drive (64GB, encrypted files)
    
- **`TWS-MOB-001`**: Mobile Phone 1 (used for communication)
    
- **`TWS-MOB-002`**: Mobile Phone 2 (used for communication)
    

---

### ## Case Scenario 3 — E-commerce Phishing Scam

Case Title: Fraudulent Shopping Portal Investigation

Case Number: FEH-2025-07-003

Lead Investigator: Hency Sharma

#### ### Detailed Chain of Custody Form (Sample)

CHAIN OF CUSTODY RECORD

(NIST SP 800-86 Compliant)

|||
|---|---|
|**Evidence ID:**|`FEH-LAP-001`|
|**Item Description:**|HP Spectre x360 Laptop, used for website hosting.|
|**Serial Number:**|`SN: 5CD0123ABC`|
|**Location Collected:**|Fake E-commerce Hub, Bengaluru - Office premises.|
|**Date/Time Collected:**|21-July-2025, 01:45 PM IST|
|**Collected By:**|Hency Sharma|

---

INTEGRITY VERIFICATION

| | |

| :--- | :--- |

| Hashing Algorithm: | SHA-256 |

| Hash Value (at collection): | c6b8f0a1d3c5b7e9a2d4f6c8b1a3e5d7f9c1b3a5e7d9f2c4ba4b1c8f21a7d3e5 |

---

CUSTODY LOG

| Date/Time | Released By (Name & Signature) | Received By (Name & Signature) | Purpose of Transfer / Remarks |

| :--- | :--- | :--- | :--- |

| 21-July-2025, 02:15 PM | Hency Sharma | Rohan Verma (Forensic Tech) | Transport to Cyber Forensics Lab, Bengaluru. |

| 21-July-2025, 03:30 PM | Rohan Verma | Priya Singh (Lab Custodian) | Logged into lab evidence locker #310. |

| 22-July-2025, 11:00 AM | Priya Singh | Aman Gill (Forensic Analyst) | For web server log and payment gateway analysis. |

| 23-July-2025, 04:00 PM | Aman Gill | Priya Singh | Returned to evidence locker #310 post-analysis. |

---

FINAL DISPOSITION

| | |

| :--- | :--- |

| Date: | 24-July-2025 |

| Action Taken: | Findings handed to Enforcement Directorate. Evidence stored securely. |

| Authorized By: | Hency Sharma |

#### ### Remaining Evidence Items Log

- **`FEH-LAP-002`**: Laptop 2 (used for website hosting)
    
- **`FEH-HDD-001`**: Hard Disk 1 (payment gateway logs)
    
- **`FEH-HDD-002`**: Hard Disk 2 (payment gateway logs)
    
- **`FEH-HDD-003`**: Hard Disk 3 (payment gateway logs)
    
- **`FEH-RTR-001`**: Router (connected to fraudulent servers)
    
- **`FEH-PD-001`**: Pen Drive (64GB, website source code)
    
- **`FEH-MOB-001`**: Mobile Phone 1 (OTP interception)
    
- **`FEH-MOB-002`**: Mobile Phone 2 (OTP interception)
    

---

### ## Case Scenario 4 — Ransomware Attack on Hospital

Case Title: MedSecure Hospital Data Encryption Attack

Case Number: MSH-2025-08-004

Lead Investigator: Hency Sharma

#### ### Detailed Chain of Custody Form (Sample)

CHAIN OF CUSTODY RECORD

(NIST SP 800-86 Compliant)

|||
|---|---|
|**Evidence ID:**|`MSH-HDD-001`|
|**Item Description:**|Dell PowerEdge Server HDD 2TB, from main patient records server.|
|**Serial Number:**|`SN: 9WM2PXYZ`|
|**Location Collected:**|MedSecure Multispecialty Hospital, Delhi - Server Room.|
|**Date/Time Collected:**|02-Aug-2025, 10:00 AM IST|
|**Collected By:**|Hency Sharma|

---

INTEGRITY VERIFICATION

| | |

| :--- | :--- |

| Hashing Algorithm: | SHA-256 |

| Hash Value (at collection): | 1a3e5d7f9c1b3a5e7d9f2c4ba4b1c8f21a7d3e5c6b8f0a1d3c5b7e9a2d4f6c8b |

---

CUSTODY LOG

| Date/Time | Released By (Name & Signature) | Received By (Name & Signature) | Purpose of Transfer / Remarks |

| :--- | :--- | :--- | :--- |

| 02-Aug-2025, 10:30 AM | Hency Sharma | Rohan Verma (Forensic Tech) | Transport to National Cyber Crime Unit Lab, Delhi. |

| 02-Aug-2025, 12:45 PM | Rohan Verma | Priya Singh (Lab Custodian) | Logged into lab evidence locker #415. |

| 03-Aug-2025, 09:30 AM | Priya Singh | Aman Gill (Forensic Analyst) | For ransomware analysis and data recovery attempt. |

| 05-Aug-2025, 06:00 PM | Aman Gill | Priya Singh | Returned to evidence locker #415 post-analysis. |

---

FINAL DISPOSITION

| | |

| :--- | :--- |

| Date: | 06-Aug-2025 |

| Action Taken: | Findings submitted to CERT-In. Evidence stored for case prosecution. |

| Authorized By: | Hency Sharma |

#### ### Remaining Evidence Items Log

- **`MSH-HDD-002`**: Hard Disk 2 (from affected server)
    
- **`MSH-HDD-003`**: Hard Disk 3 (from affected server)
    
- **`MSH-LAP-001`**: Laptop 1 (hospital IT staff)
    
- **`MSH-LAP-002`**: Laptop 2 (hospital IT staff)
    
- **`MSH-RTR-001`**: Router
    
- **`MSH-PD-001`**: Pen Drive (64GB, ransom notes)
    
- **`MSH-MOB-001`**: Mobile Phone 1 (suspected employee)
    
- **`MSH-MOB-002`**: Mobile Phone 2 (suspected employee)
    

---

### ## Case Scenario 5 — Cryptocurrency Investment Fraud

Case Title: CoinX Crypto Scam

Case Number: CXP-2025-09-005

Lead Investigator: Hency Sharma

#### ### Detailed Chain of Custody Form (Sample)

CHAIN OF CUSTODY RECORD

(NIST SP 800-86 Compliant)

|||
|---|---|
|**Evidence ID:**|`CXP-LAP-001`|
|**Item Description:**|Apple MacBook Pro 16-inch, Space Grey. Used by primary scam organizer.|
|**Serial Number:**|`SN: C02F1234PQ05`|
|**Location Collected:**|CoinX Pvt. Ltd. Office, Hyderabad - Director's Cabin.|
|**Date/Time Collected:**|18-Sept-2025, 03:20 PM IST|
|**Collected By:**|Hency Sharma|

---

INTEGRITY VERIFICATION

| | |

| :--- | :--- |

| Hashing Algorithm: | SHA-256 |

| Hash Value (at collection): | 5e7d9f2c4ba4b1c8f21a7d3e5c6b8f0a1d3c5b7e9a2d4f6c8b1a3e5d7f9c1b3a |

---

CUSTODY LOG

| Date/Time | Released By (Name & Signature) | Received By (Name & Signature) | Purpose of Transfer / Remarks |

| :--- | :--- | :--- | :--- |

| 18-Sept-2025, 04:00 PM | Hency Sharma | Rohan Verma (Forensic Tech) | Transport to Cyber Forensics Unit, Hyderabad. |

| 18-Sept-2025, 05:15 PM | Rohan Verma | Priya Singh (Lab Custodian) | Logged into lab evidence locker #520. |

| 19-Sept-2025, 10:15 AM | Priya Singh | Aman Gill (Forensic Analyst) | For blockchain analysis and transaction tracing. |

| 22-Sept-2025, 02:00 PM | Aman Gill | Priya Singh | Returned to evidence locker #520 post-analysis. |

---

FINAL DISPOSITION

| | |

| :--- | :--- |

| Date: | 23-Sept-2025 |

| Action Taken: | Findings shared with Interpol. Evidence stored for extradition and legal proceedings. |

| Authorized By: | Hency Sharma |

#### ### Remaining Evidence Items Log

- **`CXP-LAP-002`**: Laptop 2 (used by scam organizers)
    
- **`CXP-HDD-001`**: Hard Disk 1 (transaction data)
    
- **`CXP-HDD-002`**: Hard Disk 2 (transaction data)
    
- **`CXP-HDD-003`**: Hard Disk 3 (transaction data)
    
- **`CXP-RTR-001`**: Router (connected to CoinX servers)
    
- **`CXP-PD-001`**: Pen Drive (64GB, fake KYC documents)
    
- **`CXP-MOB-001`**: Mobile Phone 1 (investor communication)
    
- **`CXP-MOB-002`**: Mobile Phone 2 (investor communication)