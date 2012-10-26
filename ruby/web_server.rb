require 'sinatra'

DocRoot = File.dirname(__FILE__)
DocRoot = DocRoot+"/../www/styles"
puts File.expand_path(DocRoot)

set :public_folder, DocRoot


get '/' do
	send_file('road_rage.html')
	# File.read(File.join('www', 'road_rage.html'))
	# methodName = params[:splat][0].to_s
	# params.to_s
	# if path == 'save'
	# 	"in save"
	# end
end

get 'getUser' do
	params
end

get 'setUser' do
	params
end

get '/*' do

end



