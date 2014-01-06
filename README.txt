README

The "CVI Function Calls Extractor" project show how to countthe function calls in C source code
It works with CVI 2013
Open the CVI Function Calls Extractor.cws file in the cvi2013 directory
The code can be compiled either in 32 or 64 bits
At the end the application generates 2 reports : .txt and HTML
The .txt can be easily open with MSExcel or Libre Office since results a comma separated
On the other hand you can simply open the HTML file with any browser


Known issues
1 - Even if working correctly, when analyzing large number of files, the application may seems no responding 
    Don't worry. Just wait. For example it takes 65 sec to analyze 350 of 360 C source code of the ../samples directory
2 - Some paths are still hard coded in the code 
3 - The code is working but is messy. It needs much more comments


In the "Bench ANSI C functions" directory there is another CVI project
Based on the results of the "CVI Function Calls Extractor" project, this one is an attempt to bench some of the CVI ANSI 
function vs the same functions from Visual Studio
Using CVI, go to the cvi2013 subdirectory and open bench.cws
Using MSVC 2013, go to msvc subdirectory and open bench.sln 
