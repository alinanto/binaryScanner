@echo off
cls
echo now_running_sample1
binaryScanner sample1.bmp sample1.txt
pause
cls
echo now_running_sample2
binaryScanner sample2.bmp sample2.txt
pause
cls
echo now_running_test
binaryScanner test.bin test.txt
pause
cls
echo now_running_with_no_arguments
binaryScanner 
pause
cls
echo now_running_with_3_arguments
binaryScanner test.bin test.txt sample.txt
pause
cls
echo now_running_with_1_argument
binaryScanner test.bin
pause


