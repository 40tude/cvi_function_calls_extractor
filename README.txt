README

Feel free to branch this project if you want but please do not pull/push proposals yet.
Indeed, as january 7 2013 I'm still getting started with GitHub and I'm not really sure I know what I do... :-)
In fact I use this project as a way to learn how to use Git.

There are 2 differents projects

1 - CVI Function Calls Extractor :
This project shows how to count function calls in C source file
The project works with LabWindows/CVI 2013
Open the CVI Function Calls Extractor.cws file in the cvi2013 subdirectory
The code can be compiled either in 32 or 64 bits
At the end the application generates 2 reports : .txt and HTML
Both reports show how many time such or such function is called among a set of C source file
For example the report will tell you that among the C files of such directory, malloc() is called 22 times while printf() is called 10 times etc.
The .txt can be easily open with MSExcel or Libre Office since it is a comma separated file (CSV)
On the other hand, you can simply open the HTML file with any browser that reads HTML5
 
Known issues
1 - Even if working correctly, when analyzing large number of files/directories/sub directories, the application may seems no responding 
    Don't worry. Just wait. For example it takes 65 sec to analyze 350 of 360 C source code of the LabWindows/CVI samples directory
2 - Some paths are still hard coded in the code 
3 - The code is working but is messy. It needs much more comments and some clean up

2 - Bench ANSI C functions :
In the eponym sub directory there is a second project
Based on the results of the "CVI Function Calls Extractor" application, this project is an attempt to benchmark some of the CVI ANSI 
functions calls vs the ones coming with from Visual Studio. Typically this rank printf() in both context.
This is why the same code works within 2 different projects
Using LabWindows/CVI, go to the cvi2013 subdirectory and open bench.cws
Using MSVC 2013, go to the msvc subdirectory and open bench.sln 



