sqlite3 res.db

create table data (id, loop, cnt, t1, t2);
.separator "\t"
.import res.txt data

create table data2 (id, loop, cnt);

insert into data2 
select distinct id, loop, cnt from data;

drop table data;
vacuum;

select sum(loop),sum(loop*loop),count(loop) from data2 where id > 0;
