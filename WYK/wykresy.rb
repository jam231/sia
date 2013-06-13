def file_get_max_amount(file)
	max = 0
	File.open(file,'r') { |f|
		f.each_line { |l|
			max = [l.split[2].to_i,max].max
		}
	}
	return max
end

def wykres_file(input,maxtick)
	wyk = "
set xdata time
set timefmt \"%s\"
# set xtics 3600
set format x \"%H:%M:%S\"
set style line 1 lt rgb \"#FF0000\" lw 2

set terminal pngcairo size 1024,768
set lmargin 10
set output 'out\\#{input}.png'
set encoding iso_8859_2 
set multiplot layout 2,1
set size 1,0.8
set origin 0,0.2
plot '#{input}.txt' using 1:($2/100) with lines ls 1 notitle
set size 1,0.2
set origin 0,0.0
set ytics #{maxtick/3}
set boxwidth 0.5
set style fill solid
plot '#{input}.txt' using 1:3 with boxes notitle

"
	
	File.open("#{input}.gnuplot",'w+') { |f| f.write(wyk) }
	`gnuplot #{input}.gnuplot"`
end

(2..21).each { |num|
	maxtick = file_get_max_amount('wyk'+num.to_s+'.txt')
	wykres_file('wyk'+num.to_s,maxtick)
}