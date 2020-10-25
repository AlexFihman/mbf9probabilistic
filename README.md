# mbf9probabilistic
Calculation of 9th Dedekind number by probabilistic methods

Task1 - computation of 9th Dedekind number by probability of making a pair
To compile, you need gcc/g++ build environment

Go to src folder, run 
make

Copy to scripts folder executable file mbf9bin.

Edit job.php, set threads to number of $threads in your server, 
$init_seed to 1, $last_seed to number iterations you want to run.

Execute the php script.

Results collected in results folder. To gather results you need sqlite3.

Run following commands:

cat *.txt > m.res

sqlite3 m.db

CREATE TABLE results (
	column_1,
   	column_2,
	column_3,
	column_4,
	column_5,
	PRIMARY KEY (column_1, column_2)
);
.separator "\t"
.import m.res results
select count(*), sum(column_3), sum(column_3*column_3) from results;

How to interpret results:

Say, I got
200000|1818779564|17446530001650

It means probability of two functions to make a pair is 
1818779564/200000 * 1e-8 = 9.0939E-05

It gives D9 a value of D8^2 * 9.0939E-05 = 2.86515E+41

Sigma squared of one iteration is 
17446530001650 - (1818779564/200000)^2 = 4533672.448

Sigma of one iteration is
Sqrt(4533672.448) = 2129.242224

Sigma of all 200000 iterations is 2129.242224/sqrt(200000) = 4.761130353, which is 1.5E+38 in final value.

Final result: 2.86(51)+41, sigma: 1.5E+38



