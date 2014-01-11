README

Feel free to branch this small project if you want but please do not pull/push proposals yet.
Indeed, as january 7 2013 I'm still getting started with Git & GitHub and I'm not really sure I know exactly what I do... :-)
In fact I use this project as a way to learn how to use Git.

There are 2 differents projects

1 - CVI Function Calls Extractor :
    This project shows how to count function calls in C source file
    The project works with LabWindows/CVI 2013
    Open the CVI Function Calls Extractor.cws file which is in the cvi2013 subdirectory
    The code can be compiled either in 32 or 64 bits
    At the end the application generates 2 reports : .txt and HTML
    Both reports show how many time such or such function is called among a set of C source file
    For example the report will tell you that among a set of C files, malloc() is called 22 times while printf() is called 10 times etc.
    The .txt can be easily open with MSExcel or Libre Office since it is a comma separated file (CSV)
    On the other hand, you can simply open the HTML file with any browser that reads HTML5
     
    Known issues :
        1 - Even if working correctly, when analyzing large number of files/directories/sub directories, the application may seems no responding 
            Don't worry. Just wait. For example it takes 65 sec to analyze 350 of 360 C source code of the LabWindows/CVI samples directory
        2 - Some paths are still hard coded in the code 
        3 - The code needs more comments and some clean up
    
    Architecture and how it works :
        The code walk through the target directory recursively
        It create a list of .c files
        For each .c file it calls the clang compiler that come with CVI 2013
        The compiler generates assembly version of the .c file (this is done with multiple threads using OpenMP)
        Once done, the application read the assembly code and identify functions calls (it looks for "call printf" for example)
        The results are then stored in a ash table which is saved as a report in a csv as well as html page.
        
        Generating the assembly code is a good way to ensure you do not miss any function call. Otherwise, you need to parse the .c and use regular expression. 
        However, with such method it might be difficult to cover every function calls. Think about function calls that are embedded in macro for example.
        Using the compiler is definitively much easier and accurate.
        
2 - Bench ANSI C functions :
    In the eponym sub directory there is a second project
    Based on the results of the "CVI Function Calls Extractor" application, this project is an attempt to benchmark some of the CVI ANSI 
    functions calls vs the ones coming with from Visual Studio. Typically this rank printf() in both context.
    This is why the same code works within 2 different projects
        1 - Using LabWindows/CVI, go to the cvi2013 subdirectory and open bench.cws
        2 - Using MSVC 2013 (works with Express edition), go to the msvc subdirectory and open bench.sln.
        
    Architecture and how it works :
        I did my best to avoid the traps that come with optimizers (clang or msvc)
        This is why in most of the benches there are "fake_optimizer" local variables 
        
        The main function use function pointers to invoke each bench
        Each bench has the same structure so it should be easy to extend the code to fit your need.
        Regarding the timing the high resolution timer is used
        



