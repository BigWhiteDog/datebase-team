select weight, count(*)
from col4tab1, col5tab1
where col4tab1.id > 5 and col4tab1.id = col5tab1.id
group by weight;
