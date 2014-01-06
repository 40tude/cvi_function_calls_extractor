README

Please branch this project if you want but please do not pull/push proposals yet
Indeed, january 7 2013 I'm still getting started with GitHub and I'm not sure I know what I do... :-)
At least the code works and you know I'm working on it

Their are 2 differents projects

CVI Function Calls Extractor :
This project show how to count the function calls in C source code
It works with CVI 2013
Open the CVI Function Calls Extractor.cws file in the cvi2013 subdirectory
The code can be compiled either in 32 or 64 bits
At the end the application generates 2 reports : .txt and HTML
Both files show how many time such or such functions is called among a set of C source file
For example it will tell you that among the file is such directory, malloc() is called 22 times, printf() 10 tiems etc.
The .txt can be easily open with MSExcel or Libre Office since result a comma separated file (CSV)
On the other hand you can simply open the HTML file with any browser that read HTML5
 
Known issues
1 - Even if working correctly, when analyzing large number of files, the application may seems no responding 
    Don't worry. Just wait. For example it takes 65 sec to analyze 350 of 360 C source code of the ../samples directory
2 - Some paths are still hard coded in the code 
3 - The code is working but is messy. It needs much more comments

Bench ANSI C functions :
In the eponym sub directory there is another project
Based on the results of the "CVI Function Calls Extractor" application, this project is an attempt to benchmark some of the CVI ANSI 
functions vs the ones coming with from Visual Studio
This is why the same code works within 2 different projects
Using CVI, go to the cvi2013 subdirectory and open bench.cws
Using MSVC 2013, go to the msvc subdirectory and open bench.sln 

