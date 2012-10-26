files = Dir["*.png"]

files.each do |f|
	f_name = f.split('.')[0]
	puts "\"#{f_name}\": {\n\t\"is_solid\": false,\n\t\"image_tag\": \"#{f_name}\"\n},"
end