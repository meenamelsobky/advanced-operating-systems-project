Disk Scheduling Simulator
Overview
This is a C-based simulation tool for disk scheduling algorithms, including FCFS (First-Come, First-Served), SCAN, and C-SCAN (Circular SCAN). The program allows users to input or generate a set of cylinder requests, specify the total number of cylinders and initial head position, and evaluate the performance of different scheduling algorithms. It provides detailed output, including total head movement, seek time, and the order of requests served, with support for both manual and random request generation.
Features

Multiple Algorithms: Supports FCFS, SCAN, and C-SCAN scheduling algorithms.
Interactive Input: Users can manually enter cylinder requests or generate random requests.
Boundary Handling: SCAN and C-SCAN include both disk boundaries (0 and maximum cylinder) as requests.
Direction Control: For SCAN and C-SCAN, users can specify the initial head direction (up or down).
Reusable Requests: Run multiple algorithms on the same set of requests without re-entering data.
Continuous Operation: The program loops until the user chooses to exit.
Detailed Output: Displays the order of requests served, with classification (Initial, Request, Boundary, Jump), and total seek time.

Requirements

A C compiler (e.g., GCC).
Basic terminal interface for input/output.

Installation

Clone the repository or download the disk_scheduling.c file.
Compile the program using GCC:gcc disk_scheduling.c -o disk_scheduling


Run the executable:./disk_scheduling



Usage
Input

Total Number of Cylinders: Enter the maximum cylinder number (must be positive).
Queue Size: Specify the number of cylinder requests.
Request Input: Choose to enter requests manually or generate random ones.
Manual: Enter each cylinder number (0 to total_cylinders - 1).
Random: Automatically generates requests within the specified range.


Initial Head Position: Enter the starting position of the disk head (0 to total_cylinders - 1).
Algorithm Selection: Choose an algorithm (FCFS, SCAN, or C-SCAN).
For SCAN or C-SCAN, specify the direction (u for up, d for down).


Continue Option: After each run, choose to try another algorithm (y/n).

Output

Order of Requests Served: Lists the sequence with types (Initial, Request, Boundary, Jump).
Total Seek Time: Reports the total head movement in cylinders.
Program Continuation: Loops back for another algorithm or exits on 'n'.

Example
Enter the total number of cylinders: 200
Enter the queue size (number of requests): 5
Do you want to enter the queue manually (m) or generate random requests (r)? m
Enter 5 cylinder requests (0 to 199):
55 58 39 18 90
Enter the initial position of the disk head (0 to 199): 50
Enter the scheduling algorithm (FCFS, SCAN, C-SCAN): SCAN
Enter the initial direction (u for up, d for down): u

SCAN Scheduling (Direction: u):

Order of requests served:
Order        Cylinder     Type        
-------------------------------------
Initial      50           Initial     
1            55           Request     
2            58           Request     
3            90           Request     
4            199          Boundary    
5            39           Request     
6            18           Request     
7            0            Boundary    

Total seek time: 231 cylinders

Do you want to try another algorithm on the same requests? (y/n): n
Exiting program.

Contributing

Fork the repository.
Create a feature branch (git checkout -b feature-name).
Commit changes (git commit -m "Add feature").
Push to the branch (git push origin feature-name).
Open a pull request.

License
This project is licensed under the MIT License. See the LICENSE file for details (if applicable).
Contact
For questions or suggestions, please open an issue on the repository or contact the maintainer.
