-- insert SQL statements demo
--
--   usage:
--   $ ucasdb ./03_insert.sql


-- correct statement -- 
create table crtDemo (id int, name varchar); 

insert into crtDemo values (1, 'first record');
insert into crtDemo values (2, 'second record');
insert into crtDemo values (3, 'third record');
insert into crtDemo values (1, 'first again');





-- -- wrong statements -- 

-- -- 0. table does not exist -- 
-- insert into TableNotExist values (0, 'wrong');

-- -- 1. field miss -- 
-- insert into crtDemo values (0);

-- -- 2. value is null -- 
-- insert into crtDemo values (0, null);

-- -- 3. type mismatch -- 
-- insert into crtDemo values ('0', 'wrong');

-- -- 4. more field -- 
-- insert into crtDemo values (0, 1, 'wrong');

-- -- 5. syntax error -- 
-- insert into crtDemo values (0 'wrong');



