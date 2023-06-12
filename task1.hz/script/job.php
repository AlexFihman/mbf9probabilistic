<?php

$threads = 4;
$init_seed = 1;
$last_seed = 4;
$loops = 100;

mkdir("../result");

for ($i=$init_seed;$i<=$last_seed;$i+=$threads)
{
    if (!file_exists("../result/".$i.".txt"))
    {
        exec("./mbf9bin ".$i." ".$threads." ".$loops." > "."../result/temp.txt");
        rename("../result/temp.txt","../result/".$i.".txt");
    }
}

?>