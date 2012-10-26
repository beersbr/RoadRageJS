files = Dir["*.png"]

files.each do |f|
	f_name = f.split('.')[0]
	puts "\"#{f_name}\": \"images/#{f_name}.png\","
end
