# Plagiarism Detection System using Suffix Array (C++)

## Overview
This project implements a **plagiarism detection system** in C++ that analyzes **entire document similarity**, not just a single copied section.  
Unlike basic implementations that rely on only the *longest common substring*, this system detects **all common substrings**, accumulates the **total copied content**, and computes a **realistic plagiarism percentage**.

The project is based on **Suffix Array and LCP (Longest Common Prefix)** techniques, making it efficient and suitable for large text documents.

---

## Key Improvement Over Basic Approaches
Most academic implementations calculate plagiarism using **only one longest match**, which underestimates plagiarism when copied content is scattered.

This project:
- Detects **multiple matching substrings**
- Accumulates **total non-overlapping copied content**
- Computes plagiarism based on the **entire document**
- Avoids double counting overlapping regions

As a result, the reported plagiarism percentage reflects **overall similarity**, not just one copied paragraph.

---

## How the System Works

1. Combine both documents using a unique delimiter
2. Construct a **Suffix Array** for the combined text
3. Build the **LCP array**
4. Identify all common substrings belonging to different documents
5. Collect all matching regions
6. Remove overlapping matches to prevent double counting
7. Sum total matched characters
8. Calculate plagiarism percentage for each document

---

## Plagiarism Percentage Formula

Plagiarism % = (Total matched content / Total document length) × 100

yaml
Copy code

Plagiarism is calculated **independently** for:
- Document A
- Document B

This ensures fairness when documents differ in size.

---

## Plagiarism Severity Levels

| Percentage | Interpretation |
|----------|---------------|
| 0–10% | Very Low |
| 10–30% | Low |
| 30–60% | Moderate |
| 60%+ | High |

---

## Features
- Suffix Array construction
- LCP Array computation
- Detection of all common substrings
- Accumulation of total copied content
- Overlap-aware matching
- Plagiarism percentage and severity classification
- Built-in test case
- Custom file comparison
- Execution time measurement

---

## Time and Space Complexity
- **Time Complexity:** `O((m + n) log (m + n))`
- **Space Complexity:** `O(m + n)`

Where:
- `m` = length of Document A  
- `n` = length of Document B  

---

## File Structure

Plagiarism-Detection/
│── plagiarism_detector.cpp
│── README.md

makefile
Copy code

Optional:
│── sample_docs/
│ ├── docA.txt
│ ├── docB.txt

yaml
Copy code

---

## Compilation and Execution

### Compile
g++ plagiarism_detector.cpp -o plagiarism_detector

shell
Copy code

### Run
./plagiarism_detector

yaml
Copy code

The program allows you to:
1. Run a predefined test case
2. Compare two custom text files

---

## Limitations
- Detects **exact copied text only**
- Does not detect paraphrasing or semantic similarity
- Does not handle reworded or translated plagiarism

In real-world commercial systems, this technique is usually combined with:
- Token-based matching
- Shingling
- Semantic similarity models
- Machine learning approaches

---

## Use Cases
- Academic and university projects
- Algorithm and data-structure demonstrations
- Plagiarism detection research prototypes
- GitHub portfolio and interview discussions
