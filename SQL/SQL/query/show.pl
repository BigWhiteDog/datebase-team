#!/usr/bin/perl -w

for (my $i=1; $i <= 10; $i++) {
  my $cmd = "psql -f $i.sql -o ./out/$i.out";
  system ("$cmd");
} # for i 


