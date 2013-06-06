def wykres_file(input)
	wyk = "
set xdata time
set timefmt \"%s\"
# set xtics 3600
set format x \"%H:%M:%S\"

set terminal png size 1024,768
set output 'out\\#{input}.png'
set encoding iso_8859_2 
plot '#{input}.txt' using 1:($2/100) with lines"
	
	File.open("#{input}.gnuplot",'w+') { |f| f.write(wyk) }
	`gnuplot #{input}.gnuplot"`
end

(1..21).each { |num|
	wykres_file('wyk'+num.to_s)
}