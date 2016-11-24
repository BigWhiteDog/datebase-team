insert into people values ('123456','admin','010139','62170000','建设银行',1);
insert into orders values(0,'test','北京南','上海虹桥','hard_seat',0,'2016-11-01','123456');
insert into orders values(1,'G111','北京南','上海虹桥','hard_seat',553.0,'2016-11-01','123456789012345678');
insert into orders values(2,'G111','北京南','上海虹桥','hard_seat',553.0,'2016-11-02','123456789012345678');
insert into orders values(3,'D1','北京','沈阳','hard_seat',206.0,'2016-11-02','123456789012345678');

select o.order_id,o.real_date,t.leaving_time,o.dep_station, o.dest_station, o.one_price+5
from train as t, orders as o
where t.train_id = o.train_id
and t.station_name = o.dep_station
and '2016-11-01' <= o.real_date + t.leaving_days_offset
and o.real_date + t.leaving_days_offset <= '2016-11-02'
and o.passenger_id = '123456';



select o.order_id, o.train_id, o.dep_station, o.dest_station,o.seat_type,o.one_price, o.real_date, t.leaving_time, t.arriving_time
from train as t, orders as o
where o.order_id= 2
and t.train_id = o.train_id
and t.station_name = o.dep_station
and o.passenger_id = '123456';


(
select rs.train_id,t1.station_name,t2.station_name,t2.hard_seat_price-t1.hard_seat_price, 'hard_seat'
from remaining_seat as rs, train as t1, train as t2, station as s1, station as s2
where rs.real_date = date'2016-11-01'
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
having min(rs.hard_seat) > 0
)
except(
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
having min(rs.hard_seat) > 0
);


delete from orders where order_id= 4 and passenger_id = '123456';