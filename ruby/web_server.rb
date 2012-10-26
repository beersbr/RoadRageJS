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

get 'getUser' do
	params
end

get 'setUser' do
	params
end



