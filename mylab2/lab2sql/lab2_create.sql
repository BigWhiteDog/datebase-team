create database train12306;


create table station(
	station_name varchar(16) primary key,
	city_name varchar(16) not null
);


create table train(
	train_id char(5),
	station_order integer,
	station_name varchar(16),
	arriving_time time,
	leaving_time time,
	--staying_time integer,
	duration integer,
	hard_seat_price	decimal(5,1),
	soft_seat_price	decimal(5,1),
	hard_berth_upper_price decimal(5,1),
	hard_berth_middle_price decimal(5,1),
	hard_berth_lower_price decimal(5,1),
	soft_berth_upper_price decimal(5,1),
	soft_berth_lower_price decimal(5,1),
	leaving_days_offset	integer,
	arriving_days_offset integer,
	primary key(train_id,station_order),
	foreign key(station_name) references station(station_name)
);

create table remaining_seat(
	train_id char(5),
	station_order integer,
	real_date date,
	hard_seat integer default 5,
	soft_seat integer default 5,
	hard_berth_upper integer default 5,
	hard_berth_middle integer default 5,
	hard_berth_lower integer default 5,
	soft_berth_upper integer default 5,
	soft_berth_lower integer default 5,
	primary key(train_id,station_order,real_date),
	foreign key(train_id,station_order)references train(train_id,station_order)
);

create table people(
	passenger_id char(18) primary key,
	passenger_name varchar(16),
	passenger_phone char(10),
	passenger_card char(16),
	passenger_card_bank varchar(100),
	people_type integer not null
);

create table orders(
	order_id integer primary key,
	train_id char(5) not null,
	dep_station varchar(16),
	dest_station varchar(16),
	seat_type varchar(32) not null,
	one_price decimal(5,1) not null,
	dep_date date not null,
	passenger_id char(18) not null,
	foreign key(passenger_id)references people(passenger_id),
	foreign key(dep_station)references station(station_name),
	foreign key(dest_station)references station(station_name)
);

--copy train from '/home/dbms/Lab2/train.csv' with (format csv, delimiter ',');

create table temp(
	train_id char(5) not null,
	dep_station varchar(16),
	dest_station varchar(16),
	seat_type varchar(32) not null,
	one_price decimal(5,1) not null,
	real_date date,
	dep_date date,
	dep_station_order integer,
	dest_station_order integer,
	leaving_time time,
	duration integer	
);


update train set duration=0 where duration isnull;

update train set hard_seat_price=0 where hard_seat_price isnull;
update train set soft_seat_price=0 where soft_seat_price isnull;
update train set hard_berth_upper_price=0 where hard_berth_upper_price isnull;
update train set hard_berth_middle_price=0 where hard_berth_middle_price isnull;
update train set hard_berth_lower_price=0 where hard_berth_lower_price isnull;
update train set soft_berth_upper_price=0 where soft_berth_upper_price isnull;
update train set soft_berth_lower_price=0 where soft_berth_lower_price isnull;




