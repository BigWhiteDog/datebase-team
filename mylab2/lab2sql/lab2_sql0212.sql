
--REQUEST 3
insert into people
values($1, $2, $3, $4, $5, 0);


--REQUEST 4

select t.station_order, t.station_name, t.arriving_time, t.leaving_time, rs.hard_seat, t.hard_seat_price, rs.soft_seat, t.soft_seat_price, rs.hard_berth_upper, t.hard_berth_upper_price, rs.hard_berth_middle, t.hard_berth_middle_price, rs.hard_berth_lower, t.hard_berth_lower_price, rs.soft_berth_upper, t.soft_berth_upper_price, rs.soft_berth_lower, t.soft_berth_lower_price
from train as t, remaining_seat as rs
where t.train_id = 'G111'
and t.train_id=rs.train_id
and t.station_order=rs.station_order
and   rs.real_date = '2016-11-01';


--REQUEST 5
insert into temp(train_id,dep_station,dest_station,,leaving_time,seat_type)
select rs.train_id,t1.station_name,t2.station_name,t2.hard_seat_price-t1.hard_seat_price, 'hard_seat'
from remaining_seat as rs, train as t1, train as t2, station as s1, station as s2
where rs.real_date = date'2016-11-01'-t1.leaving_days_offset
and s1.city_name = '北京'
and s2.city_name = '沈阳'
and rs.train_id = t1.train_id
and t1.train_id = t2.train_id
and rs.station_order between t1.station_order + 1 and t2.station_order
and t1.station_order < t2.station_order
and t1.station_name = s1.station_name
and t2.station_name = s2.station_name
and (t1.hard_seat_price != 0 or t1.station_order=1)
and t2.hard_seat_price != 0
group by rs.train_id,t1.station_name,t2.station_name,t1.hard_seat_price,t2.hard_seat_price
having min(rs.hard_seat) > 0;

select * from temp order by


select rs1.train_id, rs2.train_id,s2.station_name, 'hard_seat'
from remaining_seat as rs1, remaining_seat as rs2, train as t1, train as t2, train as t3, train as t4, station as s1, station as s2, station as s3
where 
    rs1.real_date = '2016-11-01'
and rs2.real_date = '2016-11-01'
and s1.city_name = '沈阳'
and s3.city_name = '上海'

and rs1.train_id = t1.train_id
and t1.train_id = t2.train_id

and rs2.train_id = t3.train_id
and t3.train_id = t4.train_id

and s1.city_name != s2.city_name
and t2.station_name = t3.station_name
and t2.arriving_time + interval '1 hour' < t3.leaving_time
and t2.arriving_time + interval '4 hours' > t3.leaving_time 

and rs1.station_order between t1.station_order + 1 and t2.station_order
and t1.station_order < t2.station_order
and t1.station_name = s1.station_name
and t2.station_name = s2.station_name
and (t1.hard_seat_price != 0 or t1.station_order=1)
and t2.hard_seat_price != 0

and rs2.station_order between t3.station_order + 1 and t4.station_order
and t3.station_order < t4.station_order
and t3.station_name = s2.station_name
and t4.station_name = s3.station_name
and (t3.hard_seat_price != 0 or t3.station_order=1)
and t3.hard_seat_price != 0

group by rs1.train_id, rs2.train_id,s2.station_name
having (min(rs1.hard_seat) > 0 and min(rs2.hard_seat) > 0);


select rs1.train_id, rs2.train_id, 'hard_seat'
from remaining_seat as rs1, remaining_seat as rs2, train as t1, train as t2, train as t3, train as t4, station as s1, station as s2, station as s3, station as s4
where
    rs1.real_date = '2016-11-01'
and rs2.real_date = '2016-11-01'
and s1.city_name = '北京'
and s4.city_name = '上海'

and rs1.train_id = t1.train_id
and t1.train_id = t2.train_id

and rs2.train_id = t3.train_id
and t3.train_id = t4.train_id

and s1.city_name != s2.city_name
and s3.city_name != s4.city_name
and s2.city_name = s3.city_name
and t2.station_name != t3.station_name
and t2.arriving_time + interval '2 hours' < t3.leaving_time
and t2.arriving_time + interval '4 hours' > t3.leaving_time 

and rs1.station_order between t1.station_order + 1 and t2.station_order
and t1.station_order < t2.station_order
and t1.station_name = s1.station_name
and t2.station_name = s2.station_name
and (t1.hard_seat_price != 0 or t1.station_order=1)
and t2.hard_seat_price != 0

and rs2.station_order between t3.station_order + 1 and t4.station_order
and t3.station_order < t4.station_order
and t3.station_name = s3.station_name
and t4.station_name = s4.station_name
and (t3.hard_seat_price != 0 or t4.station_order=1)
and t4.hard_seat_price != 0

group by rs1.train_id, rs2.train_id ,s2.station_name, s3.station_name
having (min(rs1.hard_seat) > 0 and min(rs2.hard_seat) > 0);


--REQUEST 8
select o.order_id, o.train_id, o.dep_station, o.dest_station,o.seat_type,o.one_price, o.real_date, t.leaving_time, t.arriving_time
from train as t, orders as o
where o.order_id= $1
and t.train_id = o.train_id
and t.station_name = o.dep_station
and o.passenger_id = $2;

select o.order_id,o.real_date,t.leaving_time,o.dep_station, o.dest_station, o.one_price+5
from train as t, orders as o
where t.train_id = o.train_id
and t.station_name = o.dep_station
and $1 <= o.real_date+ t.leaving_days_offset
and o.real_date+ t.leaving_days_offset<= $2
and o.passenger_id = $3;


--REQUEST 9
select count(*)
from orders;

select sum(one_price)
from orders;

select 5*count(train_id)
from orders;

select orders.train_id, count(*)as Number
from orders
group by train_id
order by Number desc
limit 10;

update remaining_seat as rs
set rs.hard_seat= rs.hard_seat-1;
select rs.train_id,t1.station_name,t2.station_name,t2.hard_seat_price-t1.hard_seat_price, 'hard_seat'
from , train as t1, train as t2, station as s1, station as s2
where rs.real_date = date'2016-11-01'-t1.leaving_days_offset
and s1.city_name = '北京'
and s2.city_name = '沈阳'
and rs.train_id = t1.train_id
and t1.train_id = t2.train_id
and rs.station_order between t1.station_order + 1 and t2.station_order
and t1.station_order < t2.station_order
and t1.station_name = s1.station_name
and t2.station_name = s2.station_name
and (t1.hard_seat_price != 0 or t1.station_order=1)
and t2.hard_seat_price != 0
group by rs.train_id,t1.station_name,t2.station_name,t1.hard_seat_price,t2.hard_seat_price
having min(rs.hard_seat) > 0;