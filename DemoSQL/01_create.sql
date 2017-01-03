-- create table SQL statements demo
--
--   usage:
--   $ ucasdb ./01_create.sql


-- correct statement -- 
create table crtDemo (id int, name varchar); 





-- wrong statements -- 

-- 0. table existed  --
create table crtDemo (id int, name varchar); 

-- 1. miss field name --
create table crtWrong1 (int, name varchar); 

-- 2. miss data type  --
create table crtWrong2 (id, name varchar); 

-- 3. miss ','  --
create table crtWrong2 (id int  name varchar); 

-- 4. miss ';'  --
create table crtWrong2 (id int, name varchar) 

-- 5. wrong data type  --
create table crtWrong3 (id float, name varchar); 

 



