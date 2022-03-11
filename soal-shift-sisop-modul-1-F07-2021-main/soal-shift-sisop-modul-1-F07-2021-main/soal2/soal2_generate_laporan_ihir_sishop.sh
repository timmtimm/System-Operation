#!/usr/bin/bash

awk -F"\t" '
#soal a
BEGIN{ max=0; home=0; consumer=0; corporate=0; central=0; east=0; south=0; west=0 }
{ pp=$21/($18-$21)*100; if (pp>=max) { max=pp; id=$1 } }

#soal b
{ if ((match($3, "-17")) && $10=="Albuquerque") { nama[$7] } }

#soal c
{ if ($8=="Home Office") { home++ } else if ($8=="Consumer") { consumer++ } else if ($8=="Corporate") { corporate++ } }

#soal d
{ if ($13=="Central") { central+=$21 } else if ($13=="East") { east+=$21 } else if ($13=="South") { south+=$21 } else if ($13=="West") { west+=$21 } }

END {
#soal e
printf "Transaksi terakhir dengan profit percentage terbesar yaitu %d dengan presentase %d%%.\n\n", id, max

printf "Daftar nama customer di Albuquerque pada tahun 2017 antara lain:\n"
for (i in nama)
	printf "%s\n", i

if (home<=consumer && home<=corporate)
	printf "\nTipe segmen customer yang penjualannya paling sedikit adalah Home Office dengan %d transaksi.\n", home
else if (consumer<=home && consumer<=corporate)
	printf "\nTipe segmen customer yang penjualannya paling sedikit adalah Consumer dengan %d transaksi.\n", comsumer
else
	printf "\nTipe segmen customer yang penjualannya paling sedikit adalah Corporate dengan %d transaksi.\n", corporate

if (central<=east && central<=south && central<=west)
	printf "\nWilayah bagian (region) yang memiliki total keuntungan (profit) yang paling sedikit adalah Central dengan total keuntungan %f.\n", central
else if (east<=central && east<=south && east<=west)
	printf "\nWilayah bagian (region) yang memiliki total keuntungan (profit) yang paling sedikit adalah East dengan total keuntungan %f.\n", east
else if (south<=central && south<=east && south<=west)
	printf "\nWilayah bagian (region) yang memiliki total keuntungan (profit) yang paling sedikit adalah South dengan total keuntungan %f.\n", south
else
	printf "\nWilayah bagian (region) yang memiliki total keuntungan (profit) yang paling sedikit adalah West dengan total keuntungan %f.\n", west
}
' Laporan-TokoShiSop.tsv > hasil.txt
