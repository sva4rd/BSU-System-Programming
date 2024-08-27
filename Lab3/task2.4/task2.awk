awk
BEGIN { total = 0 }
{
    if ($5 >= 3000 && $5 <= 7000) {
        print toupper($1), $2, $3, $4, $5;
        total += $5;
    }
}
END { print "Total:", total }
