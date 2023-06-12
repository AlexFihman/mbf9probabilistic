This is a set of code and scripts for Calculation of 9th Dedekind Number

Folders:

src - source files
result - destination of result files
script - scripts to run calculations, and to find final result

How to compile

Install Boost libraries. On Ubuntu run following command: 
sudo apt-get install libboost-all-dev

Change dir to src/mbf9bin folder. Run following command:
make

Move executable mbf9bin to script folder:
mv mbf9bin ../../script


How to run

Install php. Version 5 and above should be ok.
Go to script folder. Modify parameters in job.php file.

Parameter $threads is number of threads to run in parallel. Make it equal
or less than number of threads CPU(s) your system can run.

On single CPU system it is usually number of cores or (number of cores)*2 if 
hyperthreading is enabled.

$init_seed -- initial seed value. Should not be equal to 0.
$last_seed -- last seed value.
$loops -- number of loops for each seed value

Run job.php by executing following command:
php job.php
