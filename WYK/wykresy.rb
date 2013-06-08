def wykres_file(input)
	wyk = "
set xdata time
set timefmt \"%s\"
# set xtics 3600
set format x \"%H:%M:%S\"

set terminal png size 1024,768
set output 'out\\#{input}.png'
set encoding iso_8859_2 
set multiplot layout 2,1
set size 1,0.8
set origin 0,0.2
plot '#{input}.txt' using 1:($2/100) with lines notitle
set size 1,0.2
set origin 0,0.0
#set ytics 10
set boxwidth 0.5
set style fill solid
plot '#{input}.txt' using 1:3 with boxes notitle

"
	
	File.open("#{input}.gnuplot",'w+') { |f| f.write(wyk) }
	`gnuplot #{input}.gnuplot"`
end

(1..21).each { |num|
	wykres_file('wyk'+num.to_s)
}