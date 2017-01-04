select weight, count(*)
from col2tab1, col5tab1
where col2tab1.id > 5 and col2tab1.id = col5tab1.id
group by weight;
