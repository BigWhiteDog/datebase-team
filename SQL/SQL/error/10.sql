-- ambiguous column
select id
from col2, col4
where col2.id = col4.id;
