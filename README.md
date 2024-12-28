Competria - A Simplified Competitive Programming Platform
1. Introduction
Competria is a desktop application designed to simulate the functionality of platforms like HackerRank, providing a simplified environment for users to solve programming problems and be assessed based on test cases. It is aimed at fostering programming skills in an interactive and user-friendly way.
The application leverages Qt Framework for development, MS Access as a database, and the QScintilla library for code editing and syntax highlighting. It offers separate interfaces for administrators and users, ensuring functionality tailored to their respective roles.
________________________________________
2. Objectives
•	Create a competitive programming platform with essential features for problem-solving.
•	Implement user authentication and role-based functionality for users and administrators.
•	Integrate a dynamic code editor using the QScintilla library.
•	Validate user solutions against pre-defined test cases for accurate evaluation.
________________________________________
 3. Key Functional Areas:
•	Admin Dashboard: Interfaces for managing users and problems.
•	Database Management: Handles database operations for users, problems, submissions, and test cases.
•	Session Management: Implements a singleton pattern to manage login states.
•	Problem Solving and Submissions: Supports user interaction for solving problems and saving results.
4. Tools and Technologies Used
1.	Development Tools:
•	Qt Framework (C++) for UI and core application logic.
•	QScintilla library for code editing and syntax highlighting.
•	GCC/G++ compilers for code compilation.
2.	Database:
•	Microsoft Access for data storage and retrieval.
3.	UI Design:
•	Figma for designing user interfaces and prototyping.
5. Features
1.	User Authentication:
•	Signup for new users.
•	Separate login interfaces for users and administrators.
2.	Admin Section:
•	Problem Manager:
                	Add, edit, delete problems.
                	Add and manage test cases for each problem.
•	User Manager:
                	Add, edit, delete user accounts.
3.	User Section:
•	View problems on a user dashboard.
•	Select a problem to solve via the problem-solving window.
•	Input code into the integrated QScintilla code editor.
•	Compile and run the code using GCC/G++.
•	Validate solutions against test cases and receive marks accordingly.
4.	Database Integration:
•	Persistent storage for problems, test cases, and user information.
