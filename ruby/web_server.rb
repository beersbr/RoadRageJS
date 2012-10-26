require 'sinatra'

docRoot = File.dirname(__FILE__)
docRoot = docRoot+"/../www"
DocRoot = File.absolute_path(docRoot)
puts DocRoot

puts "Public Root Folder: #{DocRoot}"

set :public_folder, DocRoot

get '/' do
	redirect "road_rage.html"
end

post '/saveLevel' do 
	lvl_data = params["lvl"]
	name = params["name"]

	puts "SAVING: #{lvl_data} as #{name}"

	File.open(File.join(DocRoot, "json/"+name+".json"), "w+") do |f|
		f.write(lvl_data);
	end

end

get 'getUser' do
	params
end

get 'setUser' do
	params
end



