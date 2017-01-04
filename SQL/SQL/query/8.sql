select frequency, max(id)
from col5tab1
where id > 0 and frequency >= 3
group by frequency;
