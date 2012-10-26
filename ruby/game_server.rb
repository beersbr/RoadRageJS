require 'eventmachine'
require 'em-websocket'
require 'json'

@sockets = []
@players = {}

threads_moved = [];

def socket_cleaner
	threads_moved.each do |t|
		if t == true
			;
		end
	end
	sleep(20)
end

thread = Thread.new{ socket_cleaner }


def create_players_json(not_hash)
	json_string = '['

	@players.keys.each do |k|
		if(k != not_hash)
			json_string += @players[k].to_json + ","
			# puts "sending #{k} => #{not_hash}"
		end
	end

	if(json_string[-1] == ',')
		json_string[-1] = ']'
	else
		json_string[-1] += ']'
	end

	json_string
end

EventMachine.run do
  EventMachine::WebSocket.start(:host => '10.11.3.37', :port => 9898) do |socket|

    socket.onopen do
      	@sockets << socket
    end

    socket.onmessage do |msg|
  		json_msg = JSON.parse msg


  		# puts "RECV: #{json_msg["phash"]} => #{json_msg}"

  		# update the players list
  		@players[json_msg["phash"]] = json_msg

  		player_list = create_players_json(json_msg["phash"])

		@sockets.each do |s|
			s.send player_list
		end

    end

    socket.onclose do
      @sockets.delete socket
    end

  end
end