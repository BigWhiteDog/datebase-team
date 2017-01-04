-- select single column from medium table
select id, token
from col5tab1
where frequency > 5 and token like 'pending';
