-- one to many join 
select col2tab1.id
from col2tab1, col5tab1
where col2tab1.id = col5tab1.weight;
