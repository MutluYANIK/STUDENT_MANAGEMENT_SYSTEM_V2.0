# Student Management System V2.0

A robust, terminal-based Relational Database Management System (RDBMS) written entirely in C. 
Developed with a strict emphasis on **Defensive Programming**, **Dynamic Memory Management**, and **Relational Data Integrity**, this project serves as a comprehensive tool to track students, dynamic courses, and academic enrollments.

## Key Features & Architecture

- **Relational CSV Database Engine:** Custom-built serialization and deserialization engine. The system state is strictly mapped to three relational flat files (`students.csv`, `courses.csv`, `enrollments.csv`), automatically resolving foreign keys (IDs) and rejecting orphaned data during the load process.
- **Defensive Programming & Sanitization:** Bulletproof file I/O operations. Uses advanced `strtok` manipulation and buffer overflow protections (`strncpy`) to gracefully skip corrupted, incomplete, or maliciously modified CSV lines without crashing (Segmentation Fault prevention).
- **Deep Dynamic Memory Allocation:** Completely modular struct hierarchy. Courses have dynamically sized linked lists for exams, and student enrollments dynamically allocate `malloc` arrays for scores based on the specific course's exam count.
- **State Tracking (Unsaved Changes):** Implements a professional state machine using tracking flags. The system monitors unsaved modifications and proactively intercepts exit or memory-wipe commands to prevent accidental data loss.
- **Advanced Reporting Engine:** Generates highly formatted `.txt` reports, including full academic transcripts, filtered class rosters (Passed/Failed/Pending), and global GPA summaries.

---

## Evolution: V1.0 vs. V2.0

The transition from V1.0 to V2.0 marks a paradigm shift from a basic script to a production-ready system architecture.

| Feature | Student Management System V1.0 | Student Management System V2.0 |
| :--- | :--- | :--- |
| **Data Structures** | Fixed arrays or basic linked lists. | Multi-tiered, deeply nested linked lists with dynamic array sizing. |
| **Data Storage** | Basic text storage, prone to data corruption. | 3-Table Relational CSV Database with strict integrity constraints. |
| **Memory Management** | Standard allocations. | Granular `malloc`/`free` operations ensuring **Zero Memory Leaks** upon system exit. |
| **Exam Handling** | Hardcoded exam amounts (e.g., Midterm/Final). | Dynamic exam templates per course. Prevents exceeding 100% total weight limits. |
| **Error Handling** | Basic input loops. | Comprehensive fail-safes: corrupted line skipping, `NULL` pointer evasion, and auto-break UX logic. |

---

## Roadmap: Looking Ahead to V3.0

While V2.0 solidifies the backend and data integrity, **V3.0** is planned to expand the academic logic and refactor the modularity of the system:

- **Semester & Term Tracking:** Grouping courses into specific academic semesters rather than a flat enrollment list.
- **Graduation Calculation Engine:** Automatically calculating remaining credits for graduation by cross-referencing completed courses against a predefined departmental curriculum.
- **Prerequisite & Retake Logic:** Preventing students from enrolling in courses they have already passed, while allowing retakes for failed ('FF') courses.
- **MVC Pattern Refactoring:** Further modularizing the `display` and `save` modules into a cleaner, isolated architecture for better maintainability.

---

## Compilation & Execution

This project strictly adheres to standard C libraries (`stdio.h`, `stdlib.h`, `string.h`, `ctype.h`). No external dependencies are required.

**Compile via GCC:**
```bash
gcc src/*.c -o sms
```

**Run the Application:**
```bash
./sms
```

(Ensure the executable is run in an environment where it has read/write permissions to generate the .csv and .txt database files in its root directory).

Developed by Mutlu Yanık - Karabük University, Computer Engineering
