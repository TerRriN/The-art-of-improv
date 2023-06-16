REFACTORING TEST
Passed assignment 2023-06-09
Run Summary:    Type  Total    Ran Passed Failed Inactive
              suites      2      2    n/a      0        0
               tests     34     34     34      0        0
             asserts     57     57     57      0      n/a

Elapsed time =    0.062 seconds
==1027== 
==1027== HEAP SUMMARY:
==1027==     in use at exit: 0 bytes in 0 blocks
==1027==   total heap usage: 847 allocs, 847 frees, 50,403 bytes allocated
==1027== 
==1027== All heap blocks were freed -- no leaks are possible

Will try to refactor with the goals to increase performance and decrease heap usage

--2023-06-16--
Removed list_common.h and integrated its structs
Changed retarded struct names, ex: char_val -> str_val
Rewrote Makefile with ChatGPT and changed file stucture
Currently rewriting edit merch